/*
*  written by:
*
*     // | |                                        // | |
*    //__| |     __     / __      ___     //       //__| |     __      ___      ___   /
*   / ___  |   //  ) ) //   ) ) //___) ) //       / ___  |   //  ) ) //   ) ) //   ) /
*  //    | |  //      //   / / //       //       //    | |  //      //   / / //   / /
* //     | | //      ((___/ / ((____   //       //     | | //      ((___( ( ((___/ /
*
*/
#include <Encoder.h>
const int stick_pins[2] = { 14, 15 };
const int rocker_pins[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
int stick_pos[2];
bool button_state[2];
bool rocker_pos[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
int enc_pos[2] = { 512, 512 };
Encoder enc_1(22, 21);                                      //add encoder objects
Encoder enc_2(20, 19);

void setup(){
  analogReadResolution(12);                                 //analog precision set to 12bits
  //Serial.begin(115200);
  for (int i = 0; i < sizeof(rocker_pins); i++){
    pinMode(rocker_pins[i], INPUT_PULLUP);                  //set all toggle switch pins to input_pullup
  }

  for (int i = 0; i < 2; i++){
    pinMode(stick_pins[i], INPUT);                          //set all analog pins to input - not necessary.
  }
}

void loop(){
  for (uint8_t i = 0; i < 2; i++){
    stick_pos[i] = analogRead(stick_pins[i]);               //read stick position
  }
  Joystick.X(map(stick_pos[0], 0, 4096, 1024, 0));          //send the stick position
  Joystick.Y(map(stick_pos[1], 0, 4096, 1024, 0));

  for (uint8_t i = 0; i < sizeof(rocker_pins); i++){
    rocker_pos[i] = !digitalRead(rocker_pins[i]);           //read toggle switch positions
    int button_num = i;
    button_num++;
    Joystick.button(button_num, rocker_pos[i]);             //send the button states
  }

  button_state[0] = digitalRead(10);
  if (button_state[0] != button_state[1]){
    button_state[1] = button_state[0];
    if (button_state[0] == true){
      Joystick.button(11, 1);
    } else{
      Joystick.button(11, 0);
    }
  }

  int enc_1_pos = 7 * enc_1.read();                         //read encoder status
  int enc_2_pos = 7 * enc_2.read();

  if (enc_1_pos != enc_pos[0]){
    enc_pos[0] = enc_pos[0] + enc_1_pos;                    //check for change in position
    if (enc_pos[0] > 1024){ enc_pos[0] = 1024; }            //keep the encoder value in range
    if (enc_pos[0] < 0){ enc_pos[0] = 0; }
    Joystick.sliderLeft(enc_pos[0]);                        //send the encoder position
    enc_1.write(0);                                         //clear the encoder state
  }
  if (enc_2_pos != enc_pos[1]){
    enc_pos[1] = enc_pos[1] + enc_2_pos;                    //same as encoder 1
    if (enc_pos[1] > 1024){ enc_pos[1] = 1024; }
    if (enc_pos[1] < 0){ enc_pos[1] = 0; }
    Joystick.sliderRight(enc_pos[1]);
    enc_2.write(0);
  }
  delay(10);
}
