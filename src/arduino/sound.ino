/*
The sensor outputs provided by the library are the raw
16-bit values obtained by concatenating the 8-bit high and
low accelerometer and gyro data registers. They can be
converted to units of g and dps (degrees per second) using
the conversion factors specified in the datasheet for your
particular device and full scale setting (gain).

Example: An LSM6DS33 gives an accelerometer Z axis reading
of 16276 with its default full scale setting of +/- 2 g. The
LA_So specification in the LSM6DS33 datasheet (page 11)
states a conversion factor of 0.061 mg/LSB (least
significant bit) at this FS setting, so the raw reading of
16276 corresponds to 16276 * 0.061 = 992.8 mg = 0.9928 g.
*/

#include <Wire.h>
#include <LSM6.h>

LSM6 imu;
char report[80];

const int sound_pin = 8;
int sounds[] = {262, 294, 330, 349, 392, 440, 494, 523};
int v = 0;

int sensorPin=0;

void setup()
{
  Serial.begin(9600);
  Wire.begin();

  if (!imu.init())
  {
    Serial.println("Failed to detect and initialize IMU!");
    while (1);
  }
  imu.enableDefault();

  pinMode(sound_pin, OUTPUT);
}

void loop()
{
  imu.read();

  snprintf(report, sizeof(report), "A: %6d %6d %6d    G: %6d %6d %6d",
    imu.a.x, imu.a.y, imu.a.z,
    imu.g.x, imu.g.y, imu.g.z);
  Serial.println(report);

  int value=analogRead(sensorPin);


  if(imu.a.x>5000&&imu.a.y<2000&&value>650&&value<660){
    tone(sound_pin, sounds[0], 300);
    delay(300);
    tone(sound_pin, sounds[1], 300);
    delay(300);
    tone(sound_pin, sounds[2], 300);
    delay(300);
    tone(sound_pin, sounds[3], 300);
    delay(300);
    tone(sound_pin, sounds[4], 300);
    delay(300);
    tone(sound_pin, sounds[5], 300);
    delay(300);
    tone(sound_pin, sounds[6], 300);
    delay(300);
    tone(sound_pin, sounds[7], 300);
    delay(300);
  }

  if(imu.a.x>3000&&imu.a.y>3000&&value>650&&value<660){
    tone(sound_pin, sounds[0], 200);
    delay(190);
    tone(sound_pin, sounds[7], 200);
    delay(190);
    tone(sound_pin, sounds[0], 200);
    delay(190);
    tone(sound_pin, sounds[7], 200);
    delay(190);
    tone(sound_pin, sounds[0], 200);
    delay(190);
  }

  if(imu.a.x<2000&&imu.a.y>5000&&value>650&&value<660){
    tone(sound_pin, sounds[7], 300);
    delay(300);
    tone(sound_pin, sounds[6], 300);
    delay(300);
    tone(sound_pin, sounds[5], 300);
    delay(300);
    tone(sound_pin, sounds[4], 300);
    delay(300);
    tone(sound_pin, sounds[3], 300);
    delay(300);
    tone(sound_pin, sounds[2], 300);
    delay(300);
    tone(sound_pin, sounds[1], 300);
    delay(300);
    tone(sound_pin, sounds[0], 300);
    delay(300);
  }
  
}
