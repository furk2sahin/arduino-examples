class Button{ // Butonlarımızım pin değerini ve basılıp 
          //basılmadığı değerlerini tutacak bir sınıf tanımlıyoruz.
  private:
      int pin; // Butonumuzun pin değerini int olarak tuttuk.
      bool pressed; // butona basılıp basılmadığını boolean olarak tuttuk.
  
    public:
      Button(int pinNumber, bool isPressed){ // Bu sınıftan nesne üretebilmemiz için başlatıcı constructor.
          pin = pinNumber;
          pressed = isPressed;
        }; 
  
      int getPin(){ // private olan pin değerini alabilmek için public metot yazdık.
            return pin; // pin değerimiz dönüyor
        };
  
        bool isPressed(){ // private olan basıldı değerini alabilmek için public metot yazdık.
            return pressed; // pressed değerimiz dönüyor.
        };
  
        void setPressed(bool press){
          pressed = press;
        };
};

//////////////////////////////////////////////////
int a = 7, b = 8, c = 2, d = 3, e = 4, f = 6, g = 5; // display pinlerinin bağlı olduğu pinler
int displayPins[] = {a, b, c, d, e, f, g}; // display pin dizisi

Button button1(11, false); // arttirma butonu 11. pine bağlı. başlangıçta basilmamış olarak gelcek. false
Button button2(12, false); // azaltma butonu 12. pine bağlı. başlangıçta basilmamış olarak gelcek. false
Button button3(13, true); // baslatma durdurma butonu 13. pine bağlı. başlangıçta displayler calısır olarak gelcek. true

Button buttons[] = {button1, button2, button3}; // buton dizisi olusturduk

int birlerTransistor = 9; // birler basamağı transistorumuz 9. pine bağlı
int onlarTransistor = 10; // onlar basamağı transistorumuz 10. pine bağlı

byte numbers [10][8] = { // rakamlar
  // a  b  c  d  e  f  g
    {1, 1, 1, 1, 1, 1, 0}, // 0
    {0, 1, 1, 0, 0, 0, 0}, // 1
    {1, 1, 0, 1, 1, 0, 1}, // 2
    {1, 1, 1, 1, 0, 0, 1}, // 3
    {0, 1, 1, 0, 0, 1, 1}, // 4
    {1, 0, 1, 1, 0, 1, 1}, // 5
    {1, 0, 1, 1, 1, 1, 1}, // 6
    {1, 1, 1, 0, 0, 0, 0}, // 7
    {1, 1, 1, 1, 1, 1, 1}, // 8
    {1, 1, 1, 1, 0, 1, 1} // 9
};

float sayac = 0.0; // sayacımız 0 olarak başladı
int eskiPotansDegeri = 0; // hiz potansiyometremizde değişiklik var mı diye kontrol edeceğiz
bool tekSeferlikDegerAl = true; // hiz potansiyometrenin tek seferlik değerini alacağız. Çünkü 0 dan farklı olack
int tolerans = 10; // potansiyometredeki oynama hemen algılanmasın istiyoruz bunun için tolerans değeri verdik.

boolean enablePotans = false; // başlangıçta yön potansiyometresi çalışmayacak
int pulldown=1; // butonlara tek basışta birden fazla kez basıyomuş gibi algılamasını önleme
//////////////////////////////////////////////////

void setup()
{
  Serial.begin(9600); // Seri monitörü kullanabilmek için başlattık
  for(int pin : displayPins){ // display pinlerimiz output olarak tanımlanıyor.
    pinMode(pin, OUTPUT);
  }
  
  for(Button button : buttons){// buton pinlerimiz input olarak tanımlanıyor.
    pinMode(button.getPin(), INPUT);
  }
  
  pinMode(birlerTransistor, OUTPUT);// transistor pinlerimiz output olarak tanımlanıyor.
  pinMode(onlarTransistor, OUTPUT);// transistor pinlerimiz output olarak tanımlanıyor.
}

void loop()
{  
    Serial.println(sayac); // Seri monitore sayac değerini yazdırıyoruz
    int onlar = (int)sayac / 10; // onlar basamağını bulmak için sayacımızı 10'a böldük
    int birler = (int)sayac % 10; // birler basamağını bulmak için sayacımızın 10'a modunu aldık.
  
    digitalWrite(onlarTransistor, LOW); // displaylere gönderceğimiz değer birler basamağı olduğundan,
                    // onlar basamağı transistoru LOW
    digitalWrite(birlerTransistor, HIGH);// displaylere gönderceğimiz değer birler basamağı olduğundan,
                    // birler basamağı transistoru HIGH
    displayYak(birler); // birler basamağı displayini yakacak
  delay(5); // 5 saniyelik beklemenin ardından
    clearSegment(); // display değerleri temizlenecek
    
    digitalWrite(birlerTransistor, LOW);// displaylere gönderceğimiz değer onlar basamağı olduğundan,
                    // birler basamağı transistoru LOW
    digitalWrite(onlarTransistor, HIGH);// displaylere gönderceğimiz değer onlar basamağı olduğundan,
                    // onlar basamağı transistoru HIGH
    displayYak(onlar);// onlar basamağı displayini yakacak
  delay(5); // 5 saniyelik beklemenin ardından
    clearSegment(); // display değerleri temizlenecek
 
    
    float hiz = (float)analogRead(0)/5000.0; // sayacin artis-azalis hizi 0 pinine bağlı potansiyometre ile
    // kontrol edilecektir. Burada 5000'e bölmemizin sebebi hızının çok fazla olmasını istemiyoruz.
    buttonControl(hiz); // buttonControl adlı metodumuzu hiz değerimizi göndererek çağırıyoruz.
    potansControl(analogRead(1), hiz); // potansControl adlı metodumuzu 1 pinine bağlı potansiyometre değeri
                    // ve hiz değerimizi göndererek çağırıyoruz.
}

void displayYak(int deger){ // birler veya onlar olarak gönderdiğimiz rakamı deger olarak aldık.
  for(int i = 0; i < sizeof(displayPins)/sizeof(int); i++){
    digitalWrite(displayPins[i], numbers[deger][i]); // numbers[deger], degerimize bağlı olarak,
    // numbers dizisindeki 1 ve 0 ları getirecek. eğer 1 ise displayPins[i] değeri yanacak.
  }
}

void clearSegment(){
  for(int pin : displayPins){
    digitalWrite(pin, LOW); // tüm display pinlerinin değerini LOW'a çektik.
  }
}

void buttonControl(float hiz){
   if(digitalRead(button1.getPin())){ // birinci butona basilmissa
      button1.setPressed(true); // birinci butonumuzun pressed özelliğini true yapacağız.
      button2.setPressed(false); // ikinci butonumuzun gerçekleştirdiği işlemi iptal edeceğiz.
      enablePotans = false; // buton kullanıldığı için yön potansiyometresi devre dışı.
  } 
  if(digitalRead(button2.getPin())){// ikinci butona basilmissa
      button2.setPressed(true); // ikinci butonumuzun pressed özelliğini true yapacağız.
      button1.setPressed(false); // birinci butonumuzun gerçekleştirdiği işlemi iptal edeceğiz.
      enablePotans = false; // buton kullanıldığı için yön potansiyometresi devre dışı.
  }
  if(digitalRead(button3.getPin()) && pulldown == 0){ // üçüncü butona basilmissa. 
    //burada pulldown birden fazla kez basilmis gibi sayılmasın diye koyduk. Çünkü toggle yapıyoruz.
    button3.setPressed(!button3.isPressed()); // başlatma durdurma toggle işlemi
  }
  pulldown = digitalRead(button3.getPin()); // pulldown değerini güncelledik. butona basilmissa 1 olcak
  ////////////////////////////////
  if(button1.isPressed() && button3.isPressed() && !enablePotans){ // birinci butona basilmis ve
          // üçüncü buton durdurulmamışsa ve potansiyometre kullanılmamaktaysa calisacak
    if(sayac > 99.9) // sayac 99.9 değerinden büyükse 0'a geri dönecek ve baştan baslayacak
      sayac = 0; // sayaci sifirladik.
    sayac += hiz; // sayacimizi gelen hiz değeri kadar arttirdik.
  } else if(button2.isPressed() && button3.isPressed() && !enablePotans){// birinci butona basilmis ve
          // üçüncü buton durdurulmamışsa ve potansiyometre kullanılmamaktaysa calisacak
    if(sayac < 0)// sayac 0 değerinden küçükse 99.9'a geri dönecek ve tekrar edecek
      sayac = 99.9;// sayacin değerini 99.9'a çektik.
    sayac -= hiz;// sayacimizi gelen hiz değeri kadar azalttik.
  }
}

void potansControl(int val, float hiz){
    val = map(val, 0, 1023, 0, 255); // potansiyometreler normalde 0 - 1023 arası değer alır.
                    // Bu değeri mapleyerek 0 - 255 arasına cektik.
    if(tekSeferlikDegerAl){ // program ilk defa calistiğinde potansiyometrenin değerini alacağız
      eskiPotansDegeri = val; // gelen potansiyometreyi eski potans değeri olarak atadık.
      tekSeferlikDegerAl = false; // program ilk defa calismasini bitirdi.
    }
    int fark = abs(val - eskiPotansDegeri); // yeni potans değerimizden eskisini cikardik.
                        // böylelikle potansiyometrede oynama var mı bakacağız
    if(fark > tolerans){ // eğer potansiyometrede oynama varsa ve bu fark toleransımızdan büyükse
      eskiPotansDegeri = val; // eskiPotansiyometre değerimizi güncelledik.
      enablePotans = true; // potansiyometreyi aktif hale getirdk
    }
    if(val < 128 && enablePotans && button3.isPressed()){ // potansiyometre değerimiz 128'den küçükse
                        // ve potansiyometre calisiyorsa ve üçüncü buton aktifse
       if(sayac > 99.9) // sayac 99.9 değerinden büyükse 0'a geri dönecek ve baştan baslayacak
          sayac = 0; // sayaci sifirladik.
      sayac += hiz; // sayacimizi gelen hiz değeri kadar arttirdik.
    } else if(val > 127 && enablePotans && button3.isPressed()){// potansiyometre değerimiz 127'den büyükse
                        // ve potansiyometre calisiyorsa ve üçüncü buton aktifse
        if(sayac < 0)// sayac 0 değerinden küçükse 99.9'a geri dönecek ve tekrar edecek
          sayac = 99.9;// sayacin değerini 99.9'a çektik.
      sayac -= hiz;// sayacimizi gelen hiz değeri kadar azalttik.
    }
}
