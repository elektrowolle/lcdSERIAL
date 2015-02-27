// lcdTest.ino
#include <SerialLCD.h>
#include <Wire.h>

SerialLCD * lcd;

void setup() {
  Wire.begin();
  Serial.begin(9600);
  Serial.println("Start");

  delay(1000);

  lcd = new SerialLCD();

  lcd->controlDisplay(
    true, //Display on?
    true, //Cursor Visible?
    true  //Cursor blinking?
  );

  lcd->clearDisplay();

  ///add graphics of a bottle and a clock
  unsigned char bottle[] =  {
    0b11111,
    0b01110,
    0b10001,
    0b10001,
    0b10001,
    0b11001,
    0b11111,
    0b01110,
  };

  unsigned char clock[] =  {
    0b00000,
    0b00000,
    0b01110,
    0b10101,
    0b10111,
    0b10001,
    0b01110,
    0b00000,
  };

  lcd->addGraphic(
    1,     //address
    bottle //graphic
    );
  lcd->addGraphic(2, clock);


  //draw the graphics we just put into the lcd
  lcd->writeCharacter(1);
  lcd->writeCharacter(2);

  //write some characters
  lcd->writeCharacter('h');
  lcd->writeCharacter('a');
  lcd->writeCharacter('l');
  lcd->writeCharacter('l');
  lcd->writeCharacter('o');


  //Note that the display won't change at all till here.
  //Nothing was even written. Everything is in a cache
  //so it can be written to the lcd. But you got to shift
  //them out your own as there are  million possible
  //ways of connections.
}

void loop() {
  //Let's assume the lcd is connected by i2c

  //repeat the following till there is no new data
  //in the cache
  while(lcd->isDataReady()){
    //fetch the data to be written next
    unsigned char data = lcd->getData();

    //transmit them by i2c. Assuming that 39 is the
    //address of the i2c chip.
    Wire.beginTransmission(39);
    Wire.write(data);
    Wire.endTransmission();
  }


}


//write data by a serial connection
void serialEvent(){
  //get incoming data
  unsigned char incomingByte = Serial.read();

  // say what you got:
  Serial.print("I received: ");
  Serial.println(incomingByte, HEX);

  //the lcd whitespace is 40 and not 20.
  //Put the characters on the queue
  lcd->writeCharacter(incomingByte == 20  ? 40 : incomingByte);
}