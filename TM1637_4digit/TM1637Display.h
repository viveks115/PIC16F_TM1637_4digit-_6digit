///*
//* Project Name: tm1637
//* File: TM1637Display.h 
//* Description: header file library TM1637  4 digit seven segment module 3462BS
//* Author: Gavin Lyons.
//* Complier: xc8 v2.05 compiler
//* PIC: PIC16F1619 
//* IDE:  MPLAB X v5.05
//* Development board: Microchip Curiosity Board, PIC16F1619
//* Created May 2019
//*/
//
//#ifndef __TM1637DISPLAY__
//#define __TM1637DISPLAY__
//
//#include <stdint.h>
//#include <stdbool.h>
//        
//#define SEG_A   0b00000001
//#define SEG_B   0b00000010
//#define SEG_C   0b00000100
//#define SEG_D   0b00001000
//#define SEG_E   0b00010000
//#define SEG_F   0b00100000
//#define SEG_G   0b01000000
//#define SEG_DOT 0b10000000
//#define SEG_DP  0b10000000
//
//#define BRIGHT_LOW  0x00
//#define BRIGHT_0    0x00
//#define BRIGHT_1    0x01
//#define BRIGHT_2    0x02
//#define BRIGHT_3    0x03
//#define BRIGHT_4    0x04
//#define BRIGHT_5    0x05
//#define BRIGHT_6    0x06
//#define BRIGHT_7    0x07
//#define BRIGHT_HIGH 0x0f
//
//#define TM1637_I2C_COMM1    0x40  // CmdSetData       0b01000000
//#define TM1637_I2C_COMM2    0xC0  // CmdSetAddress    0b11000000
//#define TM1637_I2C_COMM3    0x80  // CmdDisplay       0b10000000
//
////#define ON    1
////#define OFF   0
//
//#define MAXDIGITS           6     // The number of digits in display   
//
//#define DEFAULT_BIT_DELAY     100
//#define DEFAULT_SCROLL_DELAY  100
//#define DEFAULT_FLIP          false
//
//#define FRAMES(a)     sizeof(a)/6
//#define TIME_MS(t)    t
//#define TIME_S(t)     t*1000
//
//
//bool m_flipDisplay;
//uint8_t digits[MAXDIGITS];
//uint8_t m_brightness;
//    
//  //! Initialize a TM1637Display object, setting the clock and
//  //! data pins.
//  //!
//  //! @param pinClk - The number of the digital pin connected to the clock pin of the module
//  //! @param pinDIO - The number of the digital pin connected to the DIO pin of the module
//void   TM1637Display(void);
//
//  //! Sets the brightness of the display.
//  //!
//  //! The setting takes effect when a command is given to change the data being
//  //! displayed.
//  //!
//  //! @param brightness A number from 0 (lowes brightness) to 7 (highest brightness)
//  //! @param on Turn display on or off
//  void setBrightness(uint8_t brightness, bool on );
//
//  //! Display arbitrary data on the module
//  //!
//  //! This function receives raw segment values as input and displays them. The segment data
//  //! is given as a byte array, each byte corresponding to a single digit. Within each byte,
//  //! bit 0 is segment A, bit 1 is segment B etc.
//  //! The function may either set the entire display or any desirable part on its own. The first
//  //! digit is given by the @ref pos argument with 0 being the leftmost digit. The @ref length
//  //! argument is the number of digits to be set. Other digits are not affected.
//  //!
//  //! @param segments An array of size @ref length containing the raw segment values
//  //! @param length The number of digits to be modified
//  //! @param pos The position from which to start the modification (0 - leftmost, 3 - rightmost)
//  void setSegments(const uint8_t segments[], uint8_t length , uint8_t pos );
//
//  //! Displayes a decimal number
//  //!
//  //! Dispalyes the given argument as a decimal number
//  //!
//  //! @param num The number to be shown
//  //! @param leading_zero When true, leading zeros are displayed. Otherwise unnecessary digits are
//  //!        blank
//  //! @param length The number of digits to set. The user must ensure that the number to be shown
//  //!        fits to the number of digits requested (for example, if two digits are to be displayed,
//  //!        the number must be between 0 to 99)
//  //! @param pos The position most significant digit (0 - leftmost, 3 - rightmost)
//  void showNumberDec(int num, bool leading_zero, uint8_t length , uint8_t pos );
//
//  //! Displayes a decimal number, with dot control
//  //!
//  //! Dispalyes the given argument as a decimal number. The dots between the digits (or colon)
//  //! can be individually controlled
//  //!
//  //! @param num The number to be shown
//  //! @param dots Dot enable. The argument is a bitmask, with each bit corresponding to a dot
//  //!        between the digits (or colon mark, as implemented by each module). The MSB is the 
//  //!        leftmost dot of the digit being update. For example, if pos is 1, the MSB of dots
//  //!        will correspond the dot near digit no. 2 from the left. Dots are updated only on
//  //!        those digits actually being update (that is, no more than len digits)
//  //! @param leading_zero When true, leading zeros are displayed. Otherwise unnecessary digits are
//  //!        blank
//  //! @param length The number of digits to set. The user must ensure that the number to be shown
//  //!        fits to the number of digits requested (for example, if two digits are to be displayed,
//  //!        the number must be between 0 to 99)
//  //! @param pos The position least significant digit (0 - leftmost, 3 - rightmost)
//  void showNumberDecEx(int num, uint8_t dots , bool leading_zero ,uint8_t length , uint8_t pos );
//
//  //! Translate a single digit into 7 segment code
//  //!
//  //! The method accepts a number between 0 - 15 and converts it to the
//  //! code required to display the number on a 7 segment display.
//  //! Numbers between 10-15 are converted to hexadecimal digits (A-F)
//  //!
//  //! @param digit A number between 0 to 15
//  //! @return A code representing the 7 segment image of the digit (LSB - segment A;
//  //!         bit 6 - segment G; bit 7 - always zero)
//  uint8_t encodeDigit(uint8_t digit);
//
//
//   void bitDelay(void);
//   void start(void);
//   void stop(void);
//   bool writeByte(uint8_t b);
//   void showDots(uint8_t dots, uint8_t* digits);  
//   void showNumberBaseEx(int8_t base, uint32_t num, uint8_t dots , bool leading_zero, uint8_t length , uint8_t pos );
//#endif // __TM1637DISPLAY__
