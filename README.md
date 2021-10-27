##SingleWireSerial


A software serial library that uses only a single wire for half-duplex serial communication. It makes use of the input capture feature of the 16-bit timers in AVR MCUs and is for this reason highly accurate and reliable up to 125000 bps. So it can be used, for instance, for debugWIRE communication.



### Description

The SingleWireSerial library uses the *input capture* feature of the 16-bit timers in AVR MCUs. Basically, you can time stamp falling or rising edges with it, which allows for a much more accurate way of interpreting an asynchronous signal than using wait loops, provided you want to support bit rates that a reconfigurable during runtime.

However, there is a price to pay. First of all, Timer 1, the 16-bit timer on the ATmega328P, is used for this purpose and for this reason, some of the PWM pins cannot be used for analog output (Arduino pins 9 and 10 on the ATmega328 boards). Second, the Servo library cannot be used, because it uses Timer 1. Third, there is a fixed input pin, namely Arduino pin 8 on ATmega328 boards. Finally, since there is only one pin that can be used for input capture, you cannot have multiple instances of the SingleWireSoftSerial class. 

There exist also support for running the library on other AVR MCUs (see `SingleWireSerial_config.h`). It should work for all MCUs which have a 16-bit timer and the input capture feature, but it is untested.

### Installation

Just copy the directory into the library directory of your Arduino installation.

### Usage

Usage of this library is very similar to the usage of the [`SoftwareSerial`](https://www.arduino.cc/en/Reference/softwareSerial) library. For this reason, I will only describe the differences.

The main difference is that the constructor method has only one optional argument. This argument, called `twoWire`, if set to `true`, enables one to use an ordinary two-wire interface. The TX-line for an ATmega328 is in this case Arduino pin 9. For other MCUs, you find the right RX-pin and TX-pin in `SingleWireSerial_config.h`. Further, the methods [`listen()`](https://www.arduino.cc/en/Reference/SoftwareSerialListen) and [`isListening()`](https://www.arduino.cc/en/Reference/SoftwareSerialIsListening) do not exist because one can define only one instance of the `SingleWireSerial` class.

### Limits

At 125000 bps on 16 MHz MCUs, timing is very tight. If the millis timer interrupt is not disabled, then, in the *worst case*, 128 cycles are spent until the initial interrupt capture is saved and the edge flag is reconfigured so that the next edge can be captured. 128 cycles is just one bit time at 125000 bps! In other words, in this case, we are at the edge of missing an edge. The worst case, by the way, happens when the timer interrupt occurs one cycle before the input capture interrupt is raised, the currently executing instruction needs 4 cycles, in the timer interrupt the if-branch adding the fractional correction is executed, and the instruction after returning from the timer interrupt, which is executed before the capture interrupt is served, needs 4 cycles as well. Although it sounds very unlikely that all these things happen together, it means one works at the limit and small things, such as the communication speed being 1% faster, may throw the input ISR off the rails. I would suggest to disable the timer interrupt (and any other interrupts) when operating at 125000 bps and high communication reliability is required. 







