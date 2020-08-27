/* Copyright 2020 Frank Adams
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at
       http://www.apache.org/licenses/LICENSE-2.0
   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
// This software creates a USB keyboard and Trackpoint (TP) from a 
// Toshiba 2415 replacement keyboard and a TP from an HP DV9000 laptop.
// TP part number is 920-000702-04 Rev A
// The test points on the touchpad are used to solder jumpers as follows:
// T22 = 3.3V  
// T23 = Gnd   (I soldered to the ground plane)
// T10 = Clock 
// T11 = Data 
// This touchpad has active pullups so no additional pullups were required.
//
// Revision History
// Initial Release Aug 24, 2020
//
#define MODIFIERKEY_FN 0x8f   // give Fn key a fake HID code 
#define KEY_ARROW_LOCK KEY_F13  // give arrow lock key an unused code
#define KEY_TP_TOGGLE KEY_F14   // give TP Toggle key an unused code
// LED I/O connections
#define CAPS_LED 13
#define NUM_LED 15
#define SCRL_LED 16
#define ARROW_LED 14
// TP I/O connections
#define TP_CLK 0
#define TP_DATA 1
//
const byte rows_max = 16; // sets the number of rows in the matrix
const byte cols_max = 8; // sets the number of columns in the matrix
//
// Load the normal key matrix with the Teensyduino key names 
// described at www.pjrc.com/teensy/td_keyboard.html 
// A zero indicates no normal key at that location.
int normal[rows_max][cols_max] = {
  {KEY_CAPS_LOCK,KEY_A,KEY_Z,KEY_Q,KEY_1,KEY_F1,KEY_TAB,KEY_ESC},
  {0,0,0,0,0,0,0,0},  
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {KEY_X,KEY_TILDE,0,KEY_S,KEY_2,KEY_F3,KEY_W,KEY_F2},
  {KEY_C,0,KEY_PRINTSCREEN,KEY_D,KEY_E,KEY_3,KEY_F5,KEY_F4},  
  {KEY_DELETE,0,KEY_RIGHT,KEY_PERIOD,KEY_O,KEY_9,KEY_L,KEY_F12},
  {KEY_V,0,KEY_PAUSE,0,KEY_F6,KEY_R,KEY_F,KEY_4},
  {0,0,0,0,0,0,0,0},
  {KEY_SEMICOLON,0,KEY_SLASH,KEY_LEFT_BRACE,KEY_MINUS,KEY_0,KEY_P,KEY_SPACE},  
  {0,KEY_N,KEY_LEFT,0,KEY_F9,KEY_Y,KEY_H,KEY_6},
  {0,KEY_B,0,KEY_G,KEY_T,KEY_5,KEY_F8,KEY_F7},
  {KEY_INSERT,0,KEY_DOWN,KEY_COMMA,KEY_I,KEY_8,KEY_K,KEY_F11},
  {KEY_M,0,KEY_UP,KEY_J,KEY_U,KEY_7,0,KEY_F10},  
  {KEY_QUOTE,KEY_ENTER,0,KEY_RIGHT_BRACE,0,KEY_EQUAL,KEY_BACKSPACE,0},
  {KEY_END,KEY_BACKSLASH,KEY_MENU,KEY_PAGE_DOWN,KEY_HOME,0,KEY_PAGE_UP,0}
};
// Load the numlock key matrix with key names at the correct row-column location. 
// This matrix is the same as the normal matrix except for the number pad keys
// A zero indicates no numlock key at that location.
int numlock[rows_max][cols_max] = {
  {KEY_CAPS_LOCK,KEY_A,KEY_Z,KEY_Q,KEY_1,KEY_F1,KEY_TAB,KEY_ESC},
  {0,0,0,0,0,0,0,0},  
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {KEY_X,KEY_TILDE,0,KEY_S,KEY_2,KEY_F3,KEY_W,KEY_F2},
  {KEY_C,0,KEY_PRINTSCREEN,KEY_D,KEY_E,KEY_3,KEY_F5,KEY_F4},  
  {KEY_DELETE,0,KEY_RIGHT,KEYPAD_PERIOD,KEYPAD_6,KEYPAD_9,KEYPAD_3,KEY_F12},
  {KEY_V,0,KEY_PAUSE,0,KEY_F6,KEY_R,KEY_F,KEY_4},
  {0,0,0,0,0,0,0,0},
  {KEYPAD_PLUS,0,KEYPAD_SLASH,KEY_LEFT_BRACE,KEY_MINUS,KEYPAD_ASTERIX,KEYPAD_MINUS,KEY_SPACE},  
  {0,KEY_N,KEY_LEFT,0,KEY_F9,KEY_Y,KEY_H,KEY_6},
  {0,KEY_B,0,KEY_G,KEY_T,KEY_5,KEY_F8,KEY_F7},
  {KEY_INSERT,0,KEY_DOWN,KEY_COMMA,KEYPAD_5,KEYPAD_8,KEYPAD_2,KEY_F11},
  {KEYPAD_0,0,KEY_UP,KEYPAD_1,KEYPAD_4,KEYPAD_7,0,KEY_F10},  
  {KEY_QUOTE,KEY_ENTER,0,KEY_RIGHT_BRACE,0,KEY_EQUAL,KEY_BACKSPACE,0},
  {KEY_END,KEY_BACKSLASH,KEY_MENU,KEY_PAGE_DOWN,KEY_HOME,0,KEY_PAGE_UP,0}
};
// Load the arrowlock key matrix with key names at the correct row-column location. 
// This matrix is the same as the normal matrix except for the arrow pad keys
// A zero indicates no numlock key at that location.
int arrowlock[rows_max][cols_max] = {
  {KEY_CAPS_LOCK,KEY_A,KEY_Z,KEY_Q,KEY_1,KEY_F1,KEY_TAB,KEY_ESC},
  {0,0,0,0,0,0,0,0},  
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {KEY_X,KEY_TILDE,0,KEY_S,KEY_2,KEY_F3,KEY_W,KEY_F2},
  {KEY_C,0,KEY_PRINTSCREEN,KEY_D,KEY_E,KEY_3,KEY_F5,KEY_F4},  
  {KEY_DELETE,0,KEY_RIGHT,KEY_DELETE,KEYPAD_6,KEY_PAGE_UP,KEY_PAGE_DOWN,KEY_F12},
  {KEY_V,0,KEY_PAUSE,0,KEY_F6,KEY_R,KEY_F,KEY_4},
  {0,0,0,0,0,0,0,0},
  {KEY_SEMICOLON,0,KEY_SLASH,KEY_LEFT_BRACE,KEY_MINUS,KEY_0,KEY_P,KEY_SPACE},  
  {0,KEY_N,KEY_LEFT,0,KEY_F9,KEY_Y,KEY_H,KEY_6},
  {0,KEY_B,0,KEY_G,KEY_T,KEY_5,KEY_F8,KEY_F7},
  {KEY_INSERT,0,KEY_DOWN,KEY_COMMA,0,KEY_UP,KEY_DOWN,KEY_F11},
  {KEY_INSERT,0,KEY_UP,KEY_END,KEY_LEFT,KEY_HOME,0,KEY_F10},  
  {KEY_QUOTE,KEY_ENTER,0,KEY_RIGHT_BRACE,0,KEY_EQUAL,KEY_BACKSPACE,0},
  {KEY_END,KEY_BACKSLASH,KEY_MENU,KEY_PAGE_DOWN,KEY_HOME,0,KEY_PAGE_UP,0}
};
// Load the modifier key matrix with key names at the correct row-column location. 
// A zero indicates no modifier key at that location.
int modifier[rows_max][cols_max] = {
  {0,0,0,0,0,0,0,0},
  {MODIFIERKEY_LEFT_CTRL,0,0,0,0,0,0,0},
  {0,MODIFIERKEY_LEFT_SHIFT,0,0,0,0,0,MODIFIERKEY_RIGHT_SHIFT},
  {0,0,MODIFIERKEY_FN,0,0,0,MODIFIERKEY_GUI,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},  
  {0,0,0,MODIFIERKEY_LEFT_ALT,MODIFIERKEY_RIGHT_ALT,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0}
};
// Load the media key matrix with Fn key names at the correct row-column location. 
// A zero indicates no media key at that location. 
int media[rows_max][cols_max] = {
  {0,0,0,0,0,KEY_MEDIA_VOLUME_DEC,0,KEY_MEDIA_MUTE},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,KEY_MEDIA_VOLUME_INC},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,KEY_SCROLL_LOCK},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,KEY_TP_TOGGLE,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,KEY_NUM_LOCK},
  {0,0,0,0,0,0,0,KEY_ARROW_LOCK},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0}
};
// Initialize the old_key matrix with one's. 
// 1 = key not pressed, 0 = key is pressed
boolean old_key[rows_max][cols_max] = {
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1}  
};
//
// Assign the Teensy I/O row numbers
int Row_IO[rows_max] = {20,3,18,5,24,7,9,26,10,11,28,12,32,31,30,29}; 
//
// Assignb the column I/O numbers
int Col_IO[cols_max] = {19,4,17,6,25,8,33,27}; 
//
// Declare variables that will be used by functions
boolean slots_full = LOW; // Goes high when slots 1 thru 6 contain normal keys
boolean touchpad_error = LOW; // sent high when touch pad routine times out
//
// slot 1 thru slot 6 hold the normal key values to be sent over USB. 
int slot1 = 0; //value of 0 means the slot is empty and can be used.  
int slot2 = 0; 
int slot3 = 0; 
int slot4 = 0; 
int slot5 = 0; 
int slot6 = 0;
//
int mod_shift_l = 0; // These variables are sent over USB as modifier keys.
int mod_shift_r = 0; // Each is either set to 0 or MODIFIER_ ... 
int mod_ctrl_l = 0;   
int mod_ctrl_r = 0;
int mod_alt_l = 0;
int mod_alt_r = 0;
int mod_gui = 0;
//
// Function to load the key name into the first available slot
void load_slot(int key) {
  if (!slot1)  {
    slot1 = key;
  }
  else if (!slot2) {
    slot2 = key;
  }
  else if (!slot3) {
    slot3 = key;
  }
  else if (!slot4) {
    slot4 = key;
  }
  else if (!slot5) {
    slot5 = key;
  }
  else if (!slot6) {
    slot6 = key;
  }
  if (!slot1 || !slot2 || !slot3 || !slot4 || !slot5 || !slot6)  {
    slots_full = LOW; // slots are not full
  }
  else {
    slots_full = HIGH; // slots are full
  } 
}
//
// Function to clear the slot that contains the key name
void clear_slot(int key) {
  if (slot1 == key) {
    slot1 = 0;
  }
  else if (slot2 == key) {
    slot2 = 0;
  }
  else if (slot3 == key) {
    slot3 = 0;
  }
  else if (slot4 == key) {
    slot4 = 0;
  }
  else if (slot5 == key) {
    slot5 = 0;
  }
  else if (slot6 == key) {
    slot6 = 0;
  }
  if (!slot1 || !slot2 || !slot3 || !slot4 || !slot5 || !slot6)  {
    slots_full = LOW; // slots are not full
  }
  else {
    slots_full = HIGH; // slots are full
  } 
}
//
// Function to load the modifier key name into the appropriate mod variable
void load_mod(int m_key) {
  if (m_key == MODIFIERKEY_LEFT_SHIFT)  {
    mod_shift_l = m_key;
  }
  else if (m_key == MODIFIERKEY_RIGHT_SHIFT)  {
    mod_shift_r = m_key;
  }
  else if (m_key == MODIFIERKEY_LEFT_CTRL)  {
    mod_ctrl_l = m_key;
  }
  else if (m_key == MODIFIERKEY_RIGHT_CTRL)  {
    mod_ctrl_r = m_key;
  }
  else if (m_key == MODIFIERKEY_LEFT_ALT)  {
    mod_alt_l = m_key;
  }
  else if (m_key == MODIFIERKEY_RIGHT_ALT)  {
    mod_alt_r = m_key;
  }
  else if (m_key == MODIFIERKEY_GUI)  {
    mod_gui = m_key;
  }
}
//
// Function to load 0 into the appropriate mod variable
void clear_mod(int m_key) {
  if (m_key == MODIFIERKEY_LEFT_SHIFT)  {
    mod_shift_l = 0;
  }
  else if (m_key == MODIFIERKEY_RIGHT_SHIFT)  {
    mod_shift_r = 0;
  }
  else if (m_key == MODIFIERKEY_LEFT_CTRL)  {
    mod_ctrl_l = 0;
  }
  else if (m_key == MODIFIERKEY_RIGHT_CTRL)  {
    mod_ctrl_r = 0;
  }
  else if (m_key == MODIFIERKEY_LEFT_ALT)  {
    mod_alt_l = 0;
  }
  else if (m_key == MODIFIERKEY_RIGHT_ALT)  {
    mod_alt_r = 0;
  }
  else if (m_key == MODIFIERKEY_GUI)  {
    mod_gui = 0;
  }
}
//
// Function to send the modifier keys over usb
void send_mod() {
  Keyboard.set_modifier(mod_shift_l | mod_shift_r | mod_ctrl_l | mod_ctrl_r | mod_alt_l | mod_alt_r | mod_gui);
  Keyboard.send_now();
}
//
// Function to send the normal keys in the 6 slots over usb
void send_normals() {
  Keyboard.set_key1(slot1);
  Keyboard.set_key2(slot2);
  Keyboard.set_key3(slot3);
  Keyboard.set_key4(slot4);
  Keyboard.set_key5(slot5);
  Keyboard.set_key6(slot6);
  Keyboard.send_now();
}
//
// Function to set a pin to high impedance (acts like open drain output)
void go_z(int pin)
{
  pinMode(pin, INPUT);
  digitalWrite(pin, HIGH);
}
//
// Function to set a pin as an input with a pullup
void go_pu(int pin)
{
  pinMode(pin, INPUT_PULLUP);
  digitalWrite(pin, HIGH);
}
//
// Function to send a pin to a logic low
void go_0(int pin)
{
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}
//
// Function to send a pin to a logic high
void go_1(int pin)
{
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);
}
//
// *****************Functions for Touchpad***************************
//
// Function to send the touchpad a byte of data (command)
//
void tp_write(char send_data)  
{
  unsigned int timeout = 200; // breakout of loop if over this value in msec
  elapsedMillis watchdog; // zero the watchdog timer clock
  char odd_parity = 0; // clear parity bit count
// Enable the bus by floating the clock and data
  go_pu(TP_CLK); //
  go_pu(TP_DATA); //
  delayMicroseconds(250); // wait before requesting the bus
  go_0(TP_CLK); //   Send the Clock line low to request to transmit data
  delayMicroseconds(100); // wait for 100 microseconds per bus spec
  go_0(TP_DATA); //  Send the Data line low (the start bit)
  delayMicroseconds(1); //
  go_pu(TP_CLK); //   Release the Clock line so it is pulled high
  delayMicroseconds(1); // give some time to let the clock line go high
  while (digitalRead(TP_CLK) == HIGH) { // loop until the clock goes low
    if (watchdog >= timeout) { //check for infinite loop
      break; // break out of infinite loop
    }
  }
// send the 8 bits of send_data 
  for (int j=0; j<8; j++) {
    if (send_data & 1) {  //check if lsb is set
      go_pu(TP_DATA); // send a 1 to TP
      odd_parity = odd_parity + 1; // keep running total of 1's sent
    }
    else {
      go_0(TP_DATA); // send a 0 to TP
    }
    delayMicroseconds(1); // delay to let the clock settle out
    while (digitalRead(TP_CLK) == LOW) { // loop until the clock goes high
      if (watchdog >= timeout) { //check for infinite loop
        break; // break out of infinite loop
      }
    }
    delayMicroseconds(1); // delay to let the clock settle out
    while (digitalRead(TP_CLK) == HIGH) { // loop until the clock goes low
      if (watchdog >= timeout) { //check for infinite loop
        break; // break out of infinite loop
      }
    }  
    send_data = send_data >> 1; // shift data right by 1 to prepare for next loop
  }
// send the parity bit
  if (odd_parity & 1) {  //check if lsb of parity is set
    go_0(TP_DATA); // already odd so send a 0 to TP
  }
  else {
    go_pu(TP_DATA); // send a 1 to TP to make parity odd
  }   
  delayMicroseconds(1); // delay to let the clock settle out
  while (digitalRead(TP_CLK) == LOW) { // loop until the clock goes high
    if (watchdog >= timeout) { //check for infinite loop
      break; // break out of infinite loop
    }
  }
  delayMicroseconds(1); // delay to let the clock settle out
  while (digitalRead(TP_CLK) == HIGH) { // loop until the clock goes low
    if (watchdog >= timeout) { //check for infinite loop
      break; // break out of infinite loop
    }
  }
  go_pu(TP_DATA); //  Release the Data line so it goes high as the stop bit
  delayMicroseconds(80); // testing shows delay at least 40us 
  while (digitalRead(TP_CLK) == HIGH) { // loop until the clock goes low
    if (watchdog >= timeout) { //check for infinite loop
      break; // break out of infinite loop
    }
  }
  delayMicroseconds(1); // wait to let the data settle
  if (digitalRead(TP_DATA)) { // Ack bit s/b low if good transfer
  }
  while ((digitalRead(TP_CLK) == LOW) || (digitalRead(TP_DATA) == LOW)) { // loop if clock or data are low
    if (watchdog >= timeout) { //check for infinite loop
      break; // break out of infinite loop
    }
  }
// Inhibit the bus so the tp only talks when we're listening
  go_0(TP_CLK);
}
//
// Function to get a byte of data from the touchpad
//
char tp_read(void)
{
  unsigned int timeout = 200; // breakout of loop if over this value in msec
  elapsedMillis watchdog; // zero the watchdog timer clock
  char rcv_data = 0; // initialize to zero
  char mask = 1; // shift a 1 across the 8 bits to select where to load the data
  char rcv_parity = 0; // count the ones received
  go_pu(TP_CLK); // release the clock
  go_pu(TP_DATA); // release the data
  delayMicroseconds(5); // delay to let clock go high
  while (digitalRead(TP_CLK) == HIGH) { // loop until the clock goes low
    if (watchdog >= timeout) { //check for infinite loop
      break; // break out of infinite loop
    }
  }
  if (digitalRead(TP_DATA)) { // Start bit s/b low from tp
  // start bit not correct - put error handler here if desired
  }  
  delayMicroseconds(1); // delay to let the clock settle out
  while (digitalRead(TP_CLK) == LOW) { // loop until the clock goes high
    if (watchdog >= timeout) { //check for infinite loop
      break; // break out of infinite loop
    }
  }
  for (int k=0; k<8; k++) {  
    delayMicroseconds(1); // delay to let the clock settle out
    while (digitalRead(TP_CLK) == HIGH) { // loop until the clock goes low
      if (watchdog >= timeout) { //check for infinite loop
        break; // break out of infinite loop
      }
    }
    if (digitalRead(TP_DATA)) { // check if data is high
      rcv_data = rcv_data | mask; // set the appropriate bit in the rcv data
      rcv_parity++; // increment the parity bit counter
    }
    mask = mask << 1;
    delayMicroseconds(1); // delay to let the clock settle out
    while (digitalRead(TP_CLK) == LOW) { // loop until the clock goes high
      if (watchdog >= timeout) { //check for infinite loop
        break; // break out of infinite loop
      }
    }
  }
// receive parity
  delayMicroseconds(1); // delay to let the clock settle out
  while (digitalRead(TP_CLK) == HIGH) { // loop until the clock goes low
    if (watchdog >= timeout) { //check for infinite loop
      break; // break out of infinite loop
    }
  }
  if (digitalRead(TP_DATA))  { // check if received parity is high
    rcv_parity++; // increment the parity bit counter
  }
  rcv_parity = rcv_parity & 1; // mask off all bits except the lsb
  if (rcv_parity == 0) { // check for bad (even) parity
  // bad parity - pass to future error handler
  } 
  delayMicroseconds(1); // delay to let the clock settle out
  while (digitalRead(TP_CLK) == LOW) { // loop until the clock goes high
    if (watchdog >= timeout) { //check for infinite loop
      break; // break out of infinite loop
    }
  }
// stop bit
  delayMicroseconds(1); // delay to let the clock settle out
  while (digitalRead(TP_CLK) == HIGH) { // loop until the clock goes low
    if (watchdog >= timeout) { //check for infinite loop
      break; // break out of infinite loop
    }
  }
  if (digitalRead(TP_DATA) == LOW) { // check if stop bit is bad (low)
  // send bad stop bit to future error handler
  }
  delayMicroseconds(1); // delay to let the clock settle out
  while (digitalRead(TP_CLK) == LOW) { // loop until the clock goes high
    if (watchdog >= timeout) { //check for infinite loop
      break; // break out of infinite loop
    }
  }
// Inhibit the bus so the tp only talks when we're listening
  go_0(TP_CLK);
  return rcv_data; // pass the received data back
}
//
void touchpad_init()
{
  touchpad_error = LOW; // start with no error
  go_pu(TP_CLK); // float the clock and data to touchpad
  go_pu(TP_DATA);
  //  Sending reset command to touchpad
  tp_write(0xff);
  if (tp_read() != 0xfa) { // verify correct ack byte
    touchpad_error = HIGH;
  }
  delay(1000); // wait 1000ms so tp can run its self diagnostic
  //  verify proper response from tp
  if (tp_read() != 0xaa) { // verify basic assurance test passed
    touchpad_error = HIGH;
  } 
  if (tp_read() != 0x00) { // verify basic assurance test passed
    touchpad_error = HIGH;
  }
  //  Send touchpad disable code 
  tp_write(0xf5);  // tp disable 
  if (tp_read() != 0xfa) { // verify correct ack byte
//    init_error = HIGH;
  }
  // Load Mode Byte with 00 using the following special sequence from page 38.
  // Send set resolution to 0 four times followed by a set sample rate to 0x14
// #1 set resolution
  tp_write(0xe8);  // set resolution 
  if (tp_read() != 0xfa) { // verify correct ack byte
//    init_error = HIGH;
  }  
  tp_write(0x01);  // to zero 
  if (tp_read() != 0xfa) { // verify correct ack byte
//    init_error = HIGH;
  }
// #2 set resolution
  tp_write(0xe8);  // set resolution 
  if (tp_read() != 0xfa) { // verify correct ack byte
//    init_error = HIGH;
  }  
  tp_write(0x00);  // to zero 
  if (tp_read() != 0xfa) { // verify correct ack byte
//    init_error = HIGH;
  }
// #3 set resolution
  tp_write(0xe8);  // set resolution 
  if (tp_read() != 0xfa) { // verify correct ack byte
//    init_error = HIGH;
  }  
  tp_write(0x00);  // to zero 
  if (tp_read() != 0xfa) { // verify correct ack byte
//    init_error = HIGH;
  }
// #4 set resolution
  tp_write(0xe8);  // set resolution 
  if (tp_read() != 0xfa) { // verify correct ack byte
//    init_error = HIGH;
  }  
  tp_write(0x00);  // to zero 
  if (tp_read() != 0xfa) { // verify correct ack byte
//    init_error = HIGH;
  }
// Set sample rate
  tp_write(0xf3);  // set sample rate 
  if (tp_read() != 0xfa) { // verify correct ack byte
//    init_error = HIGH;
  }  
  tp_write(0x14);  // to 14 hex 
  if (tp_read() != 0xfa) { // verify correct ack byte
//    init_error = HIGH;
  } 
  // set the resolution 
  tp_write(0xe8); //  Sending resolution command
  if (tp_read() != 0xfa) { // verify correct ack byte
//    init_error = HIGH;
  } 
  tp_write(0x03); // value of 0x03 = 8 counts/mm resolution (default is 4 counts/mm)
  if (tp_read() != 0xfa) { // verify correct ack byte
//    init_error = HIGH;
  } 
  // set the sample rate 
  tp_write(0xf3); //  Sending sample rate command
  if (tp_read() != 0xfa) { // verify correct ack byte
//    init_error = HIGH;
  } 
  tp_write(0x28); // 0x28 = 40 samples per second, the default value used for Synaptics TP
  if (tp_read() != 0xfa) { // verify correct ack byte
//    init_error = HIGH;
  }  
  //  Sending remote mode code so the touchpad will send data only when polled
  tp_write(0xf0);  // remote mode 
  if (tp_read() != 0xfa) { // verify correct ack byte
//    touchpad_error = HIGH;
  } 
  //  Sending touchpad enable code (needed for Elan touchpads)
  tp_write(0xf4);  // tp enable 
  if (tp_read() != 0xfa) { // verify correct ack byte
//    touchpad_error = HIGH;
  }
}
//----------------------------------Setup-------------------------------------------
void setup() {
// Keyboard
  for (int a = 0; a < cols_max; a++) {  // loop thru all column pins 
    go_pu(Col_IO[a]); // set each column pin as an input with a pullup
  }
//
  for (int b = 0; b < rows_max; b++) {  // loop thru all row pins 
    go_z(Row_IO[b]); // set each row pin as a floating output
  } 
// Touchpad
  touchpad_init(); // reset touchpad, then set it's resolution and put it in remote mode   
}
//
// Keyboard variables
extern volatile uint8_t keyboard_leds; // 8 bits sent from Host to Teensy that give keyboard status. 
// Num lock is bit D0.
// Caps lock is bit D1.
// Scroll lock is bit D2.
// Compose is bit D3. Compose has the same HID code as the Menu key.
// Kana is bit D4. Kana switches to a Japanese key layer.
// Power is bit D5.
// Shift is bit D6.
// Do Not Disturb is bit D7.
//
boolean Fn_pressed = HIGH; // Initialize Fn key to HIGH = "not pressed".
boolean tp_on = HIGH; // Wake up with TP turned on
boolean arrow_lock = LOW; // Wake up with arrow lock turned off
//
// Touchpad variables
  char mstat; // touchpad status reg = Y overflow, X overflow, Y sign bit, X sign bit, Always 1, Middle Btn, Right Btn, Left Btn
  char mx; // touchpad x movement = 8 data bits. The sign bit is in the status register to 
           // make a 9 bit 2's complement value. Left to right on the touchpad gives a positive value. 
  char my; // touchpad y movement also 8 bits plus sign. Touchpad movement away from the user gives a positive value.
  boolean over_flow; // set if x or y movement values are bad due to overflow
  boolean left_button = 0; // on/off variable for left button = bit 0 of mstat
  boolean right_button = 0; // on/off variable for right button = bit 1 of mstat
  boolean old_left_button = 0; // on/off variable for left button status the previous polling cycle
  boolean old_right_button = 0; // on/off variable for right button status the previous polling cycle
  boolean button_change = 0; // Active high, shows when a touchpad left or right button has changed since last polling cycle
//
//---------------------------------Main Loop---------------------------------------------
//
void loop() {   
// Scan keyboard matrix with an outer loop that drives each row low and an inner loop that reads every column (with pull ups).
// The routine looks at each key's present state (by reading the column input pin) and also the previous state from the last scan
// that was 30msec ago. The status of a key that was just pressed or just released is sent over USB and the state is saved in the old_key matrix. 
// The keyboard keys will read as logic low if they are pressed (negative logic).
// The old_key matrix also uses negative logic (low=pressed). 
//
  for (int x = 0; x < rows_max; x++) {   // loop thru the rows
    go_0(Row_IO[x]); // Activate Row (send it low)
    delayMicroseconds(10); // give the row time to go low and settle out
    for (int y = 0; y < cols_max; y++) {   // loop thru the columns
// **********Modifier keys including the Fn special case
      if (modifier[x][y] != 0) {  // check if modifier key exists at this location in the array (a non-zero value)
        if (!digitalRead(Col_IO[y]) && (old_key[x][y])) {  // Read column to see if key is low (pressed) and was previously not pressed
          if (modifier[x][y] != MODIFIERKEY_FN) {   // Exclude Fn modifier key  
            load_mod(modifier[x][y]); // function reads which modifier key is pressed and loads it into the appropriate mod_... variable   
            send_mod(); // function sends the state of all modifier keys over usb including the one that just got pressed
            old_key[x][y] = LOW; // Save state of key as "pressed"
          }
          else {   // Fn is pressed
            Fn_pressed = LOW; // Fn status variable is active low
            old_key[x][y] = LOW; // old_key state is "pressed" (active low)
          }
        }
        else if (digitalRead(Col_IO[y]) && (!old_key[x][y])) {  //check if key is not pressed and was previously pressed
          if (modifier[x][y] != MODIFIERKEY_FN) { // Exclude Fn modifier key 
            clear_mod(modifier[x][y]); // function reads which modifier key was released and loads 0 into the appropriate mod_... variable
            send_mod(); // function sends all mod's over usb including the one that just released
            old_key[x][y] = HIGH; // Save state of key as "not pressed"
          }
          else {    // Fn is released
            Fn_pressed = HIGH; // Fn is no longer active
            old_key[x][y] = HIGH; // old_key state is "not pressed" 
          }
        }
      } 
// ***********end of modifier section
//
// ***********Normal keys, num lock, arrow lock keys and media keys in this section
      else if ((normal[x][y] != 0) || (media[x][y] != 0)) {  // check if normal or media key exists at this location in the array
        if (!digitalRead(Col_IO[y]) && (old_key[x][y]) && (!slots_full)) { // check if key pressed and not previously pressed and slots not full
          old_key[x][y] = LOW; // Save state of key as "pressed"
          if (Fn_pressed) {  // Fn is not pressed and normal key needs to be sent
            if (keyboard_leds & 1) {  // test if Num Lock is turned on
              load_slot(numlock[x][y]); //update first available slot with key name from numlock matrix
              send_normals(); // send all slots over USB including the key that just got pressed
            }
            else if (arrow_lock) {  // test if Arrow Lock is turned on
              load_slot(arrowlock[x][y]); //update first available slot with key name from arrowlock matrix
              send_normals(); // send all slots over USB including the key that just got pressed
            }
            else {
              load_slot(normal[x][y]); //update first available slot with key name from normal matrix
              send_normals(); // send all slots over USB including the key that just got pressed
            }
          }
          else if (media[x][y] != 0) { // Fn is pressed so send media if a key exists in the matrix
            if (media[x][y] == KEY_TP_TOGGLE) {
              tp_on = !tp_on; // invert the TP on/off control
            }
            else if (media[x][y] == KEY_ARROW_LOCK) {
              arrow_lock = !arrow_lock; // invert the arrow lock control
            }
            else {
              Keyboard.press(media[x][y]); // media key is sent using keyboard press function per PJRC    
              delay(5); // delay 5 milliseconds before releasing to make sure it gets sent over USB
              Keyboard.release(media[x][y]); // send media key release
            }
          }
        }          
        else if (digitalRead(Col_IO[y]) && (!old_key[x][y])) { //check if key is not pressed, but was previously pressed 
          old_key[x][y] = HIGH; // Save state of key as "not pressed"
          if (Fn_pressed) {  // Fn is not pressed
            if (keyboard_leds & 1) {  // test if Num Lock is turned on
              clear_slot(numlock[x][y]); //clear slot with key name from numlock matrix
              send_normals(); // send all slots over USB including the key that just got released
            }
            else if (arrow_lock) {  // test if Arrow lock is turned on
              clear_slot(arrowlock[x][y]); //clear slot with key name from arrowlock matrix
              send_normals(); // send all slots over USB including the key that just got released
            }
            else {
              clear_slot(normal[x][y]); //clear slot with key name from normal matrix
              send_normals(); // send all slots over USB including the key that just got released
            }
          }
        }
      } 
// **************end of normal, num lock, and media key section 
//
    }
    go_z(Row_IO[x]); // De-activate Row (send it to hi-z)
  }
//
// **********keyboard scan complete
//
// Control the 4 keyboard LEDs
//
  if (keyboard_leds & 1<<1) {  // mask off all bits but D1 and test if set
    go_1(CAPS_LED); // turn on the LED
  }
  else {
    go_0(CAPS_LED); // turn off the LED
  }
//
  if (keyboard_leds & 1) {  // mask off all bits but D0 and test if set
    go_1(NUM_LED); // turn on the Num Lock LED
  }
  else {
    go_0(NUM_LED); // turn off the Num Lock LED
  }
//
  if (keyboard_leds & 1<<2) {  // mask off all bits but D2 and test if set
    go_1(SCRL_LED); // turn on the Scroll Lock LED
  }
  else {
    go_0(SCRL_LED); // turn off the Scroll Lock LED
  }
//
  if (arrow_lock) {
    go_1(ARROW_LED); // turn on the Arrow Lock LED
  }
  else {
    go_0(ARROW_LED); // turn off the Arrow Lock LED
  }
//  
// *******************Touchpad section********************************************************
//
  if ((!touchpad_error) && (tp_on)) { // Did the TP pass its startup check and is the TP currently turned on? 
        //If yes, poll TP for new movement or button data
    over_flow = 0; // assume no overflow until status is received 
    tp_write(0xeb);  // request data from TP
    if (tp_read() != 0xfa) { // verify correct ack byte
    // bad ack - pass to future error handler
    }
    mstat = tp_read(); // save into status variable
    mx = tp_read(); // save into x variable
    my = tp_read(); // save into y variable
    if (((0x80 & mstat) == 0x80) || ((0x40 & mstat) == 0x40))  {   // x or y overflow bits set?
      over_flow = 1; // set the overflow flag
    }   
// change the x data from 9 bit to 8 bit 2's complement
    mx = mx & 0x7f; // mask off 8th bit
    if ((0x10 & mstat) == 0x10) {   // move the sign into 
      mx = 0x80 | mx;              // the 8th bit position
    } 
// change the y data from 9 bit to 8 bit 2's complement and then take the 2's complement 
// because y movement on ps/2 format is opposite of touchpad.move function
    my = my & 0x7f; // mask off 8th bit
    if ((0x20 & mstat) == 0x20) {   // move the sign into 
      my = 0x80 | my;              // the 8th bit position
    } 
    my = (~my + 0x01); // change the sign of y data by taking the 2's complement (invert and add 1)
// zero out mx and my if over_flow is set
    if (over_flow) { 
      mx = 0x00;       // data is garbage so zero it out
      my = 0x00;
    } 
// send the x and y data back via usb if either one is non-zero
    if ((mx != 0x00) || (my != 0x00)) {
      Mouse.move(mx,my);
    }
//
// send the touchpad left and right button status over usb if no error
    if ((0x01 & mstat) == 0x01) {   // if left button set 
      left_button = 1;   
    }
    else {   // clear left button
      left_button = 0;   
    }
    if ((0x02 & mstat) == 0x02) {   // if right button set 
      right_button = 1;   
    } 
    else {   // clear right button
      right_button = 0;  
    }
// Determine if the left or right touch pad buttons have changed since last polling cycle
    button_change = (left_button ^ old_left_button) | (right_button ^ old_right_button);
// Don't send button status if there's no change since last time. 
    if (button_change){
      Mouse.set_buttons(left_button, 0, right_button); // send button status
    }
    old_left_button = left_button; // remember new button status for next polling cycle
    old_right_button = right_button;
  }

  
  delay(25); // The overall keyboard scanning rate is about 30ms
}
