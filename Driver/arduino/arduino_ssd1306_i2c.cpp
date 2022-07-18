/*
  Kiriaus 18/7/22
*/

#include "arduino_ssd1306_i2c.h"
#include "Arduino.h"
#include <Wire.h>

static uint8_t _i2caddr;

static uint32_t write_buf( uint8_t * buf, uint16_t _size );

void init_display( uint8_t addr ) {

  _i2caddr = addr;
  Wire.begin(); // join i2c bus (address optional for master)
  Wire.setClock(1500000);  //really streching the display capabilities here, lower clock speed in case of problems
  
  uint8_t cmdbuf[] = {
    0x00,
    SSD1306_DISPLAYOFF,
    SSD1306_SETDISPLAYCLOCKDIV,
    0x80,
    SSD1306_SETMULTIPLEX,
    0x3f,
    SSD1306_SETDISPLAYOFFSET,
    0x00,
    SSD1306_SETSTARTLINE | 0x00,
    SSD1306_CHARGEPUMP,
    0x14,
    SSD1306_MEMORYMODE,
    0x00,
    SSD1306_SEGREMAP | 0x01,
    SSD1306_COMSCANDEC,
    SSD1306_SETCOMPINS,
    0x12,
    SSD1306_SETCONTRAST,
    0xcf,
    SSD1306_SETPRECHARGE,
    0xf1,
    SSD1306_SETVCOMDETECT,
    0x40,
    SSD1306_DISPLAYALLON_RESUME,
    SSD1306_NORMALDISPLAY,
    SSD1306_DISPLAYON
  };

  write_buf( cmdbuf, sizeof(cmdbuf) );
}

void update_display(video_buffer *buf) {

  uint8_t _bit = 0;
  uint8_t new_col = 0;

  uint8_t SSD1306_buffer[buf->byte_buffer_cnt];
  static uint8_t s_buff[17]; //support buffer to compensate for 32 bytes transmission limit 

  uint8_t cmdbuf[] = {
    0x00,
    SSD1306_COLUMNADDR,
    0,                      // start
    buf->size_x - 1, 		// end
    SSD1306_PAGEADDR,
    0,                      // start
    buf->size_y / 8 - 1     // end
  };

  write_buf( cmdbuf, sizeof(cmdbuf) );
  int p = 0;

  //madness to adapt from vid_buffer structure to display structure;
  for (uint8_t page = 0; page < 8; page++) {

    for (uint8_t k = 0; k < 16; k++) {
      for (uint8_t i = 0; i < 8; i++) {
        for (uint8_t j = 0; j < 8; j++) {
          _bit = (buf->vid_buf[(128 * page) + (k + (j * buf->byte_col_cnt))] & (0b10000000 >> i)) << i;
          new_col |= _bit >> (8 - j - 1) ;
          _bit = 0;
        }
        SSD1306_buffer[p++] = new_col;
        new_col = 0;
      }
    }
  }

  //write_buf( SSD1306_buffer, sizeof(SSD1306_buffer) );
  
  for (uint8_t j = 0; j < 64; j++) {
    for (uint8_t i = 0; i < 16; i++) {
      s_buff[1+i] = SSD1306_buffer[(j*16) + i];
    }
    s_buff[0] = 0x40;
    write_buf( s_buff, sizeof(s_buff) );
  }
  //write_buf( SSD1306_buffer, sizeof(SSD1306_buffer) );

}

// for submitting command sequences:
//  buf[0] must be 0x00
// for submitting bulk data (writing to display RAM):
//  buf[0] must be 0x40
static uint32_t write_buf( uint8_t * buf, uint16_t _size ) {

  uint32_t _status = TRANSFER_ERROR;

  Wire.beginTransmission(_i2caddr);
  for (int i = 0; i < _size; i++) {
    _status = Wire.write(buf[i]);
  }
  Wire.endTransmission();  
  return _status;
}
