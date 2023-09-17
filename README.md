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
Bu pot ise 2.potun üst limitini kısmak için eklenmiştir. fiziksel olarak potun değerini sadece LCD ekranda kısıyoruz.


ÖZELLİKLER:

-Sistem ilk açıldığında yön anahtarlarından herhangi biri yönlü ise sistem başlamaz ve 0 konumuna getirilmesi istenir.

-Çalışma saati eklendi.
-500 saat ve katlarında Yağ bakımı uyarısı eklendi.
