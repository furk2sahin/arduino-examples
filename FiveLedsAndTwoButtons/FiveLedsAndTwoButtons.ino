class Led{ // Ledlerimizi pin isimli değişkeni olan bir class olarak tanımladık.
  private:
      int pin; // pin adlı değişken tanımladık. integer değer tutacak
    
    public:
    Led(int pinNumber){ // Bu sınıftan nesne üretebilmemiz için başlatıcı constructor.
          pin = pinNumber;
        }; 
    
        int getPin(){ // private olan pin değerini alabilmek için public metot yazdık.
            return pin; // pin değerimiz dönüyor
        };
};

class Button{ // Butonlarımızım pin değerini ve basılıp 
          //basılmadığı değerlerini tutacak bir sınıf tanımlıyoruz.
  private:
      int pin; // Butonumuzun pin değerini int olarak tuttuk.
      bool pressed = false; // butona basılıp basılmadığını boolean olarak tuttuk.
  
    public:
      Button(int pinNumber){ // Bu sınıftan nesne üretebilmemiz için başlatıcı constructor.
          pin = pinNumber;
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

Led led1(13);
Led led2(12);
Led led3(11);
Led led4(10);
Led led5(9);
Led leds[] = {led1, led2, led3, led4, led5};

Button button1(4);
Button button2(3);
Button button3(2);
Button buttons[] = {button1, button2, button3};

void setup()
{
  for(Button button : buttons){
    pinMode(button.getPin(), INPUT);
  }
  
  for(Led led : leds){
    pinMode(led.getPin(), OUTPUT);
  }
}

void loop()
{
  if(digitalRead(button1.getPin()) && !button2.isPressed() && !button3.isPressed()){
    button1.setPressed(true);
  } else if(digitalRead(button2.getPin()) && !button1.isPressed() && !button3.isPressed()){
    button2.setPressed(true);
  } else if(digitalRead(button3.getPin()) && !button1.isPressed() && !button2.isPressed()){
    button3.setPressed(true);
  }
  
  if(button1.isPressed()){
    
    for(Led led : leds){
      digitalWrite(led.getPin(), HIGH);
        delay(500);
    }
    button1.setPressed(false);
    
  } else if(button2.isPressed()){
    
    for(int i = sizeof(leds)/sizeof(int) - 1; i >= 0; i--){
      digitalWrite(leds[i].getPin(), HIGH);
      delay(500);
    }
    button2.setPressed(false);
    
  } else if(button3.isPressed()){
      
      for(int i = 0; i < 10; i++){
          for(Led led : leds){
            digitalWrite(led.getPin(), HIGH); 
          }
          delay(500);
          for(Led led : leds){ // tüm ledler sönsün
              digitalWrite(led.getPin(), LOW);
          }
        delay(500);
       }
    button3.setPressed(false);
  }
  for(Led led : leds){ // tüm ledler sönsün
      digitalWrite(led.getPin(), LOW);
   }
}
