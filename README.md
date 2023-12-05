# artcad robot test

### Plan B1: готовый PCB-пульт

Сложность: 1/4

Удача: 4/4

Все те же радиомодули, но без проводов на пульте управления. Пульт отправляет на робота значения с джойстика (направление - 0, 1, 2, 3 или 4), тублера (0 или 1), потенциометра (от 0 до 1024), кнопки (0 или 1).
Номер радиоканала: 5.

![245615045-826f48f7-1a43-4177-9969-16ad2d988945](https://github.com/m112521/artcad/assets/85460283/64794495-4f4d-4c0c-aaf0-b312e1e6b7e7)

Пример кода на роботе:

``` c++
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
int direction = 4; 
int speed = 0;
int buttonState = 0;
int tumblrState = 0;
int ptmrVal = 0;

void setup(){
  Serial.begin(9600);
    
  radio.begin();                                        
  radio.setChannel(5);
  radio.setDataRate     (RF24_1MBPS);                   
  radio.setPALevel      (RF24_PA_HIGH);                 
  radio.openReadingPipe (1, 0x1234567890LL);           
  radio.startListening  ();                             

  for (int i = 4; i < 8; i++) {     
    pinMode(i, OUTPUT);
  }
  pinMode(RELAY_PIN, OUTPUT);
}

void loop(){
    if(radio.available()){   
        radio.read(&data, sizeof(data));             

        direction = data[0]; // 0 - если на джойстике Y>520; 1 - если на джойстике Y<450; 2 - если на джойстике X>540; 3 - если на джойстике X<520; 4 - в остальных случаях 
        speed = data[1]; // 0 или 255 в зависимости от положения джойстика (255, если джойстик отклонен значительно от центрального положения)
        buttonState = data[2]; // 0 - не нажата кнопка; 1 - нажата кнопка
        tumblrState = data[3]; // 0 или 1 в зависимости от положения тумблера
        ptmrVal = data[4]; // 0-1024 в зависимости от положения потенциометра

        Serial.println(String(data[0])+ " " + String(data[1])+ " " + String(data[2])+ " " + String(data[3])+ " " + String(data[4])+ " ");  
        //Prints: 3 255 0 1 1024 
       
       
        if (tumblrState) {
          digitalWrite(RELAY_PIN, HIGH);
        }
        else { 
          digitalWrite(RELAY_PIN, LOW);
        }

        if (direction == 0) {
          digitalWrite(DIR_1, HIGH); // направление мотора 1 - вперед
          analogWrite(SPEED_1, speed); 

          digitalWrite(DIR_2, HIGH); // направление мотора 2 - вперед
          analogWrite(SPEED_2, speed);   
        }
        else if (direction == 1) {
          digitalWrite(DIR_1, LOW);  // направление мотора 1 - назад
          analogWrite(SPEED_1, speed); 

          digitalWrite(DIR_2, LOW); // направление мотора 2 - назад
          analogWrite(SPEED_2, speed);           
        }
        else if (direction == 2) {
          digitalWrite(DIR_1, LOW); // направление мотора 1 - назад
          analogWrite(SPEED_1, speed); 

          digitalWrite(DIR_2, HIGH); // направление мотора 2 - вперед
          analogWrite(SPEED_2, speed); 
        }
        else if (direction == 3) {
          digitalWrite(DIR_1, HIGH); // направление мотора 1 - вперед
          analogWrite(SPEED_1, speed); 

          digitalWrite(DIR_2, LOW); // направление мотора 2 - назад
          analogWrite(SPEED_2, speed); 
        }             
        else { // останавливаем моторы
          analogWrite(SPEED_1, 0); 
          analogWrite(SPEED_2, 0);           
        }
    }
}
```



### Plan B2: Raspberry Pi 

Сложность: 1/4

Удача: 2/4

Raspberry Pi подключается к сети Wi-Fi. Питается от пауэрбанка по MicroUSB. 
Arduino Uno подключается к Raspberry Pi по Serial (через провод USB A-B).

По Wi-Fi с клиента (с другого компьютера в этой сети при помощи клавиатуры и кнопок WADS) на Raspberry Pi поступает команда (например, ‘w’, ‘a, ‘d’; ‘s’), а далее передается по Serial на Arduino.

В качестве клиента может выступать:

- VNC - удаленный доступ к Raspberry Pi, где запускается скрипт Python, считывающий нажатие клавиш и отправляющий соответствующую команду на Arduino по Serial. 
- JS WebSocket - на Raspberry Pi поднимается WS-сервер, а клиентов может выступать браузер на другом компе. В браузере открывается страница index.html с кнопками. 

Размеры дополнительных блоков железа (коробка с Raspberry Pi и пауэрбанк; синим обозначено пространство под провода USB A/B и Micro):

![box_size](https://github.com/m112521/artcad/assets/85460283/b7ce1243-107c-43f0-b6d2-4ef28c3de404)


Пример подключения к реальному роботу (1 - пауэрбанк; 2 - микрокомпьютер Rapberry Pi с Wi-Fi; 3 - робот на базе Arduino Uno и Motor Shield):

![Frame 20](https://github.com/m112521/artcad/assets/85460283/9b6c4264-23b7-478a-8713-0fae6668f08c)

[контур и отверстия коробки от Raspberry Pi](https://github.com/m112521/artcad/blob/rpi-rc/CAD/rpibox_footprint.dxf)


Какие сигналы (символы) прилетают на Arduino при нажатии на соответствующую кнопку (первый столбец) на клавиатуре:

| Кнопка на клавиатуре | Символ на Arduino |
|----------------------|-------------------|
| w                    | 0                 |
| d                    | 1                 |
| a                    | 2                 |
| s                    | 3                 |
| x                    | 4                 |
| backspace            | 5                 |
| enter                | 6                 |

Пример обработки сигнала на Arduino для управления направлением движения робота:

```c++
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
```
[VNC viewer для подключения к Raspberry Pi](https://www.realvnc.com/en/connect/download/viewer/)


### Plan B3: ИК-пульт

Сложность: 1/4

Удача: 2/4

К роботу нужно добавить ИК-датчик, а управление осуществлять с ИК-пульта (как от телевизора). 

Преимущества:

- почти самый простой способ;
- много кнопок на пульте.


Недостатки:

- пульт должен быть направлен на датчик;
- низкая скорость отклика.


### Plan B4: пульт управления на проводе

Сложность: 1/4

Удача: 1/4

Преимущества:
самый простой способ;
быстрый отклик;
нет проблем с беспроводной связью.

Недостатки:
провод(а) напрямую подключен к роботу.






