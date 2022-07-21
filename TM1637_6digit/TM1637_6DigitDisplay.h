#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>

#define DOTPOS0 0x80
#define DOTPOS1 0x40
#define DOTPOS2 0x20
#define DOTPOS3 0x10
#define DOTPOS4 0x08
#define DOTPOS5 0x04

#define DIGITPOS0 0
#define DIGITPOS1 1
#define DIGITPOS2 2
#define DIGITPOS3 3
#define DIGITPOS4 4
#define DIGITPOS5 5

#define DIGITLEN1 1
#define DIGITLEN2 2
#define DIGITLEN3 3
#define DIGITLEN4 4
#define DIGITLEN5 5
#define DIGITLEN6 6

unsigned char displayOrder=1; //CHANGE THIS 1 OR 0 FOR CHANGING ORDER

#define TRUE 1
#define true 1
#define false 0
#define FALSE 0

#define SEG_A   0b00000001
#define SEG_B   0b00000010
#define SEG_C   0b00000100
#define SEG_D   0b00001000
#define SEG_E   0b00010000
#define SEG_F   0b00100000
#define SEG_G   0b01000000
#define SEG_DP  0b10000000

#define BRIGHT_LOW  0x00
#define BRIGHT_0    0x00
#define BRIGHT_1    0x01
#define BRIGHT_2    0x02
#define BRIGHT_3    0x03
#define BRIGHT_4    0x04
#define BRIGHT_5    0x05
#define BRIGHT_6    0x06
#define BRIGHT_7    0x07
#define BRIGHT_HIGH 0x0f

//#define ON    1
//#define OFF   0

// TM1637 Commands
//
// Communication Sequence (Automatic Address)
//     Cmd1: Start + CmdSetData + ACK + Stop + 
//     Cmd2+Data: Start + CmdSetAddress + ACK + (Data + ACK) * N + 
//     Cmd3: Start + CmdDisplay + ACK + Stop
//
// Communication Sequence (Fixed Address)
//     Cmd1: Start + CmdSetData + ACK + Stop
//     Cmd2+Data: (Start + Command + ACK + Data + ACK) * N + Stop +
//     Cmd3: Start + Command3 + ACK + Stop
//
// CmdSetData - Data command settings (byte) - TM1637_I2C_COMM1
// B7 B6 B5 B4 B3 B2 B1 B0 - Function Description
// 0  1  0  0  _  _  0  0  - (Data read/write) Write data to display register
// 0  1  0  0  _  _  1  0  - (Data read/write) Read key scan data
// 0  1  0  0  _  0  _  _  - (Address mode) Automatic address adding
// 0  1  0  0  _  1  _  _  - (Address mode) Fixed address
// 0  1  0  0  1  _  _  _  - (Test mode) Normal mode
// 0  1  0  0  1  _  _  _  - (Test mode) Test mode
//
// CmdSetAddress - Set Address - Digit (byte) - TM1637_I2C_COMM2
// B7 B6 B5 B4 B3 B2 B1 B0 - Function Description
// 1  1  0  0  0  0  0  0  - Digit 1 - C0H Grid1
// 1  1  0  0  0  0  0  1  - Digit 2 - C1H Grid2
// 1  1  0  0  0  0  1  0  - Digit 3 - C2H Grid3
// 1  1  0  0  0  0  1  1  - Digit 4 - C3H Grid4
// 1  1  0  0  0  1  0  0  - Digit 5 - C4H Grid5
// 1  1  0  0  0  1  0  1  - Digit 6 - C5H Grid6
// 
// CmdDisplay - Set Display - Digit (byte) - TM1637_I2C_COMM3
// B7 B6 B5 B4 B3 B2 B1 B0 - Function Description
// 1  0  0  0  _  0  0  0  - Brightness - Pulse width is set as 1/16
// 1  0  0  0  _  0  0  1  - Brightness - Pulse width is set as 2/16
// 1  0  0  0  _  0  1  0  - Brightness - Pulse width is set as 4/16
// 1  0  0  0  _  0  1  1  - Brightness - Pulse width is set as 10/16
// 1  0  0  0  _  1  0  0  - Brightness - Pulse width is set as 11/16
// 1  0  0  0  _  1  0  1  - Brightness - Pulse width is set as 12/16
// 1  0  0  0  _  1  1  0  - Brightness - Pulse width is set as 13/16
// 1  0  0  0  _  1  1  1  - Brightness - Pulse width is set as 14/16
// 1  0  0  0  0  _  _  _  - Display OFF
// 1  0  0  0  1  _  _  _  - Display ON

#define TM1637_I2C_COMM1    0x40  // CmdSetData       0b01000000
#define TM1637_I2C_COMM2    0xC0  // CmdSetAddress    0b11000000
#define TM1637_I2C_COMM3    0x80  // CmdDisplay       0b10000000

#define MAXDIGITS           6    // The number of digits in display   

#define DEFAULT_BIT_DELAY     100
#define DEFAULT_SCROLL_DELAY  100
#define DEFAULT_FLIP          false

#define FRAMES(a)     sizeof(a)/6
#define TIME_MS(t)    t
#define TIME_S(t)     t*1000


  //! Initialize a TM1637TinyDisplay object, setting the clock and
  //! data pins.
  //!
  //! @param pinClk - The number of the digital pin connected to the clock pin of the module
  //! @param pinDIO - The number of the digital pin connected to the DIO pin of the module
  //! @param bitDelay - The delay, in microseconds, between bit transition on the serial
  //!                   bus connected to the display
  //! @param flip - Flip display orientation (default=false)
  void TM1637TinyDisplay6(unsigned int bitDelay,unsigned int scrollDelay, bool flip);

  //! Sets the orientation of the display.
  //!
  //! Setting this parameter to true will cause the rendering on digits to be displayed
  //! upside down.
  //!
  //! @param flip Flip display upside down true/false
  void TM1637TinyDisplay6_flipDisplay(bool flip);

  //! Sets the brightness of the display.
  //!
  //! The setting takes effect when a command is given to change the data being
  //! displayed.
  //!
  //! @param brightness A number from 0 (lowest brightness) to 7 (highest brightness)
  //! @param on Turn display on or off
  void TM1637TinyDisplay6_setBrightness(uint8_t brightness, bool on );

  //! Sets the delay used to scroll string text (in ms).
  //!
  //! The setting takes effect when a showString() command send an argument with over
  //! four characters.
  //!
  //! @param scrollDelay A number in milliseconds (default is 200)
  void TM1637TinyDisplay6_setScrolldelay(unsigned int scrollDelay );

  //! Display arbitrary data on the module
  //!
  //! This function receives raw segment values as input and displays them. The segment data
  //! is given as a byte array, each byte corresponding to a single digit. Within each byte,
  //! bit 0 is segment A, bit 1 is segment B etc.
  //! The function may either set the entire display or any desirable part on its own. The first
  //! digit is given by the @ref pos argument with 0 being the leftmost digit. The @ref length
  //! argument is the number of digits to be set. Other digits are not affected.
  //!
  //! @param segments An array of size @ref length containing the raw segment values
  //! @param length The number of digits to be modified
  //! @param pos The position from which to start the modification (0 - leftmost, 5 - rightmost)
  void TM1637TinyDisplay6_setSegments(const uint8_t segments[], uint8_t length, uint8_t pos);

  //! Clear the display
  void TM1637TinyDisplay6_clear();

  //! Display a decimal number
  //!
  //! Display the given argument as a decimal number.
  //!
  //! @param num The number to be shown
  //! @param leading_zero When true, leading zeros are displayed. Otherwise unnecessary digits are
  //!        blank. NOTE: leading zero is not supported with negative numbers.
  //! @param length The number of digits to set. The user must ensure that the number to be shown
  //!        fits to the number of digits requested (for example, if two digits are to be displayed,
  //!        the number must be between 0 to 99)
  //! @param pos The position of the most significant digit (0 - leftmost, 5 - rightmost)
  void TM1637TinyDisplay6_showNumberint(int num, bool leading_zero, uint8_t length, uint8_t pos,uint8_t dots );


  //! Display a decimal number
  //!
  //! Display the given argument as a decimal number.
  //!
  //! @param num The number to be shown
  //! @param leading_zero When true, leading zeros are displayed. Otherwise unnecessary digits are
  //!        blank. NOTE: leading zero is not supported with negative numbers.
  //! @param length The number of digits to set. The user must ensure that the number to be shown
  //!        fits to the number of digits requested (for example, if two digits are to be displayed,
  //!        the number must be between 0 to 99)
  //! @param pos The position of the most significant digit (0 - leftmost, 5 - rightmost)
  void TM1637TinyDisplay6_showNumberlong(long num, bool leading_zero, uint8_t length, uint8_t pos,uint8_t dots);


  //! Display a decimal number with floating point
  //!
  //! Display the given argument as a decimal number. Decimal point will only show up on displays
  //! that support decimal point.
  //!
  //! @param num The number to be shown
  //! @param decimal_length Format to show only this number of digits after the decimal point.
  //!        NOTE: Anything over 4 will do best fit.
  //! @param length The number of digits to set. The user must ensure that the number to be shown
  //!        fits to the number of digits requested (for example, if two digits are to be displayed,
  //!        the number must be between 0 to 99)
  //! @param pos The position of the most significant digit (0 - leftmost, 5 - rightmost)
  void TM1637TinyDisplay6_showNumberdouble(double num, uint8_t decimal_length, uint8_t length, uint8_t pos,uint8_t dots);

  //! Display a decimal number, with dot control
  //!
  //! Display the given argument as a decimal number. The dots between the digits (or colon)
  //! can be individually controlled.
  //!
  //! @param num The number to be shown
  //! @param dots Dot/Colon enable. The argument is a bitmask, with each bit corresponding to a dot
  //!        between the digits (or colon mark, as implemented by each module). i.e.
  //!        For displays with dots between each digit:
  //!        * 00.0000  (0b01000000)
  //!        * 0.00000  (0b10000000)
  //!        * 00.0000  (0b01000000)
  //!        * 000.000  (0b00100000)
  //!        * 0000.00  (0b00010000)
  //!        * 00000.0  (0b00001000)
  //!        * 000000.  (0b00000100)
  //!        * 00.00.00 (0b01010000)
  //!        For displays with just a colons:
  //!        * 00:00:00 (0b01010000)  
  //! @param leading_zero When true, leading zeros are displayed. Otherwise unnecessary digits are
  //!        blank. NOTE: leading zero is not supported with negative numbers.
  //! @param length The number of digits to set. The user must ensure that the number to be shown
  //!        fits to the number of digits requested (for example, if two digits are to be displayed,
  //!        the number must be between 0 to 99)
  //! @param pos The position of the most significant digit (0 - leftmost, 5 - rightmost)
  void TM1637TinyDisplay6_showNumberDec(long num, uint8_t dots, bool leading_zero, uint8_t length, uint8_t pos);

  //! Display a hexadecimal number, with dot control
  //!
  //! Display the given argument as a hexadecimal number. The dots between the digits (or colon)
  //! can be individually controlled.
  //!
  //! @param num The number to be shown
  //! @param dots Dot/Colon enable. The argument is a bitmask, with each bit corresponding to a dot
  //!        between the digits (or colon mark, as implemented by each module). i.e.
  //!        For displays with dots between each digit:
  //!        * 00.0000  (0b01000000)
  //!        * 0.00000  (0b10000000)
  //!        * 00.0000  (0b01000000)
  //!        * 000.000  (0b00100000)
  //!        * 0000.00  (0b00010000)
  //!        * 00000.0  (0b00001000)
  //!        * 000000.  (0b00000100)
  //!        * 00.00.00 (0b01010000)
  //!        For displays with just a colons:
  //!        * 00:00:00 (0b01010000)  
  //! @param leading_zero When true, leading zeros are displayed. Otherwise unnecessary digits are
  //!        blank
  //! @param length The number of digits to set. The user must ensure that the number to be shown
  //!        fits to the number of digits requested (for example, if two digits are to be displayed,
  //!        the number must be between 0 to 99)
  //! @param pos The position of the most significant digit (0 - leftmost, 5 - rightmost)
  void TM1637TinyDisplay6_showNumberHex(uint16_t num, uint8_t dots, bool leading_zero , uint8_t length , uint8_t pos );

  //! Translate a single digit into 7 segment code
  //!
  //! The method accepts a number between 0 - 15 and converts it to the
  //! code required to display the number on a 7 segment display.
  //! Numbers between 10-15 are converted to hexadecimal digits (A-F)
  //!
  //! @param digit A number between 0 to 15
  //! @return A code representing the 7 segment image of the digit (LSB - segment A;
  //!         bit 6 - segment G; bit 7 - always zero)
  uint8_t TM1637TinyDisplay6_encodeDigit(uint8_t digit);

  //! Display a string
  //!
  //! Display the given string and if more than 4 characters, will scroll message on display
  //!
  //! @param s The string to be shown
  //! @param scrollDelay  The delay, in microseconds to wait before scrolling to next frame
  //! @param length The number of digits to set. 
  //! @param pos The position of the most significant digit (0 - leftmost, 5 - rightmost)
  //! @param dots Dot/Colon enable. The argument is a bitmask, with each bit corresponding to a dot
  //!        between the digits (or colon mark, as implemented by each module). i.e.
  //!        For displays with dots between each digit:
  //!        * 00.0000  (0b01000000)
  //!        * 0.00000  (0b10000000)
  //!        * 00.0000  (0b01000000)
  //!        * 000.000  (0b00100000)
  //!        * 0000.00  (0b00010000)
  //!        * 00000.0  (0b00001000)
  //!        * 000000.  (0b00000100)
  //!        * 00.00.00 (0b01010000)
  //!        For displays with just a colons:
  //!        * 00:00:00 (0b01010000)  
  //! Use showString_P function for reading PROGMEM read-only flash memory space instead of RAM
  void TM1637TinyDisplay6_showString(const char s[], uint8_t length, uint8_t pos, uint8_t dots);

  //! Display a string (PROGMEM space)
  //!
  //! Display the given string and if more than 4 characters, will scroll message on display
  //! This function is for reading PROGMEM read-only flash memory space instead of RAM
  //!
  //! @param s The string to be shown
  //! @param scrollDelay  The delay, in microseconds to wait before scrolling to next frame
  //! @param length The number of digits to set. 
  //! @param pos The position of the most significant digit (0 - leftmost, 5 - rightmost)
  //! @param dots Dot/Colon enable. The argument is a bitmask, with each bit corresponding to a dot
  //!        between the digits (or colon mark, as implemented by each module). i.e.
  //!        For displays with dots between each digit:
  //!        * 00.0000  (0b01000000)
  //!        * 0.00000  (0b10000000)
  //!        * 00.0000  (0b01000000)
  //!        * 000.000  (0b00100000)
  //!        * 0000.00  (0b00010000)
  //!        * 00000.0  (0b00001000)
  //!        * 000000.  (0b00000100)
  //!        * 00.00.00 (0b01010000)
  //!        For displays with just a colons:
  //!        * 00:00:00 (0b01010000)  
  //! This function is for reading PROGMEM read-only flash memory space instead of RAM
  void TM1637TinyDisplay6_showString_P(const char s[], uint8_t length , uint8_t pos, uint8_t dots);

  //! Display a Level Indicator (both orientations)
  //!
  //! Illumiate LEDs to provide a visual indicator of level (horizontal or vertical orientation)
  //!
  //! @param level A value between 0 and 100 (representing percentage)
  //! @param horizontal Boolean (true/false) where true = horizontal, false = vertical
  void TM1637TinyDisplay6_showLevel(unsigned int level, bool horizontal);
  
  //! Display a sequence of raw LED segment data to create an animation
  //!
  //! Play thorugh an array of raw LED segment data to create a moving pattern.  
  //!
  //! const uint8_t Example[2][6] = 
  //! {
  //!  {                // frame 1
  //!   0b00001000,                                     // digit 1
  //!   0b00000000,                                     // digit 2
  //!   0b00000000,                                     // digit 3
  //!   0b00000000,                                     // digit 4
  //!   0b00000000,                                     // digit 5
  //!   0b00000000                                      // digit 6
  //!  },
  //!  {                // frame 2
  //!   0b00000000,                                     // digit 1
  //!   0b00001000,                                     // digit 2
  //!   0b00000000,                                     // digit 3
  //!   0b00000000,                                     // digit 4
  //!   0b00000000,                                     // digit 5
  //!   0b00000000                                      // digit 6
  //!  }
  //! }
  //! @param data A multi-dimensional array containing the LED segment - data[frames][4]
  //! @param frames Number of frames in the sequence to animate
  //! @param ms Time to delay between each frame
  //! The _P function is for reading PROGMEM read-only flash memory space instead of RAM
  void TM1637TinyDisplay6_showAnimation(const uint8_t data[][6], unsigned int frames , unsigned int ms);

  //! Display a sequence of raw LED segment data to create an animation (PROGMEM)
  //!
  //! Play thorugh an array of raw LED segment data to create a moving pattern.  
  //! This function is for reading PROGMEM read-only flash memory space instead of RAM
  //!
  //! @param data A multi-dimensional array containing the LED segment - data[frames][4]
  //! @param frames Number of frames in the sequence to animate
  //! @param ms Time to delay between each frame
  void TM1637TinyDisplay6_showAnimation_P(const uint8_t data[][6], unsigned int frames , unsigned int ms);

  //! Translate a single ASCII character into 7 segment code
  //!
  //! The method accepts a number between 0 - 255 and converts it to the
  //! code required to display the number on a 7 segment display.
  //! ASCII Characters between 0-32 and 128-255 are blank.
  //!
  //! @param chr A character ASCII value 
  //! @return A code representing the 7 segment image of the digit (LSB - segment A;
  //!         bit 6 - segment G; bit 7 - always zero)
  uint8_t TM1637TinyDisplay6_encodeASCII(uint8_t chr);
  
   void TM1637TinyDisplay6_bitDelay();

   void TM1637TinyDisplay6_start();

   void TM1637TinyDisplay6_stop();

   bool TM1637TinyDisplay6_writeByte(uint8_t b);

   void TM1637TinyDisplay6_showDots(uint8_t dots, uint8_t* digits);
   
   
   void TM1637TinyDisplay6_showNumberBaseExx(uint8_t base1, uint32_t num, uint8_t dots, bool leading_zero,uint8_t length, uint8_t pos);

   unsigned int m_bitDelay;
   unsigned int m_scrollDelay;
   void delay_ms(unsigned int milliseconds);
  bool m_flipDisplay;
  uint8_t m_brightness;
  uint8_t digits[MAXDIGITS];

