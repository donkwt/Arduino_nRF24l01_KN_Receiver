
#include "RF24KNReceiver.h"

RF24KNReceiver receiver;

void setup() {
  Serial.begin();
  delay(7000);

  Serial.println("receiver start");
  receiver.init();
  delay(100);

  Serial.print("searching ...");
  while (!receiver.bind()) {
    delay(1);
  }
  Serial.println(" found!");
  delay(2000);
}

void loop() {
  if (receiver.receive()) {
    Serial.println();
    Serial.print("lx: "); Serial.print(receiver.left_x); Serial.print(",  ");
    Serial.print("ly: "); Serial.print(receiver.left_y); Serial.print(",  ");
    Serial.print("rx: "); Serial.print(receiver.right_x); Serial.print(",  ");
    Serial.print("ry: "); Serial.print(receiver.right_y); Serial.print(",  ");
    Serial.print("th: "); Serial.print(receiver.switch_throttle_hold); Serial.print(",  ");
    Serial.print("lxt: "); Serial.print(receiver.left_x_trim); Serial.print(",  ");
    Serial.print("lyt: "); Serial.print(receiver.left_y_trim); Serial.print(",  ");
    Serial.print("rxt: "); Serial.print(receiver.right_x_trim); Serial.print(",  ");
    Serial.print("ryt: "); Serial.print(receiver.right_y_trim); Serial.print(",  ");
    Serial.print("thh: "); Serial.print(receiver.switch_throttle_hold); Serial.print(",  ");
    Serial.print("idle: "); Serial.print(receiver.switch_idle); Serial.print(",  ");
    Serial.print("dr: "); Serial.print(receiver.switch_dual_rate); Serial.print(",  ");
    Serial.print("3d6g: "); Serial.print(receiver.switch_3d_6g);
  }
  else if (receiver.available()) {
    Serial.print(".");
  }
  else {
    Serial.print("\r\nconnection lost!");
  }
  delay(1);
}