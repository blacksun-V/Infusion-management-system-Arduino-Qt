#define buzzer speakerPin
#include <Servo.h>
int speakerPin = 2;
int length = 15; // the number of notes
char notes[] = "ccggaagffeeddc "; // a space represents a rest
int beats[] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 };
int tempo = 300;
Servo myServo;
void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(tone);
  }
}
  
void playNote(char note, int duration) {
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };
  
  // play the tone corresponding to the note name
  for (int i = 0; i < 8; i++) {
    if (names[i] == note) {
      playTone(tones[i], duration);
    }
  }
}

void setup() {
      // put your setup code here, to run once:
    Serial.begin(9600);
    pinMode(buzzer,OUTPUT);
    digitalWrite(buzzer,LOW);
    myServo.attach(3);
    myServo.write(60);
    pinMode(21,INPUT_PULLUP);
    attachInterrupt(2,interrput,FALLING);
}

void loop() {
  //sing();
  // put your main code here, to run repeatedly:
    if(Serial.available())
    {
         char Flag=Serial.read();
         Serial.flush();
         Serial.println(Flag);
         if(Flag=='E')
         {
                for(int i=4;i>0;i--)
                sing();
          }
          myServo.write(0);
    }
}

void sing()
{
    for (int i = 0; i < length; i++) {
        if (notes[i] == ' ') {
          delay(beats[i] * tempo); // rest
        } else {
          playNote(notes[i], beats[i] * tempo);
        }
        // pause between notes
        delay(tempo / 2);
        myServo.write(0);
    }
}
void(* resetFunc) (void) = 0; 
void interrput()
{
  Serial.println("中断函数");
  resetFunc();
}
