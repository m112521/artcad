#include <SPI.h>                                         
#include <nRF24L01.h>                                    
#include <RF24.h>        

#define SPEED_1      5 
#define DIR_1        4
 
#define SPEED_2      6
#define DIR_2        7

#define RELAY_PIN 3


RF24 radio(8, 9); // nRF24L01+ (CE, CSN)
int data[5];  // создаем массив для приема данных по радиоканалу

// Создаем переменные для хранения значений с джойстика, тумблера и потенциометра
int xPosition = 0; 
int yPosition = 0;
int tumblrState = 0;
int ptmrVal = 0;

// Переменная для храения значения скорости вращения моторов
int speed = 0;

void setup(){
  Serial.begin(9600);
    
  radio.begin();                                        
  radio.setChannel(5);                                  // Указываем канал приёма данных (от 0 до 127), 5 - значит приём данных осуществляется на частоте 2,405 ГГц (на одном канале может быть только 1 приёмник и до 6 передатчиков)
  radio.setDataRate     (RF24_1MBPS);                   // Указываем скорость передачи данных (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS), RF24_1MBPS - 1Мбит/сек
  radio.setPALevel      (RF24_PA_HIGH);                 // Указываем мощность передатчика (RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_HIGH=-6dBm, RF24_PA_MAX=0dBm)
  radio.openReadingPipe (1, 0x1234567890LL);            // Открываем 1 трубу с идентификатором 0x1234567890 для приема данных (на одном канале может быть открыто до 6 разных труб, которые должны отличаться только последним байтом идентификатора)
  radio.startListening  ();                             // Включаем приемник, начинаем прослушивать открытую трубу

  // Устанавливаем режим работы пинов 
  for (int i = 4; i < 8; i++) {     
    pinMode(i, OUTPUT);
  }
  pinMode(RELAY_PIN, OUTPUT);
}

void loop(){
    // Если есть соединение с джойстиком, то ...
    if(radio.available()){   
        // ..ю то считываем из радиоканала значения с джойстика, тумблера и потенциометра - они хранятся в массиве data                             
        radio.read(&data, sizeof(data));             

        // Сохраняем значения из массива в переменные (чтобы не запутаться с индексами массива)
        xPosition = data[1];
        yPosition = data[2];
        tumblrState = data[3];
        ptmrVal = data[4];
        speed = map(ptmrVal, 0, 1023, 0, 255); // приводим значения с потенциометра к диапазаону [0,255], так как 255 - макс значение скорости для мотора 

        // Если значение на тумблере 1, то включаем реле и запускаем мотор орудия
        if (tumblrState) {
          digitalWrite(RELAY_PIN, HIGH);
        }
        else { // Если значение на тумблере 0, то выключаем реле и мотор
          digitalWrite(RELAY_PIN, LOW);
        }

        // Управление направлением вращения колес в зависимости от положения джойстика на пульте управления
        if (xPosition > 550) {
          digitalWrite(DIR_1, HIGH); // направление мотора 1 - вперед
          analogWrite(SPEED_1, speed); // скорость speed зависит от значения на потенциометре

          digitalWrite(DIR_2, HIGH); // направление мотора 2 - вперед
          analogWrite(SPEED_2, speed);   
        }
        else if (xPosition < 500) {
          digitalWrite(DIR_1, LOW);  // направление мотора 1 - назад
          analogWrite(SPEED_1, speed); 

          digitalWrite(DIR_2, LOW); // направление мотора 2 - назад
          analogWrite(SPEED_2, speed);           
        }
        else if (yPosition > 700) {
          digitalWrite(DIR_1, LOW); // направление мотора 1 - назад
          analogWrite(SPEED_1, speed); 

          digitalWrite(DIR_2, HIGH); // направление мотора 2 - вперед
          analogWrite(SPEED_2, speed); 
        }
        else if (yPosition < 300) {
          digitalWrite(DIR_1, HIGH); // направление мотора 1 - вперед
          analogWrite(SPEED_1, speed); 

          digitalWrite(DIR_2, LOW); // направление мотора 2 - назад
          analogWrite(SPEED_2, speed); 
        }             
        else { // останавливаем моторы
          analogWrite(SPEED_1, 0); 
          analogWrite(SPEED_2, 0);           
        }
      
        //Serial.println(tumblrState);
    }
}