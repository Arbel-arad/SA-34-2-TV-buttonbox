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
const int stick_pins[2] = {14, 15};
const int rocker_pins[11] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
int stick_pos[2];
bool rocker_pos[11] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
int enc_pos[2] = {512, 512};
Encoder enc_1 (22, 21);
Encoder enc_2 (20, 19);

void setup() {
  analogReadResolution(12);
  Serial.begin(115200);
  for(int i = 0 ; i < 11; i++){
    pinMode(rocker_pins[i], INPUT_PULLUP);
  }
  
  for(int i = 0 ; i < 2; i++){
    pinMode(stick_pins[i], INPUT);
  }
}

void loop() {
  for(int i = 0; i < 2; i++){
    stick_pos[i] = analogRead(stick_pins[i]);
    Serial.print(stick_pos[i]);
    Serial.print("\t");
  }
  Joystick.X(map(stick_pos[0], 0, 4096, 1024, 0));
  Joystick.Y(map(stick_pos[1], 0, 4096, 1024, 0));

  for(int i = 0; i < 11; i++){
    rocker_pos[i] = !digitalRead(rocker_pins[i]);
    Serial.print(rocker_pos[i]);
    Serial.print("\t");
    int button_num = i;
    button_num++;
    Joystick.button(button_num, rocker_pos[i]);
  }
  Serial.println();
  delay(100);

  int enc_1_pos = 7*enc_1.read();
  int enc_2_pos = 7*enc_2.read();
  
  if(enc_1_pos != enc_pos[0]){
    enc_pos[0] = enc_pos[0] + enc_1_pos;
    if(enc_pos[0] > 1024){enc_pos[0] = 1024;}
    if(enc_pos[0] < 0){enc_pos[0] = 0;}
    Joystick.sliderLeft(enc_pos[0]);
    enc_1.write(0);
  }
  if(enc_2_pos != enc_pos[1]){
    enc_pos[1] = enc_pos[1] + enc_2_pos;
    if(enc_pos[1] > 1024){enc_pos[1] = 1024;}
    if(enc_pos[1] < 0){enc_pos[1] = 0;}
    Joystick.sliderRight(enc_pos[1]);
    enc_2.write(0);
  }

}
