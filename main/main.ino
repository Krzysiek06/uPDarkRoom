#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

#define E_IDDLE  0
#define E_UP  1
#define E_DW  2
#define E_OK  3
#define E_PREV  4

uint8_t current_menu = 0;
uint8_t menu_event = E_IDDLE;

const char START1[] PROGMEM = { "Wci" "\x82" "nij ok" };
const char START2[] PROGMEM = { "\x83" "eby" " zacz" "\x80" "\x81" };

typedef struct {
  uint8_t next_state[5];            //przejścia do następnych stanów
  void (*callback)(void);           //funkcja zwrotna
  const char *first_line;           //tekst dla 1. linii LCD
  const char *second_line;          //tekst dla 2. linii LCD
} menu_item;

const menu_item menu[] = {
        //  LP UP DN OK PREV
        { { 0, 0, 0, 1, 0 },      NULL          , START1  , START2  },
            };
            
void change_menu() {
  //przejdz do nastepnego stanu
  if (menu_event < 5) {
    current_menu = menu[current_menu].next_state[menu_event];

    //wyswietl komunikaty
    lcd.clear();

    //wyświetl 1-szą linię
    if (menu[current_menu].first_line) {
      lcd.setCursor(0, 0);
      lcd.print((char*) menu[current_menu].first_line);
    }

    //wyświetl 2-gą linię
    if (menu[current_menu].second_line) {
      lcd.setCursor(1, 0);
      lcd.print((char*) menu[current_menu].second_line);
    }

    //wywolaj funkcje zwrotna
    if (menu[current_menu].callback)
      menu[current_menu].callback();

    //skasuj zdarzenie

    menu_event = 0;
  }
}

            void read_key(void) {
  if (((value > 310) && (value < 350)) && (!key_lock)) {  //wciśnięty S1
    menu_event = E_UP;
    key_lock = 1;
  } else if ((value > 550) && (value < 590) && (!key_lock)) { //wciśnięty S2
    menu_event = E_DW;
    key_lock = 1;
  } else if ((value > 730) && (value < 770) && (!key_lock)) { //wciśnięty S3
    menu_event = E_OK;
    key_lock = 1;
  } else if ((value > 870) && (value < 920) && (!key_lock)) { //naciśnięty S4
    menu_event = E_PREV;
    key_lock = 1;
  } else if (((value > 1000) && key_lock)) { //żaden przycisk nie jest naciśnięty
    key_lock = 0;
    menu_event = E_IDDLE;
  }
}

void setup() {
  // put your setup code here, to run once:
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("DarkRoom!");
}

void loop() {
    read_key();
    if (menu_event) {
      change_menu();
    }

}
