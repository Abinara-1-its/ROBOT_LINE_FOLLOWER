//mendefinisikan pin yang digunakan untuk control pin
int IN_1 = 4;
int IN_2 = 5;
int IN_3 = 4;
int IN_4 = 5;

// hcsr04 defined pin 
#include <NewPing.h> // hcsr04
#define TRIGGER_PIN  13  // Hubungkan Pin TRIG Sensor Ultrasonic pada Pin A0 Arduino
#define ECHO_PIN     12  // Hubungkan Pin TRIG Sensor Ultrasonic pada Pin A1 Arduino
#define MAX_DISTANCE 100 // Setting jarak maksimum yang kita inginkan, biasanya maksimal sensor HC-SR04 adalah 5 Meter
NewPing sonar1(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
int hcsr1;

void maju(){
  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
}

void mudur(){
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
}
void setup() {
  // put your setup code here, to run once:
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(IN_3, OUTPUT);
  pinMode(IN_4, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:

 hcsr1 = sonar1.ping_cm();
 if(hcsr1 != 0 && hcsr1 < 5){
  maju();
 }


}
