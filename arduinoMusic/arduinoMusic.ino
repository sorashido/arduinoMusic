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
#include <Kalman.h>

//IMU
LSM6 imu;//生データ
double accX, accY, accZ;
double gyroX, gyroY, gyroZ;

//Filter
//Angle
Kalman kalmanX;
Kalman kalmanY;

double gyroXangle, gyroYangle; // Angle calculate using the gyro only
double compAngleX, compAngleY; // Calculated angle using a complementary filter
double kalAngleX, kalAngleY; // Calculated angle using a Kalman filter

uint32_t timer;


//Velocity
double velX, velY, velZ = 0;
double comAccX,comAccY,comAccZ=0;  
double gravityX, gravityY, gravityZ=0;

const double alpha = 0.8;//lowpassfilter

//Log
char report[80];

//Sound
const int sound_pin = 8;

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

  delay(100);//Wait for sensor to stablize

  imu.read();
  //  Low Pass Filter
  accX = imu.a.x;// * alpha + (imuValue.ax * (1.0 -alpha));
  accY = imu.a.y;// * alpha + (imuValue.ay * (1.0 -alpha));
  accZ = imu.a.z;// * alpha + (imuValue.az * (1.0 -alpha));
  gyroX = imu.g.x;// * alpha + (imuValue.gx * (1.0 -alpha));
  gyroY = imu.g.y;// * alpha + (imuValue.gy * (1.0 -alpha));
  gyroZ = imu.g.z;// * alpha + (imuValue.gz * (1.0 -alpha));

  double roll  = atan2(accY, accZ) * RAD_TO_DEG;
  double pitch = atan(-accX / sqrt(accY * accY + accZ * accZ)) * RAD_TO_DEG;

  kalmanX.setAngle(roll); // Set starting angle
  kalmanY.setAngle(pitch);
  gyroXangle = roll;
  gyroYangle = pitch;
  compAngleX = roll;
  compAngleY = pitch;

  timer = micros();
}

void loop()
{
  imu.read();

  accX = imu.a.x;// * alpha + (imuValue.ax * (1.0 -alpha));
  accY = imu.a.y;// * alpha + (imuValue.ay * (1.0 -alpha));
  accZ = imu.a.z;// * alpha + (imuValue.az * (1.0 -alpha));
  gyroX = imu.g.x;// * alpha + (imuValue.gx * (1.0 -alpha));
  gyroY = imu.g.y;// * alpha + (imuValue.gy * (1.0 -alpha));
  gyroZ = imu.g.z;// * alpha + (imuValue.gz * (1.0 -alpha));

  
  double dt = (double)(micros() - timer) / 1000000; // Calculate delta time
  timer = micros();

  /*角度を求める kalman Filter*/
  double roll  = atan2(accY, accZ) * RAD_TO_DEG;
  double pitch = atan(-accX / sqrt(accY * accY + accZ * accZ)) * RAD_TO_DEG;
  
  double gyroXrate = gyroX / 131.0; // Convert to deg/s
  double gyroYrate = gyroY / 131.0; // Convert to deg/s

  if ((roll < -90 && kalAngleX > 90) || (roll > 90 && kalAngleX < -90)) {
    kalmanX.setAngle(roll);
    compAngleX = roll;
    kalAngleX = roll;
    gyroXangle = roll;
  } else
    kalAngleX = kalmanX.getAngle(roll, gyroXrate, dt); // Calculate the angle using a Kalman filter

  if (abs(kalAngleX) > 90)
    gyroYrate = -gyroYrate; // Invert rate, so it fits the restriced accelerometer reading
  kalAngleY = kalmanY.getAngle(pitch, gyroYrate, dt);

  gyroXangle += gyroXrate * dt; // Calculate gyro angle without any filter
  gyroYangle += gyroYrate * dt;
//  gyroXangle += kalmanX.getRate() * dt; // Calculate gyro angle using the unbiased rate
//  gyroYangle += kalmanY.getRate() * dt;

  compAngleX = 0.93 * (compAngleX + gyroXrate * dt) + 0.07 * roll; // Calculate the angle using a Complimentary filter
  compAngleY = 0.93 * (compAngleY + gyroYrate * dt) + 0.07 * pitch;

  // Reset the gyro angle when it has drifted too much
  if (gyroXangle < -180 || gyroXangle > 180)
    gyroXangle = kalAngleX;
  if (gyroYangle < -180 || gyroYangle > 180)
    gyroYangle = kalAngleY;

  /*速度を求める*/
  // 重力加速度を求める
  gravityX = alpha * gravityX + (1 - alpha) * accX;
  gravityY = alpha * gravityY + (1 - alpha) * accY;
  gravityZ = alpha * gravityZ + (1 - alpha) * accZ;

  // 補正した加速度
  comAccX = accX - gravityX;
  comAccY = accY - gravityY;
  comAccZ = accZ - gravityZ;

  if(abs(comAccX) < 200)comAccX = 0;
  if(abs(comAccY) < 200)comAccY = 0;
  if(abs(comAccZ) < 200)comAccZ = 0;

  velX = velX + comAccX*dt;
  velY = velY + comAccY*dt;
  velZ = velZ + comAccZ*dt;

  if(abs(velX) < 200)velX = 0;
  if(abs(velY) < 200)velY = 0;
  if(abs(velZ) < 200)velZ = 0;
  if(abs(comAccX) < 1)velX = 0;
  if(abs(comAccY) < 1)velY = 0;
  if(abs(comAccZ) < 1)velZ = 0;

  fastSound();

/*print*/
//  Serial.print(velX); Serial.print("\t");
//  Serial.print(velY); Serial.print("\t");
//  Serial.print(velZ); Serial.print("\t");
//  Serial.print(comAccX); Serial.print("\t");
//  Serial.print(comAccY); Serial.print("\t");
//  Serial.print(comAccZ); Serial.print("\t");
    
//  Serial.print(roll); Serial.print("\t");
//  Serial.print(gyroXangle); Serial.print("\t");
//  Serial.print(compAngleX); Serial.print("\t");
//  Serial.print(kalAngleX); Serial.print("\t");
//
//  Serial.print("\t");
//
//  Serial.print(pitch); Serial.print("\t");
//  Serial.print(gyroYangle); Serial.print("\t");
//  Serial.print(compAngleY); Serial.print("\t");
//  Serial.print(kalAngleY); Serial.print("\t");

//  Serial.print("\r\n");

  delay(2);
}

/*comment out*/
//  IMUStruct diff;
//  diff.ax = (old_imu.ax - imuValue.ax);
//  diff.ay = (old_imu.ay - imuValue.ay);
//  diff.az = (old_imu.az - imuValue.az);
//  diff.gx = (old_imu.gx - imuValue.gx);
//  diff.gy = (old_imu.gy - imuValue.gy);
//  diff.gz = (old_imu.gz - imuValue.gz);
//  if(diff.ax <10){diff.ax = 0;}
//  if(diff.ay <10){diff.ay = 0;}
//  if(diff.az <10){diff.az = 0;}
//  if(diff.gx <10){diff.gz = 0;}
//  if(diff.gy <10){diff.gy = 0;}
//  if(diff.gz <10){diff.gz = 0;}  

//  int absDiff = abs(diff.ax)+abs(diff.ay)+abs(diff.az); 
//  snprintf(report, sizeof(report), "A: %d %d %d    G: %d %d %d",
////    imu.a.x, imu.a.y, imu.a.z,
////    imu.g.x, imu.g.y, imu.g.z    
//
//    imuValue.ax, imuValue.ay, imuValue.az,
//    imuValue.gx, imuValue.gy, imuValue.gz
////
////    diff.ax, diff.ay, diff.az,
////    diff.gx, diff.gy, diff.gz
////    absDiff
//    );
//  Serial.println(report);

