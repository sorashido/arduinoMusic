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

struct IMUStruct{
  int ax;
  int ay;
  int az;
  int gx;
  int gy;
  int gz;
};

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
  static IMUStruct old_imu;

  IMUStruct aveimu; 
//  for(int i=0; i<100; i++){
    imu.read();
    
    aveimu.ax = imu.a.x;
    aveimu.ay = imu.a.y;
    aveimu.az = imu.a.z;
    aveimu.gx = imu.g.x;
    aveimu.gy = imu.g.y;
    aveimu.gz = imu.g.z;
//  }

//  aveimu.ax/=100;
//  aveimu.ay/=100;
//  aveimu.az/=100;
//  aveimu.gx/=100;
//  aveimu.gy/=100;
//  aveimu.gz/=100;

//  if()

  int value=analogRead(sensorPin);

  IMUStruct diff;
  diff.ax = (old_imu.ax - aveimu.ax)^2;
  diff.ay = (old_imu.ay - aveimu.ay)^2;
  diff.az = (old_imu.az - aveimu.az)^2;
  diff.gx = (old_imu.gx - aveimu.gx)^2;
  diff.gy = (old_imu.gy - aveimu.gy)^2;
  diff.gz = (old_imu.gz - aveimu.gz)^2;

  if(diff.ax <200){diff.ax = 0;}
  if(diff.ay <200){diff.ay = 0;}
  if(diff.az <200){diff.az = 0;}
  if(diff.gx <500){diff.gz = 0;}
  if(diff.gy <500){diff.gy = 0;}
  if(diff.gz <500){diff.gz = 0;}
  
  int absDiff = abs(diff.ax)+abs(diff.ay)+abs(diff.az); 

//  snprintf(report, sizeof(report), "D:%6d", //A: %6d %6d %6d    G: %6d %6d %6d",
//    imu.a.x, imu.a.y, imu.a.z,
//    imu.g.x, imu.g.y, imu.g.z,

//    aveimu.ax, aveimu.ay, aveimu.az,
//    aveimu.gx, aveimu.gy, aveimu.gz

//    diff.ax, diff.ay, diff.az,
//    diff.gx, diff.gy, diff.gz
//    absDiff
//    );
//  Serial.println(report);

  static int Acount = 0;
  static int Bcount = 0;
  static int Ccount = 0;

  if(absDiff > 8000){
    Acount += 1;
  }else if(absDiff > 4500){
    Bcount += 1;
  }else if(absDiff > 2500){
    Ccount += 1;
  }else{
    Acount = 0;
    Bcount = 0;
    Ccount = 0;
  }

  if(Acount > 3){
    tone(sound_pin, sounds[0], 200);
    delay(50);
    tone(sound_pin, sounds[7], 200);
    delay(50);
    tone(sound_pin, sounds[0], 200);
    delay(50);
    tone(sound_pin, sounds[7], 200);
    delay(50);
    tone(sound_pin, sounds[0], 200);
    delay(50);
    imu.read();
    Serial.println("1");
    Acount = 0;
    Bcount = 0;
    Ccount = 0;
    delay(300);
  }
  else if(Bcount > 3){
    tone(sound_pin, sounds[7], 300);
    delay(150);
    tone(sound_pin, sounds[6], 300);
    delay(150);
    tone(sound_pin, sounds[5], 300);
    delay(150);
    tone(sound_pin, sounds[4], 300);
    delay(150);
    tone(sound_pin, sounds[3], 300);
    delay(150);
    tone(sound_pin, sounds[2], 300);
    delay(150);
    tone(sound_pin, sounds[1], 300);
    delay(150);
    tone(sound_pin, sounds[0], 300);
    delay(150);  
    Serial.println("2");
    Acount = 0;
    Bcount = 0;
    Ccount = 0;
    delay(300);
//    imu.read();
  }else if(Ccount > 3){
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
    Serial.println("3");
    Acount = 0;
    Bcount = 0;
    Ccount = 0;
    delay(300);
//    imu.read();
  }

  old_imu.ax = aveimu.ax;
  old_imu.ay = aveimu.ay;
  old_imu.az = aveimu.az;
  old_imu.gx = aveimu.gx;
  old_imu.gy = aveimu.gy;
  old_imu.gz = aveimu.gz;

  imu.read();
  
//  if(imu.a.x>5000&&imu.a.y<2000){//&&value>650&&value<660){
//  }
//
//  if(imu.a.x>3000&&imu.a.y>3000){//&&value>650&&value<660){
//  }

//  if(imu.a.x<2000&&imu.a.y>5000){//&&value>650&&value<660){
//  }
}
