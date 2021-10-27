#ifndef SingleWireSerial_config_h
#define SingleWireSerial_config_h

// Which timer to use. On some MCUs there is more than one 16-bit counter.
// Default is Timer1. 
// Note that each timer posseses a different input capture pin, which is used
// as the single wire serial line.
#define ICTIMER 1

#if (ICTIMER == 0)
  #define TCCRA TCCR0A
  #define TCCRB TCCR0B
  #define TCCRC TCCR0C
  #define TCNT TCNT0
  #define OCRA OCR0A
  #define ICR  ICR0
  #define ICNC ICNC0
  #define TIMSK TIMSK0
  #define TIFR  TIFR0
  #define ICF   ICF0
  #define ICIE  ICIE0
  #define OCFA  OCF0A
  #define CS1   CS01
  #define CS0   CS00
  #define ICES  ICES0
  #define WGM2  WGM02
  #define WGM3  WGM03
  #define TIMER_CAPT_vect TIMER0_CAPT_vect
#elif (ICTIMER == 1)
  #define TCCRA TCCR1A
  #define TCCRB TCCR1B
  #define TCCRC TCCR1C
  #define TCNT TCNT1
  #define OCRA OCR1A
  #define ICR  ICR1
  #define ICNC ICNC1
  #define TIMSK TIMSK1
  #define TIFR  TIFR1
  #define ICF   ICF1
  #define ICIE  ICIE1
  #define OCFA  OCF1A
  #define CS1   CS11
  #define CS0   CS10
  #define ICES  ICES1
  #define WGM2  WGM12
  #define WGM3  WGM13
  #define TIMER_CAPT_vect TIMER1_CAPT_vect
#elif (ICTIMER == 2)
  #define TCCRA TCCR2A
  #define TCCRB TCCR2B
  #define TCCRC TCCR2C
  #define TCNT TCNT2
  #define OCRA OCR2A
  #define ICR  ICR2
  #define ICNC ICNC2
  #define TIMSK TIMSK2
  #define TIFR  TIFR2
  #define ICF   ICF2
  #define ICIE  ICIE2
  #define OCFA  OCF2A
  #define CS1   CS21
  #define CS0   CS20
  #define ICES  ICES2
  #define WGM2  WGM22
  #define WGM3  WGM23
  #define TIMER_CAPT_vect TIMER2_CAPT_vect
#elif (ICTIMER == 3)
  #define TCCRA TCCR3A
  #define TCCRB TCCR3B
  #define TCCRC TCCR3C
  #define TCNT TCNT3
  #define OCRA OCR3A
  #define ICR  ICR3
  #define ICNC ICNC3
  #define TIMSK TIMSK3
  #define TIFR  TIFR3
  #define ICF   ICF3
  #define ICIE  ICIE3
  #define OCFA  OCF3A
  #define CS1   CS31
  #define CS0   CS30
  #define ICES  ICES3
  #define WGM2  WGM32
  #define WGM3  WGM33
  #define TIMER_CAPT_vect TIMER3_CAPT_vect
#elif (ICTIMER == 4)
  #define TCCRA TCCR4A
  #define TCCRB TCCR4B
  #define TCCRC TCCR4C
  #define TCNT TCNT4
  #define OCRA OCR4A
  #define ICR  ICR4
  #define ICNC ICNC4
  #define TIMSK TIMSK4
  #define TIFR  TIFR4
  #define ICF   ICF4
  #define ICIE  ICIE4
  #define OCFA  OCF4A
  #define CS1   CS41
  #define CS0   CS40
  #define ICES  ICES4
  #define WGM2  WGM42
  #define WGM3  WGM43
  #define TIMER_CAPT_vect TIMER4_CAPT_vect
#elif (ICTIMER == 5)
  #define TCCRA TCCR5A
  #define TCCRB TCCR5B
  #define TCCRC TCCR5C
  #define TCNT TCNT5
  #define OCRA OCR5A
  #define ICR  ICR5
  #define ICNC ICNC5
  #define TIMSK TIMSK5
  #define TIFR  TIFR5
  #define ICF   ICF5
  #define ICIE  ICIE5
  #define OCFA  OCF5A
  #define CS1   CS51
  #define CS0   CS50
  #define ICES  ICES5
  #define WGM2  WGM52
  #define WGM3  WGM53
  #define TIMER_CAPT_vect TIMER5_CAPT_vect
#else
  #error "Timer is unknown"
#endif


#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega328__) || \
    defined(__AVR_ATmega168PA__) || defined(__AVR_ATmega168__)
  #if (ICTIMER == 1)
    #define ICArduinoPin 8
    #define ICDDR  DDRB
    #define ICPIN  PB0
    #define OCArduinoPin 9
    #define OCPORT PORTB
    #define OCPIN  PB1
  #else
    #error "ATmega328/168 has only Timer1!"
  #endif
#elif defined(__AVR_ATmega32U4__)
  #if (ICTIMER == 1)
    #define ICArduinoPin 4
    #define ICDDR DDRD
    #define ICPIN PD4
    #define OCArduinoPin 9
    #define OCPORT PORTB
    #define OCPIN  PB5
  #elif (ICTIMER == 3)
    #define ICArduinoPin 13
    #define ICDDR DDRC
    #define ICPIN PC7
    #define OCArduinoPin 5
    #define OCPORT PORTC
    #define OCPIN  PC6
  #else
    #error "ATmega32u4 has only Timer1 and Timer3!"
  #endif
#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  #if (ICTIMER == 4)
    #define ICArduinoPin 49
    #define ICDDR DDRL
    #define ICPIN PL0
    #define OCArduinoPin 7
    #define OCPORT PORTH
    #define OCPIN  PH4
  #elif (ICTIMER == 5)
    #define ICArduinoPin 48
    #define ICDDR DDRL
    #define ICPIN PL1
    #define OCArduinoPin 46
    #define OCPORT PORTL
    #define OCPIN  PL3
  #else
    #error "On Arduino Mega, you can only access the ICPs of Timer4 and Timer5!"
  #endif
#elif defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644__) || \
      defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega1284__)
  #if (ICTIMER == 1)
    #define ICArduinoPin 14
    #define ICDDR  DDRD
    #define ICPIN  PD6
    #define OCArduinoPin 13
    #define OCPORT PORTD
    #define OCPIN  PD5
  #elif (ICTIMER == 3)
    #define ICArduinoPin 5
    #define ICDDR  DDRB
    #define ICPIN  PB5
    #define OCArduinoPin 6
    #define OCPORT PORTB
    #define OCPIN  PB6
  #else
    #error "ATmega324/644/1284(P) has only Timer1 and Timer 3!"
  #endif
#elif defined(__AVR_ATtiny84__) ||  defined(__AVR_ATtiny44__) ||  defined(__AVR_ATtiny24__)
  #if (ICTIMER == 1)
    #define ICArduinoPin 7
    #define ICDDR  DDRA
    #define ICPIN  PA7
    #define OCArduinoPin 6
    #define OCPORT PORTA
    #define OCPIN  PA6
  #else
    #error "ATtiny84(A) has only Timer1!"
  #endif
#elif defined(__AVR_ATtiny88__) ||  defined(__AVR_ATtiny48__)
  #if (ICTIMER == 1)
    #define ICArduinoPin 8
    #define ICDDR  DDRB
    #define ICPIN  PB0
    #define OCArduinoPin 9
    #define OCPORT PORTB
    #define OCPIN  PB1
  #else
    #error "ATtiny88/ATtiny48 has only Timer1!"
  #endif
#elif defined(__AVR_ATtiny167__) ||  defined(__AVR_ATtiny87__)
  #if (ICTIMER == 1)
    #define ICArduinoPin 4
    #define ICDDR  DDRA
    #define ICPIN  PA4
    #define OCArduinoPin 11
    #define OCPORT PORTB
    #define OCPIN  PB3
  #else
    #error "ATtiny167/ATtiny87 has only Timer1!"
  #endif
#elif defined(__AVR_ATtiny1634__) 
  #if (ICTIMER == 1)
    #define ICArduinoPin 12
    #define ICDDR  DDRC
    #define ICPIN  PC1
    #define OCArduinoPin 14
    #define OCPORT PORTB
    #define OCPIN  PB3
  #else
    #error "ATtiny1634 has only Timer1!"
  #endif
#elif defined(__AVR_ATtiny841__) ||  defined(__AVR_ATtiny441__)
  #if (ICTIMER == 1)
    #define ICArduinoPin 7 // clockwise! 
    #define ICDDR  DDRA
    #define ICPIN  PA7
    #define OCArduinoPin 6
    #define OCPORT PORTA
    #define OCPIN  PA6
  #elif (ICTIMER == 2)
    #define ICArduinoPin 8 // clockwise! 
    #define ICDDR  DDRB
    #define ICPIN  PB2
    #define OCArduinoPin 0
    #define OCPORT PORTA
    #define OCPIN  PA0
  #else
    #error "ATtiny88/ATtiny48 has only Timer1 and Timer2!"
  #endif
#elif defined(__AVR_ATtiny2313__) || defined(__AVR_ATtiny4313__) 
  #if (ICTIMER == 1)
    #define ICArduinoPin 8
    #define ICDDR  DDRD
    #define ICPIN  PD6
    #define OCArduinoPin 12
    #define OCPORT PORTB
    #define OCPIN  PB3
  #else
    #error "ATtiny2313/4313 has only Timer1!"
  #endif
#elif defined(__AVR_ATtiny861__) || defined(__AVR_ATtiny461__) || defined(__AVR_ATtiny261__) 
  #if (ICTIMER == 0)
    #define ICArduinoPin 4
    #define ICDDR  DDRA
    #define ICPIN  PA4
    #define OCArduinoPin 5
    #define OCPORT PORTA
    #define OCPIN  PA5
  #else
    #error "ATtiny861/461/261 has only Timer0!"
  #endif
#elif defined(__AVR_ATtiny828__)
  #if (ICTIMER == 1)
    #define ICArduinoPin 21
    #define ICDDR  DDRC
    #define ICPIN  PC5
    #define OCArduinoPin 22
    #define OCPORT PORTC
    #define OCPIN  PC6
  #else
    #error "ATtiny828 has only Timer1!"
  #endif
#elif defined(__AVR_ATtiny85__) ||  defined(__AVR_ATtiny45__) ||  defined(__AVR_ATtiny25__)
  #error "ATtiny85/45/25 does not have a input capture unit."
#elif defined(__AVR_ATtiny43U__)
  #error "ATtiny43U does not have a input capture unit."
#else
  #error "Unknown MCU type. Input capture not yet supported."
#endif



#endif
