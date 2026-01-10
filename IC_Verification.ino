#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

#define delay_long 5000
#define delay_short 1500
#define delay_scroll 500

int max_pin = 16;
int start_pin = 30;

// LCD setup
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Keypad setup
const byte ROWS = 4;
const byte COLS = 4;

const char keys[ROWS][COLS] PROGMEM= {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

byte rowPins[ROWS] = { 2, 3, 4, 5 };  // Rows
byte colPins[COLS] = { 6, 7, 8, 9 };  // Cols

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

String inputValue = "";
char mode = ' ';  // 'A' = Manual, 'B' = Self

//Strings required
String check_message;
String error_message;
String message;

// 1-input Logic Gates
int check_7404() {
  check_message = "Checking IC 7404 (Hex Inverter)";
  scrollText(0, check_message);
  delay(delay_long);

  int flag = 0;

  int total_pins = 14;  // Total number of Pins

  int gnd_pin = 7;   // Ground Pin
  int vcc_pin = 14;  // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }
  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);    // Setting pinMode of Ground Pin
  digitalWrite(gnd_pin, LOW);  // Setting Value of Ground Pin

  pinMode(vcc_pin, OUTPUT);     // Setting pinMode of VCC Pin
  digitalWrite(vcc_pin, HIGH);  // Setting Value of VCC Pin

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    //Faulty Ground or VCC pin, Not need to check entire IC.
    error_message = "Ground or VCC pin is not working properly.";
    scrollText(1, error_message);
    delay(delay_long);
    return flag;  //Return 0
  }

  int pin;

  int ip_pins[6] = { 1, 3, 5, 9, 11, 13 };  // Input Pins of IC 7404

  for (pin = 0; pin < 6; pin++) {
    // Setting Arduino Input Pins
    if (ip_pins[pin] >= ((total_pins / 2) + 1)) {
      ip_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      ip_pins[pin] += start_pin;
    }
    pinMode((ip_pins[pin]), OUTPUT);  // Setting pinMode of Input pins
  }

  int op_pins[6] = { 2, 4, 6, 8, 10, 12 };  // Output Pins of IC 7404

  for (pin = 0; pin < 6; pin++) {
    // Setting Arduino Output Pins
    if (op_pins[pin] >= ((total_pins / 2) + 1)) {
      op_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      op_pins[pin] += start_pin;
    }
    pinMode((op_pins[pin]), INPUT);  // Setting pinMode of Output pins
  }

  // Truth Table of IC 7404
  int tt_7404[2][2] = { { 0, 1 },
                        { 1, 0 } };

  // Checking IC 7404
  for (int cir_i = 0; cir_i < 6; cir_i++) {
    int check = 1;
    
    for (int tt_i = 0; tt_i < 2; tt_i++) {
      digitalWrite(ip_pins[cir_i], tt_7404[tt_i][0]);
      if (digitalRead(op_pins[cir_i]) != tt_7404[tt_i][1]) {
        check = 0;
      }
    }
    flag += check;
    if (check) {
      message = "Gate on pin " + String(getOriginalPin(ip_pins[cir_i], total_pins)) + " and " + String(getOriginalPin(op_pins[cir_i], total_pins)) + " is WORKING.";
    } else {
      message = "Gate on pin " + String(getOriginalPin(ip_pins[cir_i], total_pins)) + " and " + String(getOriginalPin(op_pins[cir_i], total_pins)) + " is NOT WORKING.";
    }
    scrollText(1, message);
    delay(delay_short);
  }
  return flag;
}

// 2-input Logic Gates
int check_7400() {
  check_message = "Checking IC 7400 (Quad 2-Input NAND Gate)";
  scrollText(0, check_message);
  delay(delay_long);

  int flag = 0;

  int total_pins = 14;  // Total number of Pins

  int gnd_pin = 7;   // Ground Pin
  int vcc_pin = 14;  // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }
  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);    // Setting pinMode of Ground Pin
  digitalWrite(gnd_pin, LOW);  // Setting Value of Ground Pin

  pinMode(vcc_pin, OUTPUT);     // Setting pinMode of VCC Pin
  digitalWrite(vcc_pin, HIGH);  // Setting Value of VCC Pin

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    //Faulty Ground or VCC pin, Not need to check entire IC.
    error_message = "Ground or VCC pin is not working properly.";
    scrollText(1, error_message);
    delay(delay_long);
    return flag;  //Return 0
  }

  int pin, pin1, pin2;

  int ip_pins[4][2] = { { 1, 2 }, { 4, 5 }, { 10, 9 }, { 13, 12 } };  // Input Pins of IC 7400

  for (pin1 = 0; pin1 < 4; pin1++) {
    for (pin2 = 0; pin2 < 2; pin2++) {
      // Setting Arduino Input Pins
      if (ip_pins[pin1][pin2] >= ((total_pins / 2) + 1)) {
        ip_pins[pin1][pin2] += ((max_pin - total_pins) + start_pin);
      } else {
        ip_pins[pin1][pin2] += start_pin;
      }
      pinMode((ip_pins[pin1][pin2]), OUTPUT);  // Setting pinMode of Input pins
    }
  }

  int op_pins[4] = { 3, 6, 8, 11 };  // Output Pins of IC 7400

  for (pin = 0; pin < 4; pin++) {
    // Setting Arduino Output Pins
    if (op_pins[pin] >= ((total_pins / 2) + 1)) {
      op_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      op_pins[pin] += start_pin;
    }
    pinMode((op_pins[pin]), INPUT);  // Setting pinmode of Output pins
  }

  // Truth Table of IC 7400
  int tt_7400[4][3] = { { 0, 0, 1 },
                        { 0, 1, 1 },
                        { 1, 0, 1 },
                        { 1, 1, 0 } };

  // Checking IC 7400
  for (int cir_i = 0; cir_i < 4; cir_i++) {
    int check = 1;
    
    for (int tt_i = 0; tt_i < 4; tt_i++) {
      digitalWrite(ip_pins[cir_i][0], tt_7400[tt_i][0]);

      digitalWrite(ip_pins[cir_i][1], tt_7400[tt_i][1]);

      if (digitalRead(op_pins[cir_i]) != tt_7400[tt_i][2]) {
        check = 0;
      }
    }
    flag += check;
    if (check) {
      message = "Gate on pin " + String(getOriginalPin(ip_pins[cir_i][0], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][1], total_pins)) + " and " + String(getOriginalPin(op_pins[cir_i], total_pins)) + " is WORKING.";
    } else {
      message = "Gate on pin " + String(getOriginalPin(ip_pins[cir_i][0], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][1], total_pins)) + " and " + String(getOriginalPin(op_pins[cir_i], total_pins)) + " is NOT WORKING.";
    }
    scrollText(1, message);
    delay(delay_short);
  }
  return flag;
}

int check_7402() {
  check_message = "Checking IC 7402 (Quad 2-Input NOR Gate)";
  scrollText(0, check_message);
  delay(delay_long);

  int flag = 0;

  int total_pins = 14;  // Total number of Pins

  int gnd_pin = 7;   // Ground Pin
  int vcc_pin = 14;  // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }
  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);    // Setting pinMode of Ground Pin
  digitalWrite(gnd_pin, LOW);  // Setting Value of Ground Pin

  pinMode(vcc_pin, OUTPUT);     // Setting pinMode of VCC Pin
  digitalWrite(vcc_pin, HIGH);  // Setting Value of VCC Pin

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    //Faulty Ground or VCC pin, Not need to check entire IC.
    error_message = "Ground or VCC pin is not working properly.";
    scrollText(1, error_message);
    delay(delay_long);
    return flag;  //Return 0
  }

  int pin, pin1, pin2;

  int ip_pins[4][2] = { { 2, 3 }, { 5, 6 }, { 8, 9 }, { 11, 12 } };  // Input Pins of IC 7402

  for (pin1 = 0; pin1 < 4; pin1++) {
    for (pin2 = 0; pin2 < 2; pin2++) {
      // Setting Arduino Input Pins
      if (ip_pins[pin1][pin2] >= ((total_pins / 2) + 1)) {
        ip_pins[pin1][pin2] += ((max_pin - total_pins) + start_pin);
      } else {
        ip_pins[pin1][pin2] += start_pin;
      }
      pinMode((ip_pins[pin1][pin2]), OUTPUT);  // Setting pinMode of Input pins
    }
  }

  int op_pins[4] = { 1, 4, 10, 13 };  // Output Pins of IC 7402

  for (pin = 0; pin < 4; pin++) {
    // Setting Arduino Output Pins
    if (op_pins[pin] >= ((total_pins / 2) + 1)) {
      op_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      op_pins[pin] += start_pin;
    }
    pinMode((op_pins[pin]), INPUT);  // Setting pinmode of Output pins
  }

  // Truth Table of IC 7402
  int tt_7402[4][3] = { { 0, 0, 1 },
                        { 0, 1, 0 },
                        { 1, 0, 0 },
                        { 1, 1, 0 } };

  // Checking IC 7402
  for (int cir_i = 0; cir_i < 4; cir_i++) {
    int check = 1;
    
    for (int tt_i = 0; tt_i < 4; tt_i++) {
      digitalWrite(ip_pins[cir_i][0], tt_7402[tt_i][0]);

      digitalWrite(ip_pins[cir_i][1], tt_7402[tt_i][1]);

      if (digitalRead(op_pins[cir_i]) != tt_7402[tt_i][2]) {
        check = 0;
      }
    }
    flag += check;
    if (check) {
      message = "Gate on pin " + String(getOriginalPin(ip_pins[cir_i][0], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][1], total_pins)) + " and " + String(getOriginalPin(op_pins[cir_i], total_pins)) + " is WORKING.";
    } else {
      message = "Gate on pin " + String(getOriginalPin(ip_pins[cir_i][0], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][1], total_pins)) + " and " + String(getOriginalPin(op_pins[cir_i], total_pins)) + " is NOT WORKING.";
    }
    scrollText(1, message);
    delay(delay_short);
  }
  return flag;
}

int check_7408() {
  check_message = "Checking IC 7408 (Quad 2-Input AND Gate)";
  scrollText(0, check_message);
  delay(delay_long);
  int flag = 0;

  int total_pins = 14;  // Total number of Pins

  int gnd_pin = 7;   // Ground Pin
  int vcc_pin = 14;  // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }
  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);    // Setting pinMode of Ground Pin
  digitalWrite(gnd_pin, LOW);  // Setting Value of Ground Pin

  pinMode(vcc_pin, OUTPUT);     // Setting pinMode of VCC Pin
  digitalWrite(vcc_pin, HIGH);  // Setting Value of VCC Pin

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    //Faulty Ground or VCC pin, Not need to check entire IC.
    error_message = "Ground or VCC pin is not working properly.";
    scrollText(1, error_message);
    delay(delay_long);
    return flag;  //Return 0
  }

  int pin, pin1, pin2;

  int ip_pins[4][2] = { { 1, 2 }, { 4, 5 }, { 10, 9 }, { 13, 12 } };  // Input Pins of IC 7408

  for (pin1 = 0; pin1 < 4; pin1++) {
    for (pin2 = 0; pin2 < 2; pin2++) {
      // Setting Arduino Input Pins
      if (ip_pins[pin1][pin2] >= ((total_pins / 2) + 1)) {
        ip_pins[pin1][pin2] += ((max_pin - total_pins) + start_pin);
      } else {
        ip_pins[pin1][pin2] += start_pin;
      }
      pinMode((ip_pins[pin1][pin2]), OUTPUT);  // Setting pinMode of Input pins
    }
  }

  int op_pins[4] = { 3, 6, 8, 11 };  // Output Pins of IC 7408

  for (pin = 0; pin < 4; pin++) {
    // Setting Arduino Output Pins
    if (op_pins[pin] >= ((total_pins / 2) + 1)) {
      op_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      op_pins[pin] += start_pin;
    }
    pinMode((op_pins[pin]), INPUT);  // Setting pinmode of Output pins
  }

  // Truth Table of IC 7408
  int tt_7408[4][3] = { { 0, 0, 0 },
                        { 0, 1, 0 },
                        { 1, 0, 0 },
                        { 1, 1, 1 } };

  // Checking IC 7408
  for (int cir_i = 0; cir_i < 4; cir_i++) {
    int check = 1;
    
    for (int tt_i = 0; tt_i < 4; tt_i++) {
      digitalWrite(ip_pins[cir_i][0], tt_7408[tt_i][0]);

      digitalWrite(ip_pins[cir_i][1], tt_7408[tt_i][1]);

      if (digitalRead(op_pins[cir_i]) != tt_7408[tt_i][2]) {
        check = 0;
      }
    }
    flag += check;
    if (check) {
      message = "Gate on pin " + String(getOriginalPin(ip_pins[cir_i][0], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][1], total_pins)) + " and " + String(getOriginalPin(op_pins[cir_i], total_pins)) + " is WORKING.";
    } else {
      message = "Gate on pin " + String(getOriginalPin(ip_pins[cir_i][0], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][1], total_pins)) + " and " + String(getOriginalPin(op_pins[cir_i], total_pins)) + " is NOT WORKING.";
    }
    scrollText(1, message);
    delay(delay_short);
  }
  return flag;
}

int check_7428() {
  check_message = "Checking IC 7428 (Quad 2-Input NOR Gate)";
  scrollText(0, check_message);
  delay(delay_long);

  int flag = 0;

  int total_pins = 14;  // Total number of Pins

  int gnd_pin = 7;   // Ground Pin
  int vcc_pin = 14;  // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }
  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);    // Setting pinMode of Ground Pin
  digitalWrite(gnd_pin, LOW);  // Setting Value of Ground Pin

  pinMode(vcc_pin, OUTPUT);     // Setting pinMode of VCC Pin
  digitalWrite(vcc_pin, HIGH);  // Setting Value of VCC Pin

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    //Faulty Ground or VCC pin, Not need to check entire IC.
    error_message = "Ground or VCC pin is not working properly.";
    scrollText(1, error_message);
    delay(delay_long);
    return flag;  //Return 0
  }

  int pin, pin1, pin2;

  int ip_pins[4][2] = { { 2, 3 }, { 5, 6 }, { 8, 9 }, { 11, 12 } };  // Input Pins of IC 7428

  for (pin1 = 0; pin1 < 4; pin1++) {
    for (pin2 = 0; pin2 < 2; pin2++) {
      // Setting Arduino Input Pins
      if (ip_pins[pin1][pin2] >= ((total_pins / 2) + 1)) {
        ip_pins[pin1][pin2] += ((max_pin - total_pins) + start_pin);
      } else {
        ip_pins[pin1][pin2] += start_pin;
      }
      pinMode((ip_pins[pin1][pin2]), OUTPUT);  // Setting pinMode of Input pins
    }
  }

  int op_pins[4] = { 1, 4, 10, 13 };  // Output Pins of IC 7428

  for (pin = 0; pin < 4; pin++) {
    // Setting Arduino Output Pins
    if (op_pins[pin] >= ((total_pins / 2) + 1)) {
      op_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      op_pins[pin] += start_pin;
    }
    pinMode((op_pins[pin]), INPUT);  // Setting pinmode of Output pins
  }

  // Truth Table of IC 7428
  int tt_7428[4][3] = { { 0, 0, 1 },
                        { 0, 1, 0 },
                        { 1, 0, 0 },
                        { 1, 1, 0 } };

  // Checking IC 7428
  for (int cir_i = 0; cir_i < 4; cir_i++) {
    int check = 1;
    
    for (int tt_i = 0; tt_i < 4; tt_i++) {
      digitalWrite(ip_pins[cir_i][0], tt_7428[tt_i][0]);

      digitalWrite(ip_pins[cir_i][1], tt_7428[tt_i][1]);

      if (digitalRead(op_pins[cir_i]) != tt_7428[tt_i][2]) {
        check = 0;
      }
    }
    flag += check;
    if (check) {
      message = "Gate on pin " + String(getOriginalPin(ip_pins[cir_i][0], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][1], total_pins)) + " and " + String(getOriginalPin(op_pins[cir_i], total_pins)) + " is WORKING.";
    } else {
      message = "Gate on pin " + String(getOriginalPin(ip_pins[cir_i][0], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][1], total_pins)) + " and " + String(getOriginalPin(op_pins[cir_i], total_pins)) + " is NOT WORKING.";
    }
    scrollText(1, message);
    delay(delay_short);
  }
  return flag;
}

int check_7432() {
  check_message = "Checking IC 7432 (Quad 2-Input OR Gate)";
  scrollText(0, check_message);
  delay(delay_long);

  int flag = 0;

  int total_pins = 14;  // Total number of Pins

  int gnd_pin = 7;   // Ground Pin
  int vcc_pin = 14;  // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }
  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);    // Setting pinMode of Ground Pin
  digitalWrite(gnd_pin, LOW);  // Setting Value of Ground Pin

  pinMode(vcc_pin, OUTPUT);     // Setting pinMode of VCC Pin
  digitalWrite(vcc_pin, HIGH);  // Setting Value of VCC Pin

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    //Faulty Ground or VCC pin, Not need to check entire IC.
    error_message = "Ground or VCC pin is not working properly.";
    scrollText(1, error_message);
    delay(delay_long);
    return flag;  //Return 0
  }

  int pin, pin1, pin2;

  int ip_pins[4][2] = { { 1, 2 }, { 4, 5 }, { 10, 9 }, { 13, 12 } };  // Input Pins of IC 7432

  for (pin1 = 0; pin1 < 4; pin1++) {
    for (pin2 = 0; pin2 < 2; pin2++) {
      // Setting Arduino Input Pins
      if (ip_pins[pin1][pin2] >= ((total_pins / 2) + 1)) {
        ip_pins[pin1][pin2] += ((max_pin - total_pins) + start_pin);
      } else {
        ip_pins[pin1][pin2] += start_pin;
      }
      pinMode((ip_pins[pin1][pin2]), OUTPUT);  // Setting pinMode of Input pins
    }
  }

  int op_pins[4] = { 3, 6, 8, 11 };  // Output Pins of IC 7432

  for (pin = 0; pin < 4; pin++) {
    // Setting Arduino Output Pins
    if (op_pins[pin] >= ((total_pins / 2) + 1)) {
      op_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      op_pins[pin] += start_pin;
    }
    pinMode((op_pins[pin]), INPUT);  // Setting pinmode of Output pins
  }

  // Truth Table of IC 7432
  int tt_7432[4][3] = { { 0, 0, 0 },
                        { 0, 1, 1 },
                        { 1, 0, 1 },
                        { 1, 1, 1 } };

  // Checking IC 7432
  for (int cir_i = 0; cir_i < 4; cir_i++) {
    int check = 1;
    
    for (int tt_i = 0; tt_i < 4; tt_i++) {
      digitalWrite(ip_pins[cir_i][0], tt_7432[tt_i][0]);

      digitalWrite(ip_pins[cir_i][1], tt_7432[tt_i][1]);

      if (digitalRead(op_pins[cir_i]) != tt_7432[tt_i][2]) {
        check = 0;
      }
    }
    flag += check;
    if (check) {
      message = "Gate on pin " + String(getOriginalPin(ip_pins[cir_i][0], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][1], total_pins)) + " and " + String(getOriginalPin(op_pins[cir_i], total_pins)) + " is WORKING.";
    } else {
      message = "Gate on pin " + String(getOriginalPin(ip_pins[cir_i][0], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][1], total_pins)) + " and " + String(getOriginalPin(op_pins[cir_i], total_pins)) + " is NOT WORKING.";
    }
    scrollText(1, message);
    delay(delay_short);
  }
  return flag;
}

int check_7486() {
  check_message = "Checking IC 7486 (Quad 2-Input XOR Gate)";
  scrollText(0, check_message);
  delay(delay_long);

  int flag = 0;

  int gnd_pin = 7;   // Ground Pin
  int vcc_pin = 14;  // VCC Pin

  int total_pins = 14;  // Total number of Pins

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }
  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);    // Setting pinMode of Ground Pin
  digitalWrite(gnd_pin, LOW);  // Setting Value of Ground Pin

  pinMode(vcc_pin, OUTPUT);     // Setting pinMode of VCC Pin
  digitalWrite(vcc_pin, HIGH);  // Setting Value of VCC Pin

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    //Faulty Ground or VCC pin, Not need to check entire IC.
    error_message = "Ground or VCC pin is not working properly.";
    scrollText(1, error_message);
    delay(delay_long);
    return flag;  //Return 0
  }

  int pin, pin1, pin2;

  int ip_pins[4][2] = { { 1, 2 }, { 4, 5 }, { 10, 9 }, { 13, 12 } };  // Input Pins of IC 7486

  for (pin1 = 0; pin1 < 4; pin1++) {
    for (pin2 = 0; pin2 < 2; pin2++) {
      // Setting Arduino Input Pins
      if (ip_pins[pin1][pin2] >= ((total_pins / 2) + 1)) {
        ip_pins[pin1][pin2] += ((max_pin - total_pins) + start_pin);
      } else {
        ip_pins[pin1][pin2] += start_pin;
      }
      pinMode((ip_pins[pin1][pin2]), OUTPUT);  // Setting pinMode of Input pins
    }
  }

  int op_pins[4] = { 3, 6, 8, 11 };  // Output Pins of IC 7486

  for (pin = 0; pin < 4; pin++) {
    // Setting Arduino Output Pins
    if (op_pins[pin] >= ((total_pins / 2) + 1)) {
      op_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      op_pins[pin] += start_pin;
    }
    pinMode((op_pins[pin]), INPUT);  // Setting pinmode of Output pins
  }

  // Truth Table of IC 7486
  int tt_7486[4][3] = { { 0, 0, 0 },
                        { 0, 1, 1 },
                        { 1, 0, 1 },
                        { 1, 1, 0 } };

  // Checking IC 7486
  for (int cir_i = 0; cir_i < 4; cir_i++) {
    int check = 1;
    
    for (int tt_i = 0; tt_i < 4; tt_i++) {
      digitalWrite(ip_pins[cir_i][0], tt_7486[tt_i][0]);

      digitalWrite(ip_pins[cir_i][1], tt_7486[tt_i][1]);

      if (digitalRead(op_pins[cir_i]) != tt_7486[tt_i][2]) {
        check = 0;
      }
    }
    flag += check;
    if (check) {
      message = "Gate on pin " + String(getOriginalPin(ip_pins[cir_i][0], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][1], total_pins)) + " and " + String(getOriginalPin(op_pins[cir_i], total_pins)) + " is WORKING.";
    } else {
      message = "Gate on pin " + String(getOriginalPin(ip_pins[cir_i][0], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][1], total_pins)) + " and " + String(getOriginalPin(op_pins[cir_i], total_pins)) + " is NOT WORKING.";
    }
    scrollText(1, message);
    delay(delay_short);
  }
  return flag;
}

int check_74136() {
  check_message = "Checking IC 74136 (Quad 2-Input XOR Gate)";
  scrollText(0, check_message);
  delay(delay_long);

  int flag = 0;

  int total_pins = 14;  // Total number of Pins

  int gnd_pin = 7;   // Ground Pin
  int vcc_pin = 14;  // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }
  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);    // Setting pinMode of Ground Pin
  digitalWrite(gnd_pin, LOW);  // Setting Value of Ground Pin

  pinMode(vcc_pin, OUTPUT);     // Setting pinMode of VCC Pin
  digitalWrite(vcc_pin, HIGH);  // Setting Value of VCC Pin

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    //Faulty Ground or VCC pin, Not need to check entire IC.
    error_message = "Ground or VCC pin is not working properly.";
    scrollText(1, error_message);
    delay(delay_long);
    return flag;  //Return 0
  }

  int pin, pin1, pin2;

  int ip_pins[4][2] = { { 1, 2 }, { 4, 5 }, { 10, 9 }, { 13, 12 } };  // Input Pins of IC 74136

  for (pin1 = 0; pin1 < 4; pin1++) {
    for (pin2 = 0; pin2 < 2; pin2++) {
      // Setting Arduino Input Pins
      if (ip_pins[pin1][pin2] >= ((total_pins / 2) + 1)) {
        ip_pins[pin1][pin2] += ((max_pin - total_pins) + start_pin);
      } else {
        ip_pins[pin1][pin2] += start_pin;
      }
      pinMode((ip_pins[pin1][pin2]), OUTPUT);  // Setting pinMode of Input pins
    }
  }

  int op_pins[4] = { 3, 6, 8, 11 };  // Output Pins of IC 7486

  for (pin = 0; pin < 4; pin++) {
    // Setting Arduino Output Pins
    if (op_pins[pin] >= ((total_pins / 2) + 1)) {
      op_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      op_pins[pin] += start_pin;
    }
    pinMode((op_pins[pin]), INPUT);  // Setting pinmode of Output pins
  }

  // Truth Table of IC 74136
  int tt_74136[4][3] = { { 0, 0, 0 },
                         { 0, 1, 1 },
                         { 1, 0, 1 },
                         { 1, 1, 0 } };

  // Checking IC 74136
  for (int cir_i = 0; cir_i < 4; cir_i++) {
    int check = 1;
    
    for (int tt_i = 0; tt_i < 4; tt_i++) {
      digitalWrite(ip_pins[cir_i][0], tt_74136[tt_i][0]);

      digitalWrite(ip_pins[cir_i][1], tt_74136[tt_i][1]);

      if (digitalRead(op_pins[cir_i]) != tt_74136[tt_i][2]) {
        check = 0;
      }
    }
    flag += check;
    if (check) {
      message = "Gate on pin " + String(getOriginalPin(ip_pins[cir_i][0], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][1], total_pins)) + " and " + String(getOriginalPin(op_pins[cir_i], total_pins)) + " is WORKING.";
    } else {
      message = "Gate on pin " + String(getOriginalPin(ip_pins[cir_i][0], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][1], total_pins)) + " and " + String(getOriginalPin(op_pins[cir_i], total_pins)) + " is NOT WORKING.";
    }
    scrollText(1, message);
    delay(delay_short);
  }
  return flag;
}

int check_74226() {
  check_message = "Checking IC 74226 (Quad 2-Input XNOR Gate)";
  scrollText(0, check_message);
  delay(delay_long);

  int flag = 0;

  int total_pins = 14;  // Total number of Pins

  int gnd_pin = 7;   // Ground Pin
  int vcc_pin = 14;  // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }
  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);    // Setting pinMode of Ground Pin
  digitalWrite(gnd_pin, LOW);  // Setting Value of Ground Pin

  pinMode(vcc_pin, OUTPUT);     // Setting pinMode of VCC Pin
  digitalWrite(vcc_pin, HIGH);  // Setting Value of VCC Pin

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    //Faulty Ground or VCC pin, Not need to check entire IC.
    error_message = "Ground or VCC pin is not working properly.";
    scrollText(1, error_message);
    delay(delay_long);
    return flag;  //Return 0
  }

  int pin, pin1, pin2;

  int ip_pins[4][2] = { { 1, 2 }, { 4, 5 }, { 10, 9 }, { 13, 12 } };  // Input Pins of IC 74226

  for (pin1 = 0; pin1 < 4; pin1++) {
    for (pin2 = 0; pin2 < 2; pin2++) {
      // Setting Arduino Input Pins
      if (ip_pins[pin1][pin2] >= ((total_pins / 2) + 1)) {
        ip_pins[pin1][pin2] += ((max_pin - total_pins) + start_pin);
      } else {
        ip_pins[pin1][pin2] += start_pin;
      }
      pinMode((ip_pins[pin1][pin2]), OUTPUT);  // Setting pinMode of Input pins
    }
  }

  int op_pins[4] = { 3, 6, 8, 11 };  // Output Pins of IC 74226

  for (pin = 0; pin < 4; pin++) {
    // Setting Arduino Output Pins
    if (op_pins[pin] >= ((total_pins / 2) + 1)) {
      op_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      op_pins[pin] += start_pin;
    }
    pinMode((op_pins[pin]), INPUT);  // Setting pinmode of Output pins
  }

  // Truth Table of IC 7486
  int tt_74226[4][3] = { { 0, 0, 1 },
                         { 0, 1, 0 },
                         { 1, 0, 0 },
                         { 1, 1, 1 } };

  // Checking IC 74226
  for (int cir_i = 0; cir_i < 4; cir_i++) {
    int check = 1;
    
    for (int tt_i = 0; tt_i < 4; tt_i++) {
      digitalWrite(ip_pins[cir_i][0], tt_74226[tt_i][0]);

      digitalWrite(ip_pins[cir_i][1], tt_74226[tt_i][1]);

      if (digitalRead(op_pins[cir_i]) != tt_74226[tt_i][2]) {
        check = 0;
      }
    }
    flag += check;
    if (check) {
      message = "Gate on pin " + String(getOriginalPin(ip_pins[cir_i][0], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][1], total_pins)) + " and " + String(getOriginalPin(op_pins[cir_i], total_pins)) + " is WORKING.";
    } else {
      message = "Gate on pin " + String(getOriginalPin(ip_pins[cir_i][0], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][1], total_pins)) + " and " + String(getOriginalPin(op_pins[cir_i], total_pins)) + " is NOT WORKING.";
    }
    scrollText(1, message);
    delay(delay_short);
  }
  return flag;
}

int check_74266() {
  check_message = "Checking IC 74266 (Quad 2-Input XNOR Gate)";
  scrollText(0, check_message);
  delay(delay_long);

  int flag = 0;

  int total_pins = 14;  // Total number of Pins

  int gnd_pin = 7;   // Ground Pin
  int vcc_pin = 14;  // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }
  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);    // Setting pinMode of Ground Pin
  digitalWrite(gnd_pin, LOW);  // Setting Value of Ground Pin

  pinMode(vcc_pin, OUTPUT);     // Setting pinMode of VCC Pin
  digitalWrite(vcc_pin, HIGH);  // Setting Value of VCC Pin

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    //Faulty Ground or VCC pin, Not need to check entire IC.
    error_message = "Ground or VCC pin is not working properly.";
    scrollText(1, error_message);
    delay(delay_long);
    return flag;  //Return 0
  }

  int pin, pin1, pin2;

  int ip_pins[4][2] = { { 1, 2 }, { 4, 5 }, { 10, 9 }, { 13, 12 } };  // Input Pins of IC 74266

  for (pin1 = 0; pin1 < 4; pin1++) {
    for (pin2 = 0; pin2 < 2; pin2++) {
      // Setting Arduino Input Pins
      if (ip_pins[pin1][pin2] >= ((total_pins / 2) + 1)) {
        ip_pins[pin1][pin2] += ((max_pin - total_pins) + start_pin);
      } else {
        ip_pins[pin1][pin2] += start_pin;
      }
      pinMode((ip_pins[pin1][pin2]), OUTPUT);  // Setting pinMode of Input pins
    }
  }

  int op_pins[4] = { 3, 6, 8, 11 };  // Output Pins of IC 74266

  for (pin = 0; pin < 4; pin++) {
    // Setting Arduino Output Pins
    if (op_pins[pin] >= ((total_pins / 2) + 1)) {
      op_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      op_pins[pin] += start_pin;
    }
    pinMode((op_pins[pin]), INPUT);  // Setting pinmode of Output pins
  }

  // Truth Table of IC 74266
  int tt_74266[4][3] = { { 0, 0, 1 },
                         { 0, 1, 0 },
                         { 1, 0, 0 },
                         { 1, 1, 1 } };

  // Checking IC 74266
  for (int cir_i = 0; cir_i < 4; cir_i++) {
    int check = 1;
    
    for (int tt_i = 0; tt_i < 4; tt_i++) {
      digitalWrite(ip_pins[cir_i][0], tt_74266[tt_i][0]);

      digitalWrite(ip_pins[cir_i][1], tt_74266[tt_i][1]);

      if (digitalRead(op_pins[cir_i]) != tt_74266[tt_i][2]) {
        check = 0;
      }
    }
    flag += check;
    if (check) {
      message = "Gate on pin " + String(getOriginalPin(ip_pins[cir_i][0], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][1], total_pins)) + " and " + String(getOriginalPin(op_pins[cir_i], total_pins)) + " is WORKING.";
    } else {
      message = "Gate on pin " + String(getOriginalPin(ip_pins[cir_i][0], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][1], total_pins)) + " and " + String(getOriginalPin(op_pins[cir_i], total_pins)) + " is NOT WORKING.";
    }
    scrollText(1, message);
    delay(delay_short);
  }
  return flag;
}

// 3-input Logic Gates
int check_7410() {
  check_message = "Checking IC 7410 (Triple 3-Input NAND Gate)";
  scrollText(0, check_message);
  delay(delay_long);

  int flag = 0;

  int total_pins = 14;  // Total number of Pins

  int gnd_pin = 7;   // Ground Pin
  int vcc_pin = 14;  // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }
  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);    // Setting pinMode of Ground Pin
  digitalWrite(gnd_pin, LOW);  // Setting Value of Ground Pin

  pinMode(vcc_pin, OUTPUT);     // Setting pinMode of VCC Pin
  digitalWrite(vcc_pin, HIGH);  // Setting Value of VCC Pin

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    //Faulty Ground or VCC pin, Not need to check entire IC.
    error_message = "Ground or VCC pin is not working properly.";
    scrollText(1, error_message);
    delay(delay_long);
    return flag;  //Return 0
  }

  int pin, pin1, pin2;

  int ip_pins[3][3] = { { 1, 2, 13 }, { 3, 4, 5 }, { 11, 10, 9 } };  // Input Pins of IC 7410

  for (pin1 = 0; pin1 < 3; pin1++) {
    for (pin2 = 0; pin2 < 3; pin2++) {
      // Setting Arduino Input Pins
      if (ip_pins[pin1][pin2] >= ((total_pins / 2) + 1)) {
        ip_pins[pin1][pin2] += ((max_pin - total_pins) + start_pin);
      } else {
        ip_pins[pin1][pin2] += start_pin;
      }
      pinMode((ip_pins[pin1][pin2]), OUTPUT);  // Setting pinmode of Input pins
    }
  }

  int op_pins[3] = { 12, 6, 8 };  // Output Pins of IC 7410

  for (pin = 0; pin < 3; pin++) {
    // Setting Arduino Output Pins
    if (op_pins[pin] >= ((total_pins / 2) + 1)) {
      op_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      op_pins[pin] += start_pin;
    }
    pinMode((op_pins[pin]), INPUT);  // Setting pinmode of Output pins
  }

  // Truth Table of IC 7410
  int tt_7410[8][4] = { { 0, 0, 0, 1 },
                        { 0, 0, 1, 1 },
                        { 0, 1, 0, 1 },
                        { 0, 1, 1, 1 },
                        { 1, 0, 0, 1 },
                        { 1, 0, 1, 1 },
                        { 1, 1, 0, 1 },
                        { 1, 1, 1, 0 } };

  // Checking IC 7410
  for (int cir_i = 0; cir_i < 3; cir_i++) {
    int check = 1;
    
    for (int tt_i = 0; tt_i < 8; tt_i++) {
      digitalWrite(ip_pins[cir_i][0], tt_7410[tt_i][0]);

      digitalWrite(ip_pins[cir_i][1], tt_7410[tt_i][1]);

      digitalWrite(ip_pins[cir_i][2], tt_7410[tt_i][2]);

      if (digitalRead(op_pins[cir_i]) != tt_7410[tt_i][3]) {
        check = 0;
        break;
      }
    }
    flag += check;
    if (check) {
      message = "Gate on pin " + String(getOriginalPin(ip_pins[cir_i][0], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][1], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][2], total_pins)) + " and " + String(getOriginalPin(op_pins[cir_i], total_pins)) + " is WORKING.";
    } else {
      message = "Gate on pin " + String(getOriginalPin(ip_pins[cir_i][0], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][1], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][2], total_pins)) + " and " + String(getOriginalPin(op_pins[cir_i], total_pins)) + " is NOT WORKING.";
    }
    scrollText(1, message);
    delay(delay_short);
  }
  return flag;
}

int check_7411() {
  check_message = "Checking IC 7411 (Triple 3-Input AND Gate)";
  scrollText(0, check_message);
  delay(delay_long);

  int flag = 0;

  int total_pins = 14;  // Total number of Pins

  int gnd_pin = 7;   // Ground Pin
  int vcc_pin = 14;  // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }
  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);    // Setting pinMode of Ground Pin
  digitalWrite(gnd_pin, LOW);  // Setting Value of Ground Pin

  pinMode(vcc_pin, OUTPUT);     // Setting pinMode of VCC Pin
  digitalWrite(vcc_pin, HIGH);  // Setting Value of VCC Pin

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    //Faulty Ground or VCC pin, Not need to check entire IC.
    error_message = "Ground or VCC pin is not working properly.";
    scrollText(1, error_message);
    delay(delay_long);
    return flag;  //Return 0
  }

  int pin, pin1, pin2;

  int ip_pins[3][3] = { { 1, 2, 13 }, { 3, 4, 5 }, { 11, 10, 9 } };  // Input Pins of IC 7411

  for (pin1 = 0; pin1 < 3; pin1++) {
    for (pin2 = 0; pin2 < 3; pin2++) {
      // Setting Arduino Input Pins
      if (ip_pins[pin1][pin2] >= ((total_pins / 2) + 1)) {
        ip_pins[pin1][pin2] += ((max_pin - total_pins) + start_pin);
      } else {
        ip_pins[pin1][pin2] += start_pin;
      }
      pinMode((ip_pins[pin1][pin2]), OUTPUT);  // Setting pinmode of Input pins
    }
  }

  int op_pins[3] = { 12, 6, 8 };  // Output Pins of IC 7411

  for (pin = 0; pin < 3; pin++) {
    // Setting Arduino Output Pins
    if (op_pins[pin] >= ((total_pins / 2) + 1)) {
      op_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      op_pins[pin] += start_pin;
    }
    pinMode((op_pins[pin]), INPUT);  // Setting pinmode of Output pins
  }

  // Truth Table of IC 7411
  int tt_7411[8][4] = { { 0, 0, 0, 0 },
                        { 0, 0, 1, 0 },
                        { 0, 1, 0, 0 },
                        { 0, 1, 1, 0 },
                        { 1, 0, 0, 0 },
                        { 1, 0, 1, 0 },
                        { 1, 1, 0, 0 },
                        { 1, 1, 1, 1 } };

  // Checking IC 7411
  for (int cir_i = 0; cir_i < 3; cir_i++) {
    int check = 1;
    
    for (int tt_i = 0; tt_i < 8; tt_i++) {
      digitalWrite(ip_pins[cir_i][0], tt_7411[tt_i][0]);

      digitalWrite(ip_pins[cir_i][1], tt_7411[tt_i][1]);

      digitalWrite(ip_pins[cir_i][2], tt_7411[tt_i][2]);

      if (digitalRead(op_pins[cir_i]) != tt_7411[tt_i][3]) {
        check = 0;
        break;
      }
    }
    flag += check;
    if (check) {
      message = "Gate on pin " + String(getOriginalPin(ip_pins[cir_i][0], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][1], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][2], total_pins)) + " and " + String(getOriginalPin(op_pins[cir_i], total_pins)) + " is WORKING.";
    } else {
      message = "Gate on pin " + String(getOriginalPin(ip_pins[cir_i][0], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][1], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][2], total_pins)) + " and " + String(getOriginalPin(op_pins[cir_i], total_pins)) + " is NOT WORKING.";
    }
    scrollText(1, message);
    delay(delay_short);
  }
  return flag;
}

int check_7412() {
  check_message = "Checking IC 7412 (Triple 3-Input NAND Gate)";
  scrollText(0, check_message);
  delay(delay_long);

  int flag = 0;

  int total_pins = 14;  // Total number of Pins

  int gnd_pin = 7;   // Ground Pin
  int vcc_pin = 14;  // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }
  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);    // Setting pinMode of Ground Pin
  digitalWrite(gnd_pin, LOW);  // Setting Value of Ground Pin

  pinMode(vcc_pin, OUTPUT);     // Setting pinMode of VCC Pin
  digitalWrite(vcc_pin, HIGH);  // Setting Value of VCC Pin

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    //Faulty Ground or VCC pin, Not need to check entire IC.
    error_message = "Ground or VCC pin is not working properly.";
    scrollText(1, error_message);
    delay(delay_long);
    return flag;  //Return 0
  }

  int pin, pin1, pin2;

  int ip_pins[3][3] = { { 1, 2, 13 }, { 3, 4, 5 }, { 11, 10, 9 } };  // Input Pins of IC 7412

  for (pin1 = 0; pin1 < 3; pin1++) {
    for (pin2 = 0; pin2 < 3; pin2++) {
      // Setting Arduino Input Pins
      if (ip_pins[pin1][pin2] >= ((total_pins / 2) + 1)) {
        ip_pins[pin1][pin2] += ((max_pin - total_pins) + start_pin);
      } else {
        ip_pins[pin1][pin2] += start_pin;
      }
      pinMode((ip_pins[pin1][pin2]), OUTPUT);  // Setting pinmode of Input pins
    }
  }

  int op_pins[3] = { 12, 6, 8 };  // Output Pins of IC 7412

  for (pin = 0; pin < 3; pin++) {
    // Setting Arduino Output Pins
    if (op_pins[pin] >= ((total_pins / 2) + 1)) {
      op_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      op_pins[pin] += start_pin;
    }
    pinMode((op_pins[pin]), INPUT);  // Setting pinmode of Output pins
  }

  // Truth Table of IC 7412
  int tt_7412[8][4] = { { 0, 0, 0, 1 },
                        { 0, 0, 1, 1 },
                        { 0, 1, 0, 1 },
                        { 0, 1, 1, 1 },
                        { 1, 0, 0, 1 },
                        { 1, 0, 1, 1 },
                        { 1, 1, 0, 1 },
                        { 1, 1, 1, 0 } };

  // Checking IC 7412
  for (int cir_i = 0; cir_i < 3; cir_i++) {
    int check = 1;
    
    for (int tt_i = 0; tt_i < 8; tt_i++) {
      digitalWrite(ip_pins[cir_i][0], tt_7412[tt_i][0]);

      digitalWrite(ip_pins[cir_i][1], tt_7412[tt_i][1]);

      digitalWrite(ip_pins[cir_i][2], tt_7412[tt_i][2]);

      if (digitalRead(op_pins[cir_i]) != tt_7412[tt_i][3]) {
        check = 0;
        break;
      }
    }
    flag += check;
    if (check) {
      message = "Gate on pin " + String(getOriginalPin(ip_pins[cir_i][0], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][1], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][2], total_pins)) + " and " + String(getOriginalPin(op_pins[cir_i], total_pins)) + " is WORKING.";
    } else {
      message = "Gate on pin " + String(getOriginalPin(ip_pins[cir_i][0], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][1], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][2], total_pins)) + " and " + String(getOriginalPin(op_pins[cir_i], total_pins)) + " is NOT WORKING.";
    }
    scrollText(1, message);
    delay(delay_short);
  }
  return flag;
}

int check_7415() {
  check_message = "Checking IC 7411 (Triple 3-Input AND Gate)";
  scrollText(0, check_message);
  delay(delay_long);

  int flag = 0;

  int total_pins = 14;  // Total number of Pins

  int gnd_pin = 7;   // Ground Pin
  int vcc_pin = 14;  // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }
  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);    // Setting pinMode of Ground Pin
  digitalWrite(gnd_pin, LOW);  // Setting Value of Ground Pin

  pinMode(vcc_pin, OUTPUT);     // Setting pinMode of VCC Pin
  digitalWrite(vcc_pin, HIGH);  // Setting Value of VCC Pin

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    //Faulty Ground or VCC pin, Not need to check entire IC.
    error_message = "Ground or VCC pin is not working properly.";
    scrollText(1, error_message);
    delay(delay_long);
    return flag;  //Return 0
  }

  int pin, pin1, pin2;

  int ip_pins[3][3] = { { 1, 2, 13 }, { 3, 4, 5 }, { 11, 10, 9 } };  // Input Pins of IC 7415

  for (pin1 = 0; pin1 < 3; pin1++) {
    for (pin2 = 0; pin2 < 3; pin2++) {
      // Setting Arduino Input Pins
      if (ip_pins[pin1][pin2] >= ((total_pins / 2) + 1)) {
        ip_pins[pin1][pin2] += ((max_pin - total_pins) + start_pin);
      } else {
        ip_pins[pin1][pin2] += start_pin;
      }
      pinMode((ip_pins[pin1][pin2]), OUTPUT);  // Setting pinmode of Input pins
    }
  }

  int op_pins[3] = { 12, 6, 8 };  // Output Pins of IC 7415

  for (pin = 0; pin < 3; pin++) {
    // Setting Arduino Output Pins
    if (op_pins[pin] >= ((total_pins / 2) + 1)) {
      op_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      op_pins[pin] += start_pin;
    }
    pinMode((op_pins[pin]), INPUT);  // Setting pinmode of Output pins
  }

  // Truth Table of IC 7415
  int tt_7415[8][4] = { { 0, 0, 0, 0 },
                        { 0, 0, 1, 0 },
                        { 0, 1, 0, 0 },
                        { 0, 1, 1, 0 },
                        { 1, 0, 0, 0 },
                        { 1, 0, 1, 0 },
                        { 1, 1, 0, 0 },
                        { 1, 1, 1, 1 } };

  // Checking IC 7415
  for (int cir_i = 0; cir_i < 3; cir_i++) {
    int check = 1;
    
    for (int tt_i = 0; tt_i < 8; tt_i++) {
      digitalWrite(ip_pins[cir_i][0], tt_7415[tt_i][0]);

      digitalWrite(ip_pins[cir_i][1], tt_7415[tt_i][1]);

      digitalWrite(ip_pins[cir_i][2], tt_7415[tt_i][2]);

      if (digitalRead(op_pins[cir_i]) != tt_7415[tt_i][3]) {
        check = 0;
        break;
      }
    }
    flag += check;
    if (check) {
      message = "Gate on pin " + String(getOriginalPin(ip_pins[cir_i][0], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][1], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][2], total_pins)) + " and " + String(getOriginalPin(op_pins[cir_i], total_pins)) + " is WORKING.";
    } else {
      message = "Gate on pin " + String(getOriginalPin(ip_pins[cir_i][0], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][1], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][2], total_pins)) + " and " + String(getOriginalPin(op_pins[cir_i], total_pins)) + " is NOT WORKING.";
    }
    scrollText(1, message);
    delay(delay_short);
  }
  return flag;
}

int check_7427() {
  check_message = "Checking IC 7427 (Triple 3-Input NOR Gate)";
  scrollText(0, check_message);
  delay(delay_long);

  int flag = 0;

  int total_pins = 14;  // Total number of Pins

  int gnd_pin = 7;   // Ground Pin
  int vcc_pin = 14;  // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }
  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);    // Setting pinMode of Ground Pin
  digitalWrite(gnd_pin, LOW);  // Setting Value of Ground Pin

  pinMode(vcc_pin, OUTPUT);     // Setting pinMode of VCC Pin
  digitalWrite(vcc_pin, HIGH);  // Setting Value of VCC Pin

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    //Faulty Ground or VCC pin, Not need to check entire IC.
    error_message = "Ground or VCC pin is not working properly.";
    scrollText(1, error_message);
    delay(delay_long);
    return flag;  //Return 0
  }

  int pin, pin1, pin2;

  int ip_pins[3][3] = { { 1, 2, 13 }, { 3, 4, 5 }, { 11, 10, 9 } };  // Input Pins of IC 7427

  for (pin1 = 0; pin1 < 3; pin1++) {
    for (pin2 = 0; pin2 < 3; pin2++) {
      // Setting Arduino Input Pins
      if (ip_pins[pin1][pin2] >= ((total_pins / 2) + 1)) {
        ip_pins[pin1][pin2] += ((max_pin - total_pins) + start_pin);
      } else {
        ip_pins[pin1][pin2] += start_pin;
      }
      pinMode((ip_pins[pin1][pin2]), OUTPUT);  // Setting pinmode of Input pins
    }
  }

  int op_pins[3] = { 12, 6, 8 };  // Output Pins of IC 7427

  for (pin = 0; pin < 3; pin++) {
    // Setting Arduino Output Pins
    if (op_pins[pin] >= ((total_pins / 2) + 1)) {
      op_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      op_pins[pin] += start_pin;
    }
    pinMode((op_pins[pin]), INPUT);  // Setting pinmode of Output pins
  }

  // Truth Table of IC 7427
  int tt_7427[8][4] = { { 0, 0, 0, 1 },
                        { 0, 0, 1, 0 },
                        { 0, 1, 0, 0 },
                        { 0, 1, 1, 0 },
                        { 1, 0, 0, 0 },
                        { 1, 0, 1, 0 },
                        { 1, 1, 0, 0 },
                        { 1, 1, 1, 0 } };

  // Checking IC 7410
  for (int cir_i = 0; cir_i < 3; cir_i++) {
    int check = 1;
    
    for (int tt_i = 0; tt_i < 8; tt_i++) {
      digitalWrite(ip_pins[cir_i][0], tt_7427[tt_i][0]);

      digitalWrite(ip_pins[cir_i][1], tt_7427[tt_i][1]);

      digitalWrite(ip_pins[cir_i][2], tt_7427[tt_i][2]);

      if (digitalRead(op_pins[cir_i]) != tt_7427[tt_i][3]) {
        check = 0;
        break;
      }
    }
    flag += check;
    if (check) {
      message = "Gate on pin " + String(getOriginalPin(ip_pins[cir_i][0], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][1], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][2], total_pins)) + " and " + String(getOriginalPin(op_pins[cir_i], total_pins)) + " is WORKING.";
    } else {
      message = "Gate on pin " + String(getOriginalPin(ip_pins[cir_i][0], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][1], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][2], total_pins)) + " and " + String(getOriginalPin(op_pins[cir_i], total_pins)) + " is NOT WORKING.";
    }
    scrollText(1, message);
    delay(delay_short);
  }
  return flag;
}

// 4-input Logic Gates
int check_7420() {
  check_message = "Checking IC 7420 (Dual 4-Input NAND Gate)";
  scrollText(0, check_message);
  delay(delay_long);

  int flag = 0;

  int total_pins = 14;  // Total number of Pins

  int gnd_pin = 7;   // Ground Pin
  int vcc_pin = 14;  // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }
  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);    // Setting pinMode of Ground Pin
  digitalWrite(gnd_pin, LOW);  // Setting Value of Ground Pin

  pinMode(vcc_pin, OUTPUT);     // Setting pinMode of VCC Pin
  digitalWrite(vcc_pin, HIGH);  // Setting Value of VCC Pin

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    //Faulty Ground or VCC pin, Not need to check entire IC.
    error_message = "Ground or VCC pin is not working properly.";
    scrollText(1, error_message);
    delay(delay_long);
    return flag;  //Return 0
  }

  int strobe1_pin = 3;   // Strobe1  Pin
  int strobe2_pin = 11;  // Strobe2 Pin

  // Setting Arduino strobe1 pin
  if (strobe1_pin >= ((total_pins / 2) + 1)) {
    strobe1_pin += ((max_pin - total_pins) + start_pin);
  } else {
    strobe1_pin += start_pin;
  }

  // Setting Arduino strobe2 pin
  if (strobe2_pin >= ((total_pins / 2) + 1)) {
    strobe2_pin += ((max_pin - total_pins) + start_pin);
  } else {
    strobe2_pin += start_pin;
  }
  pinMode(strobe1_pin, OUTPUT);    // Setting pinMode of strobe1 Pin
  digitalWrite(strobe1_pin, LOW);  // Setting Value of strobe1 Pin

  pinMode(strobe2_pin, OUTPUT);    // Setting pinMode of strobe2 Pin
  digitalWrite(strobe2_pin, LOW);  // Setting Value of strobe2 Pin

  int pin, pin1, pin2;

  int ip_pins[2][4] = { { 1, 2, 4, 5 }, { 13, 12, 10, 9 } };  // Input Pins of IC 7420

  for (pin1 = 0; pin1 < 2; pin1++) {
    for (pin2 = 0; pin2 < 4; pin2++) {
      // Setting Arduino Input Pins
      if (ip_pins[pin1][pin2] >= ((total_pins / 2) + 1)) {
        ip_pins[pin1][pin2] += ((max_pin - total_pins) + start_pin);
      } else {
        ip_pins[pin1][pin2] += start_pin;
      }
      pinMode((ip_pins[pin1][pin2]), OUTPUT);  // Setting pinmode of Input pins
    }
  }

  int op_pins[2] = { 6, 8 };  // Output Pins of IC 7420

  for (pin = 0; pin < 2; pin++) {
    // Setting Arduino Output Pins
    if (op_pins[pin] >= ((total_pins / 2) + 1)) {
      op_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      op_pins[pin] += start_pin;
    }
    pinMode((op_pins[pin]), INPUT);  // Setting pinmode of Output pins
  }

  // Truth Table of IC 7420
  int tt_7420[16][5] = { { 0, 0, 0, 0, 1 },
                         { 0, 0, 0, 1, 1 },
                         { 0, 0, 1, 0, 1 },
                         { 0, 0, 1, 1, 1 },
                         { 0, 1, 0, 0, 1 },
                         { 0, 1, 0, 1, 1 },
                         { 0, 1, 1, 0, 1 },
                         { 0, 1, 1, 1, 1 },
                         { 1, 0, 0, 0, 1 },
                         { 1, 0, 0, 1, 1 },
                         { 1, 0, 1, 0, 1 },
                         { 1, 0, 1, 1, 1 },
                         { 1, 1, 0, 0, 1 },
                         { 1, 1, 0, 1, 1 },
                         { 1, 1, 1, 0, 1 },
                         { 1, 1, 1, 1, 0 } };
  // Checking IC 7420
  for (int cir_i = 0; cir_i < 2; cir_i++) {
    int check = 1;
    
    for (int tt_i = 0; tt_i < 16; tt_i++) {
      digitalWrite(ip_pins[cir_i][0], tt_7420[tt_i][0]);

      digitalWrite(ip_pins[cir_i][1], tt_7420[tt_i][1]);

      digitalWrite(ip_pins[cir_i][2], tt_7420[tt_i][2]);

      digitalWrite(ip_pins[cir_i][3], tt_7420[tt_i][3]);

      if (digitalRead(op_pins[cir_i]) != tt_7420[tt_i][4]) {
        check = 0;
        break;
      }
    }
    flag += check;
    if (check) {
      message = "Gate on pin " + String(getOriginalPin(ip_pins[cir_i][0], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][1], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][2], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][3], total_pins)) + " and " + String(getOriginalPin(op_pins[cir_i], total_pins)) + " is WORKING.";
    } else {
      message = "Gate on pin " + String(getOriginalPin(ip_pins[cir_i][0], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][1], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][2], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][3], total_pins)) + " and " + String(getOriginalPin(op_pins[cir_i], total_pins)) + " is NOT WORKING.";
    }
    scrollText(1, message);
    delay(delay_short);
  }
  return flag;
}

int check_7421() {
  check_message = "Checking IC 7421 (Dual 4-Input AND Gate)";
  scrollText(0, check_message);
  delay(delay_long);

  int flag = 0;

  int total_pins = 14;  // Total number of Pins

  int gnd_pin = 7;   // Ground Pin
  int vcc_pin = 14;  // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }
  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);    // Setting pinMode of Ground Pin
  digitalWrite(gnd_pin, LOW);  // Setting Value of Ground Pin

  pinMode(vcc_pin, OUTPUT);     // Setting pinMode of VCC Pin
  digitalWrite(vcc_pin, HIGH);  // Setting Value of VCC Pin

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    //Faulty Ground or VCC pin, Not need to check entire IC.
    error_message = "Ground or VCC pin is not working properly.";
    scrollText(1, error_message);
    delay(delay_long);
    return flag;  //Return 0
  }

  int strobe1_pin = 3;   // Strobe1  Pin
  int strobe2_pin = 11;  // Strobe2 Pin

  // Setting Arduino strobe1 pin
  if (strobe1_pin >= ((total_pins / 2) + 1)) {
    strobe1_pin += ((max_pin - total_pins) + start_pin);
  } else {
    strobe1_pin += start_pin;
  }

  // Setting Arduino strobe2 pin
  if (strobe2_pin >= ((total_pins / 2) + 1)) {
    strobe2_pin += ((max_pin - total_pins) + start_pin);
  } else {
    strobe2_pin += start_pin;
  }
  pinMode(strobe1_pin, OUTPUT);    // Setting pinMode of strobe1 Pin
  digitalWrite(strobe1_pin, LOW);  // Setting Value of strobe1 Pin

  pinMode(strobe2_pin, OUTPUT);    // Setting pinMode of strobe2 Pin
  digitalWrite(strobe2_pin, LOW);  // Setting Value of strobe2 Pin

  int pin, pin1, pin2;

  int ip_pins[2][4] = { { 1, 2, 4, 5 }, { 13, 12, 10, 9 } };  // Input Pins of IC 7421

  for (pin1 = 0; pin1 < 2; pin1++) {
    for (pin2 = 0; pin2 < 4; pin2++) {
      // Setting Arduino Input Pins
      if (ip_pins[pin1][pin2] >= ((total_pins / 2) + 1)) {
        ip_pins[pin1][pin2] += ((max_pin - total_pins) + start_pin);
      } else {
        ip_pins[pin1][pin2] += start_pin;
      }
      pinMode((ip_pins[pin1][pin2]), OUTPUT);  // Setting pinmode of Input pins
    }
  }

  int op_pins[2] = { 6, 8 };  // Output Pins of IC 7421

  for (pin = 0; pin < 2; pin++) {
    // Setting Arduino Output Pins
    if (op_pins[pin] >= ((total_pins / 2) + 1)) {
      op_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      op_pins[pin] += start_pin;
    }
    pinMode((op_pins[pin]), INPUT);  // Setting pinmode of Output pins
  }

  // Truth Table of IC 7421
  int tt_7421[16][5] = { { 0, 0, 0, 0, 0 },
                         { 0, 0, 0, 1, 0 },
                         { 0, 0, 1, 0, 0 },
                         { 0, 0, 1, 1, 0 },
                         { 0, 1, 0, 0, 0 },
                         { 0, 1, 0, 1, 0 },
                         { 0, 1, 1, 0, 0 },
                         { 0, 1, 1, 1, 0 },
                         { 1, 0, 0, 0, 0 },
                         { 1, 0, 0, 1, 0 },
                         { 1, 0, 1, 0, 0 },
                         { 1, 0, 1, 1, 0 },
                         { 1, 1, 0, 0, 0 },
                         { 1, 1, 0, 1, 0 },
                         { 1, 1, 1, 0, 0 },
                         { 1, 1, 1, 1, 1 } };

  // Checking IC 7421
  for (int cir_i = 0; cir_i < 2; cir_i++) {
    int check = 1;
    
    for (int tt_i = 0; tt_i < 16; tt_i++) {
      digitalWrite(ip_pins[cir_i][0], tt_7421[tt_i][0]);

      digitalWrite(ip_pins[cir_i][1], tt_7421[tt_i][1]);

      digitalWrite(ip_pins[cir_i][2], tt_7421[tt_i][2]);

      digitalWrite(ip_pins[cir_i][3], tt_7421[tt_i][3]);

      if (digitalRead(op_pins[cir_i]) != tt_7421[tt_i][4]) {
        check = 0;
        break;
      }
    }
    flag += check;
    if (check) {
      message = "Gate on pin " + String(getOriginalPin(ip_pins[cir_i][0], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][1], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][2], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][3], total_pins)) + " and " + String(getOriginalPin(op_pins[cir_i], total_pins)) + " is WORKING.";
    } else {
      message = "Gate on pin " + String(getOriginalPin(ip_pins[cir_i][0], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][1], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][2], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][3], total_pins)) + " and " + String(getOriginalPin(op_pins[cir_i], total_pins)) + " is NOT WORKING.";
    }
    scrollText(1, message);
    delay(delay_short);
  }
  return flag;
}

int check_7425() {
  check_message = "Checking IC 7425 (Dual 4-Input NOR Gate)";
  scrollText(0, check_message);
  delay(delay_long);

  int flag = 0;

  int total_pins = 14;  // Total number of Pins

  int gnd_pin = 7;   // Ground Pin
  int vcc_pin = 14;  // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }
  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);    // Setting pinMode of Ground Pin
  digitalWrite(gnd_pin, LOW);  // Setting Value of Ground Pin

  pinMode(vcc_pin, OUTPUT);     // Setting pinMode of VCC Pin
  digitalWrite(vcc_pin, HIGH);  // Setting Value of VCC Pin

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    //Faulty Ground or VCC pin, Not need to check entire IC.
    error_message = "Ground or VCC pin is not working properly.";
    scrollText(1, error_message);
    delay(delay_long);
    return flag;  //Return 0
  }

  int strobe1_pin = 3;   // Strobe1  Pin
  int strobe2_pin = 11;  // Strobe2 Pin

  // Setting Arduino strobe1 pin
  if (strobe1_pin >= ((total_pins / 2) + 1)) {
    strobe1_pin += ((max_pin - total_pins) + start_pin);
  } else {
    strobe1_pin += start_pin;
  }

  // Setting Arduino strobe2 pin
  if (strobe2_pin >= ((total_pins / 2) + 1)) {
    strobe2_pin += ((max_pin - total_pins) + start_pin);
  } else {
    strobe2_pin += start_pin;
  }
  pinMode(strobe1_pin, OUTPUT);    // Setting pinMode of strobe1 Pin
  digitalWrite(strobe1_pin, LOW);  // Setting Value of strobe1 Pin

  pinMode(strobe2_pin, OUTPUT);    // Setting pinMode of strobe2 Pin
  digitalWrite(strobe2_pin, LOW);  // Setting Value of strobe2 Pin

  int pin, pin1, pin2;

  int ip_pins[2][4] = { { 1, 2, 4, 5 }, { 13, 12, 10, 9 } };  // Input Pins of IC 7425

  for (pin1 = 0; pin1 < 2; pin1++) {
    for (pin2 = 0; pin2 < 4; pin2++) {
      // Setting Arduino Input Pins
      if (ip_pins[pin1][pin2] >= ((total_pins / 2) + 1)) {
        ip_pins[pin1][pin2] += ((max_pin - total_pins) + start_pin);
      } else {
        ip_pins[pin1][pin2] += start_pin;
      }
      pinMode((ip_pins[pin1][pin2]), OUTPUT);  // Setting pinmode of Input pins
    }
  }

  int op_pins[2] = { 6, 8 };  // Output Pins of IC 7425

  for (pin = 0; pin < 2; pin++) {
    // Setting Arduino Output Pins
    if (op_pins[pin] >= ((total_pins / 2) + 1)) {
      op_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      op_pins[pin] += start_pin;
    }
    pinMode((op_pins[pin]), INPUT);  // Setting pinmode of Output pins
  }

  // Truth Table of IC 7425
  int tt_7425[16][5] = { { 0, 0, 0, 0, 1 },
                         { 0, 0, 0, 1, 0 },
                         { 0, 0, 1, 0, 0 },
                         { 0, 0, 1, 1, 0 },
                         { 0, 1, 0, 0, 0 },
                         { 0, 1, 0, 1, 0 },
                         { 0, 1, 1, 0, 0 },
                         { 0, 1, 1, 1, 0 },
                         { 1, 0, 0, 0, 0 },
                         { 1, 0, 0, 1, 0 },
                         { 1, 0, 1, 0, 0 },
                         { 1, 0, 1, 1, 0 },
                         { 1, 1, 0, 0, 0 },
                         { 1, 1, 0, 1, 0 },
                         { 1, 1, 1, 0, 0 },
                         { 1, 1, 1, 1, 0 } };

  // Checking IC 7425
  for (int cir_i = 0; cir_i < 2; cir_i++) {
    int check = 1;
    
    for (int tt_i = 0; tt_i < 16; tt_i++) {
      digitalWrite(ip_pins[cir_i][0], tt_7425[tt_i][0]);

      digitalWrite(ip_pins[cir_i][1], tt_7425[tt_i][1]);

      digitalWrite(ip_pins[cir_i][2], tt_7425[tt_i][2]);

      digitalWrite(ip_pins[cir_i][3], tt_7425[tt_i][3]);

      if (digitalRead(op_pins[cir_i]) != tt_7425[tt_i][4]) {
        check = 0;
        break;
      }
    }
    flag += check;
    if (check) {
      message = "Gate on pin " + String(getOriginalPin(ip_pins[cir_i][0], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][1], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][2], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][3], total_pins)) + " and " + String(getOriginalPin(op_pins[cir_i], total_pins)) + " is WORKING.";
    } else {
      message = "Gate on pin " + String(getOriginalPin(ip_pins[cir_i][0], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][1], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][2], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][3], total_pins)) + " and " + String(getOriginalPin(op_pins[cir_i], total_pins)) + " is NOT WORKING.";
    }
    scrollText(1, message);
    delay(delay_short);
  }
  return flag;
}

// Arithmetic Circuits
int check_7482() {
  check_message = "Checking IC 7482 (2-bit Full Adder)";
  scrollText(0, check_message);
  delay(delay_long);

  int flag = 0;
  int total_pins = 14;

  int gnd_pin = 11;  // Ground Pin
  int vcc_pin = 4;   // VCC Pin (corrected, not 5!)

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }

  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);
  digitalWrite(gnd_pin, LOW);

  pinMode(vcc_pin, OUTPUT);
  digitalWrite(vcc_pin, HIGH);

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    //Faulty Ground or VCC pin, Not need to check entire IC.
    error_message = "Ground or VCC pin is not working properly.";
    scrollText(1, error_message);
    delay(delay_long);
    return flag;  //Return 0
  }

  // Input pins: A1,B1 and A2,B2 + C0
  // From diagram: A1(pin2), B1(pin3), A2(pin14), B2(pin13), C0(pin5)
  int ip_pins[2][2] = { { 2, 3 }, { 14, 13 } };
  int cin_pin = 5;

  for (int pin1 = 0; pin1 < 2; pin1++) {
    for (int pin2 = 0; pin2 < 2; pin2++) {
      if (ip_pins[pin1][pin2] >= ((total_pins / 2) + 1)) {
        ip_pins[pin1][pin2] += ((max_pin - total_pins) + start_pin);
      } else {
        ip_pins[pin1][pin2] += start_pin;
      }
      pinMode(ip_pins[pin1][pin2], OUTPUT);
    }
  }
  if (cin_pin >= ((total_pins / 2) + 1)) {
    cin_pin += ((max_pin - total_pins) + start_pin);
  } else {
    cin_pin += start_pin;
  }
  pinMode(cin_pin, OUTPUT);

  // Output pins
  int sum_pins[2] = { 1, 12 };  // Σ1, Σ2
  int cout_pin = 10;            // C2

  for (int i = 0; i < 2; i++) {
    if (sum_pins[i] >= ((total_pins / 2) + 1)) {
      sum_pins[i] += ((max_pin - total_pins) + start_pin);
    } else {
      sum_pins[i] += start_pin;
    }
    pinMode(sum_pins[i], INPUT);
  }
  if (cout_pin >= ((total_pins / 2) + 1)) {
    cout_pin += ((max_pin - total_pins) + start_pin);
  } else {
    cout_pin += start_pin;
  }
  pinMode(cout_pin, INPUT);

  // Truth Table of IC 7482 (2-bit Full Adder)
  // Format: {A1, B1, A2, B2, C0, Σ1, Σ2, C2}
  int tt_7482[32][8] = {
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 1, 0, 0, 0, 0, 1, 0, 0 },
    { 0, 1, 0, 0, 0, 1, 0, 0 },
    { 1, 1, 0, 0, 0, 0, 1, 0 },
    { 0, 0, 1, 0, 0, 0, 1, 0 },
    { 1, 0, 1, 0, 0, 1, 1, 0 },
    { 0, 1, 1, 0, 0, 1, 1, 0 },
    { 1, 1, 1, 0, 0, 0, 0, 1 },
    { 0, 0, 0, 1, 0, 0, 1, 0 },
    { 1, 0, 0, 1, 0, 1, 1, 0 },
    { 0, 1, 0, 1, 0, 1, 1, 0 },
    { 1, 1, 0, 1, 0, 0, 0, 1 },
    { 0, 0, 1, 1, 0, 0, 0, 1 },
    { 1, 0, 1, 1, 0, 1, 0, 1 },
    { 0, 1, 1, 1, 0, 1, 0, 1 },
    { 1, 1, 1, 1, 0, 0, 1, 1 },
    { 0, 0, 0, 0, 1, 1, 0, 0 },
    { 1, 0, 0, 0, 1, 0, 1, 0 },
    { 0, 1, 0, 0, 1, 0, 1, 0 },
    { 1, 1, 0, 0, 1, 1, 1, 0 },
    { 0, 0, 1, 0, 1, 1, 1, 0 },
    { 1, 0, 1, 0, 1, 0, 0, 1 },
    { 0, 1, 1, 0, 1, 0, 0, 1 },
    { 1, 1, 1, 0, 1, 1, 0, 1 },
    { 0, 0, 0, 1, 1, 1, 1, 0 },
    { 1, 0, 0, 1, 1, 0, 0, 1 },
    { 0, 1, 0, 1, 1, 0, 0, 1 },
    { 1, 1, 0, 1, 1, 1, 0, 1 },
    { 0, 0, 1, 1, 1, 1, 0, 1 },
    { 1, 0, 1, 1, 1, 0, 1, 1 },
    { 0, 1, 1, 1, 1, 0, 1, 1 },
    { 1, 1, 1, 1, 1, 1, 1, 1 }
  };

  // Checking IC 7482
  int check = 1;
  
  for (int tt_i = 0; tt_i < 32; tt_i++) {
    // Apply inputs
    digitalWrite(ip_pins[0][0], tt_7482[tt_i][0]);  // A1
    digitalWrite(ip_pins[0][1], tt_7482[tt_i][1]);  // B1
    digitalWrite(ip_pins[1][0], tt_7482[tt_i][2]);  // A2
    digitalWrite(ip_pins[1][1], tt_7482[tt_i][3]);  // B2
    digitalWrite(cin_pin, tt_7482[tt_i][4]);        // C0

    // Compare outputs
    if (digitalRead(sum_pins[0]) != tt_7482[tt_i][5] || digitalRead(sum_pins[1]) != tt_7482[tt_i][6] || digitalRead(cout_pin) != tt_7482[tt_i][7]) {
      check = 0;
    }
  }

  flag += check;
  if (check) {
    message = "IC 7482 is WORKING.";
  } else {
    message = "IC 7482 is NOT WORKING.";
  }
  scrollText(1, message);
  delay(delay_short);

  return flag;
}

int check_7483() {
  check_message = "Checking IC 7483 (4-Bit Binary Full Adder)";
  scrollText(0, check_message);
  delay(delay_long);

  int flag = 0;

  int work = 1;

  

  int total_pins = 16;  // Total number of Pins

  int gnd_pin = 12;  // Ground Pin
  int vcc_pin = 5;   // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += start_pin;
  } else {
    gnd_pin += start_pin;
  }
  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += start_pin;
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);
  digitalWrite(gnd_pin, LOW);

  pinMode(vcc_pin, OUTPUT);
  digitalWrite(vcc_pin, HIGH);

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    //Faulty Ground or VCC pin, Not need to check entire IC.
    error_message = "Ground or VCC pin is not working properly.";
    scrollText(1, error_message);
    delay(delay_long);
    return flag;  //Return 0
  }

  int pin;

  // Input Pins: A1–A4, B1–B4, Cin
  int ip_pins[2][4] = { { 10, 8, 3, 1 }, { 11, 7, 4, 16 } };  // A1–A4, B1–B4
  int cin_pin = 13;                                           // Carry In

  // Map Arduino pins
  for (int i = 0; i < 4; i++) {
    if (ip_pins[1][i] >= ((total_pins / 2) + 1)) {
      ip_pins[1][i] += start_pin;
    } else {
      ip_pins[1][i] += start_pin;
    }
    if (ip_pins[0][i] >= ((total_pins / 2) + 1)) {
      ip_pins[0][i] += start_pin;
    } else {
      ip_pins[0][i] += start_pin;
    }
    pinMode(ip_pins[0][i], OUTPUT);
    pinMode(ip_pins[1][i], OUTPUT);
  }
  if (cin_pin >= ((total_pins / 2) + 1))
    cin_pin += start_pin;
  else
    cin_pin += start_pin;
  pinMode(cin_pin, OUTPUT);

  // Output Pins: Σ1–Σ4, Cout
  int op_pins[5] = { 9, 6, 2, 15, 14 };  // S1–S4, C4
  for (pin = 0; pin < 5; pin++) {
    if (op_pins[pin] >= ((total_pins / 2) + 1)) {
      op_pins[pin] += start_pin;
    } else {
      op_pins[pin] += start_pin;
    }
    pinMode(op_pins[pin], INPUT);
  }

  // Truth Table: For 4-bit adder it’s too large → we test a subset of cases
  // Format: {A, B, Cin, Sum, Cout}
  // Each A and B is 4-bit number
  int tt_7483[8][5] = {
    { 0x0, 0x0, 0, 0x0, 0 },
    { 0x3, 0x4, 0, 0x7, 0 },
    { 0xF, 0x1, 0, 0x0, 1 },
    { 0x5, 0xA, 1, 0x0, 1 },
    { 0x7, 0x8, 0, 0xF, 0 },
    { 0x7, 0x8, 1, 0x0, 1 },
    { 0x9, 0x6, 0, 0xF, 0 },
    { 0x9, 0x6, 1, 0x0, 1 }
  };

  // Checking IC 7483
  for (int t = 0; t < 8; t++) {
    int check = 1;

    // Apply inputs bitwise
    for (int b = 0; b < 4; b++) {
      digitalWrite(ip_pins[0][b], (tt_7483[t][0] >> b) & 1);  // A
      digitalWrite(ip_pins[1][b], (tt_7483[t][1] >> b) & 1);  // B
    }
    digitalWrite(cin_pin, tt_7483[t][2]);

    // Expected outputs
    int expected_sum = tt_7483[t][3];
    int expected_cout = tt_7483[t][4];

    // Verify each sum bit
    for (int b = 0; b < 4; b++) {
      if (digitalRead(op_pins[b]) != ((expected_sum >> b) & 1)) {
        check = 0;
      }
    }
    if (digitalRead(op_pins[4]) != expected_cout) {
      check = 0;
    }

    flag += check;
    work *= check;
    delay(delay_short);
  }

  if (work) {
    message = "IC 7483 is WORKING.";
  } else {
    message = "IC 7483 is NOT WORKING.";
  }
  scrollText(1, message);
  return flag;
}

int check_74283() {
  check_message = "Checking IC 74283 (4-Bit Binary Full Adder)";
  scrollText(0, check_message);
  delay(delay_long);

  int flag = 0;

  int work = 1;

  

  int total_pins = 16;  // Total number of Pins

  int gnd_pin = 8;   // Ground Pin
  int vcc_pin = 16;  // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += start_pin;
  } else {
    gnd_pin += start_pin;
  }
  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += start_pin;
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);
  digitalWrite(gnd_pin, LOW);

  pinMode(vcc_pin, OUTPUT);
  digitalWrite(vcc_pin, HIGH);

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    //Faulty Ground or VCC pin, Not need to check entire IC.
    error_message = "Ground or VCC pin is not working properly.";
    scrollText(1, error_message);
    delay(delay_long);
    return flag;  //Return 0
  }

  int pin, pin1;

  // Input Pins: A1–A4, B1–B4
  int a_pins[4] = { 5, 3, 14, 12 };
  int b_pins[4] = { 6, 2, 15, 11 };
  int cin_pin = 7;

  // Map input pins to Arduino
  for (pin1 = 0; pin1 < 4; pin1++) {
    if (a_pins[pin1] >= ((total_pins / 2) + 1))
      a_pins[pin1] += start_pin;
    else
      a_pins[pin1] += start_pin;
    if (b_pins[pin1] >= ((total_pins / 2) + 1))
      b_pins[pin1] += start_pin;
    else
      b_pins[pin1] += start_pin;
    pinMode(a_pins[pin1], OUTPUT);
    pinMode(b_pins[pin1], OUTPUT);
  }
  if (cin_pin >= ((total_pins / 2) + 1))
    cin_pin += start_pin;
  else
    cin_pin += start_pin;
  pinMode(cin_pin, OUTPUT);

  // Output Pins: S1–S4, Cout
  int s_pins[4] = { 4, 1, 13, 10 };
  int cout_pin = 9;

  for (pin1 = 0; pin1 < 4; pin1++) {
    if (s_pins[pin1] >= ((total_pins / 2) + 1))
      s_pins[pin1] += start_pin;
    else
      s_pins[pin1] += start_pin;
    pinMode(s_pins[pin1], INPUT);
  }
  if (cout_pin >= ((total_pins / 2) + 1))
    cout_pin += start_pin;
  else
    cout_pin += start_pin;
  pinMode(cout_pin, INPUT);

  // Test cases {A, B, Cin, Sum, Cout}
  // (Values are 4-bit numbers, sum is also 4-bit)
  int tt_74283[6][5] = {
    { 0x0, 0x0, 0, 0x0, 0 },
    { 0x3, 0x4, 0, 0x7, 0 },
    { 0xF, 0x1, 0, 0x0, 1 },
    { 0x5, 0xA, 1, 0x0, 1 },
    { 0x7, 0x8, 0, 0xF, 0 },
    { 0x7, 0x8, 1, 0x0, 1 }
  };

  // Checking IC 74283
  for (int t = 0; t < 6; t++) {
    int check = 1;

    // Apply inputs A, B, Cin
    for (int b = 0; b < 4; b++) {
      digitalWrite(a_pins[b], (tt_74283[t][0] >> b) & 1);
      digitalWrite(b_pins[b], (tt_74283[t][1] >> b) & 1);
    }
    digitalWrite(cin_pin, tt_74283[t][2]);

    // Expected outputs
    int expected_sum = tt_74283[t][3];
    int expected_cout = tt_74283[t][4];

    // Check outputs
    for (int b = 0; b < 4; b++) {
      if (digitalRead(s_pins[b]) != ((expected_sum >> b) & 1)) {
        check = 0;
      }
    }
    if (digitalRead(cout_pin) != expected_cout)
      check = 0;

    flag += check;

    delay(delay_short);
  }

  if (work) {
    message = "IC 74283 is WORKING.";
  } else {
    message = "IC 74283 is NOT WORKING.";
  }
  scrollText(1, message);
  return flag;
}

// Combinational Circuits
int check_7442() {
  check_message = "Checking IC 7442 (BCD to Decimal Decoder)";
  scrollText(0, check_message);
  delay(delay_long);

  int flag = 0;
  int work = 1;
  int total_pins = 16;  // IC 7442 has 16 pins

  int gnd_pin = 8;   // Ground Pin
  int vcc_pin = 16;  // VCC Pin

  // Adjust Arduino mapped Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += start_pin;
  } else {
    gnd_pin += start_pin;
  }

  // Adjust Arduino mapped VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += start_pin;
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);
  digitalWrite(gnd_pin, LOW);

  pinMode(vcc_pin, OUTPUT);
  digitalWrite(vcc_pin, HIGH);

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    //Faulty Ground or VCC pin, Not need to check entire IC.
    error_message = "Ground or VCC pin is not working properly.";
    scrollText(1, error_message);
    delay(delay_long);
    return flag;  //Return 0
  }

  // Input pins: A, B, C, D
  int ip_pins[4] = { 15, 14, 13, 12 };

  for (int i = 0; i < 4; i++) {
    if (ip_pins[i] >= ((total_pins / 2) + 1)) {
      ip_pins[i] += start_pin;
    } else {
      ip_pins[i] += start_pin;
    }
    pinMode(ip_pins[i], OUTPUT);
  }

  // Output pins: Y0-Y9
  int op_pins[10] = { 1, 2, 3, 4, 5, 6, 7, 9, 10, 11 };

  for (int i = 0; i < 10; i++) {
    if (op_pins[i] >= ((total_pins / 2) + 1)) {
      op_pins[i] += start_pin;
    } else {
      op_pins[i] += start_pin;
    }
    pinMode(op_pins[i], INPUT);
  }
  
  // Truth table (BCD input → one active low output)
  int tt_7442[10][15] = {
    { 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1 },     // 0000 -> Y0 LOW
    { 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1 },  // 0001 -> Y1 LOW
    { 0, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1 },  // 0010 -> Y2 LOW
    { 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1 },  // 0011 -> Y3 LOW
    { 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1 },  // 0100 -> Y4 LOW
    { 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1 },  // 0101 -> Y5 LOW
    { 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1 },  // 0110 -> Y6 LOW
    { 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1 },  // 0111 -> Y7 LOW
    { 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1 },  // 1000 -> Y8 LOW
    { 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1 }   // 1001 -> Y9 LOW
  };

  // Checking IC 7442
  for (int tt_i = 0; tt_i < 10; tt_i++) {
    // Apply BCD inputs
    digitalWrite(ip_pins[0], tt_7442[tt_i][0]);
    digitalWrite(ip_pins[1], tt_7442[tt_i][1]);
    digitalWrite(ip_pins[2], tt_7442[tt_i][2]);
    digitalWrite(ip_pins[3], tt_7442[tt_i][3]);

    delay(10);  // small settling time

    int check = 1;

    // Validate all 10 outputs
    for (int op = 0; op < 10; op++) {
      if (digitalRead(op_pins[op]) != tt_7442[tt_i][op + 4]) {
        check = 0;
      }
    }

    flag += check;
    work *= check;
    delay(delay_short);
  }
  if (work) {
    message = "IC 7442 is WORKING.";
  } else {
    message = "IC 7442 is NOT WORKING.";
  }
  scrollText(1, message);
  return flag;
}

int check_74138() {
  check_message = "Checking IC 74138 (3-to-8 Decoder / Demultiplexer)";
  scrollText(0, check_message);
  delay(delay_long);

  int flag = 0;
  int work = 1;
  
  int total_pins = 16;  // Total pins in IC 74138

  int gnd_pin = 8;   // Ground Pin
  int vcc_pin = 16;  // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }

  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);
  digitalWrite(gnd_pin, LOW);

  pinMode(vcc_pin, OUTPUT);
  digitalWrite(vcc_pin, HIGH);

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    //Faulty Ground or VCC pin, Not need to check entire IC.
    error_message = "Ground or VCC pin is not working properly.";
    scrollText(1, error_message);
    delay(delay_long);
    return flag;  //Return 0
  }

  // Input pins A=1, B=2, C=3
  int in_pins[3] = { 1, 2, 3 };
  for (int i = 0; i < 3; i++) {
    if (in_pins[i] >= ((total_pins / 2) + 1)) {
      in_pins[i] += ((max_pin - total_pins) + start_pin);
    } else {
      in_pins[i] += start_pin;
    }
    pinMode(in_pins[i], OUTPUT);
  }

  // Enable pins G2A=4, G2B=5 (active LOW), G1=6 (active HIGH)
  int g2a = 4, g2b = 5, g1 = 6;
  int en_pins[3] = { g2a, g2b, g1 };
  for (int i = 0; i < 3; i++) {
    if (en_pins[i] >= ((total_pins / 2) + 1)) {
      en_pins[i] += ((max_pin - total_pins) + start_pin);
    } else {
      en_pins[i] += start_pin;
    }
    pinMode(en_pins[i], OUTPUT);
  }
  digitalWrite(en_pins[0], LOW);   // G2A active
  digitalWrite(en_pins[1], LOW);   // G2B active
  digitalWrite(en_pins[2], HIGH);  // G1 active

  // Output pins Y0–Y7 = 15,14,13,12,11,10,9,7
  int op_pins[8] = { 15, 14, 13, 12, 11, 10, 9, 7 };
  for (int i = 0; i < 8; i++) {
    if (op_pins[i] >= ((total_pins / 2) + 1)) {
      op_pins[i] += ((max_pin - total_pins) + start_pin);
    } else {
      op_pins[i] += start_pin;
    }
    pinMode(op_pins[i], INPUT);
  }

  // Truth table: for inputs A,B,C, only Y(ABC) = 0, others = 1
  for (int code = 0; code < 8; code++) {
    // set A,B,C
    digitalWrite(in_pins[0], code & 0x01);         // A
    digitalWrite(in_pins[1], (code >> 1) & 0x01);  // B
    digitalWrite(in_pins[2], (code >> 2) & 0x01);  // C

    delay(5);

    int check = 1;
    for (int y = 0; y < 8; y++) {
      int expected = (y == code) ? LOW : HIGH;  // active low
      if (digitalRead(op_pins[y]) != expected) {
        check = 0;
      }
    }

    flag += check;
    work *= check;
    delay(delay_short);
  }
  if (work) {
    message = "IC 74138 is WORKING.";
  } else {
    message = "IC 74138 is NOT WORKING.";
  }
  scrollText(1, message);
  return flag;
}

int check_74147() {
  check_message = "Checking IC 74147 (10-to-4 Priority Encoder)";
  scrollText(0, check_message);
  delay(delay_long);

  int flag = 0;
  int work = 1;
  
  int total_pins = 16;  // Total pins in IC 74147

  int gnd_pin = 8;   // Ground pin
  int vcc_pin = 16;  // VCC pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }

  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);
  digitalWrite(gnd_pin, LOW);

  pinMode(vcc_pin, OUTPUT);
  digitalWrite(vcc_pin, HIGH);

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    //Faulty Ground or VCC pin, Not need to check entire IC.
    error_message = "Ground or VCC pin is not working properly.";
    scrollText(1, error_message);
    delay(delay_long);
    return flag;  //Return 0
  }

  int pin;

  // INPUTS (active LOW) for digits 1..9 according to 74147 pinout:
  // digit 1 -> pin 11, 2 -> 12, 3 -> 13, 4 -> 1, 5 -> 2, 6 -> 3, 7 -> 4, 8 -> 5, 9 -> 10
  int ip_pins[9] = { 11, 12, 13, 1, 2, 3, 4, 5, 10 };

  // Configure data input Arduino pins (outputs from Arduino side)
  for (pin = 0; pin < 9; pin++) {
    if (ip_pins[pin] >= ((total_pins / 2) + 1)) {
      ip_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      ip_pins[pin] += start_pin;
    }
    pinMode(ip_pins[pin], OUTPUT);
    digitalWrite(ip_pins[pin], HIGH);  // default inactive (inputs are active-LOW)
  }

  // OUTPUTS (active LOW): D, C, B, A  -> pins 14, 6, 7, 9 respectively
  int op_pins[4] = { 14, 6, 7, 9 };  // order: D, C, B, A

  for (pin = 0; pin < 4; pin++) {
    if (op_pins[pin] >= ((total_pins / 2) + 1)) {
      op_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      op_pins[pin] += start_pin;
    }
    pinMode(op_pins[pin], INPUT);
  }

  // --- Test case 0 (implied zero) ---
  // No input active (all HIGH) => outputs should represent 0 (BCD 0000).
  // Because outputs are active-LOW, the physical pins should read HIGH for all four outputs.
  for (pin = 0; pin < 9; pin++)
    digitalWrite(ip_pins[pin], HIGH);
  delay(10);  // settle
  {
    int check = 1;
    for (int k = 0; k < 4; k++) {
      if (digitalRead(op_pins[k]) != HIGH) {
        check = 0;
        break;
      }
    }
    flag += check;
    work *= check;
    delay(delay_short);
  }

  // --- Test inputs 1..9 individually ---
  // For each n (1..9): drive that input LOW (active), all others HIGH,
  // then check outputs equal the BCD of n but as active-LOW signals.
  for (int n = 1; n <= 9; n++) {
    // set all inputs inactive (HIGH)
    for (pin = 0; pin < 9; pin++)
      digitalWrite(ip_pins[pin], HIGH);
    // activate the requested input (active LOW)
    digitalWrite(ip_pins[n - 1], LOW);

    delay(10);  // settling time

    int check = 1;
    // op_pins order: D (bit3), C (bit2), B (bit1), A (bit0)
    for (int k = 0; k < 4; k++) {
      int bit = (n >> (3 - k)) & 0x1;   // extract the BCD bit
      int expected = bit ? LOW : HIGH;  // outputs are active-LOW
      if (digitalRead(op_pins[k]) != expected) {
        check = 0;
        break;
      }
    }

    flag += check;
    work *= check;
    delay(delay_short);
  }

  if (work) {
    message = "IC 74147 is WORKING.";
  } else {
    message = "IC 74147 is NOT WORKING.";
  }
  scrollText(1, message);
  return flag;
}

int check_74148() {
  check_message = "Checking IC 74148 (8-to-3 Priority Encoder)";
  scrollText(0, check_message);
  delay(delay_long);

  int flag = 0;
  int work = 1;
  
  int total_pins = 16;  // IC 74148 is 16-pin

  int gnd_pin = 8;   // Ground Pin
  int vcc_pin = 16;  // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }

  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);
  digitalWrite(gnd_pin, LOW);

  pinMode(vcc_pin, OUTPUT);
  digitalWrite(vcc_pin, HIGH);

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    //Faulty Ground or VCC pin, Not need to check entire IC.
    error_message = "Ground or VCC pin is not working properly.";
    scrollText(1, error_message);
    delay(delay_long);
    return flag;  //Return 0
  }

  int pin;

  // Data inputs I0..I7 mapped to physical pins per datasheet:
  // I0=11, I1=12, I2=13, I3=1, I4=2, I5=3, I6=4, I7=10
  int ip_pins[8] = { 10, 11, 12, 13, 1, 2, 3, 4 };

  for (pin = 0; pin < 8; pin++) {
    if (ip_pins[pin] >= ((total_pins / 2) + 1)) {
      ip_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      ip_pins[pin] += start_pin;
    }
    pinMode(ip_pins[pin], OUTPUT);
  }

  // Enable Input (EI, active LOW) -> physical pin 5
  int ei_pin = 5;
  if (ei_pin >= ((total_pins / 2) + 1)) {
    ei_pin += ((max_pin - total_pins) + start_pin);
  } else {
    ei_pin += start_pin;
  }
  pinMode(ei_pin, OUTPUT);

  // Address outputs and flags (physical pins):
  // A2 = pin 6, A1 = pin 7, A0 = pin 9, GS = pin 14, EO = pin 15
  int op_pins[5] = { 6, 7, 9, 14, 15 };  // order: A2, A1, A0, GS, EO

  for (pin = 0; pin < 5; pin++) {
    if (op_pins[pin] >= ((total_pins / 2) + 1)) {
      op_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      op_pins[pin] += start_pin;
    }
    pinMode(op_pins[pin], INPUT);
  }

  // --- Case 1: EI = LOW (enabled) ---
  digitalWrite(ei_pin, LOW);

  for (int i = 0; i < 8; i++) {
    // Make sure only input i is active (LOW)
    for (int j = 0; j < 8; j++)
      digitalWrite(ip_pins[j], HIGH);
    digitalWrite(ip_pins[i], LOW);  // activate I_i
    delay(5);                       // settling

    // Read address outputs (active LOW => LOW means bit = 1)
    int a2 = (digitalRead(op_pins[0]) == LOW) ? 1 : 0;
    int a1 = (digitalRead(op_pins[1]) == LOW) ? 1 : 0;
    int a0 = (digitalRead(op_pins[2]) == LOW) ? 1 : 0;
    int gs = digitalRead(op_pins[3]);  // expect LOW when any input active
    int eo = digitalRead(op_pins[4]);  // expect HIGH when some input active

    int encoded = (a2 << 2) | (a1 << 1) | a0;

    int check = 1;
    if (encoded != i)
      check = 0;  // encoded value must equal input index
    if (gs != LOW)
      check = 0;  // GS must be active (LOW)
    if (eo != HIGH)
      check = 0;  // EO must be inactive (HIGH) when input present

    flag += check;
    work *= check;
    delay(delay_short);
  }

  // --- Case 2: EI = HIGH (disabled) ---
  // When EI is HIGH, outputs should be inactive (all HIGH)
  digitalWrite(ei_pin, HIGH);
  for (int j = 0; j < 8; j++)
    digitalWrite(ip_pins[j], HIGH);
  delay(5);

  int disabled_ok = 1;
  // A2,A1,A0 should be HIGH, GS should be HIGH, EO should be HIGH when EI is HIGH
  for (int k = 0; k < 3; k++)
    if (digitalRead(op_pins[k]) != HIGH)
      disabled_ok = 0;
  if (digitalRead(op_pins[3]) != HIGH)
    disabled_ok = 0;  // GS
  if (digitalRead(op_pins[4]) != HIGH)
    disabled_ok = 0;  // EO

  if (work) {
    message = "IC 74148 is WORKING.";
  } else {
    message = "IC 74148 is NOT WORKING.";
  }

  // if (disabled_ok) {
  //   lcd.print("EI=HIGH | All outputs inactive (HIGH).");
  //   flag++;
  // } else {
  //   lcd.print("EI=HIGH | Outputs NOT correct when disabled.");
  // }
  scrollText(1, message);
  return flag;
}

int check_74151() {
  check_message = "Checking IC 74151 (8-to-1 Multiplexer)";
  scrollText(0, check_message);
  delay(delay_long);

  int flag = 0;
  int work = 1;
  
  int total_pins = 16;  // Total pins in IC 74151

  int gnd_pin = 8;   // Ground pin
  int vcc_pin = 16;  // VCC pin

  // Map ground and VCC to Arduino pins
  if (gnd_pin >= ((total_pins / 2) + 1))
    gnd_pin += ((max_pin - total_pins) + start_pin);
  else
    gnd_pin += start_pin;
  if (vcc_pin >= ((total_pins / 2) + 1))
    vcc_pin += ((max_pin - total_pins) + start_pin);
  else
    vcc_pin += start_pin;

  pinMode(gnd_pin, OUTPUT);
  digitalWrite(gnd_pin, LOW);

  pinMode(vcc_pin, OUTPUT);
  digitalWrite(vcc_pin, HIGH);

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    //Faulty Ground or VCC pin, Not need to check entire IC.
    error_message = "Ground or VCC pin is not working properly.";
    scrollText(1, error_message);
    delay(delay_long);
    return flag;  //Return 0
  }

  int pin, pin1;

  // Data input pins D0–D7 (correct order per datasheet)
  int ip_pins[8] = { 4, 3, 2, 1, 15, 14, 13, 12 };
  for (pin = 0; pin < 8; pin++) {
    if (ip_pins[pin] >= ((total_pins / 2) + 1))
      ip_pins[pin] += ((max_pin - total_pins) + start_pin);
    else
      ip_pins[pin] += start_pin;
    pinMode(ip_pins[pin], OUTPUT);
  }

  // Select inputs S0, S1, S2
  int sel_pins[3] = { 11, 10, 9 };
  for (pin = 0; pin < 3; pin++) {
    if (sel_pins[pin] >= ((total_pins / 2) + 1))
      sel_pins[pin] += ((max_pin - total_pins) + start_pin);
    else
      sel_pins[pin] += start_pin;
    pinMode(sel_pins[pin], OUTPUT);
  }

  // Enable pin (active LOW)
  int en_pin = 7;
  if (en_pin >= ((total_pins / 2) + 1))
    en_pin += ((max_pin - total_pins) + start_pin);
  else
    en_pin += start_pin;
  pinMode(en_pin, OUTPUT);
  digitalWrite(en_pin, LOW);

  // Output pin Y
  int op_pin = 5;
  if (op_pin >= ((total_pins / 2) + 1))
    op_pin += ((max_pin - total_pins) + start_pin);
  else
    op_pin += start_pin;
  pinMode(op_pin, INPUT);

  // Checking IC 74151
  for (int sel = 0; sel < 8; sel++) {
    int check = 1;

    // Set select lines
    digitalWrite(sel_pins[0], sel & 0x01);
    digitalWrite(sel_pins[1], (sel >> 1) & 0x01);
    digitalWrite(sel_pins[2], (sel >> 2) & 0x01);

    // Test both 0 and 1 for the selected input
    for (int val = 0; val < 2; val++) {
      // Set all inputs HIGH first
      for (pin1 = 0; pin1 < 8; pin1++)
        digitalWrite(ip_pins[pin1], HIGH);

      // Set selected input to the test value
      digitalWrite(ip_pins[sel], val);

      if (digitalRead(op_pin) != val)
        check = 0;
    }

    flag += check;
    work *= check;
    delay(delay_short);
  }

  if (work) {
    message = "IC 74151 is WORKING.";
  } else {
    message = "IC 74151 is NOT WORKING.";
  }
  scrollText(1, message);
  return flag;
}

int check_74153() {
  check_message = "Checking IC 74153 (Dual 4-to-1 Multiplexer)";
  scrollText(0, check_message);
  delay(delay_long);

  int flag = 0;
  int total_pins = 16;  // Total number of pins

  int gnd_pin = 8;   // Ground Pin
  int vcc_pin = 16;  // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }

  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);
  digitalWrite(gnd_pin, LOW);

  pinMode(vcc_pin, OUTPUT);
  digitalWrite(vcc_pin, HIGH);

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    //Faulty Ground or VCC pin, Not need to check entire IC.
    error_message = "Ground or VCC pin is not working properly.";
    scrollText(1, error_message);
    delay(delay_long);
    return flag;  //Return 0
  }

  int pin, pin1;

  // Input pins for both multiplexers
  int ip_pins[2][4] = { { 6, 5, 4, 3 }, { 10, 11, 12, 13 } };

  for (pin1 = 0; pin1 < 2; pin1++) {
    for (pin = 0; pin < 4; pin++) {
      if (ip_pins[pin1][pin] >= ((total_pins / 2) + 1)) {
        ip_pins[pin1][pin] += ((max_pin - total_pins) + start_pin);
      } else {
        ip_pins[pin1][pin] += start_pin;
      }
      pinMode(ip_pins[pin1][pin], OUTPUT);
    }
  }

  // Select inputs (S0, S1)
  int sel_pins[2] = { 14, 2 };
  for (pin = 0; pin < 2; pin++) {
    if (sel_pins[pin] >= ((total_pins / 2) + 1)) {
      sel_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      sel_pins[pin] += start_pin;
    }
    pinMode(sel_pins[pin], OUTPUT);
  }

  // Enable pins (active LOW)
  int en_pins[2] = { 15, 1 };
  for (pin = 0; pin < 2; pin++) {
    if (en_pins[pin] >= ((total_pins / 2) + 1)) {
      en_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      en_pins[pin] += start_pin;
    }
    pinMode(en_pins[pin], OUTPUT);
    digitalWrite(en_pins[pin], LOW);
  }

  // Output pins (MUX1 MUX2)
  int op_pins[2] = { 7, 9 };
  for (pin = 0; pin < 2; pin++) {
    if (op_pins[pin] >= ((total_pins / 2) + 1)) {
      op_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      op_pins[pin] += start_pin;
    }
    pinMode(op_pins[pin], INPUT);
  }

  // Checking IC 74153
  for (int mux = 0; mux < 2; mux++) {
    int check = 1;
    
    // Test all select combinations
    for (int sel = 0; sel < 4; sel++) {
      digitalWrite(sel_pins[0], sel & 0x01);
      digitalWrite(sel_pins[1], (sel >> 1) & 0x01);

      // Test selected input with both values
      for (int val = 0; val < 2; val++) {
        // Set all inputs HIGH
        for (pin = 0; pin < 4; pin++) {
          digitalWrite(ip_pins[mux][pin], HIGH);
        }
        // Set only selected input
        digitalWrite(ip_pins[mux][sel], val);

        if (digitalRead(op_pins[mux]) != val) {
          check = 0;
        }
      }
    }

    flag += check;
    if (check) {
      message = "Multiplexer " + String(mux + 1) + "(" + String(getOriginalPin(sel_pins[0], total_pins)) + String(getOriginalPin(sel_pins[1], total_pins)) + String(getOriginalPin(ip_pins[mux][0], total_pins)) + String(getOriginalPin(ip_pins[mux][1], total_pins)) + String(getOriginalPin(ip_pins[mux][2], total_pins)) + String(getOriginalPin(ip_pins[mux][3], total_pins)) + String(getOriginalPin(op_pins[mux], total_pins)) + ")" + " is WORKING.";
    } else {
      message = "Multiplexer " + String(mux + 1) + "(" + String(getOriginalPin(sel_pins[0], total_pins)) + String(getOriginalPin(sel_pins[1], total_pins)) + String(getOriginalPin(ip_pins[mux][0], total_pins)) + String(getOriginalPin(ip_pins[mux][1], total_pins)) + String(getOriginalPin(ip_pins[mux][2], total_pins)) + String(getOriginalPin(ip_pins[mux][3], total_pins)) + String(getOriginalPin(op_pins[mux], total_pins)) + ")" + " is NOT WORKING.";
    }
    scrollText(1, message);
    delay(delay_short);
  }

  return flag;
}

int check_74157() {
  check_message = "Checking IC 74157 (Quad 2-to-1 Multiplexer)";
  scrollText(0, check_message);
  delay(delay_long);

  int flag = 0;
  int total_pins = 16;  // Total pins in IC 74157

  int gnd_pin = 8;   // Ground Pin
  int vcc_pin = 16;  // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }

  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);
  digitalWrite(gnd_pin, LOW);

  pinMode(vcc_pin, OUTPUT);
  digitalWrite(vcc_pin, HIGH);

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    //Faulty Ground or VCC pin, Not need to check entire IC.
    error_message = "Ground or VCC pin is not working properly.";
    scrollText(1, error_message);
    delay(delay_long);
    return flag;  //Return 0
  }

  int pin, pin1;

  // Data Inputs A (I0) and B (I1) for 4 multiplexers
  int ip_pins[4][2] = { { 2, 3 }, { 5, 6 }, { 11, 10 }, { 14, 13 } };

  for (pin1 = 0; pin1 < 4; pin1++) {
    for (pin = 0; pin < 2; pin++) {
      if (ip_pins[pin1][pin] >= ((total_pins / 2) + 1)) {
        ip_pins[pin1][pin] += ((max_pin - total_pins) + start_pin);
      } else {
        ip_pins[pin1][pin] += start_pin;
      }
      pinMode(ip_pins[pin1][pin], OUTPUT);
    }
  }

  // Common select pin (S) at pin 1
  int sel_pin = 1;
  if (sel_pin >= ((total_pins / 2) + 1)) {
    sel_pin += ((max_pin - total_pins) + start_pin);
  } else {
    sel_pin += start_pin;
  }
  pinMode(sel_pin, OUTPUT);

  // Enable pin (G, active LOW) at pin 15
  int en_pin = 15;
  if (en_pin >= ((total_pins / 2) + 1)) {
    en_pin += ((max_pin - total_pins) + start_pin);
  } else {
    en_pin += start_pin;
  }
  pinMode(en_pin, OUTPUT);
  digitalWrite(en_pin, LOW);  // keep enabled

  // Output pins Y for 4 multiplexers
  int op_pins[4] = { 4, 7, 9, 12 };
  for (pin = 0; pin < 4; pin++) {
    if (op_pins[pin] >= ((total_pins / 2) + 1)) {
      op_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      op_pins[pin] += start_pin;
    }
    pinMode(op_pins[pin], INPUT);
  }

  // Truth Table for a single 2:1 MUX {S, A, B, Y}
  int tt_74157[4][4] = {
    { 0, 0, 1, 0 },  // S=0 → Y=A (A=0,B=1 → Y=0)
    { 0, 1, 0, 1 },  // S=0 → Y=A (A=1,B=0 → Y=1)
    { 1, 0, 1, 1 },  // S=1 → Y=B (A=0,B=1 → Y=1)
    { 1, 1, 0, 0 }   // S=1 → Y=B (A=1,B=0 → Y=0)
  };

  // Checking IC 74157
  for (int cir_i = 0; cir_i < 4; cir_i++) {
    int check = 1;
    
    for (int tt_i = 0; tt_i < 4; tt_i++) {
      digitalWrite(sel_pin, tt_74157[tt_i][0]);
      digitalWrite(ip_pins[cir_i][0], tt_74157[tt_i][1]);
      digitalWrite(ip_pins[cir_i][1], tt_74157[tt_i][2]);

      if (digitalRead(op_pins[cir_i]) != tt_74157[tt_i][3]) {
        check = 0;
      }
    }

    flag += check;
    if (check) {
      message = "MUX on pins " + String(getOriginalPin(ip_pins[cir_i][0], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][1], total_pins)) + " and " + String(getOriginalPin(op_pins[cir_i], total_pins)) + " is WORKING.";
    } else {
      message = "MUX on pins " + String(getOriginalPin(ip_pins[cir_i][0], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][1], total_pins)) + " and " + String(getOriginalPin(op_pins[cir_i], total_pins)) + " is NOT WORKING.";
    }
    scrollText(1, message);
    delay(delay_short);
  }

  return flag;
}

int check_74158() {
  check_message = "Checking IC 74158 (Quad 2-to-1 Multiplexer, Inverted Output)";
  scrollText(0, check_message);
  delay(delay_long);

  int flag = 0;
  int total_pins = 16;  // Total pins in IC 74158

  int gnd_pin = 8;   // Ground Pin
  int vcc_pin = 16;  // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }

  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);
  digitalWrite(gnd_pin, LOW);

  pinMode(vcc_pin, OUTPUT);
  digitalWrite(vcc_pin, HIGH);

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    //Faulty Ground or VCC pin, Not need to check entire IC.
    error_message = "Ground or VCC pin is not working properly.";
    scrollText(1, error_message);
    delay(delay_long);
    return flag;  //Return 0
  }

  int pin, pin1;

  // Data Inputs A (I0) and B (I1) for 4 multiplexers
  int ip_pins[4][2] = { { 2, 3 }, { 5, 6 }, { 11, 10 }, { 14, 13 } };

  for (pin1 = 0; pin1 < 4; pin1++) {
    for (pin = 0; pin < 2; pin++) {
      if (ip_pins[pin1][pin] >= ((total_pins / 2) + 1)) {
        ip_pins[pin1][pin] += ((max_pin - total_pins) + start_pin);
      } else {
        ip_pins[pin1][pin] += start_pin;
      }
      pinMode(ip_pins[pin1][pin], OUTPUT);
    }
  }

  // Common select pin (S) at pin 1
  int sel_pin = 1;
  if (sel_pin >= ((total_pins / 2) + 1)) {
    sel_pin += ((max_pin - total_pins) + start_pin);
  } else {
    sel_pin += start_pin;
  }
  pinMode(sel_pin, OUTPUT);

  // Enable pin (G, active LOW) at pin 15
  int en_pin = 15;
  if (en_pin >= ((total_pins / 2) + 1)) {
    en_pin += ((max_pin - total_pins) + start_pin);
  } else {
    en_pin += start_pin;
  }
  pinMode(en_pin, OUTPUT);
  digitalWrite(en_pin, LOW);  // keep enabled

  // Output pins Y for 4 multiplexers
  int op_pins[4] = { 4, 7, 9, 12 };
  for (pin = 0; pin < 4; pin++) {
    if (op_pins[pin] >= ((total_pins / 2) + 1)) {
      op_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      op_pins[pin] += start_pin;
    }
    pinMode(op_pins[pin], INPUT);
  }

  // Truth Table for a single 2:1 MUX with inverted output {S, A, B, Y}
  int tt_74158[4][4] = {
    { 0, 0, 1, 1 },  // S=0 → Y = !A (A=0 → Y=1)
    { 0, 1, 0, 0 },  // S=0 → Y = !A (A=1 → Y=0)
    { 1, 0, 1, 0 },  // S=1 → Y = !B (B=1 → Y=0)
    { 1, 1, 0, 1 }   // S=1 → Y = !B (B=0 → Y=1)
  };

  // Checking IC 74158
  for (int cir_i = 0; cir_i < 4; cir_i++) {
    int check = 1;
    
    for (int tt_i = 0; tt_i < 4; tt_i++) {
      digitalWrite(sel_pin, tt_74158[tt_i][0]);
      digitalWrite(ip_pins[cir_i][0], tt_74158[tt_i][1]);
      digitalWrite(ip_pins[cir_i][1], tt_74158[tt_i][2]);

      if (digitalRead(op_pins[cir_i]) != tt_74158[tt_i][3]) {
        check = 0;
      }
    }

    flag += check;
    if (check) {
      message = "MUX (inverted) on pins " + String(getOriginalPin(ip_pins[cir_i][0], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][1], total_pins)) + " and " + String(getOriginalPin(op_pins[cir_i], total_pins)) + " is WORKING.";
    } else {
      message = "MUX (inverted) on pins " + String(getOriginalPin(ip_pins[cir_i][0], total_pins)) + "," + String(getOriginalPin(ip_pins[cir_i][1], total_pins)) + " and " + String(getOriginalPin(op_pins[cir_i], total_pins)) + " is NOT WORKING.";
    }
    scrollText(1, message);
    delay(delay_short);
  }

  return flag;
}

int check_74184() {
  check_message = "Checking IC 74184 (BCD to Binary Converter)";
  scrollText(0, check_message);
  delay(delay_long);

  int flag = 0;
  int work = 1;
  
  int total_pins = 16;

  int gnd_pin = 8;
  int vcc_pin = 16;

  // Remap Ground
  if (gnd_pin >= ((total_pins / 2) + 1))
    gnd_pin += ((max_pin - total_pins) + start_pin);
  else
    gnd_pin += start_pin;
  pinMode(gnd_pin, OUTPUT);
  digitalWrite(gnd_pin, LOW);

  // Remap VCC
  if (vcc_pin >= ((total_pins / 2) + 1))
    vcc_pin += ((max_pin - total_pins) + start_pin);
  else
    vcc_pin += start_pin;
  pinMode(vcc_pin, OUTPUT);
  digitalWrite(vcc_pin, HIGH);

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    //Faulty Ground or VCC pin, Not need to check entire IC.
    error_message = "Ground or VCC pin is not working properly.";
    scrollText(1, error_message);
    delay(delay_long);
    return flag;  //Return 0
  }

  // Input pins: A=1, B=2, C=3, D=4
  int ip_pins[4] = { 1, 2, 3, 4 };
  for (int i = 0; i < 4; i++) {
    if (ip_pins[i] >= ((total_pins / 2) + 1))
      ip_pins[i] += ((max_pin - total_pins) + start_pin);
    else
      ip_pins[i] += start_pin;
    pinMode(ip_pins[i], OUTPUT);
  }

  // Output pins: Y0=5, Y1=6, Y2=7, Y3=9, Y4=10, Y5=11, Y6=12
  int op_pins[7] = { 5, 6, 7, 9, 10, 11, 12 };
  for (int i = 0; i < 7; i++) {
    if (op_pins[i] >= ((total_pins / 2) + 1))
      op_pins[i] += ((max_pin - total_pins) + start_pin);
    else
      op_pins[i] += start_pin;
    pinMode(op_pins[i], INPUT);
  }

  // Truth table: valid BCD (0000–1001) → Binary (0–9)
  // Format: {A,B,C,D, Y0..Y6}
  int tt_74184[10][11] = {
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },  // 0
    { 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0 },  // 1
    { 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0 },  // 2
    { 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0 },  // 3
    { 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0 },  // 4
    { 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0 },  // 5
    { 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0 },  // 6
    { 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0 },  // 7
    { 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 },  // 8
    { 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0 }   // 9
  };

  // ---- Checking valid BCD inputs ----
  for (int tt_i = 0; tt_i < 10; tt_i++) {
    // Apply inputs
    for (int j = 0; j < 4; j++) {
      digitalWrite(ip_pins[j], tt_74184[tt_i][j]);
    }

    delay(5);

    int check = 1;
    for (int k = 0; k < 7; k++) {
      if (digitalRead(op_pins[k]) != tt_74184[tt_i][k + 4]) {
        check = 0;
      }
    }

    flag += check;
    work *= check;
    delay(delay_short);
  }

  // ---- Checking invalid BCD inputs (10–15) ----
  // for (int inval = 10; inval < 16; inval++) {
  //   // Apply inputs
  //   for (int j = 0; j < 4; j++) {
  //     digitalWrite(ip_pins[j], (inval >> (3 - j)) & 1);
  //   }

  //   delay(5);

  //   lcd.print("Invalid BCD input " + String(inval) + " → Output undefined.");
  //   delay(delay_short);
  // }

  if (work) {
    message = "IC 74184 is WORKING.";
  } else {
    message = "IC 74184 is NOT WORKING.";
  }
  scrollText(1, message);
  return flag;
}

// Sequential Circuits
int check_7468() {
  check_message = "Checking IC 7468 (Dual 4-bit Binary/Decade Counter)";
  scrollText(0, check_message);
  delay(delay_long);
  
  int flag = 0;
  int total_pins = 16;  // Total pins in IC 7468

  int gnd_pin = 8;   // Ground Pin
  int vcc_pin = 16;  // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }

  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);
  digitalWrite(gnd_pin, LOW);

  pinMode(vcc_pin, OUTPUT);
  digitalWrite(vcc_pin, HIGH);

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    //Faulty Ground or VCC pin, Not need to check entire IC.
    error_message = "Ground or VCC pin is not working properly.";
    scrollText(1, error_message);
    delay(delay_long);
    return flag;  //Return 0
  }

  int pin;

  // Counter A pins
  int clkA = 1, clrA = 2;
  int qA[4] = { 3, 4, 5, 6 };

  // Counter B pins
  int clkB = 9, clrB = 10;
  int qB[4] = { 11, 12, 13, 14 };

  // Map pins to Arduino
  clkA = (clkA >= ((total_pins / 2) + 1)) ? (clkA + ((max_pin - total_pins) + start_pin)) : (clkA + start_pin);
  clrA = (clrA >= ((total_pins / 2) + 1)) ? (clrA + ((max_pin - total_pins) + start_pin)) : (clrA + start_pin);
  clkB = (clkB >= ((total_pins / 2) + 1)) ? (clkB + ((max_pin - total_pins) + start_pin)) : (clkB + start_pin);
  clrB = (clrB >= ((total_pins / 2) + 1)) ? (clrB + ((max_pin - total_pins) + start_pin)) : (clrB + start_pin);

  pinMode(clkA, OUTPUT);
  pinMode(clrA, OUTPUT);
  pinMode(clkB, OUTPUT);
  pinMode(clrB, OUTPUT);

  for (pin = 0; pin < 4; pin++) {
    qA[pin] = (qA[pin] >= ((total_pins / 2) + 1)) ? (qA[pin] + ((max_pin - total_pins) + start_pin)) : (qA[pin] + start_pin);
    qB[pin] = (qB[pin] >= ((total_pins / 2) + 1)) ? (qB[pin] + ((max_pin - total_pins) + start_pin)) : (qB[pin] + start_pin);
    pinMode(qA[pin], INPUT);
    pinMode(qB[pin], INPUT);
  }

  // ----- Checking Counter A -----
  int checkA = 1;
  digitalWrite(clrA, LOW);
  delay(delay_short);
  digitalWrite(clrA, HIGH);

  for (int val = 0; val < 10; val++) {
    digitalWrite(clkA, HIGH);
    delay(delay_short);
    digitalWrite(clkA, LOW);
    delay(delay_short);

    int readVal = (digitalRead(qA[3]) << 3) | (digitalRead(qA[2]) << 2) | (digitalRead(qA[1]) << 1) | (digitalRead(qA[0]));
    if (readVal != val)
      checkA = 0;
  }

  flag += checkA;
  if (checkA) {
    message = "Counter A (pins 1-6) is WORKING.";
  } else {
    message = "Counter A (pins 1-6) is NOT WORKING.";
  }
  scrollText(1, message);
  delay(delay_short);

  // ----- Checking Counter B -----
  int checkB = 1;
  digitalWrite(clrB, LOW);
  delay(delay_short);
  digitalWrite(clrB, HIGH);

  for (int val = 0; val < 10; val++) {
    digitalWrite(clkB, HIGH);
    delay(delay_short);
    digitalWrite(clkB, LOW);
    delay(delay_short);

    int readVal = (digitalRead(qB[3]) << 3) | (digitalRead(qB[2]) << 2) | (digitalRead(qB[1]) << 1) | (digitalRead(qB[0]));
    if (readVal != val)
      checkB = 0;
  }

  flag += checkB;
  if (checkB) {
    message = "Counter B (pins 9-14) is WORKING.";
  } else {
    message = "Counter B (pins 9-14) is NOT WORKING.";
  }
  scrollText(1, message);
  delay(delay_short);

  return flag;
}

int check_7473() {
  check_message = "Checking IC 7473 (Dual JK Master-Slave Flip-Flop with Clear)";
  delay(delay_long);
  scrollText(0, check_message);
  int flag = 0;
  int total_pins = 14;  // Total pins in IC 7473
  
  // NOTE: according to the datasheet the 7473 VCC and GND are at pins 4 and 11.
  int gnd_pin = 11;  // Ground (pin 11)
  int vcc_pin = 4;   // VCC (pin 4)

  // map to Arduino connector (keeps your original mapping logic)
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);
  digitalWrite(gnd_pin, LOW);

  pinMode(vcc_pin, OUTPUT);
  digitalWrite(vcc_pin, HIGH);

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    //Faulty Ground or VCC pin, Not need to check entire IC.
    error_message = "Ground or VCC pin is not working properly.";
    scrollText(1, error_message);
    delay(delay_long);
    return flag;  //Return 0
  }

  int pin, pin1;

  // Inputs for each FF: {CLK, J, K, CLR} (pins per datasheet)
  int ip_pins[2][4] = { { 1, 14, 3, 2 },    // FF1: CLK1, J1, K1, CLR1
                        { 5, 7, 10, 6 } };  // FF2: CLK2, J2, K2, CLR2

  // map and set input pins (Arduino pins) and initialize levels
  for (pin1 = 0; pin1 < 2; pin1++) {
    for (pin = 0; pin < 4; pin++) {
      if (ip_pins[pin1][pin] >= ((total_pins / 2) + 1)) {
        ip_pins[pin1][pin] += ((max_pin - total_pins) + start_pin);
      } else {
        ip_pins[pin1][pin] += start_pin;
      }
      pinMode(ip_pins[pin1][pin], OUTPUT);
      // default: CLK=LOW, J=LOW, K=LOW, CLR=HIGH (inactive)
      if (pin == 0) {  // CLK
        digitalWrite(ip_pins[pin1][pin], LOW);
      } else if (pin == 3) {  // CLR (active LOW) - keep inactive
        digitalWrite(ip_pins[pin1][pin], HIGH);
      } else {  // J, K
        digitalWrite(ip_pins[pin1][pin], LOW);
      }
    }
  }

  // Outputs (Q, Q̅) per datasheet: FF1 -> Q=12, Q̅=13 ; FF2 -> Q=9, Q̅=8
  int op_pins[2][2] = { { 12, 13 }, { 9, 8 } };
  for (pin1 = 0; pin1 < 2; pin1++) {
    for (pin = 0; pin < 2; pin++) {
      if (op_pins[pin1][pin] >= ((total_pins / 2) + 1)) {
        op_pins[pin1][pin] += ((max_pin - total_pins) + start_pin);
      } else {
        op_pins[pin1][pin] += start_pin;
      }
      pinMode(op_pins[pin1][pin], INPUT);
    }
  }

  // small helper to generate a clean HIGH->LOW (negative) clock transition
  auto pulseClock = [&](int clkPin) {
    digitalWrite(clkPin, LOW);
    delayMicroseconds(5);
    digitalWrite(clkPin, HIGH);
    delayMicroseconds(8);
    digitalWrite(clkPin, LOW);  // falling edge occurs here
    delayMicroseconds(8);       // allow outputs to settle
  };

  // Test sequence for each flip-flop: reset baseline, then HOLD / SET / RESET / TOGGLE
  for (int ff = 0; ff < 2; ff++) {
    int check = 1;

    // 1) asynchronous clear -> ensure known baseline Q=LOW, Qbar=HIGH
    digitalWrite(ip_pins[ff][3], LOW);  // CLR active
    delayMicroseconds(10);
    digitalWrite(ip_pins[ff][3], HIGH);  // CLR release
    delay(1);
    int q = digitalRead(op_pins[ff][0]);
    int qb = digitalRead(op_pins[ff][1]);
    if (!(q == LOW && qb == HIGH))
      check = 0;

    // 2) HOLD: J=0, K=0 -> Q should remain LOW
    digitalWrite(ip_pins[ff][1], LOW);  // J
    digitalWrite(ip_pins[ff][2], LOW);  // K
    pulseClock(ip_pins[ff][0]);
    q = digitalRead(op_pins[ff][0]);
    qb = digitalRead(op_pins[ff][1]);
    if (q != LOW || qb == q)
      check = 0;

    // 3) SET: J=1, K=0 -> Q should become HIGH
    digitalWrite(ip_pins[ff][1], HIGH);
    digitalWrite(ip_pins[ff][2], LOW);
    pulseClock(ip_pins[ff][0]);
    q = digitalRead(op_pins[ff][0]);
    qb = digitalRead(op_pins[ff][1]);
    if (q != HIGH || qb == q)
      check = 0;

    // 4) RESET: J=0, K=1 -> Q should become LOW
    digitalWrite(ip_pins[ff][1], LOW);
    digitalWrite(ip_pins[ff][2], HIGH);
    pulseClock(ip_pins[ff][0]);
    q = digitalRead(op_pins[ff][0]);
    qb = digitalRead(op_pins[ff][1]);
    if (q != LOW || qb == q)
      check = 0;

    // 5) TOGGLE: J=1, K=1 -> Q should toggle (from LOW to HIGH here)
    digitalWrite(ip_pins[ff][1], HIGH);
    digitalWrite(ip_pins[ff][2], HIGH);
    pulseClock(ip_pins[ff][0]);
    q = digitalRead(op_pins[ff][0]);
    qb = digitalRead(op_pins[ff][1]);
    if (q != HIGH || qb == q)
      check = 0;

    flag += check;
    if (check) {
      message = "Flip-Flop on pins " + String(getOriginalPin(ip_pins[ff][1], total_pins)) + "," + String(getOriginalPin(ip_pins[ff][2], total_pins)) + " and " + String(getOriginalPin(op_pins[ff][0], total_pins)) + ", " + String(getOriginalPin(op_pins[ff][1], total_pins)) + " is WORKING.";
    } else {
      message = "Flip-Flop on pins " + String(getOriginalPin(ip_pins[ff][1], total_pins)) + "," + String(getOriginalPin(ip_pins[ff][2], total_pins)) + " and " + String(getOriginalPin(op_pins[ff][0], total_pins)) + ", " + String(getOriginalPin(op_pins[ff][1], total_pins)) + " is NOT WORKING.";
    }
    scrollText(1, message);
    delay(delay_short);
  }

  return flag;
}

int check_7474() {
  check_message = "Checking IC 7474 (Dual D Flip-Flop with Preset and Clear)";
  delay(delay_long);
  scrollText(0, check_message);
  int flag = 0;
  int total_pins = 14;  // IC 7474 has 14 pins
  
  int gnd_pin = 7;   // GND pin
  int vcc_pin = 14;  // VCC pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }
  pinMode(gnd_pin, OUTPUT);
  digitalWrite(gnd_pin, LOW);

  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }
  pinMode(vcc_pin, OUTPUT);
  digitalWrite(vcc_pin, HIGH);

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    //Faulty Ground or VCC pin, Not need to check entire IC.
    error_message = "Ground or VCC pin is not working properly.";
    scrollText(1, error_message);
    delay(delay_long);
    return flag;  //Return 0
  }

  int pin, pin1;

  // Inputs {D, CLK, PRE, CLR} for 2 flip-flops
  int ip_pins[2][4] = { { 2, 3, 4, 1 }, { 12, 11, 10, 13 } };

  for (pin1 = 0; pin1 < 2; pin1++) {
    for (pin = 0; pin < 4; pin++) {
      if (ip_pins[pin1][pin] >= ((total_pins / 2) + 1)) {
        ip_pins[pin1][pin] += ((max_pin - total_pins) + start_pin);
      } else {
        ip_pins[pin1][pin] += start_pin;
      }
      pinMode(ip_pins[pin1][pin], OUTPUT);
    }
  }

  // Outputs {Q, Qbar} for 2 flip-flops
  int op_pins[2][2] = { { 5, 6 }, { 9, 8 } };

  for (pin1 = 0; pin1 < 2; pin1++) {
    for (pin = 0; pin < 2; pin++) {
      if (op_pins[pin1][pin] >= ((total_pins / 2) + 1)) {
        op_pins[pin1][pin] += ((max_pin - total_pins) + start_pin);
      } else {
        op_pins[pin1][pin] += start_pin;
      }
      pinMode(op_pins[pin1][pin], INPUT);
    }
  }

  // Truth table cases {PRE, CLR, D, expected_Q}
  int tt_7474[4][4] = {
    { 1, 0, 0, 0 },  // CLR active → Q=0
    { 0, 1, 0, 1 },  // PRE active → Q=1
    { 1, 1, 0, 0 },  // Normal, D=0 → Q=0
    { 1, 1, 1, 1 }   // Normal, D=1 → Q=1
  };

  for (int ff = 0; ff < 2; ff++) {
    int check = 1;

    for (int tt_i = 0; tt_i < 4; tt_i++) {
      // Apply PRE and CLR
      digitalWrite(ip_pins[ff][2], tt_7474[tt_i][0]);
      digitalWrite(ip_pins[ff][3], tt_7474[tt_i][1]);

      // Apply D
      digitalWrite(ip_pins[ff][0], tt_7474[tt_i][2]);

      // Pulse CLK only in normal mode
      if (tt_7474[tt_i][0] == 1 && tt_7474[tt_i][1] == 1) {
        digitalWrite(ip_pins[ff][1], HIGH);
        delayMicroseconds(5);
        digitalWrite(ip_pins[ff][1], LOW);
      }
      delay(5);

      int q_val = digitalRead(op_pins[ff][0]);
      int qb_val = digitalRead(op_pins[ff][1]);

      if (q_val != tt_7474[tt_i][3])
        check = 0;
      if (qb_val != !q_val)
        check = 0;
    }

    flag += check;
    if (check) {
      message = "Flip-Flop on pins " + String(getOriginalPin(ip_pins[ff][0], total_pins)) + "," + String(getOriginalPin(ip_pins[ff][1], total_pins)) + "," + String(getOriginalPin(op_pins[ff][0], total_pins)) + "," + String(getOriginalPin(op_pins[ff][1], total_pins)) + " is WORKING.";
    } else {
      message = "Flip-Flop on pins " + String(getOriginalPin(ip_pins[ff][0], total_pins)) + "," + String(getOriginalPin(ip_pins[ff][1], total_pins)) + "," + String(getOriginalPin(op_pins[ff][0], total_pins)) + "," + String(getOriginalPin(op_pins[ff][1], total_pins)) + " is NOT WORKING.";
    }
    scrollText(1, message);
    delay(delay_short);
  }

  return flag;
}

int check_7476() {
  check_message = "Checking IC 7476 (Dual JK Flip-Flop with Preset and Clear)";
  delay(delay_long);
  scrollText(0, check_message);
  int flag = 0;
  int total_pins = 16;  // IC 7476 has 16 pins
  
  int gnd_pin = 13;  // GND pin
  int vcc_pin = 5;   // VCC pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }
  pinMode(gnd_pin, OUTPUT);
  digitalWrite(gnd_pin, LOW);

  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }
  pinMode(vcc_pin, OUTPUT);
  digitalWrite(vcc_pin, HIGH);

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    //Faulty Ground or VCC pin, Not need to check entire IC.
    error_message = "Ground or VCC pin is not working properly.";
    scrollText(1, error_message);
    delay(delay_long);
    return flag;  //Return 0
  }

  int pin, pin1;

  // Inputs {J, K, CLK, PRE, CLR} for 2 flip-flops
  int ip_pins[2][5] = { { 4, 16, 1, 2, 3 }, { 9, 12, 6, 7, 8 } };

  for (pin1 = 0; pin1 < 2; pin1++) {
    for (pin = 0; pin < 5; pin++) {
      if (ip_pins[pin1][pin] >= ((total_pins / 2) + 1)) {
        ip_pins[pin1][pin] += ((max_pin - total_pins) + start_pin);
      } else {
        ip_pins[pin1][pin] += start_pin;
      }
      pinMode(ip_pins[pin1][pin], OUTPUT);
    }
  }

  // Outputs {Q, Qbar} for 2 flip-flops
  int op_pins[2][2] = { { 15, 14 }, { 11, 10 } };

  for (pin1 = 0; pin1 < 2; pin1++) {
    for (pin = 0; pin < 2; pin++) {
      if (op_pins[pin1][pin] >= ((total_pins / 2) + 1)) {
        op_pins[pin1][pin] += ((max_pin - total_pins) + start_pin);
      } else {
        op_pins[pin1][pin] += start_pin;
      }
      pinMode(op_pins[pin1][pin], INPUT);
    }
  }

  // Truth table {J, K, PRE, CLR, expected_action}
  // -1 = Hold, -2 = Toggle
  int tt_7476[5][5] = {
    { 0, 0, 1, 1, -1 },  // Hold
    { 0, 1, 1, 1, 0 },   // Reset
    { 1, 0, 1, 1, 1 },   // Set
    { 1, 1, 1, 1, -2 },  // Toggle
    { 0, 0, 0, 1, 1 }    // PRE active → Q=1
  };

  for (int ff = 0; ff < 2; ff++) {
    int check = 1;
    int q_state = 0;  // start assumption

    for (int tt_i = 0; tt_i < 5; tt_i++) {
      // Apply J, K
      digitalWrite(ip_pins[ff][0], tt_7476[tt_i][0]);
      digitalWrite(ip_pins[ff][1], tt_7476[tt_i][1]);

      // Apply PRE, CLR
      digitalWrite(ip_pins[ff][3], tt_7476[tt_i][2]);
      digitalWrite(ip_pins[ff][4], tt_7476[tt_i][3]);

      // Pulse CLK in normal cases
      if (tt_7476[tt_i][2] == 1 && tt_7476[tt_i][3] == 1) {
        digitalWrite(ip_pins[ff][2], LOW);
        delayMicroseconds(5);
        digitalWrite(ip_pins[ff][2], HIGH);
        delayMicroseconds(5);
        digitalWrite(ip_pins[ff][2], LOW);
      }
      delay(5);

      int q_val = digitalRead(op_pins[ff][0]);
      int qb_val = digitalRead(op_pins[ff][1]);

      // Expected Q evaluation
      if (tt_7476[tt_i][4] == -1) {
        if (q_val != q_state)
          check = 0;
      } else if (tt_7476[tt_i][4] == -2) {
        q_state = !q_state;
        if (q_val != q_state)
          check = 0;
      } else {
        if (q_val != tt_7476[tt_i][4])
          check = 0;
        q_state = q_val;
      }

      if (qb_val != !q_val)
        check = 0;
    }

    flag += check;
    if (check) {
      message = "JK Flip-Flop on pins " + String(getOriginalPin(ip_pins[ff][0], total_pins)) + "," + String(getOriginalPin(ip_pins[ff][1], total_pins)) + "," + String(getOriginalPin(op_pins[ff][0], total_pins)) + "," + String(getOriginalPin(op_pins[ff][1], total_pins)) + " is WORKING.";
    } else {
      message = "JK Flip-Flop on pins" + String(getOriginalPin(ip_pins[ff][0], total_pins)) + "," + String(getOriginalPin(ip_pins[ff][1], total_pins)) + "," + String(getOriginalPin(op_pins[ff][0], total_pins)) + "," + String(getOriginalPin(op_pins[ff][1], total_pins)) + " is NOT WORKING.";
    }
    scrollText(1, message);
    delay(delay_short);
  }

  return flag;
}

int check_7478() {
  check_message = "Checking IC 7478 (Dual JK Flip-Flop with Clear & Preset)";
  delay(delay_long);
  scrollText(0, check_message);
  int flag = 0;
  int total_pins = 16;  // Total pins in IC 7478
  
  int gnd_pin = 11;  // Ground Pin
  int vcc_pin = 4;   // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }

  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);
  digitalWrite(gnd_pin, LOW);

  pinMode(vcc_pin, OUTPUT);
  digitalWrite(vcc_pin, HIGH);

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    //Faulty Ground or VCC pin, Not need to check entire IC.
    error_message = "Ground or VCC pin is not working properly.";
    scrollText(1, error_message);
    delay(delay_long);
    return flag;  //Return 0
  }

  // Pin mapping for both flip-flops (J, K, CLK, PRE, CLR, Q, Qbar)
  int jk_pins[2][7] = {
    { 3, 14, 1, 2, 5, 13, 12 },  // FF1 → J,K,CLK,PRE,CLR,Q,Qbar
    { 10, 7, 1, 6, 5, 8, 9 }     // FF2 → J,K,CLK,PRE,CLR,Q,Qbar
  };

  for (int ff = 0; ff < 2; ff++) {
    int J = jk_pins[ff][0], K = jk_pins[ff][1], CLK = jk_pins[ff][2];
    int PRE = jk_pins[ff][3], CLR = jk_pins[ff][4];
    int Q = jk_pins[ff][5], Qbar = jk_pins[ff][6];

    // Adjust pins for Arduino mapping
    int pins_ff[7] = { J, K, CLK, PRE, CLR, Q, Qbar };
    for (int p = 0; p < 7; p++) {
      if (pins_ff[p] >= ((total_pins / 2) + 1)) {
        pins_ff[p] += ((max_pin - total_pins) + start_pin);
      } else {
        pins_ff[p] += start_pin;
      }
    }

    J = pins_ff[0];
    K = pins_ff[1];
    CLK = pins_ff[2];
    PRE = pins_ff[3];
    CLR = pins_ff[4];
    Q = pins_ff[5];
    Qbar = pins_ff[6];

    // Set pin directions
    pinMode(J, OUTPUT);
    pinMode(K, OUTPUT);
    pinMode(CLK, OUTPUT);
    pinMode(PRE, OUTPUT);
    pinMode(CLR, OUTPUT);
    pinMode(Q, INPUT);
    pinMode(Qbar, INPUT);

    int check = 1;

    // --- Test Clear ---
    digitalWrite(CLR, LOW);
    digitalWrite(PRE, HIGH);
    delay(delay_short);
    if (!(digitalRead(Q) == LOW && digitalRead(Qbar) == HIGH))
      check = 0;
    digitalWrite(CLR, HIGH);

    // --- Test Preset ---
    digitalWrite(PRE, LOW);
    digitalWrite(CLR, HIGH);
    delay(delay_short);
    if (!(digitalRead(Q) == HIGH && digitalRead(Qbar) == LOW))
      check = 0;
    digitalWrite(PRE, HIGH);

    // --- Test JK Operation (Toggle mode) ---
    digitalWrite(J, HIGH);
    digitalWrite(K, HIGH);
    for (int t = 0; t < 2; t++) {
      digitalWrite(CLK, LOW);
      delay(5);
      digitalWrite(CLK, HIGH);
      delay(delay_short);
    }
    int q1 = digitalRead(Q);
    digitalWrite(CLK, LOW);
    delay(5);
    digitalWrite(CLK, HIGH);
    delay(delay_short);
    int q2 = digitalRead(Q);

    if (q1 == q2)
      check = 0;  // Q must toggle

    flag += check;
    if (check) {
      message = "JK Flip-Flop on pins " + String(getOriginalPin(J, total_pins)) + "," + String(getOriginalPin(K, total_pins)) + "," + String(getOriginalPin(Q, total_pins)) + " is WORKING.";
    } else {
      message = "JK Flip-Flop on pins " + String(getOriginalPin(J, total_pins)) + "," + String(getOriginalPin(K, total_pins)) + "," + String(getOriginalPin(Q, total_pins)) + " is NOT WORKING.";
    }
    scrollText(1, message);
    delay(delay_short);
  }

  return flag;
}

int check_74112() {
  check_message = "Checking IC 74112 (Dual J-K Master-Slave Flip-Flop, Active-LOW CLR & PRE)";
  scrollText(0, check_message);
  delay(delay_long);
  
  int flag = 0;
  int total_pins = 16;  // Total pins in IC 74112

  int gnd_pin = 8;   // Ground Pin
  int vcc_pin = 16;  // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }
  pinMode(gnd_pin, OUTPUT);
  digitalWrite(gnd_pin, LOW);

  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }
  pinMode(vcc_pin, OUTPUT);
  digitalWrite(vcc_pin, HIGH);

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    //Faulty Ground or VCC pin, Not need to check entire IC.
    error_message = "Ground or VCC pin is not working properly.";
    scrollText(1, error_message);
    delay(delay_long);
    return flag;  //Return 0
  }

  int pin;

  // Each flip-flop: J, K, CLK, CLR, PRE, Q, Q'
  int ip_pins[2][5] = { { 3, 2, 1, 15, 4 }, { 11, 12, 13, 14, 10 } };  // J,K,CLK,CLR,PRE
  int op_pins[2][2] = { { 5, 6 }, { 9, 7 } };                          // Q, Q'

  for (int ff = 0; ff < 2; ff++) {
    for (pin = 0; pin < 5; pin++) {
      if (ip_pins[ff][pin] >= ((total_pins / 2) + 1)) {
        ip_pins[ff][pin] += ((max_pin - total_pins) + start_pin);
      } else {
        ip_pins[ff][pin] += start_pin;
      }
      pinMode(ip_pins[ff][pin], OUTPUT);
    }

    for (pin = 0; pin < 2; pin++) {
      if (op_pins[ff][pin] >= ((total_pins / 2) + 1)) {
        op_pins[ff][pin] += ((max_pin - total_pins) + start_pin);
      } else {
        op_pins[ff][pin] += start_pin;
      }
      pinMode(op_pins[ff][pin], INPUT);
    }
  }

  // Test sequence for JK flip-flop
  // Conditions: {J,K,CLR,PRE,Expected Q,Expected Q'}
  int tt_74112[6][6] = {
    { 0, 0, 1, 1, -1, -1 },  // No change, depends on previous state
    { 0, 1, 1, 1, 0, 1 },    // Reset
    { 1, 0, 1, 1, 1, 0 },    // Set
    { 1, 1, 1, 1, -2, -2 },  // Toggle
    { 0, 0, 0, 1, 0, 1 },    // CLR active
    { 0, 0, 1, 0, 1, 0 }     // PRE active
  };

  for (int ff = 0; ff < 2; ff++) {
    int check = 1;
    int q_state = 0;  // assume Q=0 initially

    for (int t = 0; t < 6; t++) {
      digitalWrite(ip_pins[ff][0], tt_74112[t][0]);  // J
      digitalWrite(ip_pins[ff][1], tt_74112[t][1]);  // K
      digitalWrite(ip_pins[ff][3], tt_74112[t][2]);  // CLR
      digitalWrite(ip_pins[ff][4], tt_74112[t][3]);  // PRE

      // Clock pulse
      digitalWrite(ip_pins[ff][2], LOW);
      delayMicroseconds(5);
      digitalWrite(ip_pins[ff][2], HIGH);
      delayMicroseconds(5);
      digitalWrite(ip_pins[ff][2], LOW);

      int q_read = digitalRead(op_pins[ff][0]);
      int nq_read = digitalRead(op_pins[ff][1]);

      if (tt_74112[t][4] == -1) {
        // No change, skip check
        continue;
      } else if (tt_74112[t][4] == -2) {
        // Toggle
        q_state = !q_state;
        if (q_read != q_state || nq_read == q_state) {
          check = 0;
        }
      } else {
        if (q_read != tt_74112[t][4] || nq_read != tt_74112[t][5]) {
          check = 0;
        }
        q_state = q_read;
      }
    }

    flag += check;
    if (check) {
      message = "Flip-Flop on pins " + String(getOriginalPin(ip_pins[ff][0], total_pins)) + "," + String(getOriginalPin(ip_pins[ff][1], total_pins)) + "," + String(getOriginalPin(ip_pins[ff][2], total_pins)) + "," + String(getOriginalPin(ip_pins[ff][3], total_pins)) + "," + String(getOriginalPin(ip_pins[ff][4], total_pins)) + "," + String(getOriginalPin(op_pins[ff][0], total_pins)) + " and " + String(getOriginalPin(op_pins[ff][1], total_pins)) + " is WORKING.";
    } else {
      message = "Flip-Flop on pins " + String(getOriginalPin(ip_pins[ff][0], total_pins)) + "," + String(getOriginalPin(ip_pins[ff][1], total_pins)) + "," + String(getOriginalPin(ip_pins[ff][2], total_pins)) + "," + String(getOriginalPin(ip_pins[ff][3], total_pins)) + "," + String(getOriginalPin(ip_pins[ff][4], total_pins)) + "," + String(getOriginalPin(op_pins[ff][0], total_pins)) + " and " + String(getOriginalPin(op_pins[ff][1], total_pins)) + " is NOT WORKING.";
    }
    scrollText(1, message);
    delay(delay_short);
  }

  return flag;
}

int check_74165() {
  check_message = "Checking IC 74165 (8-bit Parallel-In/Serial-Out Shift Register)";
  scrollText(0, check_message);
  delay(delay_long);
  
  int flag = 0;
  int work = 1;
  int total_pins = 16;  // Total pins in IC 74165

  int gnd_pin = 8;   // Ground Pin
  int vcc_pin = 16;  // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }
  pinMode(gnd_pin, OUTPUT);
  digitalWrite(gnd_pin, LOW);

  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }
  pinMode(vcc_pin, OUTPUT);
  digitalWrite(vcc_pin, HIGH);

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    //Faulty Ground or VCC pin, Not need to check entire IC.
    error_message = "Ground or VCC pin is not working properly.";
    scrollText(1, error_message);
    delay(delay_long);
    return flag;  //Return 0
  }

  int pin;

  // Parallel Data Inputs D0–D7 (pins )
  int ip_pins[8] = { 11, 12, 13, 14, 3, 4, 5, 6 };
  for (pin = 0; pin < 8; pin++) {
    if (ip_pins[pin] >= ((total_pins / 2) + 1)) {
      ip_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      ip_pins[pin] += start_pin;
    }
    pinMode(ip_pins[pin], OUTPUT);
  }

  // Control pins
  int load_pin = 1;  // /PL (active LOW)
  int clk_pin = 2;   // CP (clock)
  int ce_pin = 15;   // /CE (active LOW)

  if (load_pin >= ((total_pins / 2) + 1))
    load_pin += ((max_pin - total_pins) + start_pin);
  else
    load_pin += start_pin;
  if (clk_pin >= ((total_pins / 2) + 1))
    clk_pin += ((max_pin - total_pins) + start_pin);
  else
    clk_pin += start_pin;
  if (ce_pin >= ((total_pins / 2) + 1))
    ce_pin += ((max_pin - total_pins) + start_pin);
  else
    ce_pin += start_pin;

  pinMode(load_pin, OUTPUT);
  pinMode(clk_pin, OUTPUT);
  pinMode(ce_pin, OUTPUT);

  digitalWrite(clk_pin, LOW);
  digitalWrite(ce_pin, LOW);  // enable shift register

  // Serial output Q7 (pin 9)
  int q7_pin = 9;
  if (q7_pin >= ((total_pins / 2) + 1)) {
    q7_pin += ((max_pin - total_pins) + start_pin);
  } else {
    q7_pin += start_pin;
  }
  pinMode(q7_pin, INPUT);

  // Test patterns
  int patterns[2][8] = {
    { 1, 0, 1, 0, 1, 0, 1, 0 },  // 0xAA
    { 1, 1, 0, 0, 1, 1, 0, 0 }   // 0xCC
  };

  for (int pat = 0; pat < 2; pat++) {
    // Load parallel data (active LOW pulse on /PL)
    digitalWrite(load_pin, LOW);
    for (int b = 0; b < 8; b++) {
      digitalWrite(ip_pins[b], patterns[pat][b]);
    }
    delayMicroseconds(5);
    digitalWrite(load_pin, HIGH);

    // Now read serially bit-by-bit
    int check = 1;
    for (int b = 7; b >= 0; b--) {
      int q_val = digitalRead(q7_pin);
      if (q_val != patterns[pat][b]) {
        check = 0;
      }
      // Clock pulse
      digitalWrite(clk_pin, HIGH);
      delayMicroseconds(5);
      digitalWrite(clk_pin, LOW);
      delayMicroseconds(5);
    }

    flag += check;
    work *= check;
    delay(delay_short);
  }
  if (work) {
    message = "IC 74165 is WORKING.";
    flag += work;
  } else {
    message = "IC 74165 is NOT WORKING.";
  }
  scrollText(1, message);
  return flag;
}

int check_74174() {
  check_message = "Checking IC 74174 (Hex D-type Flip-Flop with Clear)";
  scrollText(0, check_message);
  delay(delay_long);
  
  int flag = 0;
  int total_pins = 16;

  int gnd_pin = 8;
  int vcc_pin = 16;

  // GND
  if (gnd_pin >= ((total_pins / 2) + 1))
    gnd_pin += ((max_pin - total_pins) + start_pin);
  else
    gnd_pin += start_pin;
  pinMode(gnd_pin, OUTPUT);
  digitalWrite(gnd_pin, LOW);

  // VCC
  if (vcc_pin >= ((total_pins / 2) + 1))
    vcc_pin += ((max_pin - total_pins) + start_pin);
  else
    vcc_pin += start_pin;
  pinMode(vcc_pin, OUTPUT);
  digitalWrite(vcc_pin, HIGH);

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    //Faulty Ground or VCC pin, Not need to check entire IC.
    error_message = "Ground or VCC pin is not working properly.";
    scrollText(1, error_message);
    delay(delay_long);
    return flag;  //Return 0
  }

  // Data inputs (IC pins)
  int d_ic[6] = { 3, 4, 6, 11, 13, 14 };
  int d_pins[6];
  for (int i = 0; i < 6; i++) {
    d_pins[i] = (d_ic[i] >= ((total_pins / 2) + 1)) ? d_ic[i] + ((max_pin - total_pins) + start_pin) : d_ic[i] + start_pin;
    pinMode(d_pins[i], OUTPUT);
  }

  // Outputs (IC pins)
  int q_ic[6] = { 2, 5, 7, 10, 12, 15 };
  int q_pins[6];
  for (int i = 0; i < 6; i++) {
    q_pins[i] = (q_ic[i] >= ((total_pins / 2) + 1)) ? q_ic[i] + ((max_pin - total_pins) + start_pin) : q_ic[i] + start_pin;
    pinMode(q_pins[i], INPUT);
  }

  // CLK (9), CLR (1 active low)
  int clk_pin = 9;
  clk_pin = (clk_pin >= ((total_pins / 2) + 1)) ? clk_pin + ((max_pin - total_pins) + start_pin) : clk_pin + start_pin;
  pinMode(clk_pin, OUTPUT);

  int clr_pin = 1;
  clr_pin = (clr_pin >= ((total_pins / 2) + 1)) ? clr_pin + ((max_pin - total_pins) + start_pin) : clr_pin + start_pin;
  pinMode(clr_pin, OUTPUT);

  // --- Test 1: Clear ---
  digitalWrite(clr_pin, LOW);
  delayMicroseconds(50);
  digitalWrite(clr_pin, HIGH);
  delayMicroseconds(50);

  int clear_ok = 1;
  for (int i = 0; i < 6; i++) {
    if (digitalRead(q_pins[i]) != LOW)
      clear_ok = 0;
  }
  if (clear_ok) {
    message = "Clear function WORKING.";
    flag++;
  } else {
    message = "Clear function FAILED.";
  }
  scrollText(1, message);
  delay(delay_short);

  // --- Test 2: Walking-1 (set test) ---
  for (int bit = 0; bit < 6; bit++) {
    for (int i = 0; i < 6; i++)
      digitalWrite(d_pins[i], LOW);
    digitalWrite(d_pins[bit], HIGH);

    // Clock pulse
    digitalWrite(clk_pin, LOW);
    delayMicroseconds(50);
    digitalWrite(clk_pin, HIGH);
    delayMicroseconds(50);
    digitalWrite(clk_pin, LOW);

    int check = 1;
    for (int i = 0; i < 6; i++) {
      int expected = (i == bit) ? HIGH : LOW;
      if (digitalRead(q_pins[i]) != expected)
        check = 0;
    }

    flag += check;
    if (check) {
      message = "Flip-Flop on pin " + String(getOriginalPin(d_ic[bit], total_pins)) + " , " + String(getOriginalPin(q_ic[bit], total_pins)) + " is WORKING (set).";
    } else {
      message = "Flip-Flop on (pin " + String(getOriginalPin(d_ic[bit], total_pins)) + " , " + String(getOriginalPin(q_ic[bit], total_pins)) + " is NOT WORKING (set).";
    }
    scrollText(1, message);
    delay(delay_short);
  }

  // --- Test 3: Walking-0 (reset test) ---
  for (int bit = 0; bit < 6; bit++) {
    for (int i = 0; i < 6; i++)
      digitalWrite(d_pins[i], HIGH);
    digitalWrite(d_pins[bit], LOW);

    // Clock pulse
    digitalWrite(clk_pin, LOW);
    delayMicroseconds(50);
    digitalWrite(clk_pin, HIGH);
    delayMicroseconds(50);
    digitalWrite(clk_pin, LOW);

    int check = 1;
    for (int i = 0; i < 6; i++) {
      int expected = (i == bit) ? LOW : HIGH;
      if (digitalRead(q_pins[i]) != expected)
        check = 0;
    }

    flag += check;
    if (check) {
      message = "Flip-Flop on pin " + String(getOriginalPin(d_ic[bit], total_pins)) + " , " + String(getOriginalPin(q_ic[bit], total_pins)) + " is WORKING (reset).";
    } else {
      message = "Flip-Flop on pin " + String(getOriginalPin(d_ic[bit], total_pins)) + " , " + String(getOriginalPin(q_ic[bit], total_pins)) + " is NOT WORKING (reset).";
    }
    scrollText(1, message);
    delay(delay_short);
  }

  return flag;
}

int check_74175() {
  check_message = "Checking IC 74175 (Quad D-Type Flip-Flop with Clear)";
  scrollText(0, check_message);
  delay(delay_long);
  
  int flag = 0;
  int total_pins = 16;  // Total pins in IC 74175

  int gnd_pin = 8;   // Ground Pin
  int vcc_pin = 16;  // VCC Pin

  // Setting Arduino Ground pin
  gnd_pin = (gnd_pin >= ((total_pins / 2) + 1)) ? gnd_pin + ((max_pin - total_pins) + start_pin) : gnd_pin + start_pin;
  pinMode(gnd_pin, OUTPUT);
  digitalWrite(gnd_pin, LOW);

  // Setting Arduino VCC pin
  vcc_pin = (vcc_pin >= ((total_pins / 2) + 1)) ? vcc_pin + ((max_pin - total_pins) + start_pin) : vcc_pin + start_pin;
  pinMode(vcc_pin, OUTPUT);
  digitalWrite(vcc_pin, HIGH);

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    //Faulty Ground or VCC pin, Not need to check entire IC.
    error_message = "Ground or VCC pin is not working properly.";
    scrollText(1, error_message);
    delay(delay_long);
    return flag;  //Return 0
  }

  // Data Inputs D0–D3 (IC pins 4,5,12,13)
  int d_ic[4] = { 4, 5, 12, 13 };
  int d_pins[4];
  for (int i = 0; i < 4; i++) {
    d_pins[i] = (d_ic[i] >= ((total_pins / 2) + 1)) ? d_ic[i] + ((max_pin - total_pins) + start_pin) : d_ic[i] + start_pin;
    pinMode(d_pins[i], OUTPUT);
  }

  // Outputs Q0–Q3 (IC pins 2,7,10,15)
  int q_ic[4] = { 2, 7, 10, 15 };
  int q_pins[4];
  for (int i = 0; i < 4; i++) {
    q_pins[i] = (q_ic[i] >= ((total_pins / 2) + 1)) ? q_ic[i] + ((max_pin - total_pins) + start_pin) : q_ic[i] + start_pin;
    pinMode(q_pins[i], INPUT);
  }

  // Common Clock (pin 9)
  int clk_pin = 9;
  clk_pin = (clk_pin >= ((total_pins / 2) + 1)) ? clk_pin + ((max_pin - total_pins) + start_pin) : clk_pin + start_pin;
  pinMode(clk_pin, OUTPUT);

  // Asynchronous Clear (active LOW, pin 1)
  int clr_pin = 1;
  clr_pin = (clr_pin >= ((total_pins / 2) + 1)) ? clr_pin + ((max_pin - total_pins) + start_pin) : clr_pin + start_pin;
  pinMode(clr_pin, OUTPUT);

  // --- Test 1: Clear function ---
  digitalWrite(clr_pin, LOW);
  delay(delay_short);

  int clear_ok = 1;
  for (int i = 0; i < 4; i++) {
    if (digitalRead(q_pins[i]) != LOW)
      clear_ok = 0;
  }
  if (clear_ok) {
    message = "Asynchronous CLEAR function WORKING (all Q outputs LOW).";
  } else {
    message = "Asynchronous CLEAR function FAILED.";
  }
  flag += clear_ok;
  scrollText(1, message);
  digitalWrite(clr_pin, HIGH);

  // --- Test 2: Walking 1 and 0 patterns per flip-flop ---
  for (int ff = 0; ff < 4; ff++) {
    // First load a '1' on this D, '0' on others
    for (int i = 0; i < 4; i++)
      digitalWrite(d_pins[i], (i == ff) ? HIGH : LOW);

    // Clock pulse
    digitalWrite(clk_pin, LOW);
    delayMicroseconds(50);
    digitalWrite(clk_pin, HIGH);
    delayMicroseconds(50);
    digitalWrite(clk_pin, LOW);

    int set_ok = (digitalRead(q_pins[ff]) == HIGH);
    if (set_ok) {
      message = "Flip-Flop D(pin " + String(d_ic[ff]) + ") -> Q(pin " + String(q_ic[ff]) + ") WORKING (set).";
      flag++;
    } else {
      message = "Flip-Flop D(pin " + String(d_ic[ff]) + ") -> Q(pin " + String(q_ic[ff]) + ") FAILED (set).";
    }
    scrollText(1, message);
    delay(delay_short);

    // Now load a '0' on this D, '1' on others
    for (int i = 0; i < 4; i++)
      digitalWrite(d_pins[i], (i == ff) ? LOW : HIGH);

    // Clock pulse
    digitalWrite(clk_pin, LOW);
    delayMicroseconds(50);
    digitalWrite(clk_pin, HIGH);
    delayMicroseconds(50);
    digitalWrite(clk_pin, LOW);

    int reset_ok = (digitalRead(q_pins[ff]) == LOW);
    if (reset_ok) {
      message = "Flip-Flop D(pin " + String(getOriginalPin(d_ic[ff], total_pins)) + ") -> Q(pin " + String(getOriginalPin(q_ic[ff], total_pins)) + ") WORKING (reset).";
      flag++;
    } else {
      message = "Flip-Flop D(pin " + String(getOriginalPin(d_ic[ff], total_pins)) + ") -> Q(pin " + String(getOriginalPin(q_ic[ff], total_pins)) + ") FAILED (reset).";
    }
    scrollText(1, message);
    delay(delay_short);
  }

  return flag;
}

int check_74194() {
  check_message = "Checking IC 74194 (4-Bit Bidirectional Universal Shift Register)";
  scrollText(0, check_message);
  delay(delay_long);
  
  int flag = 0;
  int total_pins = 16;

  // small helper to map DIP pin -> your Arduino pin scheme
  auto mapPin = [&](int p) -> int {
    if (p >= ((total_pins / 2) + 1))
      return p + ((max_pin - total_pins) + start_pin);
    else
      return p + start_pin;
  };

  // Vcc / GND
  int gnd_pin = 8;
  int vcc_pin = 16;
  pinMode(gnd_pin, OUTPUT);
  digitalWrite(gnd_pin, LOW);
  pinMode(vcc_pin, OUTPUT);
  digitalWrite(vcc_pin, HIGH);

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    //Faulty Ground or VCC pin, Not need to check entire IC.
    error_message = "Ground or VCC pin is not working properly.";
    scrollText(1, error_message);
    delay(delay_long);
    return flag;  //Return 0
  }

  // Parallel data inputs D0..D3 (D pins on IC: 3,4,5,6)
  int d_pins[4] = { 3, 4, 5, 6 };
  for (int i = 0; i < 4; ++i) {
    pinMode(d_pins[i], OUTPUT);
    digitalWrite(d_pins[i], LOW);
  }

  // Control / serial pins (map them once and use the mapped variables everywhere)
  int dsr_pin = 2;   // Serial input for shift-right (D_SR)
  int dsl_pin = 7;   // Serial input for shift-left  (D_SL)
  int s0_pin = 9;    // S0
  int s1_pin = 10;   // S1
  int clk_pin = 11;  // CLK (rising-edge triggered)
  int clr_pin = 1;   // MR / CLR (active LOW)

  pinMode(dsr_pin, OUTPUT);
  pinMode(dsl_pin, OUTPUT);
  pinMode(s0_pin, OUTPUT);
  pinMode(s1_pin, OUTPUT);
  pinMode(clk_pin, OUTPUT);
  pinMode(clr_pin, OUTPUT);

  // Outputs Q0..Q3 (IC pins 15,14,13,12 -> Q0..Q3)
  int q_pins[4] = { 15, 14, 13, 12 };
  for (int i = 0; i < 4; ++i)
    pinMode(q_pins[i], INPUT);

  // helper to synchronous parallel-load a 4-bit array (A = D0..D3)
  auto parallel_load = [&](int data[4]) {
    // per datasheet: change mode while CLK is HIGH, then create a rising edge
    digitalWrite(clk_pin, HIGH);
    digitalWrite(s1_pin, HIGH);
    digitalWrite(s0_pin, HIGH);  // load mode
    for (int i = 0; i < 4; ++i)
      digitalWrite(d_pins[i], data[i]);
    delayMicroseconds(10);       // small settle
    digitalWrite(clk_pin, LOW);  // prepare for rising edge
    delayMicroseconds(10);
    digitalWrite(clk_pin, HIGH);  // rising edge loads data
    delayMicroseconds(50);        // let outputs settle
  };

  // --- TEST 1: Asynchronous CLEAR (MR active LOW) ---
  digitalWrite(clr_pin, LOW);
  delayMicroseconds(50);
  digitalWrite(clr_pin, HIGH);  // release clear
  delayMicroseconds(50);

  int clear_ok = 1;
  for (int i = 0; i < 4; ++i) {
    if (digitalRead(q_pins[i]) != LOW)
      clear_ok = 0;
  }
  if (clear_ok) {
    message = "Asynchronous CLEAR is WORKING.";
    flag++;
  } else {
    message = "Asynchronous CLEAR is NOT WORKING.";
  }
  scrollText(1, message);

  // Test data (use same pattern for deterministic checks)
  int test_data[4] = { 1, 0, 1, 0 };  // D0..D3

  // --- TEST 2: Parallel LOAD ---
  parallel_load(test_data);
  int load_ok = 1;
  for (int i = 0; i < 4; ++i)
    if (digitalRead(q_pins[i]) != test_data[i])
      load_ok = 0;
  if (load_ok) {
    message = "Parallel LOAD is WORKING.";
    flag++;
  } else {
    message = "Parallel LOAD is NOT WORKING.";
  }
  scrollText(1, message);

  // --- TEST 3: SHIFT RIGHT (S0=1, S1=0) ---
  // Reload a known pattern first so this test is independent:
  parallel_load(test_data);

  // set mode while clock HIGH (per datasheet), then produce rising edge
  digitalWrite(clk_pin, HIGH);
  digitalWrite(s0_pin, HIGH);   // S0=1
  digitalWrite(s1_pin, LOW);    // S1=0  -> SHIFT RIGHT
  digitalWrite(dsr_pin, HIGH);  // serial input - not used to check Q3, but define it
  delayMicroseconds(10);
  digitalWrite(clk_pin, LOW);
  delayMicroseconds(10);
  digitalWrite(clk_pin, HIGH);
  delayMicroseconds(50);

  // After one right shift, new Q3 == previous Q2 (test_data[2])
  int expected_q3 = test_data[2];
  if (digitalRead(q_pins[3]) == expected_q3) {
    message = "SHIFT RIGHT is WORKING.";
    flag++;
  } else {
    message = "SHIFT RIGHT is NOT WORKING.";
  }
  scrollText(1, message);

  // --- TEST 4: SHIFT LEFT (S0=0, S1=1) ---
  // Reload same known pattern again
  parallel_load(test_data);

  digitalWrite(clk_pin, HIGH);
  digitalWrite(s0_pin, LOW);    // S0=0
  digitalWrite(s1_pin, HIGH);   // S1=1  -> SHIFT LEFT
  digitalWrite(dsl_pin, HIGH);  // serial input for left shift (set to known)
  delayMicroseconds(10);
  digitalWrite(clk_pin, LOW);
  delayMicroseconds(10);
  digitalWrite(clk_pin, HIGH);
  delayMicroseconds(50);

  // After one left shift, new Q0 == previous Q1 (test_data[1])
  int expected_q0 = test_data[1];
  if (digitalRead(q_pins[0]) == expected_q0) {
    message = "SHIFT LEFT is WORKING.";
    flag++;
  } else {
    message = "SHIFT LEFT is NOT WORKING.";
  }
  scrollText(1, message);

  return flag;
}

int check_74279() {
  check_message = "Checking IC 74279 (Quad S-R Latch, Active LOW Inputs)";
  scrollText(0, check_message);
  delay(delay_long);

  int flag = 0;
  int total_pins = 16;

  int gnd_pin = 8;   // GND
  int vcc_pin = 16;  // VCC

  // keep your existing offset logic ( (max_pin - total_pins) + start_pin == 30 )
  if (gnd_pin >= ((total_pins / 2) + 1))
    gnd_pin += ((max_pin - total_pins) + start_pin);
  else
    gnd_pin += start_pin;
  if (vcc_pin >= ((total_pins / 2) + 1))
    vcc_pin += ((max_pin - total_pins) + start_pin);
  else
    vcc_pin += start_pin;

  pinMode(gnd_pin, OUTPUT);
  digitalWrite(gnd_pin, LOW);
  pinMode(vcc_pin, OUTPUT);
  digitalWrite(vcc_pin, HIGH);

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    //Faulty Ground or VCC pin, Not need to check entire IC.
    error_message = "Ground or VCC pin is not working properly.";
    scrollText(1, error_message);
    delay(delay_long);
    return flag;  //Return 0
  }

  int pin, pin1;

  // --- IMPORTANT: pin pairs are {S, R} (SET then RESET) per datasheet ---
  // Latch1: S = 2, R = 1, Q = 4
  // Latch2: S = 6, R = 5, Q = 7
  // Latch3: S = 12, R =10, Q = 9   (latch 3 has two S inputs but we use S1=12 here)
  // Latch4: S = 15, R =14, Q =13
  int ip_pins[4][2] = { { 2, 1 }, { 6, 5 }, { 12, 10 }, { 15, 14 } };

  for (pin1 = 0; pin1 < 4; pin1++) {
    for (pin = 0; pin < 2; pin++) {
      if (ip_pins[pin1][pin] >= ((total_pins / 2) + 1))
        ip_pins[pin1][pin] += ((max_pin - total_pins) + start_pin);
      else
        ip_pins[pin1][pin] += start_pin;
      pinMode(ip_pins[pin1][pin], OUTPUT);
      digitalWrite(ip_pins[pin1][pin], HIGH);  // inactive (HIGH) by default
    }
  }

  // Output pins Q (these were correct)
  int op_pins[4] = { 4, 7, 9, 13 };
  for (pin = 0; pin < 4; pin++) {
    if (op_pins[pin] >= ((total_pins / 2) + 1))
      op_pins[pin] += ((max_pin - total_pins) + start_pin);
    else
      op_pins[pin] += start_pin;
    pinMode(op_pins[pin], INPUT);
  }

  // Truth table tests for each latch
  for (int latch = 0; latch < 4; latch++) {
    int check = 1;
    
    // --- SET test: S = LOW, R = HIGH => Q should go HIGH ---
    digitalWrite(ip_pins[latch][0], LOW);   // S LOW
    digitalWrite(ip_pins[latch][1], HIGH);  // R HIGH
    delayMicroseconds(100);
    if (digitalRead(op_pins[latch]) != HIGH)
      check = 0;

    // back to inactive
    digitalWrite(ip_pins[latch][0], HIGH);
    digitalWrite(ip_pins[latch][1], HIGH);
    delayMicroseconds(50);

    // --- RESET test: S = HIGH, R = LOW => Q should go LOW ---
    digitalWrite(ip_pins[latch][0], HIGH);  // S HIGH
    digitalWrite(ip_pins[latch][1], LOW);   // R LOW
    delayMicroseconds(100);
    if (digitalRead(op_pins[latch]) != LOW)
      check = 0;

    // reset inputs to inactive
    digitalWrite(ip_pins[latch][0], HIGH);
    digitalWrite(ip_pins[latch][1], HIGH);
    delay(delay_short);

    flag += check;
    if (check) {
      message = "Latch on pins " + String(getOriginalPin(ip_pins[latch][0], total_pins)) + "," + String(getOriginalPin(ip_pins[latch][1], total_pins)) + " -> " + String(getOriginalPin(op_pins[latch], total_pins)) + " is WORKING.";
    } else {
      message = "Latch on pins " + String(getOriginalPin(ip_pins[latch][0], total_pins)) + "," + String(getOriginalPin(ip_pins[latch][1], total_pins)) + " -> " + String(getOriginalPin(op_pins[latch], total_pins)) + " is NOT WORKING.";
    }
    scrollText(1, message);
  }

  return flag;
}

//Self testing funtions

// 1-input Logic Gates
int self_check_7404() {

  int flag = 0;

  int total_pins = 14;  // Total number of Pins

  int gnd_pin = 7;   // Ground Pin
  int vcc_pin = 14;  // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }
  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);    // Setting pinMode of Ground Pin
  digitalWrite(gnd_pin, LOW);  // Setting Value of Ground Pin

  pinMode(vcc_pin, OUTPUT);     // Setting pinMode of VCC Pin
  digitalWrite(vcc_pin, HIGH);  // Setting Value of VCC Pin

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    return flag;  //Return 0
  }

  int pin;

  int ip_pins[6] = { 1, 3, 5, 9, 11, 13 };  // Input Pins of IC 7404

  for (pin = 0; pin < 6; pin++) {
    // Setting Arduino Input Pins
    if (ip_pins[pin] >= ((total_pins / 2) + 1)) {
      ip_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      ip_pins[pin] += start_pin;
    }
    pinMode((ip_pins[pin]), OUTPUT);  // Setting pinMode of Input pins
  }

  int op_pins[6] = { 2, 4, 6, 8, 10, 12 };  // Output Pins of IC 7404

  for (pin = 0; pin < 6; pin++) {
    // Setting Arduino Output Pins
    if (op_pins[pin] >= ((total_pins / 2) + 1)) {
      op_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      op_pins[pin] += start_pin;
    }
    pinMode((op_pins[pin]), INPUT);  // Setting pinMode of Output pins
  }

  // Truth Table of IC 7404
  int tt_7404[2][2] = { { 0, 1 },
                        { 1, 0 } };

  // Checking IC 7404
  for (int cir_i = 0; cir_i < 6; cir_i++) {
    int check = 1;

    for (int tt_i = 0; tt_i < 2; tt_i++) {
      digitalWrite(ip_pins[cir_i], tt_7404[tt_i][0]);
      if (digitalRead(op_pins[cir_i]) != tt_7404[tt_i][1]) {
        check = 0;
      }
    }
    flag += check;
  }
  return flag;
}

// 2-input Logic Gates
int self_check_7400() {

  int flag = 0;

  int total_pins = 14;  // Total number of Pins

  int gnd_pin = 7;   // Ground Pin
  int vcc_pin = 14;  // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }
  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);    // Setting pinMode of Ground Pin
  digitalWrite(gnd_pin, LOW);  // Setting Value of Ground Pin

  pinMode(vcc_pin, OUTPUT);     // Setting pinMode of VCC Pin
  digitalWrite(vcc_pin, HIGH);  // Setting Value of VCC Pin

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    return flag;  //Return 0
  }

  int pin, pin1, pin2;

  int ip_pins[4][2] = { { 1, 2 }, { 4, 5 }, { 10, 9 }, { 13, 12 } };  // Input Pins of IC 7400

  for (pin1 = 0; pin1 < 4; pin1++) {
    for (pin2 = 0; pin2 < 2; pin2++) {
      // Setting Arduino Input Pins
      if (ip_pins[pin1][pin2] >= ((total_pins / 2) + 1)) {
        ip_pins[pin1][pin2] += ((max_pin - total_pins) + start_pin);
      } else {
        ip_pins[pin1][pin2] += start_pin;
      }
      pinMode((ip_pins[pin1][pin2]), OUTPUT);  // Setting pinMode of Input pins
    }
  }

  int op_pins[4] = { 3, 6, 8, 11 };  // Output Pins of IC 7400

  for (pin = 0; pin < 4; pin++) {
    // Setting Arduino Output Pins
    if (op_pins[pin] >= ((total_pins / 2) + 1)) {
      op_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      op_pins[pin] += start_pin;
    }
    pinMode((op_pins[pin]), INPUT);  // Setting pinmode of Output pins
  }

  // Truth Table of IC 7400
  int tt_7400[4][3] = { { 0, 0, 1 },
                        { 0, 1, 1 },
                        { 1, 0, 1 },
                        { 1, 1, 0 } };

  // Checking IC 7400
  for (int cir_i = 0; cir_i < 4; cir_i++) {
    int check = 1;

    for (int tt_i = 0; tt_i < 4; tt_i++) {
      digitalWrite(ip_pins[cir_i][0], tt_7400[tt_i][0]);

      digitalWrite(ip_pins[cir_i][1], tt_7400[tt_i][1]);

      if (digitalRead(op_pins[cir_i]) != tt_7400[tt_i][2]) {
        check = 0;
      }
    }
    flag += check;
  }
  return flag;
}

int self_check_7402() {

  int flag = 0;

  int total_pins = 14;  // Total number of Pins

  int gnd_pin = 7;   // Ground Pin
  int vcc_pin = 14;  // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }
  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);    // Setting pinMode of Ground Pin
  digitalWrite(gnd_pin, LOW);  // Setting Value of Ground Pin

  pinMode(vcc_pin, OUTPUT);     // Setting pinMode of VCC Pin
  digitalWrite(vcc_pin, HIGH);  // Setting Value of VCC Pin

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {

    return flag;  //Return 0
  }

  int pin, pin1, pin2;

  int ip_pins[4][2] = { { 2, 3 }, { 5, 6 }, { 8, 9 }, { 11, 12 } };  // Input Pins of IC 7402

  for (pin1 = 0; pin1 < 4; pin1++) {
    for (pin2 = 0; pin2 < 2; pin2++) {
      // Setting Arduino Input Pins
      if (ip_pins[pin1][pin2] >= ((total_pins / 2) + 1)) {
        ip_pins[pin1][pin2] += ((max_pin - total_pins) + start_pin);
      } else {
        ip_pins[pin1][pin2] += start_pin;
      }
      pinMode((ip_pins[pin1][pin2]), OUTPUT);  // Setting pinMode of Input pins
    }
  }

  int op_pins[4] = { 1, 4, 10, 13 };  // Output Pins of IC 7402

  for (pin = 0; pin < 4; pin++) {
    // Setting Arduino Output Pins
    if (op_pins[pin] >= ((total_pins / 2) + 1)) {
      op_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      op_pins[pin] += start_pin;
    }
    pinMode((op_pins[pin]), INPUT);  // Setting pinmode of Output pins
  }

  // Truth Table of IC 7402
  int tt_7402[4][3] = { { 0, 0, 1 },
                        { 0, 1, 0 },
                        { 1, 0, 0 },
                        { 1, 1, 0 } };

  // Checking IC 7402
  for (int cir_i = 0; cir_i < 4; cir_i++) {
    int check = 1;

    for (int tt_i = 0; tt_i < 4; tt_i++) {
      digitalWrite(ip_pins[cir_i][0], tt_7402[tt_i][0]);

      digitalWrite(ip_pins[cir_i][1], tt_7402[tt_i][1]);

      if (digitalRead(op_pins[cir_i]) != tt_7402[tt_i][2]) {
        check = 0;
      }
    }
    flag += check;
  }
  return flag;
}

int self_check_7408() {

  int flag = 0;

  int total_pins = 14;  // Total number of Pins

  int gnd_pin = 7;   // Ground Pin
  int vcc_pin = 14;  // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }
  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);    // Setting pinMode of Ground Pin
  digitalWrite(gnd_pin, LOW);  // Setting Value of Ground Pin

  pinMode(vcc_pin, OUTPUT);     // Setting pinMode of VCC Pin
  digitalWrite(vcc_pin, HIGH);  // Setting Value of VCC Pin

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {

    return flag;  //Return 0
  }

  int pin, pin1, pin2;

  int ip_pins[4][2] = { { 1, 2 }, { 4, 5 }, { 10, 9 }, { 13, 12 } };  // Input Pins of IC 7408

  for (pin1 = 0; pin1 < 4; pin1++) {
    for (pin2 = 0; pin2 < 2; pin2++) {
      // Setting Arduino Input Pins
      if (ip_pins[pin1][pin2] >= ((total_pins / 2) + 1)) {
        ip_pins[pin1][pin2] += ((max_pin - total_pins) + start_pin);
      } else {
        ip_pins[pin1][pin2] += start_pin;
      }
      pinMode((ip_pins[pin1][pin2]), OUTPUT);  // Setting pinMode of Input pins
    }
  }

  int op_pins[4] = { 3, 6, 8, 11 };  // Output Pins of IC 7408

  for (pin = 0; pin < 4; pin++) {
    // Setting Arduino Output Pins
    if (op_pins[pin] >= ((total_pins / 2) + 1)) {
      op_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      op_pins[pin] += start_pin;
    }
    pinMode((op_pins[pin]), INPUT);  // Setting pinmode of Output pins
  }

  // Truth Table of IC 7408
  int tt_7408[4][3] = { { 0, 0, 0 },
                        { 0, 1, 0 },
                        { 1, 0, 0 },
                        { 1, 1, 1 } };

  // Checking IC 7408
  for (int cir_i = 0; cir_i < 4; cir_i++) {
    int check = 1;

    for (int tt_i = 0; tt_i < 4; tt_i++) {
      digitalWrite(ip_pins[cir_i][0], tt_7408[tt_i][0]);

      digitalWrite(ip_pins[cir_i][1], tt_7408[tt_i][1]);

      if (digitalRead(op_pins[cir_i]) != tt_7408[tt_i][2]) {
        check = 0;
      }
    }
    flag += check;
  }
  return flag;
}

int self_check_7428() {

  int flag = 0;

  int total_pins = 14;  // Total number of Pins

  int gnd_pin = 7;   // Ground Pin
  int vcc_pin = 14;  // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }
  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);    // Setting pinMode of Ground Pin
  digitalWrite(gnd_pin, LOW);  // Setting Value of Ground Pin

  pinMode(vcc_pin, OUTPUT);     // Setting pinMode of VCC Pin
  digitalWrite(vcc_pin, HIGH);  // Setting Value of VCC Pin

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {

    return flag;  //Return 0
  }

  int pin, pin1, pin2;

  int ip_pins[4][2] = { { 2, 3 }, { 5, 6 }, { 8, 9 }, { 11, 12 } };  // Input Pins of IC 7428

  for (pin1 = 0; pin1 < 4; pin1++) {
    for (pin2 = 0; pin2 < 2; pin2++) {
      // Setting Arduino Input Pins
      if (ip_pins[pin1][pin2] >= ((total_pins / 2) + 1)) {
        ip_pins[pin1][pin2] += ((max_pin - total_pins) + start_pin);
      } else {
        ip_pins[pin1][pin2] += start_pin;
      }
      pinMode((ip_pins[pin1][pin2]), OUTPUT);  // Setting pinMode of Input pins
    }
  }

  int op_pins[4] = { 1, 4, 10, 13 };  // Output Pins of IC 7428

  for (pin = 0; pin < 4; pin++) {
    // Setting Arduino Output Pins
    if (op_pins[pin] >= ((total_pins / 2) + 1)) {
      op_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      op_pins[pin] += start_pin;
    }
    pinMode((op_pins[pin]), INPUT);  // Setting pinmode of Output pins
  }

  // Truth Table of IC 7428
  int tt_7428[4][3] = { { 0, 0, 1 },
                        { 0, 1, 0 },
                        { 1, 0, 0 },
                        { 1, 1, 0 } };

  // Checking IC 7428
  for (int cir_i = 0; cir_i < 4; cir_i++) {
    int check = 1;

    for (int tt_i = 0; tt_i < 4; tt_i++) {
      digitalWrite(ip_pins[cir_i][0], tt_7428[tt_i][0]);

      digitalWrite(ip_pins[cir_i][1], tt_7428[tt_i][1]);

      if (digitalRead(op_pins[cir_i]) != tt_7428[tt_i][2]) {
        check = 0;
      }
    }
    flag += check;
  }
  return flag;
}

int self_check_7432() {

  int flag = 0;

  int total_pins = 14;  // Total number of Pins

  int gnd_pin = 7;   // Ground Pin
  int vcc_pin = 14;  // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }
  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);    // Setting pinMode of Ground Pin
  digitalWrite(gnd_pin, LOW);  // Setting Value of Ground Pin

  pinMode(vcc_pin, OUTPUT);     // Setting pinMode of VCC Pin
  digitalWrite(vcc_pin, HIGH);  // Setting Value of VCC Pin

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {

    return flag;  //Return 0
  }

  int pin, pin1, pin2;

  int ip_pins[4][2] = { { 1, 2 }, { 4, 5 }, { 10, 9 }, { 13, 12 } };  // Input Pins of IC 7432

  for (pin1 = 0; pin1 < 4; pin1++) {
    for (pin2 = 0; pin2 < 2; pin2++) {
      // Setting Arduino Input Pins
      if (ip_pins[pin1][pin2] >= ((total_pins / 2) + 1)) {
        ip_pins[pin1][pin2] += ((max_pin - total_pins) + start_pin);
      } else {
        ip_pins[pin1][pin2] += start_pin;
      }
      pinMode((ip_pins[pin1][pin2]), OUTPUT);  // Setting pinMode of Input pins
    }
  }

  int op_pins[4] = { 3, 6, 8, 11 };  // Output Pins of IC 7432

  for (pin = 0; pin < 4; pin++) {
    // Setting Arduino Output Pins
    if (op_pins[pin] >= ((total_pins / 2) + 1)) {
      op_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      op_pins[pin] += start_pin;
    }
    pinMode((op_pins[pin]), INPUT);  // Setting pinmode of Output pins
  }

  // Truth Table of IC 7432
  int tt_7432[4][3] = { { 0, 0, 0 },
                        { 0, 1, 1 },
                        { 1, 0, 1 },
                        { 1, 1, 1 } };

  // Checking IC 7432
  for (int cir_i = 0; cir_i < 4; cir_i++) {
    int check = 1;

    for (int tt_i = 0; tt_i < 4; tt_i++) {
      digitalWrite(ip_pins[cir_i][0], tt_7432[tt_i][0]);

      digitalWrite(ip_pins[cir_i][1], tt_7432[tt_i][1]);

      if (digitalRead(op_pins[cir_i]) != tt_7432[tt_i][2]) {
        check = 0;
      }
    }
    flag += check;
  }
  return flag;
}

int self_check_7486() {

  int flag = 0;

  int gnd_pin = 7;   // Ground Pin
  int vcc_pin = 14;  // VCC Pin

  int total_pins = 14;  // Total number of Pins

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }
  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);    // Setting pinMode of Ground Pin
  digitalWrite(gnd_pin, LOW);  // Setting Value of Ground Pin

  pinMode(vcc_pin, OUTPUT);     // Setting pinMode of VCC Pin
  digitalWrite(vcc_pin, HIGH);  // Setting Value of VCC Pin

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {

    return flag;  //Return 0
  }

  int pin, pin1, pin2;

  int ip_pins[4][2] = { { 1, 2 }, { 4, 5 }, { 10, 9 }, { 13, 12 } };  // Input Pins of IC 7486

  for (pin1 = 0; pin1 < 4; pin1++) {
    for (pin2 = 0; pin2 < 2; pin2++) {
      // Setting Arduino Input Pins
      if (ip_pins[pin1][pin2] >= ((total_pins / 2) + 1)) {
        ip_pins[pin1][pin2] += ((max_pin - total_pins) + start_pin);
      } else {
        ip_pins[pin1][pin2] += start_pin;
      }
      pinMode((ip_pins[pin1][pin2]), OUTPUT);  // Setting pinMode of Input pins
    }
  }

  int op_pins[4] = { 3, 6, 8, 11 };  // Output Pins of IC 7486

  for (pin = 0; pin < 4; pin++) {
    // Setting Arduino Output Pins
    if (op_pins[pin] >= ((total_pins / 2) + 1)) {
      op_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      op_pins[pin] += start_pin;
    }
    pinMode((op_pins[pin]), INPUT);  // Setting pinmode of Output pins
  }

  // Truth Table of IC 7486
  int tt_7486[4][3] = { { 0, 0, 0 },
                        { 0, 1, 1 },
                        { 1, 0, 1 },
                        { 1, 1, 0 } };

  // Checking IC 7486
  for (int cir_i = 0; cir_i < 4; cir_i++) {
    int check = 1;

    for (int tt_i = 0; tt_i < 4; tt_i++) {
      digitalWrite(ip_pins[cir_i][0], tt_7486[tt_i][0]);

      digitalWrite(ip_pins[cir_i][1], tt_7486[tt_i][1]);

      if (digitalRead(op_pins[cir_i]) != tt_7486[tt_i][2]) {
        check = 0;
      }
    }
    flag += check;
  }
  return flag;
}

int self_check_74136() {

  int flag = 0;

  int total_pins = 14;  // Total number of Pins

  int gnd_pin = 7;   // Ground Pin
  int vcc_pin = 14;  // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }
  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);    // Setting pinMode of Ground Pin
  digitalWrite(gnd_pin, LOW);  // Setting Value of Ground Pin

  pinMode(vcc_pin, OUTPUT);     // Setting pinMode of VCC Pin
  digitalWrite(vcc_pin, HIGH);  // Setting Value of VCC Pin

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {

    return flag;  //Return 0
  }

  int pin, pin1, pin2;

  int ip_pins[4][2] = { { 1, 2 }, { 4, 5 }, { 10, 9 }, { 13, 12 } };  // Input Pins of IC 74136

  for (pin1 = 0; pin1 < 4; pin1++) {
    for (pin2 = 0; pin2 < 2; pin2++) {
      // Setting Arduino Input Pins
      if (ip_pins[pin1][pin2] >= ((total_pins / 2) + 1)) {
        ip_pins[pin1][pin2] += ((max_pin - total_pins) + start_pin);
      } else {
        ip_pins[pin1][pin2] += start_pin;
      }
      pinMode((ip_pins[pin1][pin2]), OUTPUT);  // Setting pinMode of Input pins
    }
  }

  int op_pins[4] = { 3, 6, 8, 11 };  // Output Pins of IC 7486

  for (pin = 0; pin < 4; pin++) {
    // Setting Arduino Output Pins
    if (op_pins[pin] >= ((total_pins / 2) + 1)) {
      op_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      op_pins[pin] += start_pin;
    }
    pinMode((op_pins[pin]), INPUT);  // Setting pinmode of Output pins
  }

  // Truth Table of IC 74136
  int tt_74136[4][3] = { { 0, 0, 0 },
                         { 0, 1, 1 },
                         { 1, 0, 1 },
                         { 1, 1, 0 } };

  // Checking IC 74136
  for (int cir_i = 0; cir_i < 4; cir_i++) {
    int check = 1;

    for (int tt_i = 0; tt_i < 4; tt_i++) {
      digitalWrite(ip_pins[cir_i][0], tt_74136[tt_i][0]);

      digitalWrite(ip_pins[cir_i][1], tt_74136[tt_i][1]);

      if (digitalRead(op_pins[cir_i]) != tt_74136[tt_i][2]) {
        check = 0;
      }
    }
    flag += check;
  }
  return flag;
}

int self_check_74226() {

  int flag = 0;

  int total_pins = 14;  // Total number of Pins

  int gnd_pin = 7;   // Ground Pin
  int vcc_pin = 14;  // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }
  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);    // Setting pinMode of Ground Pin
  digitalWrite(gnd_pin, LOW);  // Setting Value of Ground Pin

  pinMode(vcc_pin, OUTPUT);     // Setting pinMode of VCC Pin
  digitalWrite(vcc_pin, HIGH);  // Setting Value of VCC Pin

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {

    return flag;  //Return 0
  }

  int pin, pin1, pin2;

  int ip_pins[4][2] = { { 1, 2 }, { 4, 5 }, { 10, 9 }, { 13, 12 } };  // Input Pins of IC 74226

  for (pin1 = 0; pin1 < 4; pin1++) {
    for (pin2 = 0; pin2 < 2; pin2++) {
      // Setting Arduino Input Pins
      if (ip_pins[pin1][pin2] >= ((total_pins / 2) + 1)) {
        ip_pins[pin1][pin2] += ((max_pin - total_pins) + start_pin);
      } else {
        ip_pins[pin1][pin2] += start_pin;
      }
      pinMode((ip_pins[pin1][pin2]), OUTPUT);  // Setting pinMode of Input pins
    }
  }

  int op_pins[4] = { 3, 6, 8, 11 };  // Output Pins of IC 74226

  for (pin = 0; pin < 4; pin++) {
    // Setting Arduino Output Pins
    if (op_pins[pin] >= ((total_pins / 2) + 1)) {
      op_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      op_pins[pin] += start_pin;
    }
    pinMode((op_pins[pin]), INPUT);  // Setting pinmode of Output pins
  }

  // Truth Table of IC 7486
  int tt_74226[4][3] = { { 0, 0, 1 },
                         { 0, 1, 0 },
                         { 1, 0, 0 },
                         { 1, 1, 1 } };

  // Checking IC 74226
  for (int cir_i = 0; cir_i < 4; cir_i++) {
    int check = 1;

    for (int tt_i = 0; tt_i < 4; tt_i++) {
      digitalWrite(ip_pins[cir_i][0], tt_74226[tt_i][0]);

      digitalWrite(ip_pins[cir_i][1], tt_74226[tt_i][1]);

      if (digitalRead(op_pins[cir_i]) != tt_74226[tt_i][2]) {
        check = 0;
      }
    }
    flag += check;
  }
  return flag;
}

int self_check_74266() {

  int flag = 0;

  int total_pins = 14;  // Total number of Pins

  int gnd_pin = 7;   // Ground Pin
  int vcc_pin = 14;  // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }
  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);    // Setting pinMode of Ground Pin
  digitalWrite(gnd_pin, LOW);  // Setting Value of Ground Pin

  pinMode(vcc_pin, OUTPUT);     // Setting pinMode of VCC Pin
  digitalWrite(vcc_pin, HIGH);  // Setting Value of VCC Pin

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {

    return flag;  //Return 0
  }

  int pin, pin1, pin2;

  int ip_pins[4][2] = { { 1, 2 }, { 4, 5 }, { 10, 9 }, { 13, 12 } };  // Input Pins of IC 74266

  for (pin1 = 0; pin1 < 4; pin1++) {
    for (pin2 = 0; pin2 < 2; pin2++) {
      // Setting Arduino Input Pins
      if (ip_pins[pin1][pin2] >= ((total_pins / 2) + 1)) {
        ip_pins[pin1][pin2] += ((max_pin - total_pins) + start_pin);
      } else {
        ip_pins[pin1][pin2] += start_pin;
      }
      pinMode((ip_pins[pin1][pin2]), OUTPUT);  // Setting pinMode of Input pins
    }
  }

  int op_pins[4] = { 3, 6, 8, 11 };  // Output Pins of IC 74266

  for (pin = 0; pin < 4; pin++) {
    // Setting Arduino Output Pins
    if (op_pins[pin] >= ((total_pins / 2) + 1)) {
      op_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      op_pins[pin] += start_pin;
    }
    pinMode((op_pins[pin]), INPUT);  // Setting pinmode of Output pins
  }

  // Truth Table of IC 74266
  int tt_74266[4][3] = { { 0, 0, 1 },
                         { 0, 1, 0 },
                         { 1, 0, 0 },
                         { 1, 1, 1 } };

  // Checking IC 74266
  for (int cir_i = 0; cir_i < 4; cir_i++) {
    int check = 1;

    for (int tt_i = 0; tt_i < 4; tt_i++) {
      digitalWrite(ip_pins[cir_i][0], tt_74266[tt_i][0]);

      digitalWrite(ip_pins[cir_i][1], tt_74266[tt_i][1]);

      if (digitalRead(op_pins[cir_i]) != tt_74266[tt_i][2]) {
        check = 0;
      }
    }
    flag += check;
  }
  return flag;
}

// 3-input Logic Gates
int self_check_7410() {

  int flag = 0;

  int total_pins = 14;  // Total number of Pins

  int gnd_pin = 7;   // Ground Pin
  int vcc_pin = 14;  // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }
  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);    // Setting pinMode of Ground Pin
  digitalWrite(gnd_pin, LOW);  // Setting Value of Ground Pin

  pinMode(vcc_pin, OUTPUT);     // Setting pinMode of VCC Pin
  digitalWrite(vcc_pin, HIGH);  // Setting Value of VCC Pin

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {

    return flag;  //Return 0
  }

  int pin, pin1, pin2;

  int ip_pins[3][3] = { { 1, 2, 13 }, { 3, 4, 5 }, { 11, 10, 9 } };  // Input Pins of IC 7410

  for (pin1 = 0; pin1 < 3; pin1++) {
    for (pin2 = 0; pin2 < 3; pin2++) {
      // Setting Arduino Input Pins
      if (ip_pins[pin1][pin2] >= ((total_pins / 2) + 1)) {
        ip_pins[pin1][pin2] += ((max_pin - total_pins) + start_pin);
      } else {
        ip_pins[pin1][pin2] += start_pin;
      }
      pinMode((ip_pins[pin1][pin2]), OUTPUT);  // Setting pinmode of Input pins
    }
  }

  int op_pins[3] = { 12, 6, 8 };  // Output Pins of IC 7410

  for (pin = 0; pin < 3; pin++) {
    // Setting Arduino Output Pins
    if (op_pins[pin] >= ((total_pins / 2) + 1)) {
      op_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      op_pins[pin] += start_pin;
    }
    pinMode((op_pins[pin]), INPUT);  // Setting pinmode of Output pins
  }

  // Truth Table of IC 7410
  int tt_7410[8][4] = { { 0, 0, 0, 1 },
                        { 0, 0, 1, 1 },
                        { 0, 1, 0, 1 },
                        { 0, 1, 1, 1 },
                        { 1, 0, 0, 1 },
                        { 1, 0, 1, 1 },
                        { 1, 1, 0, 1 },
                        { 1, 1, 1, 0 } };

  // Checking IC 7410
  for (int cir_i = 0; cir_i < 3; cir_i++) {
    int check = 1;

    for (int tt_i = 0; tt_i < 8; tt_i++) {
      digitalWrite(ip_pins[cir_i][0], tt_7410[tt_i][0]);

      digitalWrite(ip_pins[cir_i][1], tt_7410[tt_i][1]);

      digitalWrite(ip_pins[cir_i][2], tt_7410[tt_i][2]);

      if (digitalRead(op_pins[cir_i]) != tt_7410[tt_i][3]) {
        check = 0;
        break;
      }
    }
    flag += check;
  }
  return flag;
}

int self_check_7411() {

  int flag = 0;

  int total_pins = 14;  // Total number of Pins

  int gnd_pin = 7;   // Ground Pin
  int vcc_pin = 14;  // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }
  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);    // Setting pinMode of Ground Pin
  digitalWrite(gnd_pin, LOW);  // Setting Value of Ground Pin

  pinMode(vcc_pin, OUTPUT);     // Setting pinMode of VCC Pin
  digitalWrite(vcc_pin, HIGH);  // Setting Value of VCC Pin

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {

    return flag;  //Return 0
  }

  int pin, pin1, pin2;

  int ip_pins[3][3] = { { 1, 2, 13 }, { 3, 4, 5 }, { 11, 10, 9 } };  // Input Pins of IC 7411

  for (pin1 = 0; pin1 < 3; pin1++) {
    for (pin2 = 0; pin2 < 3; pin2++) {
      // Setting Arduino Input Pins
      if (ip_pins[pin1][pin2] >= ((total_pins / 2) + 1)) {
        ip_pins[pin1][pin2] += ((max_pin - total_pins) + start_pin);
      } else {
        ip_pins[pin1][pin2] += start_pin;
      }
      pinMode((ip_pins[pin1][pin2]), OUTPUT);  // Setting pinmode of Input pins
    }
  }

  int op_pins[3] = { 12, 6, 8 };  // Output Pins of IC 7411

  for (pin = 0; pin < 3; pin++) {
    // Setting Arduino Output Pins
    if (op_pins[pin] >= ((total_pins / 2) + 1)) {
      op_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      op_pins[pin] += start_pin;
    }
    pinMode((op_pins[pin]), INPUT);  // Setting pinmode of Output pins
  }

  // Truth Table of IC 7411
  int tt_7411[8][4] = { { 0, 0, 0, 0 },
                        { 0, 0, 1, 0 },
                        { 0, 1, 0, 0 },
                        { 0, 1, 1, 0 },
                        { 1, 0, 0, 0 },
                        { 1, 0, 1, 0 },
                        { 1, 1, 0, 0 },
                        { 1, 1, 1, 1 } };

  // Checking IC 7411
  for (int cir_i = 0; cir_i < 3; cir_i++) {
    int check = 1;

    for (int tt_i = 0; tt_i < 8; tt_i++) {
      digitalWrite(ip_pins[cir_i][0], tt_7411[tt_i][0]);

      digitalWrite(ip_pins[cir_i][1], tt_7411[tt_i][1]);

      digitalWrite(ip_pins[cir_i][2], tt_7411[tt_i][2]);

      if (digitalRead(op_pins[cir_i]) != tt_7411[tt_i][3]) {
        check = 0;
        break;
      }
    }
    flag += check;
  }
  return flag;
}

int self_check_7412() {

  int flag = 0;

  int total_pins = 14;  // Total number of Pins

  int gnd_pin = 7;   // Ground Pin
  int vcc_pin = 14;  // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }
  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);    // Setting pinMode of Ground Pin
  digitalWrite(gnd_pin, LOW);  // Setting Value of Ground Pin

  pinMode(vcc_pin, OUTPUT);     // Setting pinMode of VCC Pin
  digitalWrite(vcc_pin, HIGH);  // Setting Value of VCC Pin

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {

    return flag;  //Return 0
  }

  int pin, pin1, pin2;

  int ip_pins[3][3] = { { 1, 2, 13 }, { 3, 4, 5 }, { 11, 10, 9 } };  // Input Pins of IC 7412

  for (pin1 = 0; pin1 < 3; pin1++) {
    for (pin2 = 0; pin2 < 3; pin2++) {
      // Setting Arduino Input Pins
      if (ip_pins[pin1][pin2] >= ((total_pins / 2) + 1)) {
        ip_pins[pin1][pin2] += ((max_pin - total_pins) + start_pin);
      } else {
        ip_pins[pin1][pin2] += start_pin;
      }
      pinMode((ip_pins[pin1][pin2]), OUTPUT);  // Setting pinmode of Input pins
    }
  }

  int op_pins[3] = { 12, 6, 8 };  // Output Pins of IC 7412

  for (pin = 0; pin < 3; pin++) {
    // Setting Arduino Output Pins
    if (op_pins[pin] >= ((total_pins / 2) + 1)) {
      op_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      op_pins[pin] += start_pin;
    }
    pinMode((op_pins[pin]), INPUT);  // Setting pinmode of Output pins
  }

  // Truth Table of IC 7412
  int tt_7412[8][4] = { { 0, 0, 0, 1 },
                        { 0, 0, 1, 1 },
                        { 0, 1, 0, 1 },
                        { 0, 1, 1, 1 },
                        { 1, 0, 0, 1 },
                        { 1, 0, 1, 1 },
                        { 1, 1, 0, 1 },
                        { 1, 1, 1, 0 } };

  // Checking IC 7412
  for (int cir_i = 0; cir_i < 3; cir_i++) {
    int check = 1;

    for (int tt_i = 0; tt_i < 8; tt_i++) {
      digitalWrite(ip_pins[cir_i][0], tt_7412[tt_i][0]);

      digitalWrite(ip_pins[cir_i][1], tt_7412[tt_i][1]);

      digitalWrite(ip_pins[cir_i][2], tt_7412[tt_i][2]);

      if (digitalRead(op_pins[cir_i]) != tt_7412[tt_i][3]) {
        check = 0;
        break;
      }
    }
    flag += check;
  }
  return flag;
}

int self_check_7415() {

  int flag = 0;

  int total_pins = 14;  // Total number of Pins

  int gnd_pin = 7;   // Ground Pin
  int vcc_pin = 14;  // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }
  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);    // Setting pinMode of Ground Pin
  digitalWrite(gnd_pin, LOW);  // Setting Value of Ground Pin

  pinMode(vcc_pin, OUTPUT);     // Setting pinMode of VCC Pin
  digitalWrite(vcc_pin, HIGH);  // Setting Value of VCC Pin

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {

    return flag;  //Return 0
  }

  int pin, pin1, pin2;

  int ip_pins[3][3] = { { 1, 2, 13 }, { 3, 4, 5 }, { 11, 10, 9 } };  // Input Pins of IC 7415

  for (pin1 = 0; pin1 < 3; pin1++) {
    for (pin2 = 0; pin2 < 3; pin2++) {
      // Setting Arduino Input Pins
      if (ip_pins[pin1][pin2] >= ((total_pins / 2) + 1)) {
        ip_pins[pin1][pin2] += ((max_pin - total_pins) + start_pin);
      } else {
        ip_pins[pin1][pin2] += start_pin;
      }
      pinMode((ip_pins[pin1][pin2]), OUTPUT);  // Setting pinmode of Input pins
    }
  }

  int op_pins[3] = { 12, 6, 8 };  // Output Pins of IC 7415

  for (pin = 0; pin < 3; pin++) {
    // Setting Arduino Output Pins
    if (op_pins[pin] >= ((total_pins / 2) + 1)) {
      op_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      op_pins[pin] += start_pin;
    }
    pinMode((op_pins[pin]), INPUT);  // Setting pinmode of Output pins
  }

  // Truth Table of IC 7415
  int tt_7415[8][4] = { { 0, 0, 0, 0 },
                        { 0, 0, 1, 0 },
                        { 0, 1, 0, 0 },
                        { 0, 1, 1, 0 },
                        { 1, 0, 0, 0 },
                        { 1, 0, 1, 0 },
                        { 1, 1, 0, 0 },
                        { 1, 1, 1, 1 } };

  // Checking IC 7415
  for (int cir_i = 0; cir_i < 3; cir_i++) {
    int check = 1;

    for (int tt_i = 0; tt_i < 8; tt_i++) {
      digitalWrite(ip_pins[cir_i][0], tt_7415[tt_i][0]);

      digitalWrite(ip_pins[cir_i][1], tt_7415[tt_i][1]);

      digitalWrite(ip_pins[cir_i][2], tt_7415[tt_i][2]);

      if (digitalRead(op_pins[cir_i]) != tt_7415[tt_i][3]) {
        check = 0;
        break;
      }
    }
    flag += check;
  }
  return flag;
}

int self_check_7427() {

  int flag = 0;

  int total_pins = 14;  // Total number of Pins

  int gnd_pin = 7;   // Ground Pin
  int vcc_pin = 14;  // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }
  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);    // Setting pinMode of Ground Pin
  digitalWrite(gnd_pin, LOW);  // Setting Value of Ground Pin

  pinMode(vcc_pin, OUTPUT);     // Setting pinMode of VCC Pin
  digitalWrite(vcc_pin, HIGH);  // Setting Value of VCC Pin

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {

    return flag;  //Return 0
  }

  int pin, pin1, pin2;

  int ip_pins[3][3] = { { 1, 2, 13 }, { 3, 4, 5 }, { 11, 10, 9 } };  // Input Pins of IC 7427

  for (pin1 = 0; pin1 < 3; pin1++) {
    for (pin2 = 0; pin2 < 3; pin2++) {
      // Setting Arduino Input Pins
      if (ip_pins[pin1][pin2] >= ((total_pins / 2) + 1)) {
        ip_pins[pin1][pin2] += ((max_pin - total_pins) + start_pin);
      } else {
        ip_pins[pin1][pin2] += start_pin;
      }
      pinMode((ip_pins[pin1][pin2]), OUTPUT);  // Setting pinmode of Input pins
    }
  }

  int op_pins[3] = { 12, 6, 8 };  // Output Pins of IC 7427

  for (pin = 0; pin < 3; pin++) {
    // Setting Arduino Output Pins
    if (op_pins[pin] >= ((total_pins / 2) + 1)) {
      op_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      op_pins[pin] += start_pin;
    }
    pinMode((op_pins[pin]), INPUT);  // Setting pinmode of Output pins
  }

  // Truth Table of IC 7427
  int tt_7427[8][4] = { { 0, 0, 0, 1 },
                        { 0, 0, 1, 0 },
                        { 0, 1, 0, 0 },
                        { 0, 1, 1, 0 },
                        { 1, 0, 0, 0 },
                        { 1, 0, 1, 0 },
                        { 1, 1, 0, 0 },
                        { 1, 1, 1, 0 } };

  // Checking IC 7410
  for (int cir_i = 0; cir_i < 3; cir_i++) {
    int check = 1;

    for (int tt_i = 0; tt_i < 8; tt_i++) {
      digitalWrite(ip_pins[cir_i][0], tt_7427[tt_i][0]);

      digitalWrite(ip_pins[cir_i][1], tt_7427[tt_i][1]);

      digitalWrite(ip_pins[cir_i][2], tt_7427[tt_i][2]);

      if (digitalRead(op_pins[cir_i]) != tt_7427[tt_i][3]) {
        check = 0;
        break;
      }
    }
    flag += check;
  }
  return flag;
}

// 4-input Logic Gates
int self_check_7420() {

  int flag = 0;

  int total_pins = 14;  // Total number of Pins

  int gnd_pin = 7;   // Ground Pin
  int vcc_pin = 14;  // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }
  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);    // Setting pinMode of Ground Pin
  digitalWrite(gnd_pin, LOW);  // Setting Value of Ground Pin

  pinMode(vcc_pin, OUTPUT);     // Setting pinMode of VCC Pin
  digitalWrite(vcc_pin, HIGH);  // Setting Value of VCC Pin

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    return flag;  //Return 0
  }

  int strobe1_pin = 3;   // Strobe1  Pin
  int strobe2_pin = 11;  // Strobe2 Pin

  // Setting Arduino strobe1 pin
  if (strobe1_pin >= ((total_pins / 2) + 1)) {
    strobe1_pin += ((max_pin - total_pins) + start_pin);
  } else {
    strobe1_pin += start_pin;
  }

  // Setting Arduino strobe2 pin
  if (strobe2_pin >= ((total_pins / 2) + 1)) {
    strobe2_pin += ((max_pin - total_pins) + start_pin);
  } else {
    strobe2_pin += start_pin;
  }
  pinMode(strobe1_pin, OUTPUT);    // Setting pinMode of strobe1 Pin
  digitalWrite(strobe1_pin, LOW);  // Setting Value of strobe1 Pin

  pinMode(strobe2_pin, OUTPUT);    // Setting pinMode of strobe2 Pin
  digitalWrite(strobe2_pin, LOW);  // Setting Value of strobe2 Pin

  if (digitalRead(strobe1_pin) != 0 || digitalRead(strobe2_pin) != 0) {
    return flag;  //Return 0
  }

  int pin, pin1, pin2;

  int ip_pins[2][4] = { { 1, 2, 4, 5 }, { 13, 12, 10, 9 } };  // Input Pins of IC 7420

  for (pin1 = 0; pin1 < 2; pin1++) {
    for (pin2 = 0; pin2 < 4; pin2++) {
      // Setting Arduino Input Pins
      if (ip_pins[pin1][pin2] >= ((total_pins / 2) + 1)) {
        ip_pins[pin1][pin2] += ((max_pin - total_pins) + start_pin);
      } else {
        ip_pins[pin1][pin2] += start_pin;
      }
      pinMode((ip_pins[pin1][pin2]), OUTPUT);  // Setting pinmode of Input pins
    }
  }

  int op_pins[2] = { 6, 8 };  // Output Pins of IC 7420

  for (pin = 0; pin < 2; pin++) {
    // Setting Arduino Output Pins
    if (op_pins[pin] >= ((total_pins / 2) + 1)) {
      op_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      op_pins[pin] += start_pin;
    }
    pinMode((op_pins[pin]), INPUT);  // Setting pinmode of Output pins
  }

  // Truth Table of IC 7420
  int tt_7420[16][5] = { { 0, 0, 0, 0, 1 },
                         { 0, 0, 0, 1, 1 },
                         { 0, 0, 1, 0, 1 },
                         { 0, 0, 1, 1, 1 },
                         { 0, 1, 0, 0, 1 },
                         { 0, 1, 0, 1, 1 },
                         { 0, 1, 1, 0, 1 },
                         { 0, 1, 1, 1, 1 },
                         { 1, 0, 0, 0, 1 },
                         { 1, 0, 0, 1, 1 },
                         { 1, 0, 1, 0, 1 },
                         { 1, 0, 1, 1, 1 },
                         { 1, 1, 0, 0, 1 },
                         { 1, 1, 0, 1, 1 },
                         { 1, 1, 1, 0, 1 },
                         { 1, 1, 1, 1, 0 } };
  // Checking IC 7420
  for (int cir_i = 0; cir_i < 2; cir_i++) {
    int check = 1;

    for (int tt_i = 0; tt_i < 16; tt_i++) {
      digitalWrite(ip_pins[cir_i][0], tt_7420[tt_i][0]);

      digitalWrite(ip_pins[cir_i][1], tt_7420[tt_i][1]);

      digitalWrite(ip_pins[cir_i][2], tt_7420[tt_i][2]);

      digitalWrite(ip_pins[cir_i][3], tt_7420[tt_i][3]);

      if (digitalRead(op_pins[cir_i]) != tt_7420[tt_i][4]) {
        check = 0;
        break;
      }
    }
    flag += check;
  }
  return flag;
}

int self_check_7421() {

  int flag = 0;

  int total_pins = 14;  // Total number of Pins

  int gnd_pin = 7;   // Ground Pin
  int vcc_pin = 14;  // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }
  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);    // Setting pinMode of Ground Pin
  digitalWrite(gnd_pin, LOW);  // Setting Value of Ground Pin

  pinMode(vcc_pin, OUTPUT);     // Setting pinMode of VCC Pin
  digitalWrite(vcc_pin, HIGH);  // Setting Value of VCC Pin

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {

    return flag;  //Return 0
  }

  int strobe1_pin = 3;   // Strobe1  Pin
  int strobe2_pin = 11;  // Strobe2 Pin

  // Setting Arduino strobe1 pin
  if (strobe1_pin >= ((total_pins / 2) + 1)) {
    strobe1_pin += ((max_pin - total_pins) + start_pin);
  } else {
    strobe1_pin += start_pin;
  }

  // Setting Arduino strobe2 pin
  if (strobe2_pin >= ((total_pins / 2) + 1)) {
    strobe2_pin += ((max_pin - total_pins) + start_pin);
  } else {
    strobe2_pin += start_pin;
  }
  pinMode(strobe1_pin, OUTPUT);    // Setting pinMode of strobe1 Pin
  digitalWrite(strobe1_pin, LOW);  // Setting Value of strobe1 Pin

  pinMode(strobe2_pin, OUTPUT);    // Setting pinMode of strobe2 Pin
  digitalWrite(strobe2_pin, LOW);  // Setting Value of strobe2 Pin

  if (digitalRead(strobe1_pin) != 0 || digitalRead(strobe2_pin) != 0) {

    return flag;  //Return 0
  }

  int pin, pin1, pin2;

  int ip_pins[2][4] = { { 1, 2, 4, 5 }, { 13, 12, 10, 9 } };  // Input Pins of IC 7421

  for (pin1 = 0; pin1 < 2; pin1++) {
    for (pin2 = 0; pin2 < 4; pin2++) {
      // Setting Arduino Input Pins
      if (ip_pins[pin1][pin2] >= ((total_pins / 2) + 1)) {
        ip_pins[pin1][pin2] += ((max_pin - total_pins) + start_pin);
      } else {
        ip_pins[pin1][pin2] += start_pin;
      }
      pinMode((ip_pins[pin1][pin2]), OUTPUT);  // Setting pinmode of Input pins
    }
  }

  int op_pins[2] = { 6, 8 };  // Output Pins of IC 7421

  for (pin = 0; pin < 2; pin++) {
    // Setting Arduino Output Pins
    if (op_pins[pin] >= ((total_pins / 2) + 1)) {
      op_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      op_pins[pin] += start_pin;
    }
    pinMode((op_pins[pin]), INPUT);  // Setting pinmode of Output pins
  }

  // Truth Table of IC 7421
  int tt_7421[16][5] = { { 0, 0, 0, 0, 0 },
                         { 0, 0, 0, 1, 0 },
                         { 0, 0, 1, 0, 0 },
                         { 0, 0, 1, 1, 0 },
                         { 0, 1, 0, 0, 0 },
                         { 0, 1, 0, 1, 0 },
                         { 0, 1, 1, 0, 0 },
                         { 0, 1, 1, 1, 0 },
                         { 1, 0, 0, 0, 0 },
                         { 1, 0, 0, 1, 0 },
                         { 1, 0, 1, 0, 0 },
                         { 1, 0, 1, 1, 0 },
                         { 1, 1, 0, 0, 0 },
                         { 1, 1, 0, 1, 0 },
                         { 1, 1, 1, 0, 0 },
                         { 1, 1, 1, 1, 1 } };

  // Checking IC 7421
  for (int cir_i = 0; cir_i < 2; cir_i++) {
    int check = 1;

    for (int tt_i = 0; tt_i < 16; tt_i++) {
      digitalWrite(ip_pins[cir_i][0], tt_7421[tt_i][0]);

      digitalWrite(ip_pins[cir_i][1], tt_7421[tt_i][1]);

      digitalWrite(ip_pins[cir_i][2], tt_7421[tt_i][2]);

      digitalWrite(ip_pins[cir_i][3], tt_7421[tt_i][3]);

      if (digitalRead(op_pins[cir_i]) != tt_7421[tt_i][4]) {
        check = 0;
        break;
      }
    }
    flag += check;
  }
  return flag;
}

int self_check_7425() {

  int flag = 0;

  int total_pins = 14;  // Total number of Pins

  int gnd_pin = 7;   // Ground Pin
  int vcc_pin = 14;  // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }
  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);    // Setting pinMode of Ground Pin
  digitalWrite(gnd_pin, LOW);  // Setting Value of Ground Pin

  pinMode(vcc_pin, OUTPUT);     // Setting pinMode of VCC Pin
  digitalWrite(vcc_pin, HIGH);  // Setting Value of VCC Pin

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    return flag;  //Return 0
  }

  int strobe1_pin = 3;   // Strobe1  Pin
  int strobe2_pin = 11;  // Strobe2 Pin

  // Setting Arduino strobe1 pin
  if (strobe1_pin >= ((total_pins / 2) + 1)) {
    strobe1_pin += ((max_pin - total_pins) + start_pin);
  } else {
    strobe1_pin += start_pin;
  }

  // Setting Arduino strobe2 pin
  if (strobe2_pin >= ((total_pins / 2) + 1)) {
    strobe2_pin += ((max_pin - total_pins) + start_pin);
  } else {
    strobe2_pin += start_pin;
  }
  pinMode(strobe1_pin, OUTPUT);    // Setting pinMode of strobe1 Pin
  digitalWrite(strobe1_pin, LOW);  // Setting Value of strobe1 Pin

  pinMode(strobe2_pin, OUTPUT);    // Setting pinMode of strobe2 Pin
  digitalWrite(strobe2_pin, LOW);  // Setting Value of strobe2 Pin

  if (digitalRead(strobe1_pin) != 0 || digitalRead(strobe2_pin) != 0) {
    return flag;  //Return 0
  }

  int pin, pin1, pin2;

  int ip_pins[2][4] = { { 1, 2, 4, 5 }, { 13, 12, 10, 9 } };  // Input Pins of IC 7425

  for (pin1 = 0; pin1 < 2; pin1++) {
    for (pin2 = 0; pin2 < 4; pin2++) {
      // Setting Arduino Input Pins
      if (ip_pins[pin1][pin2] >= ((total_pins / 2) + 1)) {
        ip_pins[pin1][pin2] += ((max_pin - total_pins) + start_pin);
      } else {
        ip_pins[pin1][pin2] += start_pin;
      }
      pinMode((ip_pins[pin1][pin2]), OUTPUT);  // Setting pinmode of Input pins
    }
  }

  int op_pins[2] = { 6, 8 };  // Output Pins of IC 7425

  for (pin = 0; pin < 2; pin++) {
    // Setting Arduino Output Pins
    if (op_pins[pin] >= ((total_pins / 2) + 1)) {
      op_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      op_pins[pin] += start_pin;
    }
    pinMode((op_pins[pin]), INPUT);  // Setting pinmode of Output pins
  }

  // Truth Table of IC 7425
  int tt_7425[16][5] = { { 0, 0, 0, 0, 1 },
                         { 0, 0, 0, 1, 0 },
                         { 0, 0, 1, 0, 0 },
                         { 0, 0, 1, 1, 0 },
                         { 0, 1, 0, 0, 0 },
                         { 0, 1, 0, 1, 0 },
                         { 0, 1, 1, 0, 0 },
                         { 0, 1, 1, 1, 0 },
                         { 1, 0, 0, 0, 0 },
                         { 1, 0, 0, 1, 0 },
                         { 1, 0, 1, 0, 0 },
                         { 1, 0, 1, 1, 0 },
                         { 1, 1, 0, 0, 0 },
                         { 1, 1, 0, 1, 0 },
                         { 1, 1, 1, 0, 0 },
                         { 1, 1, 1, 1, 0 } };

  // Checking IC 7425
  for (int cir_i = 0; cir_i < 2; cir_i++) {
    int check = 1;

    for (int tt_i = 0; tt_i < 16; tt_i++) {
      digitalWrite(ip_pins[cir_i][0], tt_7425[tt_i][0]);

      digitalWrite(ip_pins[cir_i][1], tt_7425[tt_i][1]);

      digitalWrite(ip_pins[cir_i][2], tt_7425[tt_i][2]);

      digitalWrite(ip_pins[cir_i][3], tt_7425[tt_i][3]);

      if (digitalRead(op_pins[cir_i]) != tt_7425[tt_i][4]) {
        check = 0;
        break;
      }
    }
    flag += check;
  }
  return flag;
}

// Arithmetic Circuits
int self_check_7482() {

  int flag = 0;

  int total_pins = 14;

  int gnd_pin = 11;  // Ground Pin
  int vcc_pin = 4;   // VCC Pin (corrected, not 5!)

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }

  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);
  digitalWrite(gnd_pin, LOW);

  pinMode(vcc_pin, OUTPUT);
  digitalWrite(vcc_pin, HIGH);

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    return flag;  //Return 0
  }

  // Input pins: A1,B1 and A2,B2 + C0
  // From diagram: A1(pin2), B1(pin3), A2(pin14), B2(pin13), C0(pin5)
  int ip_pins[2][2] = { { 2, 3 }, { 14, 13 } };
  int cin_pin = 5;

  for (int pin1 = 0; pin1 < 2; pin1++) {
    for (int pin2 = 0; pin2 < 2; pin2++) {
      if (ip_pins[pin1][pin2] >= ((total_pins / 2) + 1)) {
        ip_pins[pin1][pin2] += ((max_pin - total_pins) + start_pin);
      } else {
        ip_pins[pin1][pin2] += start_pin;
      }
      pinMode(ip_pins[pin1][pin2], OUTPUT);
    }
  }
  if (cin_pin >= ((total_pins / 2) + 1)) {
    cin_pin += ((max_pin - total_pins) + start_pin);
  } else {
    cin_pin += start_pin;
  }
  pinMode(cin_pin, OUTPUT);

  // Output pins
  int sum_pins[2] = { 1, 12 };  // Σ1, Σ2
  int cout_pin = 10;            // C2

  for (int i = 0; i < 2; i++) {
    if (sum_pins[i] >= ((total_pins / 2) + 1)) {
      sum_pins[i] += ((max_pin - total_pins) + start_pin);
    } else {
      sum_pins[i] += start_pin;
    }
    pinMode(sum_pins[i], INPUT);
  }
  if (cout_pin >= ((total_pins / 2) + 1)) {
    cout_pin += ((max_pin - total_pins) + start_pin);
  } else {
    cout_pin += start_pin;
  }
  pinMode(cout_pin, INPUT);

  // Truth Table of IC 7482 (2-bit Full Adder)
  // Format: {A1, B1, A2, B2, C0, Σ1, Σ2, C2}
  int tt_7482[32][8] = {
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 1, 0, 0, 0, 0, 1, 0, 0 },
    { 0, 1, 0, 0, 0, 1, 0, 0 },
    { 1, 1, 0, 0, 0, 0, 1, 0 },
    { 0, 0, 1, 0, 0, 0, 1, 0 },
    { 1, 0, 1, 0, 0, 1, 1, 0 },
    { 0, 1, 1, 0, 0, 1, 1, 0 },
    { 1, 1, 1, 0, 0, 0, 0, 1 },
    { 0, 0, 0, 1, 0, 0, 1, 0 },
    { 1, 0, 0, 1, 0, 1, 1, 0 },
    { 0, 1, 0, 1, 0, 1, 1, 0 },
    { 1, 1, 0, 1, 0, 0, 0, 1 },
    { 0, 0, 1, 1, 0, 0, 0, 1 },
    { 1, 0, 1, 1, 0, 1, 0, 1 },
    { 0, 1, 1, 1, 0, 1, 0, 1 },
    { 1, 1, 1, 1, 0, 0, 1, 1 },
    { 0, 0, 0, 0, 1, 1, 0, 0 },
    { 1, 0, 0, 0, 1, 0, 1, 0 },
    { 0, 1, 0, 0, 1, 0, 1, 0 },
    { 1, 1, 0, 0, 1, 1, 1, 0 },
    { 0, 0, 1, 0, 1, 1, 1, 0 },
    { 1, 0, 1, 0, 1, 0, 0, 1 },
    { 0, 1, 1, 0, 1, 0, 0, 1 },
    { 1, 1, 1, 0, 1, 1, 0, 1 },
    { 0, 0, 0, 1, 1, 1, 1, 0 },
    { 1, 0, 0, 1, 1, 0, 0, 1 },
    { 0, 1, 0, 1, 1, 0, 0, 1 },
    { 1, 1, 0, 1, 1, 1, 0, 1 },
    { 0, 0, 1, 1, 1, 1, 0, 1 },
    { 1, 0, 1, 1, 1, 0, 1, 1 },
    { 0, 1, 1, 1, 1, 0, 1, 1 },
    { 1, 1, 1, 1, 1, 1, 1, 1 }
  };

  // Checking IC 7482
  int check = 1;

  for (int tt_i = 0; tt_i < 32; tt_i++) {
    // Apply inputs
    digitalWrite(ip_pins[0][0], tt_7482[tt_i][0]);  // A1
    digitalWrite(ip_pins[0][1], tt_7482[tt_i][1]);  // B1
    digitalWrite(ip_pins[1][0], tt_7482[tt_i][2]);  // A2
    digitalWrite(ip_pins[1][1], tt_7482[tt_i][3]);  // B2
    digitalWrite(cin_pin, tt_7482[tt_i][4]);        // C0

    // Compare outputs
    if (digitalRead(sum_pins[0]) != tt_7482[tt_i][5] || digitalRead(sum_pins[1]) != tt_7482[tt_i][6] || digitalRead(cout_pin) != tt_7482[tt_i][7]) {
      check = 0;
    }
  }

  flag += check;

  return flag;
}

int self_check_7483() {

  int flag = 0;

  int work = 1;

  int total_pins = 16;  // Total number of Pins

  int gnd_pin = 12;  // Ground Pin
  int vcc_pin = 5;   // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += start_pin;
  } else {
    gnd_pin += start_pin;
  }
  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += start_pin;
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);
  digitalWrite(gnd_pin, LOW);

  pinMode(vcc_pin, OUTPUT);
  digitalWrite(vcc_pin, HIGH);

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {

    return flag;  //Return 0
  }

  int pin;

  // Input Pins: A1–A4, B1–B4, Cin
  int ip_pins[2][4] = { { 10, 8, 3, 1 }, { 11, 7, 4, 16 } };  // A1–A4, B1–B4
  int cin_pin = 13;                                           // Carry In

  // Map Arduino pins
  for (int i = 0; i < 4; i++) {
    if (ip_pins[1][i] >= ((total_pins / 2) + 1)) {
      ip_pins[1][i] += start_pin;
    } else {
      ip_pins[1][i] += start_pin;
    }
    if (ip_pins[0][i] >= ((total_pins / 2) + 1)) {
      ip_pins[0][i] += start_pin;
    } else {
      ip_pins[0][i] += start_pin;
    }
    pinMode(ip_pins[0][i], OUTPUT);
    pinMode(ip_pins[1][i], OUTPUT);
  }
  if (cin_pin >= ((total_pins / 2) + 1))
    cin_pin += start_pin;
  else
    cin_pin += start_pin;
  pinMode(cin_pin, OUTPUT);

  // Output Pins: Σ1–Σ4, Cout
  int op_pins[5] = { 9, 6, 2, 15, 14 };  // S1–S4, C4
  for (pin = 0; pin < 5; pin++) {
    if (op_pins[pin] >= ((total_pins / 2) + 1)) {
      op_pins[pin] += start_pin;
    } else {
      op_pins[pin] += start_pin;
    }
    pinMode(op_pins[pin], INPUT);
  }

  // Truth Table: For 4-bit adder it’s too large → we test a subset of cases
  // Format: {A, B, Cin, Sum, Cout}
  // Each A and B is 4-bit number
  int tt_7483[8][5] = {
    { 0x0, 0x0, 0, 0x0, 0 },
    { 0x3, 0x4, 0, 0x7, 0 },
    { 0xF, 0x1, 0, 0x0, 1 },
    { 0x5, 0xA, 1, 0x0, 1 },
    { 0x7, 0x8, 0, 0xF, 0 },
    { 0x7, 0x8, 1, 0x0, 1 },
    { 0x9, 0x6, 0, 0xF, 0 },
    { 0x9, 0x6, 1, 0x0, 1 }
  };

  // Checking IC 7483
  for (int t = 0; t < 8; t++) {
    int check = 1;

    // Apply inputs bitwise
    for (int b = 0; b < 4; b++) {
      digitalWrite(ip_pins[0][b], (tt_7483[t][0] >> b) & 1);  // A
      digitalWrite(ip_pins[1][b], (tt_7483[t][1] >> b) & 1);  // B
    }
    digitalWrite(cin_pin, tt_7483[t][2]);

    // Expected outputs
    int expected_sum = tt_7483[t][3];
    int expected_cout = tt_7483[t][4];

    // Verify each sum bit
    for (int b = 0; b < 4; b++) {
      if (digitalRead(op_pins[b]) != ((expected_sum >> b) & 1)) {
        check = 0;
      }
    }
    if (digitalRead(op_pins[4]) != expected_cout) {
      check = 0;
    }

    flag += check;
    work *= check;
  }

  return flag;
}

int self_check_74283() {

  int flag = 0;

  int work = 1;

  int total_pins = 16;  // Total number of Pins

  int gnd_pin = 8;   // Ground Pin
  int vcc_pin = 16;  // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += start_pin;
  } else {
    gnd_pin += start_pin;
  }
  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += start_pin;
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);
  digitalWrite(gnd_pin, LOW);

  pinMode(vcc_pin, OUTPUT);
  digitalWrite(vcc_pin, HIGH);

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    return flag;  //Return 0
  }

  int pin, pin1;

  // Input Pins: A1–A4, B1–B4
  int a_pins[4] = { 5, 3, 14, 12 };
  int b_pins[4] = { 6, 2, 15, 11 };
  int cin_pin = 7;

  // Map input pins to Arduino
  for (pin1 = 0; pin1 < 4; pin1++) {
    if (a_pins[pin1] >= ((total_pins / 2) + 1))
      a_pins[pin1] += start_pin;
    else
      a_pins[pin1] += start_pin;
    if (b_pins[pin1] >= ((total_pins / 2) + 1))
      b_pins[pin1] += start_pin;
    else
      b_pins[pin1] += start_pin;
    pinMode(a_pins[pin1], OUTPUT);
    pinMode(b_pins[pin1], OUTPUT);
  }
  if (cin_pin >= ((total_pins / 2) + 1))
    cin_pin += start_pin;
  else
    cin_pin += start_pin;
  pinMode(cin_pin, OUTPUT);

  // Output Pins: S1–S4, Cout
  int s_pins[4] = { 4, 1, 13, 10 };
  int cout_pin = 9;

  for (pin1 = 0; pin1 < 4; pin1++) {
    if (s_pins[pin1] >= ((total_pins / 2) + 1))
      s_pins[pin1] += start_pin;
    else
      s_pins[pin1] += start_pin;
    pinMode(s_pins[pin1], INPUT);
  }
  if (cout_pin >= ((total_pins / 2) + 1))
    cout_pin += start_pin;
  else
    cout_pin += start_pin;
  pinMode(cout_pin, INPUT);

  // Test cases {A, B, Cin, Sum, Cout}
  // (Values are 4-bit numbers, sum is also 4-bit)
  int tt_74283[6][5] = {
    { 0x0, 0x0, 0, 0x0, 0 },
    { 0x3, 0x4, 0, 0x7, 0 },
    { 0xF, 0x1, 0, 0x0, 1 },
    { 0x5, 0xA, 1, 0x0, 1 },
    { 0x7, 0x8, 0, 0xF, 0 },
    { 0x7, 0x8, 1, 0x0, 1 }
  };

  // Checking IC 74283
  for (int t = 0; t < 6; t++) {
    int check = 1;

    // Apply inputs A, B, Cin
    for (int b = 0; b < 4; b++) {
      digitalWrite(a_pins[b], (tt_74283[t][0] >> b) & 1);
      digitalWrite(b_pins[b], (tt_74283[t][1] >> b) & 1);
    }
    digitalWrite(cin_pin, tt_74283[t][2]);

    // Expected outputs
    int expected_sum = tt_74283[t][3];
    int expected_cout = tt_74283[t][4];

    // Check outputs
    for (int b = 0; b < 4; b++) {
      if (digitalRead(s_pins[b]) != ((expected_sum >> b) & 1)) {
        check = 0;
      }
    }
    if (digitalRead(cout_pin) != expected_cout)
      check = 0;

    flag += check;
  }

  return flag;
}

// Combinational Circuits
int self_check_7442() {

  int flag = 0;

  int work = 1;
  int total_pins = 16;  // IC 7442 has 16 pins

  int gnd_pin = 8;   // Ground Pin
  int vcc_pin = 16;  // VCC Pin

  // Adjust Arduino mapped Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += start_pin;
  } else {
    gnd_pin += start_pin;
  }

  // Adjust Arduino mapped VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += start_pin;
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);
  digitalWrite(gnd_pin, LOW);

  pinMode(vcc_pin, OUTPUT);
  digitalWrite(vcc_pin, HIGH);

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    return flag;  //Return 0
  }

  // Input pins: A, B, C, D
  int ip_pins[4] = { 15, 14, 13, 12 };

  for (int i = 0; i < 4; i++) {
    if (ip_pins[i] >= ((total_pins / 2) + 1)) {
      ip_pins[i] += start_pin;
    } else {
      ip_pins[i] += start_pin;
    }
    pinMode(ip_pins[i], OUTPUT);
  }

  // Output pins: Y0-Y9
  int op_pins[10] = { 1, 2, 3, 4, 5, 6, 7, 9, 10, 11 };

  for (int i = 0; i < 10; i++) {
    if (op_pins[i] >= ((total_pins / 2) + 1)) {
      op_pins[i] += start_pin;
    } else {
      op_pins[i] += start_pin;
    }
    pinMode(op_pins[i], INPUT);
  }

  // Truth table (BCD input → one active low output)
  int tt_7442[10][15] = {
    { 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1 },     // 0000 -> Y0 LOW
    { 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1 },  // 0001 -> Y1 LOW
    { 0, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1 },  // 0010 -> Y2 LOW
    { 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1 },  // 0011 -> Y3 LOW
    { 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1 },  // 0100 -> Y4 LOW
    { 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1 },  // 0101 -> Y5 LOW
    { 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1 },  // 0110 -> Y6 LOW
    { 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1 },  // 0111 -> Y7 LOW
    { 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1 },  // 1000 -> Y8 LOW
    { 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1 }   // 1001 -> Y9 LOW
  };

  // Checking IC 7442
  for (int tt_i = 0; tt_i < 10; tt_i++) {
    // Apply BCD inputs
    digitalWrite(ip_pins[0], tt_7442[tt_i][0]);
    digitalWrite(ip_pins[1], tt_7442[tt_i][1]);
    digitalWrite(ip_pins[2], tt_7442[tt_i][2]);
    digitalWrite(ip_pins[3], tt_7442[tt_i][3]);

    delay(10);  // small settling time

    int check = 1;

    // Validate all 10 outputs
    for (int op = 0; op < 10; op++) {
      if (digitalRead(op_pins[op]) != tt_7442[tt_i][op + 4]) {
        check = 0;
      }
    }

    flag += check;
    work *= check;
  }

  return flag;
}

int self_check_74138() {

  int flag = 0;
  int work = 1;


  int total_pins = 16;  // Total pins in IC 74138

  int gnd_pin = 8;   // Ground Pin
  int vcc_pin = 16;  // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }

  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);
  digitalWrite(gnd_pin, LOW);

  pinMode(vcc_pin, OUTPUT);
  digitalWrite(vcc_pin, HIGH);

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    return flag;  //Return 0
  }

  // Input pins A=1, B=2, C=3
  int in_pins[3] = { 1, 2, 3 };
  for (int i = 0; i < 3; i++) {
    if (in_pins[i] >= ((total_pins / 2) + 1)) {
      in_pins[i] += ((max_pin - total_pins) + start_pin);
    } else {
      in_pins[i] += start_pin;
    }
    pinMode(in_pins[i], OUTPUT);
  }

  // Enable pins G2A=4, G2B=5 (active LOW), G1=6 (active HIGH)
  int g2a = 4, g2b = 5, g1 = 6;
  int en_pins[3] = { g2a, g2b, g1 };
  for (int i = 0; i < 3; i++) {
    if (en_pins[i] >= ((total_pins / 2) + 1)) {
      en_pins[i] += ((max_pin - total_pins) + start_pin);
    } else {
      en_pins[i] += start_pin;
    }
    pinMode(en_pins[i], OUTPUT);
  }
  digitalWrite(en_pins[0], LOW);   // G2A active
  digitalWrite(en_pins[1], LOW);   // G2B active
  digitalWrite(en_pins[2], HIGH);  // G1 active

  // Output pins Y0–Y7 = 15,14,13,12,11,10,9,7
  int op_pins[8] = { 15, 14, 13, 12, 11, 10, 9, 7 };
  for (int i = 0; i < 8; i++) {
    if (op_pins[i] >= ((total_pins / 2) + 1)) {
      op_pins[i] += ((max_pin - total_pins) + start_pin);
    } else {
      op_pins[i] += start_pin;
    }
    pinMode(op_pins[i], INPUT);
  }

  // Truth table: for inputs A,B,C, only Y(ABC) = 0, others = 1
  for (int code = 0; code < 8; code++) {
    // set A,B,C
    digitalWrite(in_pins[0], code & 0x01);         // A
    digitalWrite(in_pins[1], (code >> 1) & 0x01);  // B
    digitalWrite(in_pins[2], (code >> 2) & 0x01);  // C

    int check = 1;
    for (int y = 0; y < 8; y++) {
      int expected = (y == code) ? LOW : HIGH;  // active low
      if (digitalRead(op_pins[y]) != expected) {
        check = 0;
      }
    }

    flag += check;
    work *= check;
  }

  return flag;
}

int self_check_74147() {

  int flag = 0;
  int work = 1;


  int total_pins = 16;  // Total pins in IC 74147

  int gnd_pin = 8;   // Ground pin
  int vcc_pin = 16;  // VCC pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }

  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);
  digitalWrite(gnd_pin, LOW);

  pinMode(vcc_pin, OUTPUT);
  digitalWrite(vcc_pin, HIGH);

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {

    return flag;  //Return 0
  }

  int pin;

  // INPUTS (active LOW) for digits 1..9 according to 74147 pinout:
  // digit 1 -> pin 11, 2 -> 12, 3 -> 13, 4 -> 1, 5 -> 2, 6 -> 3, 7 -> 4, 8 -> 5, 9 -> 10
  int ip_pins[9] = { 11, 12, 13, 1, 2, 3, 4, 5, 10 };

  // Configure data input Arduino pins (outputs from Arduino side)
  for (pin = 0; pin < 9; pin++) {
    if (ip_pins[pin] >= ((total_pins / 2) + 1)) {
      ip_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      ip_pins[pin] += start_pin;
    }
    pinMode(ip_pins[pin], OUTPUT);
    digitalWrite(ip_pins[pin], HIGH);  // default inactive (inputs are active-LOW)
  }

  // OUTPUTS (active LOW): D, C, B, A  -> pins 14, 6, 7, 9 respectively
  int op_pins[4] = { 14, 6, 7, 9 };  // order: D, C, B, A

  for (pin = 0; pin < 4; pin++) {
    if (op_pins[pin] >= ((total_pins / 2) + 1)) {
      op_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      op_pins[pin] += start_pin;
    }
    pinMode(op_pins[pin], INPUT);
  }

  // --- Test case 0 (implied zero) ---
  // No input active (all HIGH) => outputs should represent 0 (BCD 0000).
  // Because outputs are active-LOW, the physical pins should read HIGH for all four outputs.
  for (pin = 0; pin < 9; pin++)
    digitalWrite(ip_pins[pin], HIGH);
  delay(10);  // settle
  {
    int check = 1;
    for (int k = 0; k < 4; k++) {
      if (digitalRead(op_pins[k]) != HIGH) {
        check = 0;
        break;
      }
    }
    flag += check;
    work *= check;
  }

  // --- Test inputs 1..9 individually ---
  // For each n (1..9): drive that input LOW (active), all others HIGH,
  // then check outputs equal the BCD of n but as active-LOW signals.
  for (int n = 1; n <= 9; n++) {
    // set all inputs inactive (HIGH)
    for (pin = 0; pin < 9; pin++)
      digitalWrite(ip_pins[pin], HIGH);
    // activate the requested input (active LOW)
    digitalWrite(ip_pins[n - 1], LOW);

    delay(10);  // settling time

    int check = 1;
    // op_pins order: D (bit3), C (bit2), B (bit1), A (bit0)
    for (int k = 0; k < 4; k++) {
      int bit = (n >> (3 - k)) & 0x1;   // extract the BCD bit
      int expected = bit ? LOW : HIGH;  // outputs are active-LOW
      if (digitalRead(op_pins[k]) != expected) {
        check = 0;
        break;
      }
    }

    flag += check;
    work *= check;
  }

  return flag;
}

int self_check_74148() {

  int flag = 0;
  int work = 1;


  int total_pins = 16;  // IC 74148 is 16-pin

  int gnd_pin = 8;   // Ground Pin
  int vcc_pin = 16;  // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }

  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);
  digitalWrite(gnd_pin, LOW);

  pinMode(vcc_pin, OUTPUT);
  digitalWrite(vcc_pin, HIGH);

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    return flag;  //Return 0
  }

  int pin;

  // Data inputs I0..I7 mapped to physical pins per datasheet:
  // I0=11, I1=12, I2=13, I3=1, I4=2, I5=3, I6=4, I7=10
  int ip_pins[8] = { 10, 11, 12, 13, 1, 2, 3, 4 };

  for (pin = 0; pin < 8; pin++) {
    if (ip_pins[pin] >= ((total_pins / 2) + 1)) {
      ip_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      ip_pins[pin] += start_pin;
    }
    pinMode(ip_pins[pin], OUTPUT);
  }

  // Enable Input (EI, active LOW) -> physical pin 5
  int ei_pin = 5;
  if (ei_pin >= ((total_pins / 2) + 1)) {
    ei_pin += ((max_pin - total_pins) + start_pin);
  } else {
    ei_pin += start_pin;
  }
  pinMode(ei_pin, OUTPUT);

  // Address outputs and flags (physical pins):
  // A2 = pin 6, A1 = pin 7, A0 = pin 9, GS = pin 14, EO = pin 15
  int op_pins[5] = { 6, 7, 9, 14, 15 };  // order: A2, A1, A0, GS, EO

  for (pin = 0; pin < 5; pin++) {
    if (op_pins[pin] >= ((total_pins / 2) + 1)) {
      op_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      op_pins[pin] += start_pin;
    }
    pinMode(op_pins[pin], INPUT);
  }

  // --- Case 1: EI = LOW (enabled) ---
  digitalWrite(ei_pin, LOW);

  for (int i = 0; i < 8; i++) {
    // Make sure only input i is active (LOW)
    for (int j = 0; j < 8; j++)
      digitalWrite(ip_pins[j], HIGH);
    digitalWrite(ip_pins[i], LOW);  // activate I_i
    delay(5);                       // settling

    // Read address outputs (active LOW => LOW means bit = 1)
    int a2 = (digitalRead(op_pins[0]) == LOW) ? 1 : 0;
    int a1 = (digitalRead(op_pins[1]) == LOW) ? 1 : 0;
    int a0 = (digitalRead(op_pins[2]) == LOW) ? 1 : 0;
    int gs = digitalRead(op_pins[3]);  // expect LOW when any input active
    int eo = digitalRead(op_pins[4]);  // expect HIGH when some input active

    int encoded = (a2 << 2) | (a1 << 1) | a0;

    int check = 1;
    if (encoded != i)
      check = 0;  // encoded value must equal input index
    if (gs != LOW)
      check = 0;  // GS must be active (LOW)
    if (eo != HIGH)
      check = 0;  // EO must be inactive (HIGH) when input present

    flag += check;
    work *= check;
  }

  // --- Case 2: EI = HIGH (disabled) ---
  // When EI is HIGH, outputs should be inactive (all HIGH)
  digitalWrite(ei_pin, HIGH);
  for (int j = 0; j < 8; j++)
    digitalWrite(ip_pins[j], HIGH);
  delay(5);

  int disabled_ok = 1;
  // A2,A1,A0 should be HIGH, GS should be HIGH, EO should be HIGH when EI is HIGH
  for (int k = 0; k < 3; k++)
    if (digitalRead(op_pins[k]) != HIGH)
      disabled_ok = 0;
  if (digitalRead(op_pins[3]) != HIGH)
    disabled_ok = 0;  // GS
  if (digitalRead(op_pins[4]) != HIGH)
    disabled_ok = 0;  // EO

  return flag;
}

int self_check_74151() {

  int flag = 0;
  int work = 1;


  int total_pins = 16;  // Total pins in IC 74151

  int gnd_pin = 8;   // Ground pin
  int vcc_pin = 16;  // VCC pin

  // Map ground and VCC to Arduino pins
  if (gnd_pin >= ((total_pins / 2) + 1))
    gnd_pin += ((max_pin - total_pins) + start_pin);
  else
    gnd_pin += start_pin;
  if (vcc_pin >= ((total_pins / 2) + 1))
    vcc_pin += ((max_pin - total_pins) + start_pin);
  else
    vcc_pin += start_pin;

  pinMode(gnd_pin, OUTPUT);
  digitalWrite(gnd_pin, LOW);

  pinMode(vcc_pin, OUTPUT);
  digitalWrite(vcc_pin, HIGH);

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {

    return flag;  //Return 0
  }

  int pin, pin1;

  // Data input pins D0–D7 (correct order per datasheet)
  int ip_pins[8] = { 4, 3, 2, 1, 15, 14, 13, 12 };
  for (pin = 0; pin < 8; pin++) {
    if (ip_pins[pin] >= ((total_pins / 2) + 1))
      ip_pins[pin] += ((max_pin - total_pins) + start_pin);
    else
      ip_pins[pin] += start_pin;
    pinMode(ip_pins[pin], OUTPUT);
  }

  // Select inputs S0, S1, S2
  int sel_pins[3] = { 11, 10, 9 };
  for (pin = 0; pin < 3; pin++) {
    if (sel_pins[pin] >= ((total_pins / 2) + 1))
      sel_pins[pin] += ((max_pin - total_pins) + start_pin);
    else
      sel_pins[pin] += start_pin;
    pinMode(sel_pins[pin], OUTPUT);
  }

  // Enable pin (active LOW)
  int en_pin = 7;
  if (en_pin >= ((total_pins / 2) + 1))
    en_pin += ((max_pin - total_pins) + start_pin);
  else
    en_pin += start_pin;
  pinMode(en_pin, OUTPUT);
  digitalWrite(en_pin, LOW);

  // Output pin Y
  int op_pin = 5;
  if (op_pin >= ((total_pins / 2) + 1))
    op_pin += ((max_pin - total_pins) + start_pin);
  else
    op_pin += start_pin;
  pinMode(op_pin, INPUT);

  // Checking IC 74151
  for (int sel = 0; sel < 8; sel++) {
    int check = 1;

    // Set select lines
    digitalWrite(sel_pins[0], sel & 0x01);
    digitalWrite(sel_pins[1], (sel >> 1) & 0x01);
    digitalWrite(sel_pins[2], (sel >> 2) & 0x01);

    // Test both 0 and 1 for the selected input
    for (int val = 0; val < 2; val++) {
      // Set all inputs HIGH first
      for (pin1 = 0; pin1 < 8; pin1++)
        digitalWrite(ip_pins[pin1], HIGH);

      // Set selected input to the test value
      digitalWrite(ip_pins[sel], val);

      if (digitalRead(op_pin) != val)
        check = 0;
    }
    flag += check;
    work *= check;
  }

  return flag;
}

int self_check_74153() {

  int flag = 0;


  int total_pins = 16;  // Total number of pins

  int gnd_pin = 8;   // Ground Pin
  int vcc_pin = 16;  // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }

  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);
  digitalWrite(gnd_pin, LOW);

  pinMode(vcc_pin, OUTPUT);
  digitalWrite(vcc_pin, HIGH);

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    return flag;  //Return 0
  }

  int pin, pin1;

  // Input pins for both multiplexers
  int ip_pins[2][4] = { { 6, 5, 4, 3 }, { 10, 11, 12, 13 } };

  for (pin1 = 0; pin1 < 2; pin1++) {
    for (pin = 0; pin < 4; pin++) {
      if (ip_pins[pin1][pin] >= ((total_pins / 2) + 1)) {
        ip_pins[pin1][pin] += ((max_pin - total_pins) + start_pin);
      } else {
        ip_pins[pin1][pin] += start_pin;
      }
      pinMode(ip_pins[pin1][pin], OUTPUT);
    }
  }

  // Select inputs (S0, S1)
  int sel_pins[2] = { 14, 2 };
  for (pin = 0; pin < 2; pin++) {
    if (sel_pins[pin] >= ((total_pins / 2) + 1)) {
      sel_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      sel_pins[pin] += start_pin;
    }
    pinMode(sel_pins[pin], OUTPUT);
  }

  // Enable pins (active LOW)
  int en_pins[2] = { 15, 1 };
  for (pin = 0; pin < 2; pin++) {
    if (en_pins[pin] >= ((total_pins / 2) + 1)) {
      en_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      en_pins[pin] += start_pin;
    }
    pinMode(en_pins[pin], OUTPUT);
    digitalWrite(en_pins[pin], LOW);
  }

  // Output pins (MUX1 MUX2)
  int op_pins[2] = { 7, 9 };
  for (pin = 0; pin < 2; pin++) {
    if (op_pins[pin] >= ((total_pins / 2) + 1)) {
      op_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      op_pins[pin] += start_pin;
    }
    pinMode(op_pins[pin], INPUT);
  }

  // Checking IC 74153
  for (int mux = 0; mux < 2; mux++) {
    int check = 1;

    // Test all select combinations
    for (int sel = 0; sel < 4; sel++) {
      digitalWrite(sel_pins[0], sel & 0x01);
      digitalWrite(sel_pins[1], (sel >> 1) & 0x01);

      // Test selected input with both values
      for (int val = 0; val < 2; val++) {
        // Set all inputs HIGH
        for (pin = 0; pin < 4; pin++) {
          digitalWrite(ip_pins[mux][pin], HIGH);
        }
        // Set only selected input
        digitalWrite(ip_pins[mux][sel], val);

        if (digitalRead(op_pins[mux]) != val) {
          check = 0;
        }
      }
    }

    flag += check;
  }

  return flag;
}

int self_check_74157() {

  int flag = 0;


  int total_pins = 16;  // Total pins in IC 74157

  int gnd_pin = 8;   // Ground Pin
  int vcc_pin = 16;  // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }

  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);
  digitalWrite(gnd_pin, LOW);

  pinMode(vcc_pin, OUTPUT);
  digitalWrite(vcc_pin, HIGH);

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    return flag;  //Return 0
  }

  int pin, pin1;

  // Data Inputs A (I0) and B (I1) for 4 multiplexers
  int ip_pins[4][2] = { { 2, 3 }, { 5, 6 }, { 11, 10 }, { 14, 13 } };

  for (pin1 = 0; pin1 < 4; pin1++) {
    for (pin = 0; pin < 2; pin++) {
      if (ip_pins[pin1][pin] >= ((total_pins / 2) + 1)) {
        ip_pins[pin1][pin] += ((max_pin - total_pins) + start_pin);
      } else {
        ip_pins[pin1][pin] += start_pin;
      }
      pinMode(ip_pins[pin1][pin], OUTPUT);
    }
  }

  // Common select pin (S) at pin 1
  int sel_pin = 1;
  if (sel_pin >= ((total_pins / 2) + 1)) {
    sel_pin += ((max_pin - total_pins) + start_pin);
  } else {
    sel_pin += start_pin;
  }
  pinMode(sel_pin, OUTPUT);

  // Enable pin (G, active LOW) at pin 15
  int en_pin = 15;
  if (en_pin >= ((total_pins / 2) + 1)) {
    en_pin += ((max_pin - total_pins) + start_pin);
  } else {
    en_pin += start_pin;
  }
  pinMode(en_pin, OUTPUT);
  digitalWrite(en_pin, LOW);  // keep enabled

  // Output pins Y for 4 multiplexers
  int op_pins[4] = { 4, 7, 9, 12 };
  for (pin = 0; pin < 4; pin++) {
    if (op_pins[pin] >= ((total_pins / 2) + 1)) {
      op_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      op_pins[pin] += start_pin;
    }
    pinMode(op_pins[pin], INPUT);
  }

  // Truth Table for a single 2:1 MUX {S, A, B, Y}
  int tt_74157[4][4] = {
    { 0, 0, 1, 0 },  // S=0 → Y=A (A=0,B=1 → Y=0)
    { 0, 1, 0, 1 },  // S=0 → Y=A (A=1,B=0 → Y=1)
    { 1, 0, 1, 1 },  // S=1 → Y=B (A=0,B=1 → Y=1)
    { 1, 1, 0, 0 }   // S=1 → Y=B (A=1,B=0 → Y=0)
  };

  // Checking IC 74157
  for (int cir_i = 0; cir_i < 4; cir_i++) {
    int check = 1;

    for (int tt_i = 0; tt_i < 4; tt_i++) {
      digitalWrite(sel_pin, tt_74157[tt_i][0]);
      digitalWrite(ip_pins[cir_i][0], tt_74157[tt_i][1]);
      digitalWrite(ip_pins[cir_i][1], tt_74157[tt_i][2]);

      if (digitalRead(op_pins[cir_i]) != tt_74157[tt_i][3]) {
        check = 0;
      }
    }

    flag += check;
  }

  return flag;
}

int self_check_74158() {

  int flag = 0;


  int total_pins = 16;  // Total pins in IC 74158

  int gnd_pin = 8;   // Ground Pin
  int vcc_pin = 16;  // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }

  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);
  digitalWrite(gnd_pin, LOW);

  pinMode(vcc_pin, OUTPUT);
  digitalWrite(vcc_pin, HIGH);

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {

    return flag;  //Return 0
  }

  int pin, pin1;

  // Data Inputs A (I0) and B (I1) for 4 multiplexers
  int ip_pins[4][2] = { { 2, 3 }, { 5, 6 }, { 11, 10 }, { 14, 13 } };

  for (pin1 = 0; pin1 < 4; pin1++) {
    for (pin = 0; pin < 2; pin++) {
      if (ip_pins[pin1][pin] >= ((total_pins / 2) + 1)) {
        ip_pins[pin1][pin] += ((max_pin - total_pins) + start_pin);
      } else {
        ip_pins[pin1][pin] += start_pin;
      }
      pinMode(ip_pins[pin1][pin], OUTPUT);
    }
  }

  // Common select pin (S) at pin 1
  int sel_pin = 1;
  if (sel_pin >= ((total_pins / 2) + 1)) {
    sel_pin += ((max_pin - total_pins) + start_pin);
  } else {
    sel_pin += start_pin;
  }
  pinMode(sel_pin, OUTPUT);

  // Enable pin (G, active LOW) at pin 15
  int en_pin = 15;
  if (en_pin >= ((total_pins / 2) + 1)) {
    en_pin += ((max_pin - total_pins) + start_pin);
  } else {
    en_pin += start_pin;
  }
  pinMode(en_pin, OUTPUT);
  digitalWrite(en_pin, LOW);  // keep enabled

  // Output pins Y for 4 multiplexers
  int op_pins[4] = { 4, 7, 9, 12 };
  for (pin = 0; pin < 4; pin++) {
    if (op_pins[pin] >= ((total_pins / 2) + 1)) {
      op_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      op_pins[pin] += start_pin;
    }
    pinMode(op_pins[pin], INPUT);
  }

  // Truth Table for a single 2:1 MUX with inverted output {S, A, B, Y}
  int tt_74158[4][4] = {
    { 0, 0, 1, 1 },  // S=0 → Y = !A (A=0 → Y=1)
    { 0, 1, 0, 0 },  // S=0 → Y = !A (A=1 → Y=0)
    { 1, 0, 1, 0 },  // S=1 → Y = !B (B=1 → Y=0)
    { 1, 1, 0, 1 }   // S=1 → Y = !B (B=0 → Y=1)
  };

  // Checking IC 74158
  for (int cir_i = 0; cir_i < 4; cir_i++) {
    int check = 1;

    for (int tt_i = 0; tt_i < 4; tt_i++) {
      digitalWrite(sel_pin, tt_74158[tt_i][0]);
      digitalWrite(ip_pins[cir_i][0], tt_74158[tt_i][1]);
      digitalWrite(ip_pins[cir_i][1], tt_74158[tt_i][2]);

      if (digitalRead(op_pins[cir_i]) != tt_74158[tt_i][3]) {
        check = 0;
      }
    }

    flag += check;
  }
  return flag;
}

int self_check_74184() {

  int flag = 0;


  int work = 1;

  int total_pins = 16;

  int gnd_pin = 8;
  int vcc_pin = 16;

  // Remap Ground
  if (gnd_pin >= ((total_pins / 2) + 1))
    gnd_pin += ((max_pin - total_pins) + start_pin);
  else
    gnd_pin += start_pin;
  pinMode(gnd_pin, OUTPUT);
  digitalWrite(gnd_pin, LOW);

  // Remap VCC
  if (vcc_pin >= ((total_pins / 2) + 1))
    vcc_pin += ((max_pin - total_pins) + start_pin);
  else
    vcc_pin += start_pin;
  pinMode(vcc_pin, OUTPUT);
  digitalWrite(vcc_pin, HIGH);

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    return flag;  //Return 0
  }

  // Input pins: A=1, B=2, C=3, D=4
  int ip_pins[4] = { 1, 2, 3, 4 };
  for (int i = 0; i < 4; i++) {
    if (ip_pins[i] >= ((total_pins / 2) + 1))
      ip_pins[i] += ((max_pin - total_pins) + start_pin);
    else
      ip_pins[i] += start_pin;
    pinMode(ip_pins[i], OUTPUT);
  }

  // Output pins: Y0=5, Y1=6, Y2=7, Y3=9, Y4=10, Y5=11, Y6=12
  int op_pins[7] = { 5, 6, 7, 9, 10, 11, 12 };
  for (int i = 0; i < 7; i++) {
    if (op_pins[i] >= ((total_pins / 2) + 1))
      op_pins[i] += ((max_pin - total_pins) + start_pin);
    else
      op_pins[i] += start_pin;
    pinMode(op_pins[i], INPUT);
  }

  // Truth table: valid BCD (0000–1001) → Binary (0–9)
  // Format: {A,B,C,D, Y0..Y6}
  int tt_74184[10][11] = {
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },  // 0
    { 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0 },  // 1
    { 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0 },  // 2
    { 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0 },  // 3
    { 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0 },  // 4
    { 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0 },  // 5
    { 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0 },  // 6
    { 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0 },  // 7
    { 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 },  // 8
    { 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0 }   // 9
  };

  // ---- Checking valid BCD inputs ----
  for (int tt_i = 0; tt_i < 10; tt_i++) {
    // Apply inputs
    for (int j = 0; j < 4; j++) {
      digitalWrite(ip_pins[j], tt_74184[tt_i][j]);
    }

    int check = 1;
    for (int k = 0; k < 7; k++) {
      if (digitalRead(op_pins[k]) != tt_74184[tt_i][k + 4]) {
        check = 0;
      }
    }

    flag += check;
    work *= check;
  }


  return flag;
}

// Sequential Circuits
int self_check_7468() {

  int flag = 0;


  int total_pins = 16;  // Total pins in IC 7468

  int gnd_pin = 8;   // Ground Pin
  int vcc_pin = 16;  // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }

  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);
  digitalWrite(gnd_pin, LOW);

  pinMode(vcc_pin, OUTPUT);
  digitalWrite(vcc_pin, HIGH);

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    return flag;  //Return 0
  }

  int pin;

  // Counter A pins
  int clkA = 1, clrA = 2;
  int qA[4] = { 3, 4, 5, 6 };

  // Counter B pins
  int clkB = 9, clrB = 10;
  int qB[4] = { 11, 12, 13, 14 };

  // Map pins to Arduino
  clkA = (clkA >= ((total_pins / 2) + 1)) ? (clkA + ((max_pin - total_pins) + start_pin)) : (clkA + start_pin);
  clrA = (clrA >= ((total_pins / 2) + 1)) ? (clrA + ((max_pin - total_pins) + start_pin)) : (clrA + start_pin);
  clkB = (clkB >= ((total_pins / 2) + 1)) ? (clkB + ((max_pin - total_pins) + start_pin)) : (clkB + start_pin);
  clrB = (clrB >= ((total_pins / 2) + 1)) ? (clrB + ((max_pin - total_pins) + start_pin)) : (clrB + start_pin);

  pinMode(clkA, OUTPUT);
  pinMode(clrA, OUTPUT);
  pinMode(clkB, OUTPUT);
  pinMode(clrB, OUTPUT);

  for (pin = 0; pin < 4; pin++) {
    qA[pin] = (qA[pin] >= ((total_pins / 2) + 1)) ? (qA[pin] + ((max_pin - total_pins) + start_pin)) : (qA[pin] + start_pin);
    qB[pin] = (qB[pin] >= ((total_pins / 2) + 1)) ? (qB[pin] + ((max_pin - total_pins) + start_pin)) : (qB[pin] + start_pin);
    pinMode(qA[pin], INPUT);
    pinMode(qB[pin], INPUT);
  }

  // ----- Checking Counter A -----
  int checkA = 1;
  digitalWrite(clrA, LOW);

  digitalWrite(clrA, HIGH);

  for (int val = 0; val < 10; val++) {
    digitalWrite(clkA, HIGH);

    digitalWrite(clkA, LOW);

    int readVal = (digitalRead(qA[3]) << 3) | (digitalRead(qA[2]) << 2) | (digitalRead(qA[1]) << 1) | (digitalRead(qA[0]));
    if (readVal != val)
      checkA = 0;
  }

  flag += checkA;

  // ----- Checking Counter B -----
  int checkB = 1;
  digitalWrite(clrB, LOW);

  digitalWrite(clrB, HIGH);

  for (int val = 0; val < 10; val++) {
    digitalWrite(clkB, HIGH);

    digitalWrite(clkB, LOW);

    int readVal = (digitalRead(qB[3]) << 3) | (digitalRead(qB[2]) << 2) | (digitalRead(qB[1]) << 1) | (digitalRead(qB[0]));
    if (readVal != val)
      checkB = 0;
  }

  flag += checkB;

  return flag;
}

int self_check_7473() {

  int flag = 0;

  int total_pins = 14;  // Total pins in IC 7473

  // NOTE: according to the datasheet the 7473 VCC and GND are at pins 4 and 11.
  int gnd_pin = 11;  // Ground (pin 11)
  int vcc_pin = 4;   // VCC (pin 4)

  // map to Arduino connector (keeps your original mapping logic)
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);
  digitalWrite(gnd_pin, LOW);

  pinMode(vcc_pin, OUTPUT);
  digitalWrite(vcc_pin, HIGH);

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {

    return flag;  //Return 0
  }

  int pin, pin1;

  // Inputs for each FF: {CLK, J, K, CLR} (pins per datasheet)
  int ip_pins[2][4] = { { 1, 14, 3, 2 },    // FF1: CLK1, J1, K1, CLR1
                        { 5, 7, 10, 6 } };  // FF2: CLK2, J2, K2, CLR2

  // map and set input pins (Arduino pins) and initialize levels
  for (pin1 = 0; pin1 < 2; pin1++) {
    for (pin = 0; pin < 4; pin++) {
      if (ip_pins[pin1][pin] >= ((total_pins / 2) + 1)) {
        ip_pins[pin1][pin] += ((max_pin - total_pins) + start_pin);
      } else {
        ip_pins[pin1][pin] += start_pin;
      }
      pinMode(ip_pins[pin1][pin], OUTPUT);
      // default: CLK=LOW, J=LOW, K=LOW, CLR=HIGH (inactive)
      if (pin == 0) {  // CLK
        digitalWrite(ip_pins[pin1][pin], LOW);
      } else if (pin == 3) {  // CLR (active LOW) - keep inactive
        digitalWrite(ip_pins[pin1][pin], HIGH);
      } else {  // J, K
        digitalWrite(ip_pins[pin1][pin], LOW);
      }
    }
  }

  // Outputs (Q, Q̅) per datasheet: FF1 -> Q=12, Q̅=13 ; FF2 -> Q=9, Q̅=8
  int op_pins[2][2] = { { 12, 13 }, { 9, 8 } };
  for (pin1 = 0; pin1 < 2; pin1++) {
    for (pin = 0; pin < 2; pin++) {
      if (op_pins[pin1][pin] >= ((total_pins / 2) + 1)) {
        op_pins[pin1][pin] += ((max_pin - total_pins) + start_pin);
      } else {
        op_pins[pin1][pin] += start_pin;
      }
      pinMode(op_pins[pin1][pin], INPUT);
    }
  }

  // small helper to generate a clean HIGH->LOW (negative) clock transition
  auto pulseClock = [&](int clkPin) {
    digitalWrite(clkPin, LOW);
    delayMicroseconds(5);
    digitalWrite(clkPin, HIGH);
    delayMicroseconds(8);
    digitalWrite(clkPin, LOW);  // falling edge occurs here
    delayMicroseconds(8);       // allow outputs to settle
  };

  // Test sequence for each flip-flop: reset baseline, then HOLD / SET / RESET / TOGGLE
  for (int ff = 0; ff < 2; ff++) {
    int check = 1;

    // 1) asynchronous clear -> ensure known baseline Q=LOW, Qbar=HIGH
    digitalWrite(ip_pins[ff][3], LOW);  // CLR active
    delayMicroseconds(10);
    digitalWrite(ip_pins[ff][3], HIGH);  // CLR release
    delay(1);
    int q = digitalRead(op_pins[ff][0]);
    int qb = digitalRead(op_pins[ff][1]);
    if (!(q == LOW && qb == HIGH))
      check = 0;

    // 2) HOLD: J=0, K=0 -> Q should remain LOW
    digitalWrite(ip_pins[ff][1], LOW);  // J
    digitalWrite(ip_pins[ff][2], LOW);  // K
    pulseClock(ip_pins[ff][0]);
    q = digitalRead(op_pins[ff][0]);
    qb = digitalRead(op_pins[ff][1]);
    if (q != LOW || qb == q)
      check = 0;

    // 3) SET: J=1, K=0 -> Q should become HIGH
    digitalWrite(ip_pins[ff][1], HIGH);
    digitalWrite(ip_pins[ff][2], LOW);
    pulseClock(ip_pins[ff][0]);
    q = digitalRead(op_pins[ff][0]);
    qb = digitalRead(op_pins[ff][1]);
    if (q != HIGH || qb == q)
      check = 0;

    // 4) RESET: J=0, K=1 -> Q should become LOW
    digitalWrite(ip_pins[ff][1], LOW);
    digitalWrite(ip_pins[ff][2], HIGH);
    pulseClock(ip_pins[ff][0]);
    q = digitalRead(op_pins[ff][0]);
    qb = digitalRead(op_pins[ff][1]);
    if (q != LOW || qb == q)
      check = 0;

    // 5) TOGGLE: J=1, K=1 -> Q should toggle (from LOW to HIGH here)
    digitalWrite(ip_pins[ff][1], HIGH);
    digitalWrite(ip_pins[ff][2], HIGH);
    pulseClock(ip_pins[ff][0]);
    q = digitalRead(op_pins[ff][0]);
    qb = digitalRead(op_pins[ff][1]);
    if (q != HIGH || qb == q)
      check = 0;

    flag += check;
  }

  return flag;
}

int self_check_7474() {

  int flag = 0;

  int total_pins = 14;  // IC 7474 has 14 pins

  int gnd_pin = 7;   // GND pin
  int vcc_pin = 14;  // VCC pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }
  pinMode(gnd_pin, OUTPUT);
  digitalWrite(gnd_pin, LOW);

  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }
  pinMode(vcc_pin, OUTPUT);
  digitalWrite(vcc_pin, HIGH);

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {

    return flag;  //Return 0
  }

  int pin, pin1;

  // Inputs {D, CLK, PRE, CLR} for 2 flip-flops
  int ip_pins[2][4] = { { 2, 3, 4, 1 }, { 12, 11, 10, 13 } };

  for (pin1 = 0; pin1 < 2; pin1++) {
    for (pin = 0; pin < 4; pin++) {
      if (ip_pins[pin1][pin] >= ((total_pins / 2) + 1)) {
        ip_pins[pin1][pin] += ((max_pin - total_pins) + start_pin);
      } else {
        ip_pins[pin1][pin] += start_pin;
      }
      pinMode(ip_pins[pin1][pin], OUTPUT);
    }
  }

  // Outputs {Q, Qbar} for 2 flip-flops
  int op_pins[2][2] = { { 5, 6 }, { 9, 8 } };

  for (pin1 = 0; pin1 < 2; pin1++) {
    for (pin = 0; pin < 2; pin++) {
      if (op_pins[pin1][pin] >= ((total_pins / 2) + 1)) {
        op_pins[pin1][pin] += ((max_pin - total_pins) + start_pin);
      } else {
        op_pins[pin1][pin] += start_pin;
      }
      pinMode(op_pins[pin1][pin], INPUT);
    }
  }

  // Truth table cases {PRE, CLR, D, expected_Q}
  int tt_7474[4][4] = {
    { 1, 0, 0, 0 },  // CLR active → Q=0
    { 0, 1, 0, 1 },  // PRE active → Q=1
    { 1, 1, 0, 0 },  // Normal, D=0 → Q=0
    { 1, 1, 1, 1 }   // Normal, D=1 → Q=1
  };

  for (int ff = 0; ff < 2; ff++) {
    int check = 1;

    for (int tt_i = 0; tt_i < 4; tt_i++) {
      // Apply PRE and CLR
      digitalWrite(ip_pins[ff][2], tt_7474[tt_i][0]);
      digitalWrite(ip_pins[ff][3], tt_7474[tt_i][1]);

      // Apply D
      digitalWrite(ip_pins[ff][0], tt_7474[tt_i][2]);

      // Pulse CLK only in normal mode
      if (tt_7474[tt_i][0] == 1 && tt_7474[tt_i][1] == 1) {
        digitalWrite(ip_pins[ff][1], HIGH);
        delayMicroseconds(5);
        digitalWrite(ip_pins[ff][1], LOW);
      }
      delay(5);

      int q_val = digitalRead(op_pins[ff][0]);
      int qb_val = digitalRead(op_pins[ff][1]);

      if (q_val != tt_7474[tt_i][3])
        check = 0;
      if (qb_val != !q_val)
        check = 0;
    }

    flag += check;
  }

  return flag;
}

int self_check_7476() {

  int flag = 0;

  int total_pins = 16;  // IC 7476 has 16 pins

  int gnd_pin = 13;  // GND pin
  int vcc_pin = 5;   // VCC pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }
  pinMode(gnd_pin, OUTPUT);
  digitalWrite(gnd_pin, LOW);

  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }
  pinMode(vcc_pin, OUTPUT);
  digitalWrite(vcc_pin, HIGH);

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    return flag;  //Return 0
  }

  int pin, pin1;

  // Inputs {J, K, CLK, PRE, CLR} for 2 flip-flops
  int ip_pins[2][5] = { { 4, 16, 1, 2, 3 }, { 9, 12, 6, 7, 8 } };

  for (pin1 = 0; pin1 < 2; pin1++) {
    for (pin = 0; pin < 5; pin++) {
      if (ip_pins[pin1][pin] >= ((total_pins / 2) + 1)) {
        ip_pins[pin1][pin] += ((max_pin - total_pins) + start_pin);
      } else {
        ip_pins[pin1][pin] += start_pin;
      }
      pinMode(ip_pins[pin1][pin], OUTPUT);
    }
  }

  // Outputs {Q, Qbar} for 2 flip-flops
  int op_pins[2][2] = { { 15, 14 }, { 11, 10 } };

  for (pin1 = 0; pin1 < 2; pin1++) {
    for (pin = 0; pin < 2; pin++) {
      if (op_pins[pin1][pin] >= ((total_pins / 2) + 1)) {
        op_pins[pin1][pin] += ((max_pin - total_pins) + start_pin);
      } else {
        op_pins[pin1][pin] += start_pin;
      }
      pinMode(op_pins[pin1][pin], INPUT);
    }
  }

  // Truth table {J, K, PRE, CLR, expected_action}
  // -1 = Hold, -2 = Toggle
  int tt_7476[5][5] = {
    { 0, 0, 1, 1, -1 },  // Hold
    { 0, 1, 1, 1, 0 },   // Reset
    { 1, 0, 1, 1, 1 },   // Set
    { 1, 1, 1, 1, -2 },  // Toggle
    { 0, 0, 0, 1, 1 }    // PRE active → Q=1
  };

  for (int ff = 0; ff < 2; ff++) {
    int check = 1;
    int q_state = 0;  // start assumption

    for (int tt_i = 0; tt_i < 5; tt_i++) {
      // Apply J, K
      digitalWrite(ip_pins[ff][0], tt_7476[tt_i][0]);
      digitalWrite(ip_pins[ff][1], tt_7476[tt_i][1]);

      // Apply PRE, CLR
      digitalWrite(ip_pins[ff][3], tt_7476[tt_i][2]);
      digitalWrite(ip_pins[ff][4], tt_7476[tt_i][3]);

      // Pulse CLK in normal cases
      if (tt_7476[tt_i][2] == 1 && tt_7476[tt_i][3] == 1) {
        digitalWrite(ip_pins[ff][2], LOW);
        delayMicroseconds(5);
        digitalWrite(ip_pins[ff][2], HIGH);
        delayMicroseconds(5);
        digitalWrite(ip_pins[ff][2], LOW);
      }
      delay(5);

      int q_val = digitalRead(op_pins[ff][0]);
      int qb_val = digitalRead(op_pins[ff][1]);

      // Expected Q evaluation
      if (tt_7476[tt_i][4] == -1) {
        if (q_val != q_state)
          check = 0;
      } else if (tt_7476[tt_i][4] == -2) {
        q_state = !q_state;
        if (q_val != q_state)
          check = 0;
      } else {
        if (q_val != tt_7476[tt_i][4])
          check = 0;
        q_state = q_val;
      }

      if (qb_val != !q_val)
        check = 0;
    }

    flag += check;
  }

  return flag;
}

int self_check_7478() {

  int flag = 0;

  int total_pins = 16;  // Total pins in IC 7478

  int gnd_pin = 11;  // Ground Pin
  int vcc_pin = 4;   // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }

  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }

  pinMode(gnd_pin, OUTPUT);
  digitalWrite(gnd_pin, LOW);

  pinMode(vcc_pin, OUTPUT);
  digitalWrite(vcc_pin, HIGH);

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    return flag;  //Return 0
  }

  // Pin mapping for both flip-flops (J, K, CLK, PRE, CLR, Q, Qbar)
  int jk_pins[2][7] = {
    { 3, 14, 1, 2, 5, 13, 12 },  // FF1 → J,K,CLK,PRE,CLR,Q,Qbar
    { 10, 7, 1, 6, 5, 8, 9 }     // FF2 → J,K,CLK,PRE,CLR,Q,Qbar
  };

  for (int ff = 0; ff < 2; ff++) {
    int J = jk_pins[ff][0], K = jk_pins[ff][1], CLK = jk_pins[ff][2];
    int PRE = jk_pins[ff][3], CLR = jk_pins[ff][4];
    int Q = jk_pins[ff][5], Qbar = jk_pins[ff][6];

    // Adjust pins for Arduino mapping
    int pins_ff[7] = { J, K, CLK, PRE, CLR, Q, Qbar };
    for (int p = 0; p < 7; p++) {
      if (pins_ff[p] >= ((total_pins / 2) + 1)) {
        pins_ff[p] += ((max_pin - total_pins) + start_pin);
      } else {
        pins_ff[p] += start_pin;
      }
    }

    J = pins_ff[0];
    K = pins_ff[1];
    CLK = pins_ff[2];
    PRE = pins_ff[3];
    CLR = pins_ff[4];
    Q = pins_ff[5];
    Qbar = pins_ff[6];

    // Set pin directions
    pinMode(J, OUTPUT);
    pinMode(K, OUTPUT);
    pinMode(CLK, OUTPUT);
    pinMode(PRE, OUTPUT);
    pinMode(CLR, OUTPUT);
    pinMode(Q, INPUT);
    pinMode(Qbar, INPUT);

    int check = 1;

    // --- Test Clear ---
    digitalWrite(CLR, LOW);
    digitalWrite(PRE, HIGH);

    if (!(digitalRead(Q) == LOW && digitalRead(Qbar) == HIGH))
      check = 0;
    digitalWrite(CLR, HIGH);

    // --- Test Preset ---
    digitalWrite(PRE, LOW);
    digitalWrite(CLR, HIGH);

    if (!(digitalRead(Q) == HIGH && digitalRead(Qbar) == LOW))
      check = 0;
    digitalWrite(PRE, HIGH);

    // --- Test JK Operation (Toggle mode) ---
    digitalWrite(J, HIGH);
    digitalWrite(K, HIGH);
    for (int t = 0; t < 2; t++) {
      digitalWrite(CLK, LOW);
      delay(5);
      digitalWrite(CLK, HIGH);
    }
    int q1 = digitalRead(Q);
    digitalWrite(CLK, LOW);
    delay(5);
    digitalWrite(CLK, HIGH);

    int q2 = digitalRead(Q);

    if (q1 == q2)
      check = 0;  // Q must toggle

    flag += check;
  }

  return flag;
}

int self_check_74112() {

  int flag = 0;

  int total_pins = 16;  // Total pins in IC 74112

  int gnd_pin = 8;   // Ground Pin
  int vcc_pin = 16;  // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }
  pinMode(gnd_pin, OUTPUT);
  digitalWrite(gnd_pin, LOW);

  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }
  pinMode(vcc_pin, OUTPUT);
  digitalWrite(vcc_pin, HIGH);

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    return flag;  //Return 0
  }

  int pin;

  // Each flip-flop: J, K, CLK, CLR, PRE, Q, Q'
  int ip_pins[2][5] = { { 3, 2, 1, 15, 4 }, { 11, 12, 13, 14, 10 } };  // J,K,CLK,CLR,PRE
  int op_pins[2][2] = { { 5, 6 }, { 9, 7 } };                          // Q, Q'

  for (int ff = 0; ff < 2; ff++) {
    for (pin = 0; pin < 5; pin++) {
      if (ip_pins[ff][pin] >= ((total_pins / 2) + 1)) {
        ip_pins[ff][pin] += ((max_pin - total_pins) + start_pin);
      } else {
        ip_pins[ff][pin] += start_pin;
      }
      pinMode(ip_pins[ff][pin], OUTPUT);
    }

    for (pin = 0; pin < 2; pin++) {
      if (op_pins[ff][pin] >= ((total_pins / 2) + 1)) {
        op_pins[ff][pin] += ((max_pin - total_pins) + start_pin);
      } else {
        op_pins[ff][pin] += start_pin;
      }
      pinMode(op_pins[ff][pin], INPUT);
    }
  }

  // Test sequence for JK flip-flop
  // Conditions: {J,K,CLR,PRE,Expected Q,Expected Q'}
  int tt_74112[6][6] = {
    { 0, 0, 1, 1, -1, -1 },  // No change, depends on previous state
    { 0, 1, 1, 1, 0, 1 },    // Reset
    { 1, 0, 1, 1, 1, 0 },    // Set
    { 1, 1, 1, 1, -2, -2 },  // Toggle
    { 0, 0, 0, 1, 0, 1 },    // CLR active
    { 0, 0, 1, 0, 1, 0 }     // PRE active
  };

  for (int ff = 0; ff < 2; ff++) {
    int check = 1;
    int q_state = 0;  // assume Q=0 initially

    for (int t = 0; t < 6; t++) {
      digitalWrite(ip_pins[ff][0], tt_74112[t][0]);  // J
      digitalWrite(ip_pins[ff][1], tt_74112[t][1]);  // K
      digitalWrite(ip_pins[ff][3], tt_74112[t][2]);  // CLR
      digitalWrite(ip_pins[ff][4], tt_74112[t][3]);  // PRE

      // Clock pulse
      digitalWrite(ip_pins[ff][2], LOW);
      delayMicroseconds(5);
      digitalWrite(ip_pins[ff][2], HIGH);
      delayMicroseconds(5);
      digitalWrite(ip_pins[ff][2], LOW);

      int q_read = digitalRead(op_pins[ff][0]);
      int nq_read = digitalRead(op_pins[ff][1]);

      if (tt_74112[t][4] == -1) {
        // No change, skip check
        continue;
      } else if (tt_74112[t][4] == -2) {
        // Toggle
        q_state = !q_state;
        if (q_read != q_state || nq_read == q_state) {
          check = 0;
        }
      } else {
        if (q_read != tt_74112[t][4] || nq_read != tt_74112[t][5]) {
          check = 0;
        }
        q_state = q_read;
      }
    }

    flag += check;
  }

  return flag;
}

int self_check_74165() {

  int flag = 0;
  int work = 1;

  int total_pins = 16;  // Total pins in IC 74165

  int gnd_pin = 8;   // Ground Pin
  int vcc_pin = 16;  // VCC Pin

  // Setting Arduino Ground pin
  if (gnd_pin >= ((total_pins / 2) + 1)) {
    gnd_pin += ((max_pin - total_pins) + start_pin);
  } else {
    gnd_pin += start_pin;
  }
  pinMode(gnd_pin, OUTPUT);
  digitalWrite(gnd_pin, LOW);

  // Setting Arduino VCC pin
  if (vcc_pin >= ((total_pins / 2) + 1)) {
    vcc_pin += ((max_pin - total_pins) + start_pin);
  } else {
    vcc_pin += start_pin;
  }
  pinMode(vcc_pin, OUTPUT);
  digitalWrite(vcc_pin, HIGH);

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    return flag;  //Return 0
  }

  int pin;

  // Parallel Data Inputs D0–D7 (pins )
  int ip_pins[8] = { 11, 12, 13, 14, 3, 4, 5, 6 };
  for (pin = 0; pin < 8; pin++) {
    if (ip_pins[pin] >= ((total_pins / 2) + 1)) {
      ip_pins[pin] += ((max_pin - total_pins) + start_pin);
    } else {
      ip_pins[pin] += start_pin;
    }
    pinMode(ip_pins[pin], OUTPUT);
  }

  // Control pins
  int load_pin = 1;  // /PL (active LOW)
  int clk_pin = 2;   // CP (clock)
  int ce_pin = 15;   // /CE (active LOW)

  if (load_pin >= ((total_pins / 2) + 1))
    load_pin += ((max_pin - total_pins) + start_pin);
  else
    load_pin += start_pin;
  if (clk_pin >= ((total_pins / 2) + 1))
    clk_pin += ((max_pin - total_pins) + start_pin);
  else
    clk_pin += start_pin;
  if (ce_pin >= ((total_pins / 2) + 1))
    ce_pin += ((max_pin - total_pins) + start_pin);
  else
    ce_pin += start_pin;

  pinMode(load_pin, OUTPUT);
  pinMode(clk_pin, OUTPUT);
  pinMode(ce_pin, OUTPUT);

  digitalWrite(clk_pin, LOW);
  digitalWrite(ce_pin, LOW);  // enable shift register

  // Serial output Q7 (pin 9)
  int q7_pin = 9;
  if (q7_pin >= ((total_pins / 2) + 1)) {
    q7_pin += ((max_pin - total_pins) + start_pin);
  } else {
    q7_pin += start_pin;
  }
  pinMode(q7_pin, INPUT);

  // Test patterns
  int patterns[2][8] = {
    { 1, 0, 1, 0, 1, 0, 1, 0 },  // 0xAA
    { 1, 1, 0, 0, 1, 1, 0, 0 }   // 0xCC
  };

  for (int pat = 0; pat < 2; pat++) {
    // Load parallel data (active LOW pulse on /PL)
    digitalWrite(load_pin, LOW);
    for (int b = 0; b < 8; b++) {
      digitalWrite(ip_pins[b], patterns[pat][b]);
    }
    delayMicroseconds(5);
    digitalWrite(load_pin, HIGH);

    // Now read serially bit-by-bit
    int check = 1;
    for (int b = 7; b >= 0; b--) {
      int q_val = digitalRead(q7_pin);
      if (q_val != patterns[pat][b]) {
        check = 0;
      }
      // Clock pulse
      digitalWrite(clk_pin, HIGH);
      delayMicroseconds(5);
      digitalWrite(clk_pin, LOW);
      delayMicroseconds(5);
    }

    flag += check;
    work *= check;
  }

  return flag;
}

int self_check_74174() {

  int flag = 0;

  int total_pins = 16;

  int gnd_pin = 8;
  int vcc_pin = 16;

  // GND
  if (gnd_pin >= ((total_pins / 2) + 1))
    gnd_pin += ((max_pin - total_pins) + start_pin);
  else
    gnd_pin += start_pin;
  pinMode(gnd_pin, OUTPUT);
  digitalWrite(gnd_pin, LOW);

  // VCC
  if (vcc_pin >= ((total_pins / 2) + 1))
    vcc_pin += ((max_pin - total_pins) + start_pin);
  else
    vcc_pin += start_pin;
  pinMode(vcc_pin, OUTPUT);
  digitalWrite(vcc_pin, HIGH);

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    return flag;  //Return 0
  }

  // Data inputs (IC pins)
  int d_ic[6] = { 3, 4, 6, 11, 13, 14 };
  int d_pins[6];
  for (int i = 0; i < 6; i++) {
    d_pins[i] = (d_ic[i] >= ((total_pins / 2) + 1)) ? d_ic[i] + ((max_pin - total_pins) + start_pin) : d_ic[i] + start_pin;
    pinMode(d_pins[i], OUTPUT);
  }

  // Outputs (IC pins)
  int q_ic[6] = { 2, 5, 7, 10, 12, 15 };
  int q_pins[6];
  for (int i = 0; i < 6; i++) {
    q_pins[i] = (q_ic[i] >= ((total_pins / 2) + 1)) ? q_ic[i] + ((max_pin - total_pins) + start_pin) : q_ic[i] + start_pin;
    pinMode(q_pins[i], INPUT);
  }

  // CLK (9), CLR (1 active low)
  int clk_pin = 9;
  clk_pin = (clk_pin >= ((total_pins / 2) + 1)) ? clk_pin + ((max_pin - total_pins) + start_pin) : clk_pin + start_pin;
  pinMode(clk_pin, OUTPUT);

  int clr_pin = 1;
  clr_pin = (clr_pin >= ((total_pins / 2) + 1)) ? clr_pin + ((max_pin - total_pins) + start_pin) : clr_pin + start_pin;
  pinMode(clr_pin, OUTPUT);

  // --- Test 1: Clear ---
  digitalWrite(clr_pin, LOW);
  delayMicroseconds(50);
  digitalWrite(clr_pin, HIGH);
  delayMicroseconds(50);

  int clear_ok = 1;
  for (int i = 0; i < 6; i++) {
    if (digitalRead(q_pins[i]) != LOW)
      clear_ok = 0;
  }

  // --- Test 2: Walking-1 (set test) ---
  for (int bit = 0; bit < 6; bit++) {
    for (int i = 0; i < 6; i++)
      digitalWrite(d_pins[i], LOW);
    digitalWrite(d_pins[bit], HIGH);

    // Clock pulse
    digitalWrite(clk_pin, LOW);
    delayMicroseconds(50);
    digitalWrite(clk_pin, HIGH);
    delayMicroseconds(50);
    digitalWrite(clk_pin, LOW);

    int check = 1;
    for (int i = 0; i < 6; i++) {
      int expected = (i == bit) ? HIGH : LOW;
      if (digitalRead(q_pins[i]) != expected)
        check = 0;
    }

    flag += check;
  }

  // --- Test 3: Walking-0 (reset test) ---
  for (int bit = 0; bit < 6; bit++) {
    for (int i = 0; i < 6; i++)
      digitalWrite(d_pins[i], HIGH);
    digitalWrite(d_pins[bit], LOW);

    // Clock pulse
    digitalWrite(clk_pin, LOW);
    delayMicroseconds(50);
    digitalWrite(clk_pin, HIGH);
    delayMicroseconds(50);
    digitalWrite(clk_pin, LOW);

    int check = 1;
    for (int i = 0; i < 6; i++) {
      int expected = (i == bit) ? LOW : HIGH;
      if (digitalRead(q_pins[i]) != expected)
        check = 0;
    }

    flag += check;
  }

  return flag;
}

int self_check_74175() {

  int flag = 0;

  int total_pins = 16;  // Total pins in IC 74175

  int gnd_pin = 8;   // Ground Pin
  int vcc_pin = 16;  // VCC Pin

  // Setting Arduino Ground pin
  gnd_pin = (gnd_pin >= ((total_pins / 2) + 1)) ? gnd_pin + ((max_pin - total_pins) + start_pin) : gnd_pin + start_pin;
  pinMode(gnd_pin, OUTPUT);
  digitalWrite(gnd_pin, LOW);

  // Setting Arduino VCC pin
  vcc_pin = (vcc_pin >= ((total_pins / 2) + 1)) ? vcc_pin + ((max_pin - total_pins) + start_pin) : vcc_pin + start_pin;
  pinMode(vcc_pin, OUTPUT);
  digitalWrite(vcc_pin, HIGH);

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {
    return flag;  //Return 0
  }

  // Data Inputs D0–D3 (IC pins 4,5,12,13)
  int d_ic[4] = { 4, 5, 12, 13 };
  int d_pins[4];
  for (int i = 0; i < 4; i++) {
    d_pins[i] = (d_ic[i] >= ((total_pins / 2) + 1)) ? d_ic[i] + ((max_pin - total_pins) + start_pin) : d_ic[i] + start_pin;
    pinMode(d_pins[i], OUTPUT);
  }

  // Outputs Q0–Q3 (IC pins 2,7,10,15)
  int q_ic[4] = { 2, 7, 10, 15 };
  int q_pins[4];
  for (int i = 0; i < 4; i++) {
    q_pins[i] = (q_ic[i] >= ((total_pins / 2) + 1)) ? q_ic[i] + ((max_pin - total_pins) + start_pin) : q_ic[i] + start_pin;
    pinMode(q_pins[i], INPUT);
  }

  // Common Clock (pin 9)
  int clk_pin = 9;
  clk_pin = (clk_pin >= ((total_pins / 2) + 1)) ? clk_pin + ((max_pin - total_pins) + start_pin) : clk_pin + start_pin;
  pinMode(clk_pin, OUTPUT);

  // Asynchronous Clear (active LOW, pin 1)
  int clr_pin = 1;
  clr_pin = (clr_pin >= ((total_pins / 2) + 1)) ? clr_pin + ((max_pin - total_pins) + start_pin) : clr_pin + start_pin;
  pinMode(clr_pin, OUTPUT);

  // --- Test 1: Clear function ---
  digitalWrite(clr_pin, LOW);

  int clear_ok = 1;
  for (int i = 0; i < 4; i++) {
    if (digitalRead(q_pins[i]) != LOW)
      clear_ok = 0;
  }

  flag += clear_ok;

  digitalWrite(clr_pin, HIGH);

  // --- Test 2: Walking 1 and 0 patterns per flip-flop ---
  for (int ff = 0; ff < 4; ff++) {
    // First load a '1' on this D, '0' on others
    for (int i = 0; i < 4; i++)
      digitalWrite(d_pins[i], (i == ff) ? HIGH : LOW);

    // Clock pulse
    digitalWrite(clk_pin, LOW);
    delayMicroseconds(50);
    digitalWrite(clk_pin, HIGH);
    delayMicroseconds(50);
    digitalWrite(clk_pin, LOW);

    int set_ok = (digitalRead(q_pins[ff]) == HIGH);

    // Now load a '0' on this D, '1' on others
    for (int i = 0; i < 4; i++)
      digitalWrite(d_pins[i], (i == ff) ? LOW : HIGH);

    // Clock pulse
    digitalWrite(clk_pin, LOW);
    delayMicroseconds(50);
    digitalWrite(clk_pin, HIGH);
    delayMicroseconds(50);
    digitalWrite(clk_pin, LOW);

    int reset_ok = (digitalRead(q_pins[ff]) == LOW);
  }

  return flag;
}

int self_check_74194() {

  int flag = 0;

  int total_pins = 16;

  // small helper to map DIP pin -> your Arduino pin scheme
  auto mapPin = [&](int p) -> int {
    if (p >= ((total_pins / 2) + 1))
      return p + ((max_pin - total_pins) + start_pin);
    else
      return p + start_pin;
  };

  // Vcc / GND
  int gnd_pin = 8;
  int vcc_pin = 16;
  pinMode(gnd_pin, OUTPUT);
  digitalWrite(gnd_pin, LOW);
  pinMode(vcc_pin, OUTPUT);
  digitalWrite(vcc_pin, HIGH);

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {

    return flag;  //Return 0
  }

  // Parallel data inputs D0..D3 (D pins on IC: 3,4,5,6)
  int d_pins[4] = { 3, 4, 5, 6 };
  for (int i = 0; i < 4; ++i) {
    pinMode(d_pins[i], OUTPUT);
    digitalWrite(d_pins[i], LOW);
  }

  // Control / serial pins (map them once and use the mapped variables everywhere)
  int dsr_pin = 2;   // Serial input for shift-right (D_SR)
  int dsl_pin = 7;   // Serial input for shift-left  (D_SL)
  int s0_pin = 9;    // S0
  int s1_pin = 10;   // S1
  int clk_pin = 11;  // CLK (rising-edge triggered)
  int clr_pin = 1;   // MR / CLR (active LOW)

  pinMode(dsr_pin, OUTPUT);
  pinMode(dsl_pin, OUTPUT);
  pinMode(s0_pin, OUTPUT);
  pinMode(s1_pin, OUTPUT);
  pinMode(clk_pin, OUTPUT);
  pinMode(clr_pin, OUTPUT);

  // Outputs Q0..Q3 (IC pins 15,14,13,12 -> Q0..Q3)
  int q_pins[4] = { 15, 14, 13, 12 };
  for (int i = 0; i < 4; ++i)
    pinMode(q_pins[i], INPUT);

  // helper to synchronous parallel-load a 4-bit array (A = D0..D3)
  auto parallel_load = [&](int data[4]) {
    // per datasheet: change mode while CLK is HIGH, then create a rising edge
    digitalWrite(clk_pin, HIGH);
    digitalWrite(s1_pin, HIGH);
    digitalWrite(s0_pin, HIGH);  // load mode
    for (int i = 0; i < 4; ++i)
      digitalWrite(d_pins[i], data[i]);
    delayMicroseconds(10);       // small settle
    digitalWrite(clk_pin, LOW);  // prepare for rising edge
    delayMicroseconds(10);
    digitalWrite(clk_pin, HIGH);  // rising edge loads data
    delayMicroseconds(50);        // let outputs settle
  };

  // --- TEST 1: Asynchronous CLEAR (MR active LOW) ---
  digitalWrite(clr_pin, LOW);
  delayMicroseconds(50);
  digitalWrite(clr_pin, HIGH);  // release clear
  delayMicroseconds(50);

  int clear_ok = 1;
  for (int i = 0; i < 4; ++i) {
    if (digitalRead(q_pins[i]) != LOW)
      clear_ok = 0;
  }

  // Test data (use same pattern for deterministic checks)
  int test_data[4] = { 1, 0, 1, 0 };  // D0..D3

  // --- TEST 2: Parallel LOAD ---
  parallel_load(test_data);
  int load_ok = 1;
  for (int i = 0; i < 4; ++i)
    if (digitalRead(q_pins[i]) != test_data[i])
      load_ok = 0;

  // --- TEST 3: SHIFT RIGHT (S0=1, S1=0) ---
  // Reload a known pattern first so this test is independent:
  parallel_load(test_data);

  // set mode while clock HIGH (per datasheet), then produce rising edge
  digitalWrite(clk_pin, HIGH);
  digitalWrite(s0_pin, HIGH);   // S0=1
  digitalWrite(s1_pin, LOW);    // S1=0  -> SHIFT RIGHT
  digitalWrite(dsr_pin, HIGH);  // serial input - not used to check Q3, but define it
  delayMicroseconds(10);
  digitalWrite(clk_pin, LOW);
  delayMicroseconds(10);
  digitalWrite(clk_pin, HIGH);
  delayMicroseconds(50);

  // After one right shift, new Q3 == previous Q2 (test_data[2])
  int expected_q3 = test_data[2];

  parallel_load(test_data);

  digitalWrite(clk_pin, HIGH);
  digitalWrite(s0_pin, LOW);    // S0=0
  digitalWrite(s1_pin, HIGH);   // S1=1  -> SHIFT LEFT
  digitalWrite(dsl_pin, HIGH);  // serial input for left shift (set to known)
  delayMicroseconds(10);
  digitalWrite(clk_pin, LOW);
  delayMicroseconds(10);
  digitalWrite(clk_pin, HIGH);
  delayMicroseconds(50);

  // After one left shift, new Q0 == previous Q1 (test_data[1])
  int expected_q0 = test_data[1];

  return flag;
}

int self_check_74279() {

  int flag = 0;

  int total_pins = 16;

  int gnd_pin = 8;   // GND
  int vcc_pin = 16;  // VCC

  // keep your existing offset logic ( (max_pin - total_pins) + start_pin == 30 )
  if (gnd_pin >= ((total_pins / 2) + 1))
    gnd_pin += ((max_pin - total_pins) + start_pin);
  else
    gnd_pin += start_pin;
  if (vcc_pin >= ((total_pins / 2) + 1))
    vcc_pin += ((max_pin - total_pins) + start_pin);
  else
    vcc_pin += start_pin;

  pinMode(gnd_pin, OUTPUT);
  digitalWrite(gnd_pin, LOW);
  pinMode(vcc_pin, OUTPUT);
  digitalWrite(vcc_pin, HIGH);

  if (digitalRead(gnd_pin) != 0 || digitalRead(vcc_pin) != 1) {

    return flag;  //Return 0
  }

  int pin, pin1;

  // --- IMPORTANT: pin pairs are {S, R} (SET then RESET) per datasheet ---
  // Latch1: S = 2, R = 1, Q = 4
  // Latch2: S = 6, R = 5, Q = 7
  // Latch3: S = 12, R =10, Q = 9   (latch 3 has two S inputs but we use S1=12 here)
  // Latch4: S = 15, R =14, Q =13
  int ip_pins[4][2] = { { 2, 1 }, { 6, 5 }, { 12, 10 }, { 15, 14 } };

  for (pin1 = 0; pin1 < 4; pin1++) {
    for (pin = 0; pin < 2; pin++) {
      if (ip_pins[pin1][pin] >= ((total_pins / 2) + 1))
        ip_pins[pin1][pin] += ((max_pin - total_pins) + start_pin);
      else
        ip_pins[pin1][pin] += start_pin;
      pinMode(ip_pins[pin1][pin], OUTPUT);
      digitalWrite(ip_pins[pin1][pin], HIGH);  // inactive (HIGH) by default
    }
  }

  // Output pins Q (these were correct)
  int op_pins[4] = { 4, 7, 9, 13 };
  for (pin = 0; pin < 4; pin++) {
    if (op_pins[pin] >= ((total_pins / 2) + 1))
      op_pins[pin] += ((max_pin - total_pins) + start_pin);
    else
      op_pins[pin] += start_pin;
    pinMode(op_pins[pin], INPUT);
  }

  // Truth table tests for each latch
  for (int latch = 0; latch < 4; latch++) {
    int check = 1;

    // --- SET test: S = LOW, R = HIGH => Q should go HIGH ---
    digitalWrite(ip_pins[latch][0], LOW);   // S LOW
    digitalWrite(ip_pins[latch][1], HIGH);  // R HIGH
    delayMicroseconds(100);
    if (digitalRead(op_pins[latch]) != HIGH)
      check = 0;

    // back to inactive
    digitalWrite(ip_pins[latch][0], HIGH);
    digitalWrite(ip_pins[latch][1], HIGH);
    delayMicroseconds(50);

    // --- RESET test: S = HIGH, R = LOW => Q should go LOW ---
    digitalWrite(ip_pins[latch][0], HIGH);  // S HIGH
    digitalWrite(ip_pins[latch][1], LOW);   // R LOW
    delayMicroseconds(100);
    if (digitalRead(op_pins[latch]) != LOW)
      check = 0;

    // reset inputs to inactive
    digitalWrite(ip_pins[latch][0], HIGH);
    digitalWrite(ip_pins[latch][1], HIGH);
    flag += check;
  }
  return flag;
}

int getOriginalPin(int pin, int total_pin) {
  if (total_pin == max_pin) {
    return (pin - start_pin);
  } else {
    if (pin >= (start_pin + (total_pin / 2) + 1)) {
      return (pin - ((max_pin - total_pin) + start_pin));
    } else {
      return (pin - start_pin);
    }
  }
}


void setup() {
  // lcd.begin(16, 2);
  Wire.begin();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("..IC  VERIFIER..");
  lcd.setCursor(0, 1);
  lcd.print("--CS Dept (VC)--");
  delay(delay_long);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("A: Manual Test");
  lcd.setCursor(0, 1);
  lcd.print("B: Self Test");
}

void loop() {
  char key = keypad.getKey();

  if (mode == ' ') {
    // No mode chosen yet
    if (key == 'A') {
      mode = 'A';
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Manual Test Mode");
      lcd.setCursor(0, 1);
      lcd.print("Enter IC Number:");
      inputValue = "";
    } else if (key == 'B') {
      mode = 'B';
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Self Test Mode");
      lcd.setCursor(0, 1);
      lcd.print("Enter Pin Number:");
      inputValue = "";
    }
  } else {
    // Already in a mode (taking input)
    if (key) {
      if (key >= '0' && key <= '9') {
        inputValue += key;
        updateInputDisplay();
      } else if (key == '*') {  // Backspace
        if (inputValue.length() > 0) {
          inputValue.remove(inputValue.length() - 1);
          updateInputDisplay();
        }
      } else if (key == '#') {  // Enter/Finish
        lcd.clear();
        if (mode == 'A') {
          int icNumber = inputValue.toInt();
          lcd.setCursor(0, 0);
          lcd.print("IC Number:");
          lcd.setCursor(0, 1);
          lcd.print(inputValue);

          switch (icNumber) {
            // 1-input logic gate
            case 7404:
              check_7404();
              break;

            // 2-input logic gates
            case 7400:
              check_7400();
              break;
            case 7402:
              check_7402();
              break;
            case 7408:
              check_7408();
              break;
            case 7428:
              check_7428();
              break;
            case 7432:
              check_7432();
              break;
            case 7486:
              check_7486();
              break;
            case 74136:
              check_74136();
              break;
            case 74226:
              check_74226();
              break;
            case 74266:
              check_74266();
              break;

            // 3-input logic gates
            case 7410:
              check_7410();
              break;
            case 7411:
              check_7411();
              break;
            case 7412:
              check_7412();
              break;
            case 7415:
              check_7415();
              break;
            case 7427:
              check_7427();
              break;
            // 4-input logic gates
            case 7420:
              check_7420();
              break;
            case 7421:
              check_7421();
              break;
            case 7425:
              check_7425();
              break;
            // Arithmetic Circuits
            case 7482:
              check_7482();
              break;
            case 7483:
              check_7483();
              break;
            case 74283:
              check_74283();
              break;

            // Combinational Circuits
            case 7442:
              check_7442();
              break;
            case 74138:
              check_74138();
              break;
            case 74147:
              check_74147();
              break;
            case 74148:
              check_74148();
              break;
            case 74151:
              check_74151();
              break;
            case 74153:
              check_74153();
              break;
            case 74157:
              check_74157();
              break;
            case 74158:
              check_74158();
              break;
            case 74184:
              check_74184();
              break;

            // Sequential Circuits
            case 7468:
              check_7468();
              break;
            case 7473:
              check_7473();
              break;
            case 7474:
              check_7474();
              break;
            case 7476:
              check_7476();
              break;
            case 7478:
              check_7478();
              break;
            case 74112:
              check_74112();
              break;
            case 74165:
              check_74165();
              break;
            case 74174:
              check_74174();
              break;
            case 74175:
              check_74175();
              break;
            case 74194:
              check_74194();
              break;
            case 74279:
              check_74279();
              break;

            // Default Case
            default:
              lcd.clear();
              String noIC = "IC not listed...";
              scrollText(0, noIC);
              break;
          }
        } else if (mode == 'B') {
          int pinNumber = inputValue.toInt();
          lcd.setCursor(0, 0);
          lcd.print("Pin Number:");
          lcd.setCursor(0, 1);
          lcd.print(inputValue);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Verifying...");
          delay(delay_long);
          switch (pinNumber) {
            case 14:
              if (self_check_7404()) {  // 1-input Logic Gates
                check_7404();
                verificationDone();

              } else if (self_check_7400()) {  // 2-input Logic Gates
                check_7400();
                verificationDone();

              } else if (self_check_7402()) {
                check_7402();
                verificationDone();

              } else if (self_check_7408()) {
                check_7408();
                verificationDone();

              } else if (self_check_7428()) {
                check_7428();
                verificationDone();

              } else if (self_check_7432()) {
                check_7432();
                verificationDone();

              } else if (self_check_7486()) {
                check_7486();
                verificationDone();

              } else if (self_check_74136()) {
                check_74136();
                verificationDone();

              } else if (self_check_74226()) {
                check_74226();
                verificationDone();

              } else if (self_check_74266()) {
                check_74266();
                verificationDone();

              } else if (self_check_7410()) {  // 3-input Logic Gates
                check_7410();
                verificationDone();

              } else if (self_check_7411()) {
                check_7411();
                verificationDone();

              } else if (self_check_7412()) {
                check_7412();
                verificationDone();

              } else if (self_check_7415()) {
                check_7415();
                verificationDone();

              } else if (self_check_7427()) {
                check_7427();
                verificationDone();

              } else if (self_check_7420()) {  // 4-input Logic Gates
                check_7420();
                verificationDone();

              } else if (self_check_7421()) {
                check_7421();
                verificationDone();

              } else if (self_check_7425()) {
                check_7425();
                verificationDone();

              } else if (self_check_7482()) {  // Arithmetic Circuits
                check_7482();
                verificationDone();

              } else if (self_check_7473()) {  // Sequential Circuits
                check_7473();
                verificationDone();

              } else if (self_check_7474()) {
                check_7474();
                verificationDone();

              } else {
                scrollText(1, "IC is not recognizable");
              }
              break;
            case 16:
              if (self_check_7483()) {  // Arithmetic Circuits
                check_7483();
                verificationDone();

              } else if (self_check_74283()) {
                check_74283();
                verificationDone();

              } else if (self_check_7442()) {  // Combinational Circuits
                check_7442();
                verificationDone();

              } else if (self_check_74138()) {
                check_74138();
                verificationDone();

              } else if (self_check_74147()) {
                check_74147();
                verificationDone();

              } else if (self_check_74148()) {
                check_74148();
                verificationDone();

              } else if (self_check_74151()) {
                check_74151();
                verificationDone();

              } else if (self_check_74153()) {
                check_74153();
                verificationDone();

              } else if (self_check_74157()) {
                check_74157();
                verificationDone();

              } else if (self_check_74158()) {
                check_74158();
                verificationDone();

              } else if (self_check_74184()) {
                check_74184();
                verificationDone();

              } else if (self_check_7468()) {  // Sequential Circuits
                check_7468();
                verificationDone();

              } else if (self_check_7476()) {
                check_7476();
                verificationDone();

              } else if (self_check_7478()) {
                check_7478();
                verificationDone();

              } else if (self_check_74112()) {
                check_74112();
                verificationDone();

              } else if (self_check_74165()) {
                check_74165();
                verificationDone();

              } else if (self_check_74174()) {
                check_74174();
                verificationDone();

              } else if (self_check_74175()) {
                check_74175();
                verificationDone();

              } else if (self_check_74194()) {
                check_74194();
                verificationDone();

              } else if (self_check_74279()) {
                check_74279();
                verificationDone();

              } else {
                scrollText(1, "IC is not recognizable");
              }
              break;
            default:
              String noPin = "No IC of " + String(pinNumber) + " pins is available";
              scrollText(1, noPin);
              break;
          }
        }
        delay(delay_long);  // Show result for 'delay_long' seconds
        resetPrompt();
      }
    }
  }
}

void updateInputDisplay() {
  lcd.setCursor(0, 1);
  lcd.print("                ");  // clear line
  lcd.setCursor(0, 1);
  lcd.print(inputValue);
}

void resetPrompt() {
  mode = ' ';
  inputValue = "";
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("A: Manual Test");
  lcd.setCursor(0, 1);
  lcd.print("B: Self Test");
}

void scrollText(int row, String message) {
  if (message.length() <= 16) {
    // If the message fits, just print it
    lcd.setCursor(0, row);
    lcd.print(message);
    return;
  }

  // Scroll if message is longer
  for (int i = 0; i < message.length() - 16 + 1; i++) {
    lcd.setCursor(0, row);
    lcd.print(message.substring(i, i + 16));
    delay(delay_scroll);
  }
}
//Verification done.
void verificationDone() {
    scrollText(1, F("Verification is completed"));
}
