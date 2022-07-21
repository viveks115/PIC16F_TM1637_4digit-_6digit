#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
//#include "TM1637Display.h"
#include "TM1637_6DigitDisplay.h"
#include "mcc_generated_files/mcc.h"
#include "TM1637Display.h"
// Digit sequence map for 6 digit displays
const uint8_t digitmap[] = { 2, 1, 0, 5, 4, 3 }; 

//
//      A
//     ---
//  F |   | B
//     -G-
//  E |   | C
//     ---
//      D
const uint8_t digitToSegment[] = {
 // XGFEDCBA
  0b00111111,    // 0
  0b00000110,    // 1
  0b01011011,    // 2
  0b01001111,    // 3
  0b01100110,    // 4
  0b01101101,    // 5
  0b01111101,    // 6
  0b00000111,    // 7
  0b01111111,    // 8
  0b01101111,    // 9
  0b01110111,    // A
  0b01111100,    // b
  0b00111001,    // C
  0b01011110,    // d
  0b01111001,    // E
  0b01110001     // F
  };

// ASCII Map - Index 0 starts at ASCII 32
const uint8_t asciiToSegment[]={
   0b00000000, // 032 (Space)
   0b00110000, // 033 !
   0b00100010, // 034 "
   0b01000001, // 035 #
   0b01101101, // 036 $
   0b01010010, // 037 %
   0b01111100, // 038 &
   0b00100000, // 039 '
   0b00111001, // 040 (
   0b00001111, // 041 )
   0b00100001, // 042 *
   0b01110000, // 043 +
   0b00001000, // 044 ,
   0b01000000, // 045 -
   0b00001000, // 046 .
   0b01010010, // 047 /
   0b00111111, // 048 0
   0b00000110, // 049 1
   0b01011011, // 050 2
   0b01001111, // 051 3
   0b01100110, // 052 4
   0b01101101, // 053 5
   0b01111101, // 054 6
   0b00000111, // 055 7
   0b01111111, // 056 8
   0b01101111, // 057 9
   0b01001000, // 058 :
   0b01001000, // 059 ;
   0b00111001, // 060 <
   0b01001000, // 061 =
   0b00001111, // 062 >
   0b01010011, // 063 ?
   0b01011111, // 064 @
   0b01110111, // 065 A
   0b01111100, // 066 B
   0b00111001, // 067 C
   0b01011110, // 068 D
   0b01111001, // 069 E
   0b01110001, // 070 F
   0b00111101, // 071 G
   0b01110110, // 072 H
   0b00000110, // 073 I
   0b00011110, // 074 J
   0b01110110, // 075 K
   0b00111000, // 076 L
   0b00010101, // 077 M
   0b00110111, // 078 N
   0b00111111, // 079 O
   0b01110011, // 080 P
   0b01100111, // 081 Q
   0b00110001, // 082 R
   0b01101101, // 083 S
   0b01111000, // 084 T
   0b00111110, // 085 U
   0b00011100, // 086 V
   0b00101010, // 087 W
   0b01110110, // 088 X
   0b01101110, // 089 Y
   0b01011011, // 090 Z
   0b00111001, // 091 [
   0b01100100, // 092 (backslash)
   0b00001111, // 093 ]
   0b00100011, // 094 ^
   0b00001000, // 095 _
   0b00100000, // 096 `
   0b01110111, // 097 a
   0b01111100, // 098 b
   0b01011000, // 099 c
   0b01011110, // 100 d
   0b01111001, // 101 e
   0b01110001, // 102 f
   0b01101111, // 103 g
   0b01110100, // 104 h
   0b00000100, // 105 i
   0b00011110, // 106 j
   0b01110110, // 107 k
   0b00011000, // 108 l
   0b00010101, // 109 m
   0b01010100, // 110 n
   0b01011100, // 111 o
   0b01110011, // 112 p
   0b01100111, // 113 q
   0b01010000, // 114 r
   0b01101101, // 115 s
   0b01111000, // 116 t
   0b00011100, // 117 u
   0b00011100, // 118 v
   0b00101010, // 119 w
   0b01110110, // 120 x
   0b01101110, // 121 y
   0b01011011, // 122 z
   0b00111001, // 123 {
   0b00110000, // 124 |
   0b00001111, // 125 }
   0b01000000, // 126 ~
   0b00000000, // 127 
};

static const uint8_t minusSegments = 0b01000000;
static const uint8_t degreeSegments = 0b01100011;

void TM1637TinyDisplay6(unsigned int bitDelay, unsigned int scrollDelay, bool flip)
{
  // Timing configurations
  m_bitDelay = bitDelay;
  m_scrollDelay = scrollDelay;
  // Flip 
  m_flipDisplay = flip;
  
  // Set the pin direction and default value.
  // Both pins are set as inputs, allowing the pull-up resistors to pull them up
    pinClk_SetDigitalInput() ;
    pinDIO_SetDigitalInput() ;
}

void TM1637TinyDisplay6_flipDisplay(bool flip)
{
  m_flipDisplay = flip;
}

void TM1637TinyDisplay6_setBrightness(uint8_t brightness, bool on)
{
  m_brightness = (brightness & 0x07) | (on? 0x08 : 0x00);
}

void TM1637TinyDisplay6_setScrolldelay(unsigned int scrollDelay)
{
  m_scrollDelay = scrollDelay;
}

void TM1637TinyDisplay6_setSegments(const uint8_t segments[], uint8_t length, uint8_t pos)
{
  uint8_t dot = 0;
  // Adjust for flip
  if(m_flipDisplay) pos = MAXDIGITS - pos - length;

  // Adjust pos for 6 digit display
  if(displayOrder==0)
  {
  int index = pos + length - 1;
  if (index > MAXDIGITS) index = index - MAXDIGITS;
  pos = digitmap[index];
  }
  // Write COMM1
  TM1637TinyDisplay6_start();
  TM1637TinyDisplay6_writeByte(TM1637_I2C_COMM1);
  TM1637TinyDisplay6_stop();

  // Write COMM2 + first digit address
  TM1637TinyDisplay6_start();
  TM1637TinyDisplay6_writeByte(TM1637_I2C_COMM2 + (pos & 0x07));

  // Write the data bytes
  if(m_flipDisplay) {
    for (uint8_t k=0; k < length; k++) {
      dot = 0;
      if((k - 1) >= 0) {
        dot = segments[k - 1] & 0b10000000;
      }
      uint8_t orig = segments[k];
      uint8_t flip = ((orig >> 3) & 0b00000111) + 
        ((orig << 3) & 0b00111000) + (orig & 0b01000000) + dot;
      TM1637TinyDisplay6_writeByte(flip);
    }
  }
  else {
    for (uint8_t k=0; k < length; k++) {
      // 6 digit display - send in reverse order
        if(displayOrder)
            TM1637TinyDisplay6_writeByte(segments[k]);
        else
            TM1637TinyDisplay6_writeByte(segments[length - 1 - k]); 
      
    }
  }
  TM1637TinyDisplay6_stop();

  // Write COMM3 + brightness
  TM1637TinyDisplay6_start();
  TM1637TinyDisplay6_writeByte(TM1637_I2C_COMM3 + (m_brightness & 0x0f));
  TM1637TinyDisplay6_stop();
}

void TM1637TinyDisplay6_clear()
{
  // digits[MAXDIGITS] output array to render
  memset(digits,0,sizeof(digits));
  TM1637TinyDisplay6_setSegments(digits,MAXDIGITS,0);
}

void TM1637TinyDisplay6_showNumberint(int num, bool leading_zero, uint8_t length, uint8_t pos,uint8_t dots)
{
  TM1637TinyDisplay6_showNumberlong((long)num, leading_zero, length, pos,dots);
}

void TM1637TinyDisplay6_showNumberlong(long num, bool leading_zero, uint8_t length, uint8_t pos,uint8_t dots)
{
  if(leading_zero) {
    TM1637TinyDisplay6_showNumberDec(num, 0, leading_zero, length, pos);
  }
  else {
    TM1637TinyDisplay6_showNumberdouble((double)num, 0, length, pos,dots);
  }
}

void TM1637TinyDisplay6_showNumberdouble(double num, uint8_t decimal_length, uint8_t length, uint8_t pos,uint8_t dots)
{
  int num_len = 0;              
  long inum = abs((long)num);  
  int decimal_places = 0;       
  double value = 0.0;
  bool negative = false; 
  bool leading_zero = false; 
  // digits[MAXDIGITS] output array to render
  memset(digits,0,sizeof(digits));

  // determine length of whole number part of num
  while(inum != 0) {  
    inum = inum / 10;
    num_len++;
  }
  if(num < 0) {
    num_len++; // make space for negative
    negative = true;
  }
  if(abs(num)<1) {
    num_len++; // make space for 0. prefix
    leading_zero = true;
  }
  // make sure we can display number otherwise show overflow
  if(num_len > length) {
    TM1637TinyDisplay6_showString("------", length, pos,dots); // overflow symbol
    return;
  }
  // how many decimal places can we show?
  decimal_places = length - num_len; 
  if(decimal_places > decimal_length) {
    decimal_places = decimal_length;
  }
  // construct whole number representation of num
  value = num;
  for(int x=0; x < decimal_places; x++) {
    value = value * 10.00;
  }
  if(num>0) value = value + 0.5; // round up
  if(num<0) value = value - 0.5; // round down
  inum = abs((long)value);

// render display array
  if (inum == 0 && !leading_zero) {
    digits[length-1] = TM1637TinyDisplay6_encodeDigit(0);
  }
  else {		
    int decimal_pos = length - 1 - decimal_places;
    for(int i = length-1; i >= 0; --i) {
      uint8_t digit = inum % 10;
      
      if (digit == 0 && inum == 0 &&
        (leading_zero == false || (i < decimal_pos))) {
        // Blank out any leading zeros except for 0.x case
        digits[i] = 0;

        // Add negative sign for negative number
        if (negative) {
          digits[i] = minusSegments;
          negative = false;
        }
      }
      else
        digits[i] = TM1637TinyDisplay6_encodeDigit(digit);
      if(i == decimal_pos && decimal_length > 0) {
        digits[i] += 0b10000000; // add decimal point
      }
      inum /= 10;
    }
  }
  TM1637TinyDisplay6_setSegments(digits, length, pos);
}

void TM1637TinyDisplay6_showNumberDec(long num, uint8_t dots, bool leading_zero,
                                    uint8_t length, uint8_t pos)
{
  TM1637TinyDisplay6_showNumberBaseExx(num < 0? -10 : 10, num < 0? -num : num, dots, leading_zero, length, pos);
}

void TM1637TinyDisplay6_showNumberHex(uint16_t num, uint8_t dots, bool leading_zero,
                                    uint8_t length, uint8_t pos)
{
  TM1637TinyDisplay6_showNumberBaseExx(16, num, dots, leading_zero, length, pos);
}

void TM1637TinyDisplay6_showNumberBaseExx(uint8_t base1, uint32_t num, uint8_t dots, bool leading_zero,uint8_t length, uint8_t pos)
{
  bool negative = false;
  if (base1 < 0) {
    base1 = -base1;
    negative = true;
  }

  // uint8_t digits[MAXDIGITS];

  if (num == 0 && !leading_zero) {
    // Singular case - take care separately
    for(uint8_t i = 0; i < (length-1); i++) {
      digits[i] = 0;
    }
    digits[length-1] = TM1637TinyDisplay6_encodeDigit(0);
  }
  else {		
    for(int i = length-1; i >= 0; --i) {
      uint8_t digit = num % base1;

      if (digit == 0 && num == 0 && leading_zero == false)
          // Leading zero is blank
        digits[i] = 0;
      else
        digits[i] = TM1637TinyDisplay6_encodeDigit(digit);
        
      if (digit == 0 && num == 0 && negative) {
        digits[i] = minusSegments;
        negative = false;
      }

      num /= base1;
    }
  }
  if(dots != 0) {
    TM1637TinyDisplay6_showDots(dots, digits);
  }
  TM1637TinyDisplay6_setSegments(digits, length, pos);
}

void TM1637TinyDisplay6_showString(const char s[], uint8_t length, uint8_t pos, uint8_t dots)
{
  // digits[MAXDIGITS] output array to render
  memset(digits,0,sizeof(digits));

  // Basic Display
  if (strlen(s) <= MAXDIGITS) {
    for (int x = 0; x < strlen(s); x++) {
      digits[x] = TM1637TinyDisplay6_encodeASCII(s[x]);
    }
    if(dots != 0) {
      TM1637TinyDisplay6_showDots(dots, digits);
    }
    TM1637TinyDisplay6_setSegments(digits, length, pos);
  }
  // Scrolling Display
  if (strlen(s) > MAXDIGITS) {
    // Scroll text on display if too long
    for (int x = 0; x < (MAXDIGITS-1); x++) {  // Scroll message on
      int y;
      for (y = 0; y < (MAXDIGITS-1); y++) {
        // shift left
        digits[y] = digits[y+1];
      }
      digits[y] = TM1637TinyDisplay6_encodeASCII(s[x]);
      TM1637TinyDisplay6_setSegments(digits, length, pos);
      delay_ms(m_scrollDelay);
    }
    for (size_t x = (MAXDIGITS-1); x < strlen(s); x++) { // Scroll through string
      int y;
      for (y = 0; y < (MAXDIGITS-1); y++) {
        // shift left
        digits[y] = digits[y+1];
      }
      digits[y] = TM1637TinyDisplay6_encodeASCII(s[x]);
      TM1637TinyDisplay6_setSegments(digits, length, pos);
      delay_ms(m_scrollDelay);
    }
    for (int x = 0; x < (MAXDIGITS); x++) {  // Scroll message off
          int y;
      for (y = 0; y < (MAXDIGITS-1); y++) {
        // shift left
        digits[y] = digits[y+1];
      }
      digits[y] = 0;
      TM1637TinyDisplay6_setSegments(digits, length, pos);
      delay_ms(m_scrollDelay);
    }
  }
}

void TM1637TinyDisplay6_showString_P(const char s[], uint8_t length, uint8_t pos, uint8_t dots) 
{
  // digits[MAXDIGITS] output array to render
  memset(digits,0,sizeof(digits));

  // Basic Display
  if (strlen(s) <= MAXDIGITS) {
    for (int x = 0; x < strlen(s); x++) {
      digits[x] = TM1637TinyDisplay6_encodeASCII(s[x]);
    }
    if(dots != 0) {
      TM1637TinyDisplay6_showDots(dots, digits);
    }
    TM1637TinyDisplay6_setSegments(digits, length, pos);
  }
  else {
    // Scroll text on display if too long
    for (int x = 0; x < (MAXDIGITS-1); x++) {  // Scroll message on
      int y;
      for (y = 0; y < (MAXDIGITS-1); y++) {
        // shift left
        digits[y] = digits[y+1];
      }
      digits[y] = TM1637TinyDisplay6_encodeASCII(s[x]);
      TM1637TinyDisplay6_setSegments(digits, length, pos);
      delay_ms(m_scrollDelay);
    }

    for (size_t x = (MAXDIGITS-1); x < strlen(s); x++) { // Scroll through string
      int y;
      for (y = 0; y < (MAXDIGITS-1); y++) {
        // shift left
        digits[y] = digits[y+1];
      }
      digits[y] = TM1637TinyDisplay6_encodeASCII(s[x]);
      TM1637TinyDisplay6_setSegments(digits, length, pos);
      delay_ms(m_scrollDelay);
    }

    for (int x = 0; x < (MAXDIGITS); x++) {  // Scroll message off
      int y;
      for (y = 0; y < (MAXDIGITS-1); y++) {
        // shift left
        digits[y] = digits[y+1];
      }
      digits[y] = 0;
      TM1637TinyDisplay6_setSegments(digits, length, pos);
      delay_ms(m_scrollDelay);
    }
  }
}

void TM1637TinyDisplay6_showLevel(unsigned int level, bool horizontal) 
{
  // digits[MAXDIGITS] output array to render
  memset(digits,0,sizeof(digits));

  uint8_t digit = 0b00000000;

  if(level>100) level=100;

  if(horizontal) {
    // Must fit within 3 bars
    int bars = (int)((level*3)/100.0);
    if(bars == 0 && level > 0) bars = 1; // Only level=0 turns off display
    switch(bars) {
      case 1:
        digit = 0b00001000;
        break;
      case 2:
        digit = 0b01001000;
        break;
      case 3:
        digit = 0b01001001;
        break;
      default: // Keep at zero
        break;
    }
    for(int x = 0; x < MAXDIGITS; x++) {
      digits[x] = digit;
    }
  }
  else {
    // Must fit within (MAXDIGITS * 2) bars
    int bars = (int)((level*(MAXDIGITS*2))/100.0);
    if(bars == 0 && level > 0) bars = 1;
    for(int x = 0; x<MAXDIGITS; x++) { // for each digit
      int left = bars-(x*2);
      if(left > 1) digits[x] = 0b00110110;
      if(left == 1) digits[x] = 0b00110000;
    }
  }
  TM1637TinyDisplay6_setSegments(digits,MAXDIGITS,0);
}

void TM1637TinyDisplay6_showAnimation(const uint8_t data[][6], unsigned int frames, unsigned int ms)
{
  // Animation sequence
  for (unsigned int x = 0; x < frames; x++) {
    TM1637TinyDisplay6_setSegments(data[x],6,0);
    delay_ms(ms);
  }
}

void TM1637TinyDisplay6_showAnimation_P(const uint8_t data[][6], unsigned int frames, unsigned int ms)
{
  // Animation sequence for data stored in PROGMEM flash memory
  uint8_t digits[6] = {0,0,0,0,0,0};
  for (unsigned int x = 0; x < frames; x++) {
    for(unsigned int a = 0; a < 6; a++) {
          digits[a] = data[x][a];
    }
    TM1637TinyDisplay6_setSegments(digits,6,0);
    delay_ms(ms);
  }
}

void TM1637TinyDisplay6_bitDelay()
{
  __delay_us(0);//75
}

void TM1637TinyDisplay6_start()
{
  pinDIO_SetDigitalOutput();
  TM1637TinyDisplay6_bitDelay();
}

void TM1637TinyDisplay6_stop()
{
    pinDIO_SetDigitalOutput();
	TM1637TinyDisplay6_bitDelay();
	pinClk_SetDigitalInput() ;
	TM1637TinyDisplay6_bitDelay();
	pinDIO_SetDigitalInput() ;
	TM1637TinyDisplay6_bitDelay();
}

bool TM1637TinyDisplay6_writeByte(uint8_t b)
{
  uint8_t data = b;

  // 8 Data Bits
  for(uint8_t i = 0; i < 8; i++) {
    // CLK low
    pinClk_SetDigitalOutput();
    TM1637TinyDisplay6_bitDelay();

    // Set data bit
    if (data & 0x01)
      pinDIO_SetDigitalInput();
    else
      pinDIO_SetDigitalOutput();

    TM1637TinyDisplay6_bitDelay();

    // CLK high
    pinClk_SetDigitalInput();
    TM1637TinyDisplay6_bitDelay();
    data = data >> 1;
  }

  // Wait for acknowledge
  // CLK to zero
  pinClk_SetDigitalOutput();
  pinDIO_SetDigitalInput();
  TM1637TinyDisplay6_bitDelay();
 
 // CLK to high
  pinClk_SetDigitalInput();
  TM1637TinyDisplay6_bitDelay();
  uint8_t ack = pinDIO_GetValue();
  if (ack == 0)
    pinDIO_SetDigitalOutput() ;

  TM1637TinyDisplay6_bitDelay();
  pinClk_SetDigitalOutput();
  TM1637TinyDisplay6_bitDelay();

  return ack;
}

void TM1637TinyDisplay6_showDots(uint8_t dots, uint8_t* digits)
{
  for(int i = 0; i < MAXDIGITS; ++i)
  {
      digits[i] |= (dots & 0x80);
      dots <<= 1;
  }
}

uint8_t TM1637TinyDisplay6_encodeDigit(uint8_t digit)
{
  // return digitToSegment[digit & 0x0f] using PROGMEM
  return digitToSegment[digit & 0x0f];
}

uint8_t TM1637TinyDisplay6_encodeASCII(uint8_t chr)
{
  if(chr == 176) return degreeSegments;   // Degree mark
  if(chr > 127 || chr < 32) return 0;     // Blank
  // return asciiToSegment[chr - 32] using PROGMEM
  return asciiToSegment [chr - 32];
}
void delay_ms(unsigned int milliseconds)
 {
   while(milliseconds > 0)
   {
       __delay_ms(1);
      milliseconds--;
    }
 }