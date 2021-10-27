/*
 SingleWireSerial test

 Receives from the hardware serial, sends to software serial.
 Receives from software serial, sends to hardware serial.

 Note:
 RX/TX is fixed. It is pin 8 for UNO, for other MCUs it varies (see SingleWireSerial_config.h)

 created back in the mists of time
 modified 25 May 2012
 by Tom Igoe
 based on Mikal Hart's example

 This example code is in the public domain.

 */
#include <SingleWireSerial.h>

SingleWireSoftSeria mySerial; // TX

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

