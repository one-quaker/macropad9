#include "MatrixKeypad.h"
#include <stdint.h>
#include <Arduino.h>
#include <BasicEncoder.h>
#include "Keyboard.h"
#include "Mouse.h"

uint8_t MODE = 0;
uint8_t MODE_MAX = 2;

const int8_t pinA = 16;
const int8_t pinB = 10;
int8_t cur_enc01 = 0;
int8_t prev_enc01 = 0;
int8_t dir_enc01 = 0;

const uint8_t rown = 4; //4 rows
const uint8_t coln = 3; //3 columns
uint8_t rowPins[rown] = {A1, A0, 15, 14}; //frist row is connect to pin 10, second to 9...
uint8_t colPins[coln] = {7, 8, 9}; //frist column is connect to pin 6, second to 5...
char keymap[rown][coln] = 
  {{'1','2','3'}, //key of the frist row frist column is '1', frist row second column column is '2'
   {'4','5','6'}, //key of the second row frist column is '4', second row second column column is '5'
   {'7','8','9'},
   {'X','X','0'},
   };
char key;

MatrixKeypad_t *keypad; //keypad is the variable that you will need to pass to the other functions
BasicEncoder encoder(pinA, pinB);


void setup() {
	Serial.begin(9600);
  Keyboard.begin();
  Mouse.begin();
	keypad = MatrixKeypad_create((char*)keymap /* don't forget to do this cast */, rowPins, colPins, rown, coln); //creates the keypad object
}

void loop() {
  keyboard_loop();
  encoder_loop();
}

void keyboard_loop() {
  MatrixKeypad_scan(keypad); //scans for a key press event
	if(MatrixKeypad_hasKey(keypad)){ //if a key was pressed
		key = MatrixKeypad_getKey(keypad); //get the key
    keyproc(key);
		Serial.println(key); //prints the pressed key to the serial output
	}
}

void keyproc(char kcode) {
  if (key == '0') {
    MODE++;
    if (MODE > MODE_MAX) {
      MODE = 0;
    }
    Serial.print("MODE: ");
    Serial.println(MODE);
  }

  switch (MODE) {
    case 0:
      keyproc_fusion(kcode);
      break;
    default:
      Serial.println("Not implemented mode");
      break;
  }
}

void keyproc_fusion(char kcode) {
  switch (kcode) {
    case '3':
      Keyboard.print("i");
      break;
    case '6':
      Keyboard.print("f");
      delay(100);
      break;
    case '9':
      Keyboard.print("e");
      break;
    case '2':
      Keyboard.print("d");
      break;
    case '5':
      Keyboard.print("l");
      break;
    case '8':
      Keyboard.print("c");
      break;
    case '1':
      Keyboard.press(KEY_ESC);
      Keyboard.release(KEY_ESC);
      break;
    case '4':
      Keyboard.print("r");
      break;
    case '7':
      Keyboard.print("o");
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
      case 0:
        if (dir_enc01 > 0) {
          Mouse.move(0, 0, -1);
        } else {
          Mouse.move(0, 0, 1);
        }
        break;
      case 1:
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
      
    } // end switch

    prev_enc01 = cur_enc01;
  } 
}