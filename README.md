# HiK_Control_Project-V3.X

Bu yazılım USART ile aldığı verilere göre Dc servo motoru yöneten bir kart için yapılmıştır.

Çalışma Mantığı :
Sistemde 3 adet POT vardır. 
1.POT: Dc Servo motorumuz içindir.
  Bu pottan gelen veriyi ADC işlemi ile sayısallaştırıp sürücü motorumuz için gerekli olan "s" harfi ile birleştirip USART ile sürücü yazılımına gönderir.
  Bununla birlikte bu sayısal değeri LCD ekrana basar.
  Sistemdeki 2 adet yön anahtarından Dc servonun anahtarında hangi yön varsa o yönü ekrana basar ve 
  eğer eğer yön geri ise 1.pottan gelen veriyi negatif hale getirerek usart ile gönderir.

2.POT: RPM Potu
AC servo motorumuzun hızını bu pot ile kontrol eder ve hızı ekrana ondalık 1 basamak yazabilecek hassasiyette yazdırız.

3. pot: Sınır
Bu pot ise 2.potun üst limitini kısmak için eklenmiştir. 2.Potun değerini bu potun oranı ile sadece LCD ekranda sanal  kısıyoruz.


ÖZELLİKLER:

-Sistem ilk açıldığında yön anahtarlarından herhangi biri yönlü ise sistem başlamaz ve 0 konumuna getirilmesi istenir.

-Çalışma saati eklendi.("eprom en fazla 8 bit değer tutabildiği için
çalışma saati 255in üstüne çıktığında epromda farklı bir alanda carpan değişkeni tutularak çalışma saati hesaplanır.")

-500 saat ve katlarında Yağ bakımı uyarısı eklendi.

-Feed moduna yumuşak kalkış eklendi.
![image](https://github.com/user-attachments/assets/c66f460f-2a65-4f9c-9b3e-feff6e86b78f)
![DSC_4383](https://github.com/user-attachments/assets/c2d5f420-e4cc-4beb-8092-1bf44a23f84d)
![DSC_4382](https://github.com/user-attachments/assets/1b3aee57-de90-4efc-a79b-0dda7d50196e)
![DSC_4385](https://github.com/user-attachments/assets/7cef709e-e5b5-44f4-a6ec-d29383591938)



