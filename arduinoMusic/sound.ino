
//Sound
int sounds[] = {262, 294, 330, 349, 392, 440, 494, 523};

/*音階*/
#define l_do sounds[0]
#define re sounds[1]
#define mi sounds[2]
#define fa sounds[3]
#define sol sounds[4]
#define la sounds[5]
#define si sounds[6]
#define h_do sounds[7]

void sound1(int speed){
    tone(sound_pin, la, 300);
    delay(speed);
    tone(sound_pin, mi, 300);
    delay(speed);
    tone(sound_pin, fa, 300);
    delay(speed);
    tone(sound_pin, mi, 300);
    delay(speed);
    tone(sound_pin, l_do, 300);
    delay(speed);
    tone(sound_pin, re, 300);
    delay(speed);
    tone(sound_pin, si, 300);
    delay(speed);
}

void sound2(int speed){
    tone(sound_pin, mi, 300);
    delay(speed);
    delay(speed/2);
    tone(sound_pin, re, 300);
    delay(speed);
    tone(sound_pin, l_do, 300);
    delay(speed);
    tone(sound_pin, re, 300);
    delay(speed);
    delay(speed/2);
    tone(sound_pin, l_do, 300);
    delay(speed);
    tone(sound_pin, si, 300);
    delay(speed);
    tone(sound_pin, l_do, 300);
    delay(speed);
    tone(sound_pin, si, 300);
    delay(speed);
    tone(sound_pin, sol, 300);
    delay(speed);
    tone(sound_pin, sol, 300);
    delay(speed);
    tone(sound_pin, la, 300);
    delay(speed);    
}

void sound3(int speed){
    tone(sound_pin, la, 300);
    delay(speed);
    delay(speed/2);
    tone(sound_pin, la, 300);
    delay(speed);
    tone(sound_pin, si, 300);
    delay(speed);
    tone(sound_pin, h_do, 300);
    delay(speed);
    tone(sound_pin, re, 300);
    delay(speed);
    tone(sound_pin, sol, 300);
    delay(speed);
    delay(speed/2);
    tone(sound_pin, mi, 300);
    delay(speed);
    tone(sound_pin, re, 300);
    delay(speed);
    tone(sound_pin, h_do, 300);
    delay(speed);
    tone(sound_pin, si, 300);
    delay(speed);
}

void sound4(int speed){
    tone(sound_pin, la, 300);
    delay(speed);
    tone(sound_pin, la, 300);
    delay(speed);
    tone(sound_pin, l_do, 300);
    delay(speed);
    tone(sound_pin, re, 300);
    delay(speed);
    tone(sound_pin, mi, 300);
    delay(speed);
    tone(sound_pin, mi, 300);
    delay(speed);
    tone(sound_pin, fa, 300);
    delay(speed);
    tone(sound_pin, mi, 300);
    delay(speed);
}


//void baseSound(){
//  
//}

