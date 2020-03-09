/*
  Advanced_I2C.ino
  Brian R Taylor
  brian.taylor@bolderflight.com

  Copyright (c) 2017 Bolder Flight Systems

  Permission is hereby granted, free of charge, to any person obtaining a copy of this software
  and associated documentation files (the "Software"), to deal in the Software without restriction,
  including without limitation the rights to use, copy, modify, merge, publish, distribute,
  sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all copies or
  substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
  BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "MPU9250.h"

// an MPU9250 object with the MPU-9250 sensor on I2C bus 0 with address 0x68
MPU9250 IMU(Wire, 0x68);
int status;

void setup() {
  // serial to display data
  Serial.begin(115200);
  while (!Serial) {}

  // start communication with IMU
  status = IMU.begin();
  if (status < 0) {
    Serial.println("IMU initialization unsuccessful");
    Serial.println("Check IMU wiring or try cycling power");
    Serial.print("Status: ");
    Serial.println(status);
    while (1) {}
  }
  // setting the accelerometer full scale range to +/-8G
  IMU.setAccelRange(MPU9250::ACCEL_RANGE_2G);
  // setting the gyroscope full scale range to +/-500 deg/s
  IMU.setGyroRange(MPU9250::GYRO_RANGE_500DPS);
  // setting DLPF bandwidth to 20 Hz
  IMU.setDlpfBandwidth(MPU9250::DLPF_BANDWIDTH_20HZ);
  // setting SRD to 19 for a 50 Hz update rate
  IMU.setSrd(19);
}

float pre_getAccelX_mss = 0;
float pre_getAccelY_mss = 0;
float pre_getAccelZ_mss = 0;

float new_getAccelX_mss = 0;
float new_getAccelY_mss = 0;
float new_getAccelZ_mss = 0;

float dx = 0;
float dy = 0;
float dz = 0;

float g_rms = 0;

void loop() {
  // read the sensor
  IMU.readSensor();

  // display the data
  new_getAccelX_mss = IMU.getAccelX_mss();
  new_getAccelY_mss = IMU.getAccelY_mss();
  new_getAccelZ_mss = IMU.getAccelZ_mss();

  dx = new_getAccelX_mss - pre_getAccelX_mss;
  dy = new_getAccelY_mss - pre_getAccelY_mss;
  dz = new_getAccelZ_mss - pre_getAccelZ_mss;

  g_rms = sqrt(dx *dx + dy *dy + dz *dz);

//  Serial.print(dx, 6);
//  Serial.print("\t");
//  Serial.print(dy, 6);
//  Serial.print("\t");
//  Serial.print(dz, 6);
//  Serial.print("\t");
  Serial.println(0.5*g_rms*1000*0.01*0.01,6);
  
  delay(20);

  pre_getAccelX_mss = new_getAccelX_mss;
  pre_getAccelY_mss = new_getAccelY_mss;
  pre_getAccelZ_mss = new_getAccelZ_mss;

}
