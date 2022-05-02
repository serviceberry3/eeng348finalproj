#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

SoftwareSerial HC12(10, 11); //HC-12 TX Pin, HC-12 RX Pin
int LED = 3;
LiquidCrystal_I2C lcd(0x27,20,4);

enum state {
  LETTER,
  LAT,
  LON
};

const String A = ".-", B = "-...", C = "-.-.", D = "-..",
             E = ".", F = "..-.", G = "--.", H = "....",
             I = "..", J = ".---", K = "-.-", L = ".-..",
             M = "--", N = "-.", O = "---", P = ".--.",
             Q = "--.-", R = ".-.", S = "...", T = "-",
             U = "..-", V = "...-", W = ".--", X = "-..-",
             Y = "-.--", Z = "--..";
             
String morse_letter = "";         
int delay_time = 500;
const unsigned long time_threshold = 250;
int debounce_delay = 50;

String lat = "";
String lon = "";

state reader = LETTER;

void setup() {
  Serial.begin(9600);
  HC12.begin(9600); 
  lcd.init();              
  lcd.backlight();
}

void loop() { 
  while (HC12.available()) {
    char val = HC12.read();

    switch(reader) {
      //currently in morse letter reading state
      case LETTER:
        //a zero signifies the end of a letter
        if (int(val) == 1) {
          Serial.print("Receiving letter: ");
          char letter = morse_to_letter(morse_letter);
          
          if (letter != 0) {
            lcd.print(letter);
            Serial.println(letter);
          }
          else {
            Serial.println("UNKNOWN");
          }
            
          morse_letter = "";

          //should now be in latitude reading state
          reader = LAT;
        }
        else if (int(val) == 2 && morse_letter != "") {
          morse_letter.remove(morse_letter.length() - 1);
        }
        //otherwise we're reading the letter, so append dash or dot to the buffer string for current letter being read
        else {
          morse_letter += val;
        }
        break;

      //currently in location reading state
      case LAT:
        //we've read in the location, so print it to display
        if (int(val) == 1) {
          Serial.print("Received lat: ");
          Serial.println(lat);
          reader = LON;
          lat = "";
        }
        else {
          lat += val;
        }
        break;
      case LON:
        //we've read in the location, so print it to display
        if (int(val) == 1) {
          Serial.print("Received lon: ");
          Serial.println(lon);
          reader = LETTER;
          lon = "";
        }
        else {
          lon += val;
        }
        break;
    }
  }  
}


char morse_to_letter(String letter)
{
  if (letter == A)      return 'A';
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
