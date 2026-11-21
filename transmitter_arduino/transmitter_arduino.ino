#include <Wire.h>
#include <MPU6050.h>
#include <math.h>

MPU6050 mpu;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("0");
    while (true);
  }
}

void loop() {
  int16_t ax, ay, az, gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  float pitch = ay / 16384.0;
  float roll  = ax / 16384.0;

  int spd = 0;
  String cmd = "S";

  float deadzone = 0.2;

  if (abs(pitch) > abs(roll)) {
    if (pitch > deadzone) {
      spd = constrain((int)(pitch * 400), 0, 255);
      cmd = "F" + String(spd);
    } else if (pitch < -deadzone) {
      spd = constrain((int)(abs(pitch) * 400), 0, 255);
      cmd = "B" + String(spd);
    } else {
      cmd = "S";
    }
  } else {
    if (roll > deadzone) {
      spd = constrain((int)(roll * 400), 0, 255);
      cmd = "R" + String(spd);
    } else if (roll < -deadzone) {
      spd = constrain((int)(abs(roll) * 400), 0, 255);
      cmd = "L" + String(spd);
    } else {
      cmd = "S";
    }
  }

  Serial.println(cmd);
  delay(100);
}