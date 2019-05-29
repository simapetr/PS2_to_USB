#include "HID.h"
#include <PS2KeyAdvanced.h>

#define d_gpio_data 2
#define d_gpio_clk_irq 3

#define d_mod_left_ctrl 0x01
#define d_mod_left_shift 0x02
#define d_mod_left_alt 0x04
#define d_mod_left_gui 0x08
#define d_mod_right_ctrl 0x10
#define d_mod_right_shift 0x20
#define d_mod_right_alt 0x40
#define d_mod_right_gui 0x80

#define d_key_left_ctrl 0x08
#define d_key_left_shift 0x06
#define d_key_left_alt 0x0A
#define d_key_left_gui 0x0C
#define d_key_right_ctrl 0x09
#define d_key_right_shift 0x07
#define d_key_right_alt 0x0B
#define d_key_right_gui 0x0D

#define d_key_numlock 0x01
#define d_key_scrolllock 0x02
#define d_key_capslock 0x03
#define d_key_lock 0xFA

typedef struct
{
  uint8_t modifiers_ui8;
  uint8_t reserved_ui8;
  uint8_t keys_sui8[6];
} key_report;

static const uint8_t hid_report_descriptor_sui8[] PROGMEM =
{
  //  Keyboard
  0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)  // 47
  0x09, 0x06,                    // USAGE (Keyboard)
  0xa1, 0x01,                    // COLLECTION (Application)
  0x85, 0x02,                    // REPORT_ID (2)
  0x05, 0x07,                    // USAGE_PAGE (Keyboard)
  
  0x19, 0xe0,                    // USAGE_MINIMUM (Keyboard LeftControl)
  0x29, 0xe7,                    // USAGE_MAXIMUM (Keyboard Right GUI)
  0x15, 0x00,                    // LOGICAL_MINIMUM (0)
  0x25, 0x01,                    // LOGICAL_MAXIMUM (1)
  0x75, 0x01,                    // REPORT_SIZE (1)
  
  0x95, 0x08,                    // REPORT_COUNT (8)
  0x81, 0x02,                    // INPUT (Data,Var,Abs)
  0x95, 0x01,                    // REPORT_COUNT (1)
  0x75, 0x08,                    // REPORT_SIZE (8)
  0x81, 0x03,                    // INPUT (Cnst,Var,Abs)
  
  0x95, 0x06,                    // REPORT_COUNT (6)
  0x75, 0x08,                    // REPORT_SIZE (8)
  0x15, 0x00,                    // LOGICAL_MINIMUM (0)
  0x25, 0x73,                    // LOGICAL_MAXIMUM (115)
  0x05, 0x07,                    // USAGE_PAGE (Keyboard)
  
  0x19, 0x00,                    // USAGE_MINIMUM (Reserved (no event indicated))
  0x29, 0x73,                    // USAGE_MAXIMUM (Keyboard Application)
  0x81, 0x00,                    // INPUT (Data,Ary,Abs)
  0xc0,                          // END_COLLECTION
};

static const uint8_t scan_2_hid[]=
{
  0x00, 0x53, 0x47, 0x39, 0x46, 0x00, 0x48, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x65, 0x00, // 16
  0x00, 0x4A, 0x4D, 0x4B, 0x4E, 0x50, 0x4F, 0x52, 0x51, 0x49, 0x4C, 0x29, 0x2A, 0x2B, 0x28, 0x2C, // 32
  0x62, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 0x60, 0x61, 0x63, 0x58, 0x57, 0x56, 0x55, 0x54, // 48
  0x27, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x34, 0x36, 0x2D, 0x37, 0x38, 0x00, // 64
  0x35, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, // 80
  0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x33, 0x31, 0x2F, 0x30, 0x2E, // 96
  0x00, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x00, 0x00, 0x00, // 112
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 128
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 144
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 160
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 176
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 192
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 208
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 224
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 240
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 256
};

uint16_t scan_code_ui16;
key_report data_key_report;

uint8_t key_cnt_ui8 = 0;
volatile uint8_t key_ui8 = 0;

PS2KeyAdvanced keyboard_ps2keyadvanced;

void setup()
{
  // Set serial port debug
  //Serial.begin( 115200 );
  delay(100);
  // Initialization USB HID keyboard stack
  static HIDSubDescriptor node_hidsubdescriptor(hid_report_descriptor_sui8, sizeof(hid_report_descriptor_sui8));
  HID().AppendDescriptor(&node_hidsubdescriptor);
  data_key_report.modifiers_ui8 = 0x00;
  data_key_report.keys_sui8[0] = 0x00;
  data_key_report.keys_sui8[1] = 0x00;
  data_key_report.keys_sui8[2] = 0x00;
  data_key_report.keys_sui8[3] = 0x00;
  data_key_report.keys_sui8[4] = 0x00;
  data_key_report.keys_sui8[5] = 0x00;
  // Initialization PS2 driver
  keyboard_ps2keyadvanced.begin( d_gpio_data, d_gpio_clk_irq );
  // Activate numlock
  keyboard_ps2keyadvanced.setLock(PS2_LOCK_NUM);
}


void loop()
{
  // Check PS2 is connected
  if( keyboard_ps2keyadvanced.available() )
  {
    // Read scan code
    scan_code_ui16 = keyboard_ps2keyadvanced.read();
    // Control code valid
    if( scan_code_ui16 > 0 )
    {
      switch((scan_code_ui16 & 0x00FF))
      {
        case d_key_left_ctrl:
        {
          if (scan_code_ui16 & 0x8000)
          {
            data_key_report.modifiers_ui8 &= (uint8_t)~((uint8_t)d_mod_left_ctrl);
          }
          else
          {
            data_key_report.modifiers_ui8 |= d_mod_left_ctrl;
          }
        }
        break;
        case d_key_left_shift:
        {
          if (scan_code_ui16 & 0x8000)
          {
            data_key_report.modifiers_ui8 &= (uint8_t)~((uint8_t)d_mod_left_shift);
          }
          else
          {
            data_key_report.modifiers_ui8 |= d_mod_left_shift;
          }
        }
        break;
        case d_key_left_alt:
        {
          if (scan_code_ui16 & 0x8000)
          {
            data_key_report.modifiers_ui8 &= (uint8_t)~((uint8_t)d_mod_left_alt);
          }
          else
          {
            data_key_report.modifiers_ui8 |= d_mod_left_alt;
          }
        }
        break;
        case d_key_left_gui:
        {
          if (scan_code_ui16 & 0x8000)
          {
            data_key_report.modifiers_ui8 &= (uint8_t)~((uint8_t)d_mod_left_gui);
          }
          else
          {
            data_key_report.modifiers_ui8 |= d_mod_left_gui;
          }
        }
        break;
        case d_key_right_ctrl:
        {
          if (scan_code_ui16 & 0x8000)
          {
            data_key_report.modifiers_ui8 &= (uint8_t)~((uint8_t)d_mod_right_ctrl);
          }
          else
          {
            data_key_report.modifiers_ui8 |= d_mod_right_ctrl;
          }
        }
        break;
        case d_key_right_shift:
        {
          if (scan_code_ui16 & 0x8000)
          {
            data_key_report.modifiers_ui8 &= (uint8_t)~((uint8_t)d_mod_right_shift);
          }
          else
          {
            data_key_report.modifiers_ui8 |= d_mod_right_shift;
          }
        }
        break;
        case d_key_right_alt:
        {
          if (scan_code_ui16 & 0x8000)
          {
            data_key_report.modifiers_ui8 &= (uint8_t)~((uint8_t) d_mod_right_alt);
          }
          else
          {
            data_key_report.modifiers_ui8 |= d_mod_right_alt;
          }
        }
        break;
        case d_key_right_gui:
        {
          if (scan_code_ui16 & 0x8000)
          {
            data_key_report.modifiers_ui8 &= (uint8_t)~((uint8_t)d_mod_right_gui);
          }
          else
          {
            data_key_report.modifiers_ui8 |= d_mod_right_gui;
          }
        }
        break;
        case d_key_numlock:
        case d_key_scrolllock:
        case d_key_capslock:
        {
          key_ui8 = scan_2_hid[(scan_code_ui16 & 0x00FF)];
          if (
              data_key_report.keys_sui8[0] != key_ui8 && data_key_report.keys_sui8[1] != key_ui8 && 
              data_key_report.keys_sui8[2] != key_ui8 && data_key_report.keys_sui8[3] != key_ui8 &&
              data_key_report.keys_sui8[4] != key_ui8 && data_key_report.keys_sui8[5] != key_ui8
             )
          {
            for (key_cnt_ui8 = 0; key_cnt_ui8 < 6; key_cnt_ui8++)
            {
              if (data_key_report.keys_sui8[key_cnt_ui8] == 0x00)
              {
                data_key_report.keys_sui8[key_cnt_ui8] = key_ui8;
                break;
              }
            }
          }
        }
        break;
        case d_key_lock:
        {
          data_key_report.modifiers_ui8 = 0x00;
          data_key_report.keys_sui8[0] = 0x00;
          data_key_report.keys_sui8[1] = 0x00;
          data_key_report.keys_sui8[2] = 0x00;
          data_key_report.keys_sui8[3] = 0x00;
          data_key_report.keys_sui8[4] = 0x00;
          data_key_report.keys_sui8[5] = 0x00;
        }
        break;
        default:
        {
          key_ui8 = scan_2_hid[(scan_code_ui16 & 0x00FF)];
          if (scan_code_ui16 & 0x8000)
          {
            for (key_cnt_ui8 = 0; key_cnt_ui8 < 6; key_cnt_ui8++)
            {
              if (0 != key_ui8 && data_key_report.keys_sui8[key_cnt_ui8] == key_ui8)
              {
                data_key_report.keys_sui8[key_cnt_ui8] = 0x00;
              }
            }
          }
          else
          {
            if (
                data_key_report.keys_sui8[0] != key_ui8 && data_key_report.keys_sui8[1] != key_ui8 && 
                data_key_report.keys_sui8[2] != key_ui8 && data_key_report.keys_sui8[3] != key_ui8 &&
                data_key_report.keys_sui8[4] != key_ui8 && data_key_report.keys_sui8[5] != key_ui8
               )
            {
              for (key_cnt_ui8 = 0; key_cnt_ui8 < 6; key_cnt_ui8++)
              {
                if (data_key_report.keys_sui8[key_cnt_ui8] == 0x00)
                {
                  data_key_report.keys_sui8[key_cnt_ui8] = key_ui8;
                  break;
                }
              }
            }
          }
        }
        break;
      }
      /*Serial.print(data_key_report.modifiers_ui8, HEX);
      Serial.print("-");
      Serial.print(data_key_report.keys_sui8[0], HEX);
      Serial.print(",");
      Serial.print(data_key_report.keys_sui8[1], HEX);
      Serial.print(",");
      Serial.print(data_key_report.keys_sui8[2], HEX);
      Serial.print(",");
      Serial.print(data_key_report.keys_sui8[3], HEX);
      Serial.print(",");
      Serial.print(data_key_report.keys_sui8[4], HEX);
      Serial.print(",");
      Serial.print(data_key_report.keys_sui8[5], HEX);
      Serial.print("-");
      Serial.println((scan_code_ui16 & 0x00FF));*/
      HID().SendReport(2,&data_key_report,sizeof(key_report));
    }
  }
}
