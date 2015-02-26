#ifndef __SERIAL_LCD_H__
#define  __SERIAL_LCD_H__
// Register
#ifndef LCD_RS_REGISTER
#define LCD_RS_REGISTER           0
#endif
#ifndef LCD_RW_REGISTER
#define LCD_RW_REGISTER           1
#endif
#ifndef LCD_EN_REGISTER
#define LCD_EN_REGISTER           2
#endif
#ifndef LCD_BACKLIGHT
#define LCD_BACKLIGHT             3
#endif

#ifndef LCD_B4_REGISTER
#define LCD_B4_REGISTER           4
#endif
#ifndef LCD_B5_REGISTER
#define LCD_B5_REGISTER           5
#endif
#ifndef LCD_B6_REGISTER
#define LCD_B6_REGISTER           6
#endif
#ifndef LCD_B7_REGISTER
#define LCD_B7_REGISTER           7
#endif


// Clear Display -------------- 0b00000001
#ifndef LCD_CLEAR_DISPLAY
#define LCD_CLEAR_DISPLAY        0x01
#endif


// Cursor Home ---------------- 0b0000001x
#ifndef LCD_CURSOR_HOME
#define LCD_CURSOR_HOME          0x02
#endif

// Function Set --------------- 0b001xxxxx
#ifndef LCD_FUNCTION_SET
#define LCD_FUNCTION_SET          0x20
#endif
#ifndef LCD_8_BIT_MODE
#define LCD_8_BIT_MODE            0x10
#endif
#ifndef LCD_4_BIT_MODE
#define LCD_4_BIT_MODE            0x00
#endif
#ifndef LCD_ONE_LINE
#define LCD_ONE_LINE              0x00
#endif
#ifndef LCD_TWO_LINE
#define LCD_TWO_LINE              0x08
#endif
#ifndef LCD_FONT_SET_ONE
#define LCD_FONT_SET_ONE          0x00
#endif
#ifndef LCD_FONT_SET_TWO
#define LCD_FONT_SET_TWO          0x04
#endif

// Set Entry Mode ------------- 0b000001xx
#ifndef LCD_SET_ENTRY
#define LCD_SET_ENTRY            0x04
#endif

#ifndef LCD_ENTRY_DECREASE
#define LCD_ENTRY_DECREASE       0x00
#endif
#ifndef LCD_ENTRY_INCREASE
#define LCD_ENTRY_INCREASE       0x02
#endif
#ifndef LCD_ENTRY_NOSHIFT
#define LCD_ENTRY_NOSHIFT        0x00
#endif
#ifndef LCD_ENTRY_SHIFT
#define LCD_ENTRY_SHIFT          0x01
#endif

// Set Display ---------------- 0b00001xxx
#ifndef LCD_SET_DISPLAY
#define LCD_SET_DISPLAY          0x08
#endif

#ifndef LCD_DISPLAY_OFF
#define LCD_DISPLAY_OFF          0x00
#endif
#ifndef LCD_DISPLAY_ON
#define LCD_DISPLAY_ON           0x04
#endif
#ifndef LCD_CURSOR_OFF
#define LCD_CURSOR_OFF           0x00
#endif
#ifndef LCD_CURSOR_ON
#define LCD_CURSOR_ON            0x02
#endif
#ifndef LCD_BLINKING_OFF
#define LCD_BLINKING_OFF         0x00
#endif
#ifndef LCD_BLINKING_ON
#define LCD_BLINKING_ON          0x01
#endif

// Set Shift ------------------ 0b0001xxxx
#ifndef LCD_SET_SHIFT
#define LCD_SET_SHIFT            0x10
#endif

#ifndef LCD_CURSOR_MOVE
#define LCD_CURSOR_MOVE          0x00
#endif
#ifndef LCD_DISPLAY_SHIFT
#define LCD_DISPLAY_SHIFT        0x08
#endif
#ifndef LCD_SHIFT_LEFT
#define LCD_SHIFT_LEFT           0x00
#endif
#ifndef LCD_SHIFT_RIGHT
#define LCD_SHIFT_RIGHT          0x01
#endif

#ifndef LCD_ENABLE
#define LCD_ENABLE               0x04
#endif
#ifndef SET_CG_RAM_ADDRESS
#define SET_CG_RAM_ADDRESS       0x40
#endif
#ifndef SET_DD_RAM_ADDRESS
#define SET_DD_RAM_ADDRESS       0x40
#endif

#ifndef _BIT
#define _BIT(i) (0x01 << i)
#endif



class SerialLCD
{
    //char data;
    bool backlight;
    unsigned char * queue;
    unsigned char queueCursor;
    bool isDRAM;


public:
    unsigned char queueLength;
    unsigned char queueSize;
    SerialLCD(         bool twoLines   = false,
                       bool sndFontSet = false,
              unsigned char _queueSize = 32
             );

    ~SerialLCD();
    bool isDataReady();
    unsigned char getData();

    void moveCursorHome ();
    void moveCursorRight();
    void moveCursorLeft ();
    void moveCursor(bool bottomLine, char position);


    void clearDisplay   ();
    void controlDisplay (
                         bool displayOn,
                         bool cursorOn,
                         bool cursorBlinking
                        );

    void shiftDisplayLeft();
    void shiftDisplayRight();

    bool writeCharacter(unsigned char _c);
    bool writeGraphic  ();

    bool addGraphic    (unsigned char id, unsigned char * graphic);

private:
    unsigned char mapChar(
                 unsigned char cmd,
                 bool rs ,
                 bool rw ,
                 bool en ,
                 bool msb
                );

    bool addCommand(
                    unsigned char cmd,
                    bool rs,
                    bool rw
                   );


    void setUpDisplay(bool _8bit, bool twoLines, bool sndFont);

};

#endif //__SERIAL_LCD_H__