#include "Keypad.h"
#include <stdint.h>
#include <Arduino.h>
#include <BasicEncoder.h>
#include "HID-Project.h"
#include "lcdgfx.h"

uint8_t MODE = 0;
uint8_t MODE_MAX = 5;
uint8_t last_mode = MODE;

const uint8_t F3603D_MODE = 0;
const uint8_t F360SKETCH_MODE = 1;
const uint8_t YT_MODE = 2;
const uint8_t MOUSE_MODE = 3;
const uint8_t GIT_MODE = 4;
const uint8_t TERM_MODE = 5;

const int8_t pinA = 16;
const int8_t pinB = 10;
int8_t cur_enc01 = 0;
int8_t prev_enc01 = 0;
int8_t dir_enc01 = 0;
int8_t mouse_speed = 10;

const byte ROWS = 4;
const byte COLS = 3;
uint8_t rowPins[ROWS] = {A1, A0, 15, 14}; //frist row is connect to pin 10, second to 9...
uint8_t colPins[COLS] = {7, 8, 9}; //frist column is connect to pin 6, second to 5...
char keys[ROWS][COLS] = {
  {'1','4','7'},
  {'X','2','5'},
  {'8','X','3'},
  {'6','9','0'},
};
String msg;
bool show_display = true;

Keypad kpd = Keypad(makeKeymap(keys), colPins, rowPins, COLS, ROWS); //swap rows and cols, library bug?
BasicEncoder encoder(pinA, pinB);
DisplaySSD1306_128x32_I2C display(-1); // or (-1,{busId, addr, scl, sda, frequency})

void setup() {
	Serial.begin(9600);
  Keyboard.begin();
  Mouse.begin();
  display.begin();
  display.setFixedFont(ssd1306xled_font8x16);

  update_display();
}

void loop() {
  keyboard_loop();
  encoder_loop();

  if (MODE != last_mode) {
    last_mode = MODE;
    update_display();
  }
}

void keyboard_loop() {
  if (kpd.getKeys()) {
    for (int i=0; i<LIST_MAX; i++) {  // Scan the whole key list.
      if (kpd.key[i].stateChanged) {  // Only find keys that have changed state.
        switch (kpd.key[i].kstate) {  // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
          case PRESSED:
            msg = " PRESSED.";
            keyproc(kpd.key[i].kchar);
            break;
          case HOLD:
            msg = " HOLD.";
            break;
          case RELEASED:
            msg = " RELEASED.";
            break;
          case IDLE:
            msg = " IDLE.";
        } // end switch

        Serial.print("Key ");
        Serial.print(kpd.key[i].kchar);
        Serial.println(msg);
      } // end if stateChanged
    } // end for
  } // end if getKeys
} // end keyboard loop

void keyproc(char kcode) {
  if (kcode == '0') {
    MODE++;
    if (MODE > MODE_MAX) {
      MODE = 0;
    }
    Serial.print("MODE: ");
    Serial.println(MODE);
  }

  proc_mode(kcode);

}

void proc_mode(char kcode) {
  switch (MODE) {
    case F3603D_MODE:
      keyproc_f360_3d(kcode);
      break;
    case F360SKETCH_MODE:
      keyproc_f360_sketch(kcode);
      break;
    case YT_MODE:
      keyproc_yt(kcode);
      break;
    case MOUSE_MODE:
      keyproc_mouse(kcode);
      break;
    case GIT_MODE:
      break;
    case TERM_MODE:
      break;
    default:
      Serial.println("Not implemented mode");
      break;
  }
}


void update_display() {
  display.clear();

  switch (MODE) {
    case F3603D_MODE:
      display.printFixed(0, 0, "f360 3d", STYLE_NORMAL);
      break;
    case F360SKETCH_MODE:
      display.printFixed(0, 0, "f360 sketch", STYLE_NORMAL);
      break;
    case YT_MODE:
      display.printFixed(0, 0, "youtube", STYLE_NORMAL);
      break;
    case MOUSE_MODE:
      display.printFixed(0, 0, "mouse", STYLE_NORMAL);
      break;
    case GIT_MODE:
      display.printFixed(0, 0, "code", STYLE_NORMAL);
      break;
    case TERM_MODE:
      display.printFixed(0, 0, "terminal", STYLE_NORMAL);
      break;
    default:
      Serial.println('display update fail');
  }
}

void keyproc_f360_3d(char kcode) {
  switch (kcode) {
    case '1':
      Keyboard.press(KEY_ESC);
      Keyboard.release(KEY_ESC);
      break;
    case '2':
      Keyboard.press(KEY_BACKSPACE);
      Keyboard.release(KEY_BACKSPACE);
      break;
    case '3':
      Keyboard.press(KEY_ENTER);
      Keyboard.release(KEY_ENTER);
      break;
    case '4':
      Keyboard.print("j");
      break;
    case '5':
      Keyboard.print("q");
      break;
    case '6':
      Keyboard.print("e");
      break;
    case '7':
      Keyboard.print("a");
      break;
    case '8':
      Keyboard.print("h");
      break;
    case '9':
      Keyboard.print("f");
      break;
    default:
      Serial.println("skip button");
      break;
  }
}

void keyproc_f360_sketch(char kcode) {
  switch (kcode) {
    case '1':
      Keyboard.press(KEY_ESC);
      Keyboard.release(KEY_ESC);
      break;
    case '2':
      Keyboard.press(KEY_ENTER);
      Keyboard.release(KEY_ENTER);
      break;
    case '3':
      Keyboard.print("d");
      break;
    case '4':
      Keyboard.print("r");
      break;
    case '5':
      Keyboard.print("l");
      break;
    case '6':
      Keyboard.print("c");
      break;
    case '7':
      Keyboard.print("p");
      break;
    case '8':
      Keyboard.print("o");
      break;
    case '9':
      Keyboard.print("t");
      break;
    default:
      Serial.println("skip button");
      break;
  }
}

void keyproc_yt(char kcode) {
  switch (kcode) {
    case '1':
      Keyboard.press(KEY_ESC);
      Keyboard.release(KEY_ESC);
      break;
    case '2':
      Keyboard.print("m");
      break;
    case '3':
      Keyboard.print("c");
      break;
    case '4':
      Keyboard.print("j");
      break;
    case '5':
      Keyboard.print("k");
      break;
    case '6':
      Keyboard.print("l");
      break;
    case '7':
      Keyboard.print("i");
      break;
    case '8':
      Keyboard.print("f");
      break;
    case '9':
      Keyboard.print("t");
      break;
    default:
      Serial.println("skip button");
      break;
  }
}

void keyproc_mouse(char kcode) {
  switch (kcode) {
    case '1':
      Mouse.click(MOUSE_LEFT);
      break;
    case '2':
      Mouse.click(MOUSE_MIDDLE);
      break;
    case '3':
      Mouse.click(MOUSE_RIGHT);
      break;
    case '4':
      Mouse.move(0, 0, mouse_speed);
      break;
    case '5':
      Mouse.move(0, -mouse_speed, 0);
      break;
    case '6':
      Mouse.move(0, 0, -mouse_speed);
      break;
    case '7':
      Mouse.move(-mouse_speed, 0, 0);
      break;
    case '8':
      Mouse.move(0, mouse_speed, 0);
      break;
    case '9':
      Mouse.move(mouse_speed, 0, 0);
      break;
    default:
      Serial.println("skip button");
      break;
  }
}

void encoder_loop() {
  encoder.service();
  int encoder_change = encoder.get_change();

  if (encoder_change) {
    cur_enc01 = encoder.get_count();
    Serial.println(cur_enc01);

    if (cur_enc01 > prev_enc01) {
      dir_enc01 = 1;
    } else if (cur_enc01 < prev_enc01) {
      dir_enc01 = -1; 
    }

    switch (MODE) {
      case F3603D_MODE:
        if (dir_enc01 > 0) {
          Mouse.move(0, 0, -1);
        } else {
          Mouse.move(0, 0, 1);
        }
        break;
      case F360SKETCH_MODE:
        if (dir_enc01 > 0) {
          Keyboard.press(KEY_LEFT_GUI);
          Keyboard.press('z');
          Keyboard.release(KEY_LEFT_GUI);
          Keyboard.release('z');
        } else {
          Keyboard.press(KEY_LEFT_GUI);
          Keyboard.press(KEY_LEFT_SHIFT);
          Keyboard.press('z');
          Keyboard.release(KEY_LEFT_GUI);
          Keyboard.release(KEY_LEFT_SHIFT);
          Keyboard.release('z');
        }
        break;
      case YT_MODE:
        if (dir_enc01 > 0) {
          Keyboard.press(MEDIA_VOLUME_DOWN);
          Keyboard.release(MEDIA_VOLUME_DOWN);
        } else {
          Keyboard.press(MEDIA_VOLUME_UP);
          Keyboard.release(MEDIA_VOLUME_UP);
        }
        break;
      case MOUSE_MODE:
        if (dir_enc01 > 0) {
          Mouse.move(0, 0, -1);
        } else {
          Mouse.move(0, 0, 1);
        }
        break;
      
    } // end switch

    prev_enc01 = cur_enc01;
  }
}
