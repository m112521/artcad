# artcad robot test

### Plan B1: готовый PCB-пульт

Все те же радиомодули, но без проводов на пульте управления. Пульт отправляет на робота значения с джойстика (X, Y - от 0 до 1024), тублера (0 или 1), потенциометра (от 0 до 1024), кнопки (0 или 1).

![245615045-826f48f7-1a43-4177-9969-16ad2d988945](https://github.com/m112521/artcad/assets/85460283/64794495-4f4d-4c0c-aaf0-b312e1e6b7e7)

Пример кода на роботе:

``` c++
// read from data[]
```





### Plan B2: Raspberry Pi 

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

### Plan B3: ИК-пульт

К роботу нужно добавить ИК-датчик, а управление осуществлять с ИК-пульта (как от телевизора). 

Преимущества:

- почти самый простой способ;
- много кнопок на пульте.


Недостатки:

- пульт должен быть направлен на датчик;
- низкая скорость отклика.



