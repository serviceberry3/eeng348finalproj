#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

SoftwareSerial HC12(10, 11); // HC-12 TX Pin, HC-12 RX Pin
int LED = 3;
LiquidCrystal_I2C lcd(0x27,20,4);

//#define push1 8
//#define push2 7
const String A = ".-", B = "-...", C = "-.-.", D = "-..",
             E = ".", F = "..-.", G = "--.", H = "....",
             I = "..", J = ".---", K = "-.-", L = ".-..",
             M = "--", N = "-.", O = "---", P = ".--.",
             Q = "--.-", R = ".-.", S = "...", T = "-",
             U = "..-", V = "...-", W = ".--", X = "-..-",
             Y = "-.--", Z = "--..";
String morse_letter = "";
String message = "";             
int delay_time = 500;
//void morse_dot;
//void morse_dash;
const unsigned long time_threshold = 250;
//char space = "/";
int debounce_delay = 50;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  HC12.begin(9600); 
  //pinMode(push1, INPUT);
  //pinMode(push2, INPUT);
  pinMode(LED,OUTPUT);
  //lcd.init();                      // initialize the lcd
  //lcd.backlight();
  Serial.println("Start");
  
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED, HIGH);
  //Serial.println("1");
 
  while (HC12.available()) {        // If HC-12 has data
    //Serial.println("2");
    char val = HC12.read();
    Serial.println(val);
    Serial.println(int(val));
    morse_letter += val;
    //analogWrite(LED,val);
    if (int(val)==1){
      Serial.print("Sending");
      Serial.println(morse_letter);
      message += morse_to_letter(morse_letter);
      Serial.println(message);
      morse_letter = "";
    }
  }
/*
  int state1, state2;
  static int last_state1, last_state2;
  static float start_time1, end_time1, time1;
  state1 = digitalRead(push1);
  state2 = digitalRead(push2);
 
  if(state1 == HIGH && last_state1 == LOW){
    start_time1 = millis();
  }
  else if(state1 == LOW && last_state1 == HIGH){
    end_time1 = millis();
    time1 = end_time1 - start_time1;
    if (time1 < time_threshold){
      morse_letter += ".";
      Serial.println(morse_letter);
    }
   
    else {
      morse_letter += "-";
      Serial.println(morse_letter);
    }
  }
  if (state2 == HIGH){
    message += morse_to_letter(morse_letter);
    Serial.println(morse_letter);
    Serial.println(message);
    morse_letter = "";
    delay(delay_time);
    lcd.clear();
    lcd.print(message);
  }
  last_state1 = state1;
  last_state2 = state2;

  */
  
}


char morse_to_letter(String letter)
{
  if(letter == A)      return 'A';
  else if(letter == B) return 'B';
  else if(letter == C) return 'C';
  else if(letter == D) return 'D';
  else if(letter == E) return 'E';
  else if(letter == F) return 'F';
  else if(letter == G) return 'G';
  else if(letter == H) return 'H';
  else if(letter == I) return 'I';
  else if(letter == J) return 'J';
  else if(letter == K) return 'K';
  else if(letter == L) return 'L';
  else if(letter == M) return 'M';
  else if(letter == N) return 'N';
  else if(letter == O) return 'O';
  else if(letter == P) return 'P';
  else if(letter == Q) return 'Q';
  else if(letter == R) return 'R';
  else if(letter == S) return 'S';
  else if(letter == T) return 'T';
  else if(letter == U) return 'U';
  else if(letter == V) return 'V';
  else if(letter == W) return 'W';
  else if(letter == X) return 'X';
  else if(letter == Y) return 'Y';
  else if(letter == Z) return 'Z';
  return ' ';
}
