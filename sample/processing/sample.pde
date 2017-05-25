/*sample3*/
//import processing.serial.*;
//Serial myPort;
//int numFrames = 2;
//int currentFrame = 0;
//PImage[] images = new PImage[numFrames];

//void setup(){
//  size(400, 400);
//  images[0] = loadImage("images/01.png");
//  images[1] = loadImage("images/02.png"); 
//  myPort = new Serial(this, Serial.list()[3], 9600);
//}

//void draw(){
//  background(255);
//  if(myPort.available() > 0){
//    currentFrame = myPort.read();
//  }
//  image(images[currentFrame], width/2 - images[currentFrame].width/2, height/2 -images[currentFrame].height/2);
//}

/*sample2*/
import processing.serial.*;
Serial myPort;
import processing.sound.*;
SoundFile file;
int flag;

void setup(){
  size(400, 400);
  myPort = new Serial(this, Serial.list()[3], 9600);
  file = new SoundFile(this, "/Users/tela/Documents/Processing/sample/music/1.aif");
  flag = 0;
}

void draw(){
  background(255);
  if(myPort.available() > 0){
    flag = myPort.read();
  }
  if(flag == 1){
    fill(0);
    file.play();
    flag = 0;
  }else{
    fill(200);
  }
  rect(50,50,100,100);
}

/*sample1*/
//import processing.serial.*;
//Serial myPort;
//void setup(){
//  size(400, 400);
//  myPort = new Serial(this, Serial.list()[3], 9600);
//}
//void draw(){
//  background(255);
//  if(myPort.available() > 0){
//    if(myPort.read()==0) fill(0); else fill(200);
//  }
//  rect(50,50,100,100);
//}