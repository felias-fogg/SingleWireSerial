/*
 SingleWireSerial test

 Receives from the hardware serial, sends to software serial.
 Receives from software serial, sends to hardware serial.

 Note:
 RX/TX is fixed. It is pin 8/9 for UNO, for other MCUs it varies (see SingleWireSerial_config.h).
 If one instantiates the class with the argument false (or no argument at all), it is one-wire
 on pin 8. Then you need a small circuit to join the RX/TX line of the UART you use in order to 
 communicate with the UNO (see my blog post on that: https://hinterm-ziel.de/index.php/2021/10/30/one-line-only/).
 

 created back in the mists of time
 modified 25 May 2012
 by Tom Igoe
 based on Mikal Hart's example

 This example code is in the public domain.

 */
#include <SingleWireSerial.h>

SingleWireSerial mySerial(false); // make it a one-wire instance!

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(19200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.println("Goodnight moon!");

  // set the data rate for the SoftwareSerial port
  mySerial.begin(19200);
  mySerial.println("Hello, world?");
}

void loop() { // run over and over
  if (mySerial.available()) {
    Serial.write(mySerial.read());
  }
  if (Serial.available()) {
    mySerial.write(Serial.read());
  }
}

