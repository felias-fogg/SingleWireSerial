# SingleWireSerial


A software serial library that uses only a single wire for half-duplex serial communication. It makes use of the input capture and output compare match features of the 16-bit timers in AVR MCUs and is for this reason highly accurate and reliable up to 250 kbps. So it can be used, for instance, for debugWIRE communication.



## Description

The SingleWireSerial library uses the *input capture* and the *output compare match* features of the 16-bit timers in AVR MCUs. Basically, you can time stamp falling or rising edges with the input capture feature and can accurately time intervals with the output compare match feature. This allows for a much more accurate way of interpreting an asynchronous signal than using wait loops, provided you want to support bit rates that are reconfigurable during runtime.

However, there is a price to pay. First of all, Timer 1, the 16-bit timer on the ATmega328P, is used for this purpose and for this reason, some of the PWM pins cannot be used for analog output (Arduino pins 9 and 10 on the ATmega328 boards). Second, the Servo library cannot be used, because it uses Timer 1. Third, there is a fixed input pin, namely Arduino pin 8 on ATmega328 boards. Fourth, since there is only one pin that can be used for input capture, you cannot have multiple instances of the SingleWireSerial class. 

There exist also support for running the library on other AVR MCUs (see `SingleWireSerial_config.h`). It should work for all MCUs which have a 16-bit timer and the input capture feature, but it is untested. Furthermore, you can also use it as a two wire library, but the output pin is also fixed (for the Arduino UNO it is pin 9).

## Installation

Just copy the directory into the library directory of your Arduino installation.

## Usage

Usage of this library is very similar to the usage of the [`SoftwareSerial`](https://www.arduino.cc/en/Reference/softwareSerial) library. For this reason, only the differences to it are described here.

The main difference is that the constructor method has only one optional argument. This argument, called `twoWire`, if set to `true`, enables one to use an ordinary two-wire interface. The TX-line for an ATmega328 is in this case Arduino pin 9. For other MCUs, you find the RX-pin and TX-pin in `SingleWireSerial_config.h`. Further, the methods [`listen()`](https://www.arduino.cc/en/Reference/SoftwareSerialListen) and [`isListening()`](https://www.arduino.cc/en/Reference/SoftwareSerialIsListening) do not exist because one can define only one instance of the `SingleWireSerial` class.

## Limits

At 125 kbps at 16 MHz, timing is tight. If the millis timer interrupt is enabled, then, in the *worst case*, [**6.6 µs** are spent in the millis interrupt](https://hinterm-ziel.de/index.php/2021/10/20/timing-the-timekeeper/). In the worst case, it takes **3.8 µs** to set up the machinery in the receiving ISR after a falling edge. The critical point is at **12 µs** after a falling edge, at which we want to sample. So, there is no problem at 125 kbps provided the *only* interrupt is the millis interrupt.

At higher communication rates, e.g., at 250 kbps, the millis interrupt needs to be disabled when one wants to receive bytes reliably. And this goes for other interrupts without saying.

On the lower limit, it should be possible to go as low as 40 bps. However, note that all interrupts are blocked when writing a byte and when receiving a byte.

## Background

If you want to learn more about the background of the library, you might want read this [blog post](https://hinterm-ziel.de/index.php/2021/10/30/one-line-only/).
