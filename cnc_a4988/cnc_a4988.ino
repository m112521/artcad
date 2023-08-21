#include <AccelStepper.h>

AccelStepper Xaxis(1, 2, 5); // pin 2 = step, pin 5 = direction // Y: step=3, dir=6; // Z: step=4 dir=7
AccelStepper Yaxis(1, 3, 6);

const byte enablePin = 8;

void setup()
{
   pinMode(enablePin, OUTPUT);
   digitalWrite(enablePin, LOW);

   Xaxis.setMaxSpeed(12800);
   Xaxis.setSpeed(1000); 

   Yaxis.setMaxSpeed(12800);
   Yaxis.setSpeed(1000);
}

void loop()
{
   Xaxis.runSpeed();
   Yaxis.runSpeed();
}