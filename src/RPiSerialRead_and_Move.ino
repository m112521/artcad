#define SPEED_1      5 
#define DIR_1        4
 
#define SPEED_2      6
#define DIR_2        7

int command = '0';
bool weaponState = false;

void setup(){
  Serial.begin(9600);

  for (int i = 4; i < 8; i++) {     
    pinMode(i, OUTPUT);
  }
}

void loop(){
   if (Serial.available() > 0) {
      command = (Serial.read()); 
      switch (command) {
        case '0': {
          digitalWrite(DIR_1, LOW); // set direction
          analogWrite(SPEED_1, 255); // set speed

          digitalWrite(DIR_2, LOW); // set direction
          analogWrite(SPEED_2, 255); // set speed

          break;
        }
        case '3': {
          digitalWrite(DIR_1, HIGH); 
          analogWrite(SPEED_1, 255); 

          digitalWrite(DIR_2, HIGH); 
          analogWrite(SPEED_2, 255); 

          break;
        }
        case '1': { 
          digitalWrite(DIR_1, HIGH); 
          analogWrite(SPEED_1, 255);

          digitalWrite(DIR_2, LOW); 
          analogWrite(SPEED_2, 255); 

          break;
        }
        case '2': { 
          digitalWrite(DIR_1, LOW); 
          analogWrite(SPEED_1, 255); 

          digitalWrite(DIR_2, HIGH); 
          analogWrite(SPEED_2, 255);
          
          break;
        }
        case '4': { // stop motors
          analogWrite(SPEED_1, 0); 
          analogWrite(SPEED_2, 0);  
          break;
        }
        case '6': {
          // turn on/off orudie here
          weaponState = !weaponState;
        }
      }
  }
}