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

int scroll_ctr = 0;

state reader = LETTER;

String msg = "";

int which_data_to_disp = 0;

long data_switch_timer = 0;

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
            msg += letter;
            Serial.println(letter);
          }
          else {
            Serial.println("UNKNOWN");
          }
            
          morse_letter = "";

          //should now be in latitude reading state
          lat = "";
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
          lon = "";
          reader = LON;
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
        }
        else {
          lon += val;
        }
        break;
    }
  }  


  if (++data_switch_timer > 5) {
    which_data_to_disp = 1 - which_data_to_disp;
    setup_data_labels(which_data_to_disp);
    data_switch_timer = 0;
  }

  display_data(which_data_to_disp);
  delay(600);
}

void setup_data_labels(int which) {
  lcd.clear();

  if (which == 0) {
    lcd.print("MSG:"); 
  }
  else {
    lcd.print("LAT: ");
    lcd.setCursor(0, 1);
    lcd.print("LON: ");
  }
}


void display_data(int which) {
  if (which == 0) {
    display_msg();
  }
  else {
    display_loc();
  }
}


void display_msg() {
  //check if scrolling is necessary
  if (msg.length() + 4 > 16) {
      lcd.scrollDisplayLeft();
      if (++scroll_ctr == msg.length() + 12) {
        scroll_ctr = 0;
        lcd.clear();
      }
  }
  lcd.setCursor(5, 0);
  lcd.print(msg);
}

void display_loc() {
  lcd.setCursor(5, 0);
  lcd.print(lat);
  lcd.setCursor(5, 1);
  lcd.print(lon);
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
