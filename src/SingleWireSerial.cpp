/*
  SingleWireSerial.h - A software serial library that uses only
  one wire to connect two systems in half-duplex mode. In addition,
  it uses timer 1 and its input capture feature in order to support high bit rates.
  It is loosely based on SoftwareSerial, but uses a completely
  different method for reading and writing. 

  Since one usually has only one input capture pin, it does not make
  sense to have more than one instance. 

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

*/

// When _DEBUG  == 1, then some bit pulses are sent to all PORTC-pins (i.e.,
// analog and I2C pins. The on/off toggle needs 2 cycles, but may, of course,
// disturb the timing a bit. When _LOGDEBUG == 1, some info is printed
// using the ordinary Serial connection (if it is open).
#define _DEBUG 1
#define _LOGDEBUG 0
#define _NAKED_ISR 1
// 
// Includes
// 
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <Arduino.h>
#include <SingleWireSerial.h>
#include <util/delay_basic.h>

// Statics
//
bool SingleWireSerial::_twoWire;
uint16_t SingleWireSerial::_bitDelay;
uint16_t SingleWireSerial::_oneAndAHalfBitDelay;
uint16_t SingleWireSerial::_endOfByte;

uint8_t SingleWireSerial::_buffer_overflow;
uint8_t SingleWireSerial::_setICfalling;
uint8_t SingleWireSerial::_setICrising asm("setRisingEdge") __attribute__ ((used));
uint8_t SingleWireSerial::_setCTC;

uint8_t SingleWireSerial::_receive_buffer[_SS_MAX_RX_BUFF]; 
volatile uint8_t SingleWireSerial::_receive_buffer_tail = 0;
volatile uint8_t SingleWireSerial::_receive_buffer_head = 0;

//
// Debugging
//
// This function generates a brief pulse
// for debugging or measuring on an oscilloscope.
#if _DEBUG
inline __attribute__ ((always_inline)) void DebugPulse(byte signal)
{
  PORTC |= signal;
  PORTC &= ~signal;
}
#else
inline __attribute__ ((always_inline)) void DebugPulse(__attribute__ ((unused)) byte signal) {}
#endif

//
// Interrupt handling
//

/* static */
inline __attribute__ ((always_inline)) void SingleWireSerial::handle_interrupt()
{
  uint8_t ch, state;
  uint16_t elapsed,  advanced; // read input capture register
  uint16_t start;
  byte * bufptr;

#if _NAKED_ISR
  // one only has to save SREG, r0-r1, r18-r27, 30-31
  // since only these are the registers that are
  // used in a C-routine without restoring them
  asm volatile("push r22 \n\t"
	       "push r23 \n\t"
	       "lds r22, setRisingEdge ; load code for raising edge \n\t"
	       "sts %[TCCRBaddr], r22 ; write to control register \n\t"
	       "lds  r22, %A[ICRaddr] ; load ICR low byte\n\t"
	       "lds  r23, %B[ICRaddr] ; load ICR high byte\n\t"
#ifdef _DEBUG
	       "sbi %[PORTCaddr], 0\n\t"
	       "cbi %[PORTCaddr], 0\n\t"
#endif
	       "push r0 ; save temp reg\n\t"
	       "in r0, __SREG__ ; save status reg\n\t"
	       "push r0\n\t"
	       "push r1\n\t"
	       "eor r1, r1 ; clear r1\n\t"
	       "push r18\n\t"
	       "push r19\n\t"
	       "push r20\n\t"
	       "push r21\n\t"
	       "push r24\n\t"
	       "push r25\n\t"
	       "push r26\n\t"
	       "push r27\n\t"
	       "push r30\n\t"
	       "push r31\n\t"
	       "movw %[START], r22\n\t"
	       : [START] "=d" (start)
	       : [ICRaddr] "M" (&ICR),
		 [TCCRBaddr] "M" (&TCCRB),
		 [EDGEUP] "M" (_BV(ICES)),
		 [PORTCaddr] "I" (_SFR_IO_ADDR(PORTC))
	       );
#else
  DebugPulse(0x01);
  start = ICR;
  TCCRB |= _setICrising; // set edge detector to raising edge
#endif
  setRxIntMsk(false); // disable the ICR interrupts
  ch = 0;
  state = 0;
  advanced = _oneAndAHalfBitDelay;

  // tail points to where byte goes
  bufptr = &_receive_buffer[_receive_buffer_tail];
  // if buffer full, set the overflow flag
  uint8_t next = (_receive_buffer_tail + 1) % _SS_MAX_RX_BUFF;
  if (next != _receive_buffer_head)
    {
      // that is where next byte shall go
      _receive_buffer_tail = next;
    } 
  else 
    {
      DebugPulse(0x08);
      _buffer_overflow = true;
    }
  DebugPulse(0x01);
  
  while (advanced <= _endOfByte) {
    if (TIFR & _BV(ICF)) { // capture flag has been set
      DebugPulse(0x04);
      TIFR |= _BV(ICF); // clear flag
      TCCRB ^= _BV(ICES); // toggle edge detector;
      state ^= 0x80;
    }
    elapsed = TCNT - start;  // 16 bit unsigned arithmetic gives correct duration
    if (elapsed > advanced) { 
      ch >>=1;
      ch |= state;
      advanced = advanced + _bitDelay;
    }
  }

  *bufptr = ch; // save new byte
  TCCRB &= ~_BV(ICES); // set edge detector to falling edge

  setRxIntMsk(true); // and enable input capture interrupt again
  DebugPulse(0x02);
#if _NAKED_ISR
    asm volatile(
	       "pop r31\n\t"
	       "pop r30\n\t"
	       "pop r27\n\t"
	       "pop r26\n\t"
	       "pop r25\n\t"
	       "pop r24\n\t"
	       "pop r21\n\t"
	       "pop r20\n\t"
	       "pop r19\n\t"
	       "pop r18\n\t"
	       "pop r1\n\t"
	       "pop r0\n\t"
	       "out __SREG__, r0\n\t"
	       "pop r0\n\t"
	       "pop r23\n\t"
	       "pop r22 \n\t"
#ifdef _DEBUG
	       "sbi %[PORTCaddr], 0\n\t"
	       "cbi %[PORTCaddr], 0\n\t"
#endif
	       "reti"
	       :
	       : [PORTCaddr] "I" (_SFR_IO_ADDR(PORTC))
	       );
#endif
}

#if _NAKED_ISR 
ISR(TIMER_CAPT_vect, ISR_NAKED)
#else
ISR(TIMER_CAPT_vect)
#endif   
{
  SingleWireSerial::handle_interrupt();
}

//
// Constructor
//
SingleWireSerial::SingleWireSerial(bool twoWire)
{
  _twoWire = twoWire;
  _buffer_overflow = false;
  pinMode(ICArduinoPin, INPUT);
  if (twoWire) {
    digitalWrite(OCArduinoPin, HIGH);
    pinMode(OCArduinoPin, OUTPUT);
  }
}

//
// Destructor
//
SingleWireSerial::~SingleWireSerial()
{
  end();
}

inline __attribute__ ((always_inline))void SingleWireSerial::setRxIntMsk(bool enable)
{
  if (enable) {
    TCCRB = _setICfalling; // look for falling edge of start bit
    TIFR |= _BV(ICF); // clear input capture flag
    TIMSK |= _BV(ICIE); // enable interrupt
  } else
    TIMSK &= ~_BV(ICIE); // disable input capture interrupt
}

//
// Public methods
//

void SingleWireSerial::begin(long speed)
{
  
  // Precalculate the various delays
  uint32_t bit_delay100 = (F_CPU*100 / speed);
  uint8_t prescaler;

  if (bit_delay100 > 200000UL) {
    bit_delay100 = bit_delay100/8;
    prescaler = _BV(CS1); // prescaler = 8
  } else {
    prescaler = _BV(CS0); // prescaler = 1
  }
  _setICfalling = _BV(ICNC) | prescaler;
  _setICrising = _setICfalling | _BV(ICES);
  _setCTC = _BV(WGM2) | prescaler;

  _bitDelay = (bit_delay100+50)/100; // bit delay time in timer1 ticks
  _oneAndAHalfBitDelay = (bit_delay100+bit_delay100/2+50)/100; // delay until first sample time point
  _endOfByte = _oneAndAHalfBitDelay + (7*_bitDelay); // last sample timepoint 

#if _LOGDEBUG
  Serial.print(F("bit_delay100="));
  Serial.println(bit_delay100);
  Serial.print(F("prescaler="));
  Serial.println(prescaler);
  Serial.print(F("_bitDelay="));
  Serial.println(_bitDelay);
  Serial.print(F("_oneAndAHalfBitDelay="));
  Serial.println(_oneAndAHalfBitDelay);
  Serial.print(F("_endOfByte="));
  Serial.println(_endOfByte);
  Serial.print(F("_setCTC="));
  Serial.println(_setCTC,BIN);
#endif
  TCCRA = 0;
  TCCRC = 0;

  setRxIntMsk(true);

#if _DEBUG
  DDRC |= 0x3F;
#endif

}

void SingleWireSerial::end()
{
  _receive_buffer_tail = _receive_buffer_head;
  setRxIntMsk(false);
}


// Read data from buffer
int SingleWireSerial::read()
{
  // Empty buffer?
  if (_receive_buffer_head == _receive_buffer_tail)
    return -1;

  // Read from "head"
  uint8_t d = _receive_buffer[_receive_buffer_head]; // grab next byte
  _receive_buffer_head = (_receive_buffer_head + 1) % _SS_MAX_RX_BUFF;
  return d;
}

int SingleWireSerial::available()
{
  return ((unsigned int)(_SS_MAX_RX_BUFF + _receive_buffer_head - _receive_buffer_tail)) % _SS_MAX_RX_BUFF;
}

size_t SingleWireSerial::write(uint8_t ch)
{
#if _LOGDEBUG
  //Serial.print(F("char="));
  //Serial.write(b);
  //Serial.println();
#endif
  uint8_t oldSREG = SREG;

  setRxIntMsk(false);
  TCCRA = 0;
  TCCRC = 0;
  TCCRB = _setCTC;
  OCRA = _bitDelay-1;

  cli(); // interrupts off

  TCNT = 0;
  TIFR |= _BV(OCFA);
  DebugPulse(0x01);
  
  if (!_twoWire) {
    TCNT = 0;
    ICDDR |= _BV(ICPIN);   // startbit
    for (uint8_t i = 8; i > 0; --i) {
      while (!(TIFR & _BV(OCFA)));
      if (ch & 1)
	ICDDR &= ~_BV(ICPIN); // make output high-impedance
      else
	ICDDR |= _BV(ICPIN); // pull-down
      TIFR |= _BV(OCFA);
      ch >>= 1;
    }
    while (!(TIFR & _BV(OCFA)));
    ICDDR &= ~_BV(ICPIN); // make output again high-impedance for stop bit
  } else { // twoWire!
    TCNT = 0;
    OCPORT &= ~_BV(OCPIN);  // startbit
    for (uint8_t i = 8; i > 0; --i) {
      while (!(TIFR & _BV(OCFA)));
      //      DebugPulse();
      if (ch & 1)
	OCPORT |= _BV(OCPIN); // make output high
      else
	OCPORT &= ~_BV(OCPIN); // make output low
      TIFR |= _BV(OCFA);
      ch >>= 1;
    }
    while (!(TIFR & _BV(OCFA)));
    OCPORT |= _BV(OCPIN); // make output again high for stop bit
  }
  TIFR |= _BV(OCFA); // clear overflow flag

  SREG = oldSREG; // enable interrupts again

  while (!(TIFR & _BV(OCFA))); // wait for stop bit to finish
  setRxIntMsk(true); //enable input capture input interrupts again
  DebugPulse(0x01);
  return 1;
}

void SingleWireSerial::flush()
{
  // There is no tx buffering, simply return
}

int SingleWireSerial::peek()
{
  // Empty buffer?
  if (_receive_buffer_head == _receive_buffer_tail)
    return -1;

  // Read from "head"
  return _receive_buffer[_receive_buffer_head];
}
