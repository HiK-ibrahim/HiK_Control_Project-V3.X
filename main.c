#define _XTAL_FREQ 4000000
//16.09.2023 Y�N-YAZ-YA?BAKIM fixed
//lcd pinleri tan?mlamalari
#define RS RB5
#define EN RB4
#define D4 RB3
#define D5 RB2
#define D6 RB1
#define D7 RB0
#define RW RD6
//y�n pinleri tan?mlamalari
#define FWD RE0
#define REW RE1
#define RewFEAD  RC0
#define FwdFEAD  RE2
#define FwdLMT RC1
#define RewLMT RC2

#define AcEror RD0
#define DcEror RC3

#include <xc.h>
#include "lcd.h"
#include <stdio.h>
#include "uart.h"
#include "16f877a_Conf.c"

unsigned int kesmeSayaci = 0;
struct Time {
    unsigned int hours;
    unsigned int minutes;
    unsigned int seconds;
};

struct Time currentTime = {0, 0, 0}; // Saat, dakika ve saniye cinsinden s�re



void writeEEPROM(unsigned int address, unsigned int data) {
    // EEPROM'a veri yazma i?lemi
    EEADR = address;   // EEPROM adresini ayarla
    EEDATA = data;     // EEPROM verisini ayarla
    WREN = 1;          // EEPROM yazma izni ver
    GIE = 0;           // Kesmeleri devre d??? b?rak
    EECON2 = 0x55;     // 
    EECON2 = 0xAA;     // 
    WR = 1;            // EEPROM yazma i?lemini ba?lat
    GIE = 1;           // Kesmeleri etkinle?tir
    WREN = 0;          // EEPROM yazma iznini kapat
    while (WR == 1);   // Yazma i?lemi tamamlanana kadar bekle
}

unsigned int readEEPROM(unsigned int address) {
    // EEPROM'dan veri okuma i?lemi
    EEADR = address;   // EEPROM adresini ayarla
    RD = 1;            // EEPROM okuma i?lemini ba?lat
    return EEDATA;     // Okunan veriyi d�nd�r
}

void incrementTime(struct Time* time) {
    time->seconds++;  
    if (time->seconds >= 60) {
        time->seconds = 0;
        time->minutes++;
        if (time->minutes >= 60) {
            time->minutes = 0;
            time->hours++;
            
        }
    }
}

// Kesme i�in gerekli tan?mlamalar
void __interrupt() timer_isr(void) {
    if (T0IF) { // Zamanlay?c? 0 kesmesi mi?
        T0IF = 0; // Kesme bayra??n? s?f?rla
        TMR0 = 61; // Zamanlay?c? de?erini ayarla (256 - 250 = 6)
       
        kesmeSayaci++;
        if (kesmeSayaci==20){
            kesmeSayaci = 0 ;
            incrementTime(&currentTime);                
        }        
    }
}

int main()
{
  unsigned int a;
  
  //lcd pinleri
  TRISBbits.TRISB0 = 0; // D7 pini cikis  olarak ayarla
  TRISBbits.TRISB1 = 0; // D6 pini cikis  olarak ayarla
  TRISBbits.TRISB2 = 0; // D5 pini cikis  olarak ayarla
  TRISBbits.TRISB3 = 0; // D4 Pini cikis  olarak ayarla
  TRISBbits.TRISB4 = 0; // en pini cikis  olarak ayarla
  TRISBbits.TRISB5 = 0; // rs pini �ikis  olarak ayarla
  //lcd pinleri biti?
  TRISA4 = 0;           // RA4 BUZZER Pini cikis olarak ayarla
 
 TRISE0 = 1; // sag giris olarak 
 TRISE1 = 1; // sol giris olarak ayarla  Y�N P?NLER?
 TRISC1 = 1; // limit giris 
 TRISC2 = 1; // limit giris
 TRISE2 = 1; //SOL 400HIZ Giris
 TRISC0 = 1; //SAG 400HIZ giris set
 TRISC3 = 1; // dc servo eror pin giris
 TRISD0 = 1; // AC SERVO EROR G?R?S
 TRISA1 = 1; //// RA1 PiNi dc servonun speed nin potu giri? 
    Lcd_Init();
  //uart pinleri
 TRISC6 = 0; // TX Pin set as output
 TRISC7 = 1; // RX Pin set as input

  //buzzer ba?lang?�ta kapat
  __delay_ms(100); //
  RA4 = 0; // Buzz
  //buzzer bitti
   
  // EEPROM'dan kaydedilmi? s�reyi oku
    currentTime.hours = readEEPROM(0x00);
    currentTime.minutes = readEEPROM(0x01);
    currentTime.seconds = readEEPROM(0x02);
    char lcdText[9]; // HH:MM:SS + null karakteri i�in yeterli bo?luk

    OPTION_REGbits.T0CS = 0; // Timer0 i�in dahili osilat�r� kullan
    OPTION_REGbits.PSA = 0;  // Prescaler'? Timer0'a ata
    OPTION_REGbits.PS2 = 1;  // Prescaler b�lme oran?n? 1:256 yap
    OPTION_REGbits.PS1 = 1;
    OPTION_REGbits.PS0 = 1;
  
     // Kesmeleri etkinle?tir
    INTCONbits.GIE = 1;    // Genel kesmeleri etkinle?tir
    INTCONbits.PEIE = 1;   // Periferik kesmeleri etkinle?tir
    INTCONbits.T0IE = 1;   // Timer0 kesmelerini etkinle?tir
//kesme biti?  
  
    Lcd_Clear();
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String("UZMANLAR MAKINA");
    Lcd_Set_Cursor(2,1);
    Lcd_Write_String("www.borvek.com");
    Lcd_Set_Cursor(2,17);
    Lcd_Write_String("V2");
     
    __delay_ms(1000);
    Lcd_Clear();
    
   
// UART ayarlar?
    
    SPBRG = ((_XTAL_FREQ/16)/19200) - 1; // SPBRG de?eri 19200 baud h?z? i�in hesapland?
    UART_Initialize();
unsigned int displayValue = 0; // G�ncel de?eri saklamak i�in bir de?i?ken

//uart bitti

 /// ADC yap?land?rmas?
    ADCON1bits.ADFM = 1; // Sonu�lar sa?dan sola hizalan?r (8-bit sonu� i�in)
    ADCON1bits.PCFG = 0b100; // T�m pinler dijital giri?/�?k?? olarak ayarlan?r, RA1 RA2 RA3 analog kal?r
    ADCON0bits.ADCS = 0b10; // Fosc/32 h?z?nda d�n�?�m (uygun bir de?er se�ebilirsiniz)
    ADCON0bits.ADON = 1; // ADC'yi etkinle?tir

    float adcValue1; // Birinci potansiyometre de?eri
    unsigned int adcValue2; // ?kinci potansiyometre de?eri
    unsigned int adcValue3; // trimpot de?eri
    float rpmtofloat; // Sonucu saklayacak sabit kayan nokta de?i?ken
    // adc bitti
unsigned int value = 0; // Ba?lang?� de?eri
unsigned int FWDartan = 0; 


while(1){
    


if( !DcEror && !AcEror){
     if ((currentTime.hours == 0 || currentTime.hours == 501 || currentTime.hours == 502) && currentTime.minutes == 1) {
    
    Lcd_Clear();
    int i=0 ;
    for(; i < 10; i++)
{
   Lcd_Set_Cursor(1, 1);     
Lcd_Write_String("YAG BAKIMI YAPINIZ");
Lcd_Set_Cursor(2, 1);     
Lcd_Write_String("OIL THE GEARS");
RA4 = 1;
} 
    Lcd_Clear();
 
    RA4 = 0;    
}
   //500 YAG BAKIM UYARISI
if ((currentTime.hours == 500 || currentTime.hours == 501 || currentTime.hours == 502) && currentTime.minutes == 0) {
    
    Lcd_Clear();
    int i=0 ;
    for(; i < 10; i++)
{
   Lcd_Set_Cursor(1, 1);     
Lcd_Write_String("YAG BAKIMI YAPINIZ");
Lcd_Set_Cursor(2, 1);     
Lcd_Write_String("OIL THE GEARS");
RA4 = 1;
} 
    Lcd_Clear();
 
    RA4 = 0;    
}
  //1000 YAG BAKIM UYARISI
if ((currentTime.hours == 1000 || currentTime.hours == 1001 || currentTime.hours == 1002) && currentTime.minutes == 0) {
    
    Lcd_Clear();
    int i=0 ;
    for(; i < 10; i++)
{
   Lcd_Set_Cursor(1, 1);     
Lcd_Write_String("YAG BAKIMI YAPINIZ");
Lcd_Set_Cursor(2, 1);     
Lcd_Write_String("OIL THE GEARS");
RA4 = 1;
} 
    Lcd_Clear();
 
    RA4 = 0;    
}
  //1500 YAG BAKIM UYARISI
if ((currentTime.hours == 1500 || currentTime.hours == 1501 || currentTime.hours == 1502) && currentTime.minutes == 0) {
    
    Lcd_Clear();
    int i=0 ;
    for(; i < 10; i++)
{
   Lcd_Set_Cursor(1, 1);     
Lcd_Write_String("YAG BAKIMI YAPINIZ");
Lcd_Set_Cursor(2, 1);     
Lcd_Write_String("OIL THE GEARS");
RA4 = 1;
} 
    Lcd_Clear();
 
    RA4 = 0;    
}
  //2000 YAG BAKIM UYARISI
if ((currentTime.hours == 2000 || currentTime.hours == 2001 || currentTime.hours == 2002) && currentTime.minutes == 0) {
    
    Lcd_Clear();
    int i=0 ;
    for(; i < 10; i++)
{
   Lcd_Set_Cursor(1, 1);     
Lcd_Write_String("YAG BAKIMI YAPINIZ");
Lcd_Set_Cursor(2, 1);     
Lcd_Write_String("OIL THE GEARS");
RA4 = 1;
} 
    Lcd_Clear();
 
    RA4 = 0;    
}
  //2500 YAG BAKIM UYARISI
if ((currentTime.hours == 2500 || currentTime.hours == 2501 || currentTime.hours == 2502) && currentTime.minutes == 0) {
    
    Lcd_Clear();
    int i=0 ;
    for(; i < 10; i++)
{
   Lcd_Set_Cursor(1, 1);     
Lcd_Write_String("YAG BAKIMI YAPINIZ");
Lcd_Set_Cursor(2, 1);     
Lcd_Write_String("OIL THE GEARS");
RA4 = 1;
} 
    Lcd_Clear();
 
    RA4 = 0;    
}



        // Saati ve dakikay? do?ru formatta birle?tirip lcdText'e yaz
        sprintf(lcdText, "%5uh %02um", currentTime.hours, currentTime.minutes);

        // LCD'ye saati yazd?r
     Lcd_Set_Cursor(1, 10);
     Lcd_Write_String("  ");
     Lcd_Set_Cursor(1, 11);
     Lcd_Write_String(lcdText); // Saat, dakika ve saniyeyi yazd?r
    
   
        // EEPROM'a g�ncel s�reyi yaz
        writeEEPROM(0x00, currentTime.hours);
        writeEEPROM(0x01, currentTime.minutes);
        writeEEPROM(0x02, currentTime.seconds);
        
 // rpm potansiyometre de?eri
    ADCON0bits.CHS = 0b0011; // RA3 pini se�ildi (Analog giri? pininiz)
    ADCON0bits.GO = 1; // ADC d�n�?�m�n� ba?lat
    while (ADCON0bits.GO); // D�n�?�m�n tamamlanmas?n? bekle
    adcValue2 = (ADRESH << 8) | ADRESL;
  
    unsigned int integerPart = adcValue2*3 ; // Tam say? k?sm?
    
    int binler = integerPart/1000;
    int yuzler = (integerPart-binler*1000)/100;
    int onlar  = (integerPart-binler*1000-yuzler*100)/10;
    int birler = integerPart%10;
    
   
    
  // feed rate pot de?eri 
     
        ADCON0bits.CHS = 0b0001; // RA1 pini se�ildi (Analog giri? pininiz)
        __delay_us(50);
        ADCON0bits.GO = 1; // ADC d�n�?�m�n� ba?lat
        while (ADCON0bits.GO); // D�n�?�m�n tamamlanmas?n? bekle
        adcValue1 = (ADRESH << 8) | ADRESL;
        unsigned int displayValue = (int)adcValue1 >> 1; // ADC sonucunu 0-255 aras?na d�n�?t�r
        float gostermeliklcd = adcValue1 / 1024 * 1000;
       
    // ���nc�( kart?n �st�ndeki trimpot) de?eri
    ADCON0bits.CHS = 0b0010; // RA2 pini se�ildi (Analog giri? pininiz)
    ADCON0bits.GO = 1; // ADC d�n�?�m�n� ba?lat
    while (ADCON0bits.GO); // D�n�?�m�n tamamlanmas?n? bekle
    adcValue3 = (ADRESH << 8) | ADRESL;
    unsigned int rpmValue = adcValue3 >> 1; // ADC sonucunu 0-255 aras?na d�n�?t�r
            
// feed rate pot de?erini LCD'ye yazd?r
        Lcd_Set_Cursor(2, 1);
        Lcd_Write_String("FeedRate:");
        char adcString1[4];
        sprintf(adcString1, "%003u", (int)gostermeliklcd);
        Lcd_Set_Cursor(2, 10);
        Lcd_Write_String(adcString1);

// rpm potansiyometre de?erini LCD'ye yazd?r
        Lcd_Set_Cursor(1, 1);
        Lcd_Write_String("RPM:");
        char rpmString[5]; // Virg�lden sonraki 1 basamak i�in 5 hane yeterlidir
sprintf(rpmString, "%u%u%u.%u", binler,yuzler,onlar,birler);
Lcd_Set_Cursor(1, 5);
Lcd_Write_String(rpmString);


            
//y�nler basla

//fwd limit 
 if (FwdLMT == 1 && (FWD==1 || FwdFEAD==1))
        {   
            Lcd_Set_Cursor(2,13 );
            Lcd_Write_String(" FWD LMT");         
            UART_Write_Text("s0\r\n");          
        }
// fwd fead
 else if ((FwdFEAD == 1 && FWD==1) || FwdFEAD==1 && REW==0)
        {
            Lcd_Set_Cursor(2, 13);
            Lcd_Write_String(" FWD 1.6K");  
              if (displayValue < 800) {
            char uartMessage[8];
            
            FWDartan+=80; // De?eri art?r
            if (FWDartan > 800) {
                FWDartan = 800; // De?eri 400 olarak sabitle
            }
             sprintf(uartMessage, "s%d\r\n", FWDartan);
            UART_Write_Text(uartMessage);
            
        }
        }
//fwd        
        else if (FWD == 1 )
        {
            Lcd_Set_Cursor(2, 13);
             Lcd_Write_String(" FWD      ");
             
                char combinedText[20];
        sprintf(combinedText, "s%d\r\n", displayValue);
        UART_Write_Text(combinedText);
    
        }

        // REW KANALI
        else if (RewLMT == 1 && (REW==1 || RewFEAD==1))
        {
//rew limit          
            Lcd_Set_Cursor(2, 13);
            Lcd_Write_String(" REW LMT");         
            UART_Write_Text("s0\r\n");
        }
//rew fead
        else if ((RewFEAD == 1 && REW==1) || (RewFEAD==1 && FWD==0) )
        {
        Lcd_Set_Cursor(2, 13);
        Lcd_Write_String(" REW 1.6K");
        
         if (displayValue < -800) {
            char uartMessage[8];
            FWDartan-=80; // De?eri art?r
            
            if (FWDartan < -800) {
                FWDartan = -800; // De?eri 255 olarak sabitle
            }
            
             sprintf(uartMessage, "s%d\r\n", FWDartan);
            UART_Write_Text(uartMessage);
        
        
            
        }
//rew
    } else if (REW == 1  ) {
        Lcd_Set_Cursor(2, 13);
        Lcd_Write_String(" REW        ");
      
        int negativeDisplayValue = -displayValue;
    char combinedText[20];
    sprintf(combinedText, "s%d\r\n", negativeDisplayValue);
    UART_Write_Text(combinedText);
        
     
    } else {
        FWDartan = 0; // Durumda hi�bir d�?me bas?lmam??sa de?eri s?f?rla
        Lcd_Set_Cursor(2, 13);
        Lcd_Write_String(" STOP        ");
        UART_Write_Text("s0\r\n");
        
    }
    
//y�nler bitii  
         
        //limit d�?melerinde buzzer a� kapa
        if((FwdLMT==1) &&( FWD==1 || FwdFEAD  )){
            
            RA4=1;
        }
        else if(( RewLMT==1) && (REW==1 || RewFEAD)){
            RA4=1;
            }
       
        else {
            RA4=0;
        }
        //limitlerde buzzer kontrolu son
        
  }   
else if( DcEror==1) {
    
         //dc servo error

    
    Lcd_Set_Cursor(1,1);         
            Lcd_Write_String("   FEED RATE ERROR       ");
    Lcd_Set_Cursor(2,1);
            Lcd_Write_String("   ILERLEME HATASI        ");      
     UART_Write_Text("s0\r\n");
      RA4=1;
      __delay_ms(1000);
      RA4=0;
      __delay_ms(3000);
      RA4=1;
    
  
} 
// ac eror
     else if (AcEror == 1 ){
    UART_Write_Text("s0\r\n");
    Lcd_Set_Cursor(1,1);         
            Lcd_Write_String("   ROTATION ERROR      ");
    Lcd_Set_Cursor(2,1);
            Lcd_Write_String("   DONME HATASI        ");
                RA4=1;
      __delay_ms(1000);
      RA4=0;
      __delay_ms(3000);
      RA4=1;
}
    }
  return 0;
  
  
}