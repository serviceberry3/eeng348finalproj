#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <TinyGPSPlus.h>

SoftwareSerial HC12(10, 11); //HC-12 TX Pin, HC-12 RX Pin
LiquidCrystal_I2C lcd(0x27,20,4);

#define push1 8
#define push2 7

static const int RXPin = 4, TXPin = 3; //rxpin should connect to pin labeled tx on gps module, and vice versa
static const uint32_t GPSBaud = 9600; //GPS data rate

uint32_t satellite_ctr = 0;

String lat = "INVALID";
String lon = "INVALID";

//The TinyGPSPlus object
TinyGPSPlus gps;

//The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

void displayInfo()
{
  if (gps.location.isValid())
  {
    lat = String(gps.location.lat(), 6);
    lon = String(gps.location.lng(), 6);
    Serial.print("Location: ");
    Serial.print(gps.location.lat(), 6);
    Serial.print(",");
    Serial.print(gps.location.lng(), 6);
  }
  else
  {
    lat = "INVALID";
    lon = "INVALID";
    Serial.print("Location: ");
    Serial.print("INVALID");
  }

  Serial.print("  Date/Time: ");
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print("/");
    Serial.print(gps.date.day());
    Serial.print("/");
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print("INVALID");
  }

  Serial.print(" ");
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print("0");
    Serial.print(gps.time.hour());
    Serial.print(":");
    if (gps.time.minute() < 10) Serial.print("0");
    Serial.print(gps.time.minute());
    Serial.print(":");
    if (gps.time.second() < 10) Serial.print("0");
    Serial.print(gps.time.second());
    Serial.print(".");
    if (gps.time.centisecond() < 10) Serial.print("0");
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print("INVALID");
  }

  Serial.println();
}

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
const unsigned long time_threshold = 250;

long debounce_delay = 100;
long start_debounce_time = 0;

static unsigned long last_interrupt_time = 0;

static int last_state1, last_state2;

boolean ignore_morse_button = false;
  

void setup() {
  Serial.begin(9600);
  HC12.begin(9600); 

  //button 1 (for the morse)
  pinMode(push1, INPUT);

  pinMode(push2, INPUT);
 
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);

  digitalWrite(A3, HIGH);
  digitalWrite(A2, LOW);
  
  lcd.init();                    
  lcd.backlight();

  ss.begin(GPSBaud);
}

void loop() {
  int state1, state2;
  static float start_time1, end_time1, time1;

  //read in the pushbutton vals
  state1 = digitalRead(push1);
  state2 = digitalRead(push2);

  //only allow if debounce delay has passed
  if (millis() > start_debounce_time + debounce_delay) {
    start_debounce_time = millis();

    //the morse button is being pressed down
    if (state1 == HIGH && last_state1 == LOW) {
      start_time1 = millis();
    }
  
    //the morse button is being released
    else if (state1 == LOW && last_state1 == HIGH) {
      end_time1 = millis();
      time1 = end_time1 - start_time1;

      if (ignore_morse_button == true) {
        ignore_morse_button = false;
      }
      else {
        //determine whether was dash or dot based on how long button held down
        if (time1 < time_threshold) {
          HC12.write(".");
          morse_letter += ".";
          lcd.print(".");
        }
       
        else {
           HC12.write("-");
           morse_letter += "-";
           lcd.print("-");
        }
      }
    }
  
    //the end signal is being given
    if (state2 == HIGH && last_state2 == LOW) {
      //if backspace request
      if (state1 == HIGH && morse_letter != "") {
        morse_letter.remove(morse_letter.length() - 1);
        //Serial.println(morse_letter);
        lcd.clear();
        lcd.print(message);
        lcd.print(morse_letter);
        ignore_morse_button = true;

        //write a backspace signal to the receiver
        HC12.write(2);
      }
      else {
        //write a 1 to signal end of letter
        HC12.write(1);
  
        //send latitude
        for (int i = 0; i < lat.length(); i++) {
          HC12.write(lat[i]);
        }
        HC12.write(1);
  
        //send longitude
        for (int i = 0; i < lon.length(); i++) {
          HC12.write(lon[i]);
        }
        HC12.write(1);
  
        Serial.print("Sending letter: ");
        Serial.print(morse_letter);
  
        char letter = morse_to_letter(morse_letter);
  
        Serial.print(" = ");
        
        if (letter != 0) {
          Serial.println(letter);
          message += letter;
        }
        else {
          Serial.println("UNKNOWN");
        }
  
        lcd.clear();
        lcd.print(message);
        
        //clear letter
        morse_letter = "";
    
        //delay half a second
        delay(delay_time);
      }
    }
    last_state2 = state2;
    last_state1 = state1;
  }


  //GPS CODE
  if (++satellite_ctr == 100000) {
      Serial.print("Num satellites: ");
      Serial.println(gps.satellites.value());
  
      while (ss.available() > 0) {
        if (gps.encode(ss.read())) {
          displayInfo();
        }
        else {
          //Serial.println("gps encode() fail");
        }
    }
      satellite_ctr = 0;
  }
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
  return 0;
}
