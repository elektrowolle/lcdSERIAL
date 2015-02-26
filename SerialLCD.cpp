#include "SerialLCD.h"

SerialLCD::SerialLCD(
                     bool twoLines,
                     bool sndFontSet,
                     unsigned char _queueSize
                    ) :
    backlight(true),
    queueCursor(0),
    queueSize(_queueSize * 4),
    queueLength(0),
    isDRAM(true)

{
    this->queue = new unsigned char[this->queueSize];

    //this->setUpDisplay(false, twoLines, sndFontSet);
}


bool SerialLCD::isDataReady()
{
    return this->queueLength > 0;
}


bool SerialLCD::addCommand(
                           unsigned char cmd,
                           bool rs,
                           bool rw
                          )
{
    if(this->queueLength + 4 >= this->queueSize)
        return false;

    this->queue[(this->queueCursor + this->queueLength) % this->queueSize]
        = this->mapChar(cmd, rs, rw, true, true);

    this->queueLength++;

    this->queue[(this->queueCursor + this->queueLength) % this->queueSize]
        = this->mapChar(cmd, rs, rw, false, true);

    this->queueLength++;

    this->queue[(this->queueCursor + this->queueLength) % this->queueSize]
        = this->mapChar(cmd, rs, rw, true, false);

    this->queueLength++;

    this->queue[(this->queueCursor + this->queueLength) % this->queueSize]
        = this->mapChar(cmd, rs, rw, false, false);

    this->queueLength++;

    return true;
}

unsigned char SerialLCD::mapChar(
                         unsigned char cmd,
                         bool rs,
                         bool rw,
                         bool en,
                         bool msb
                        )
{
    unsigned char result = 0;

    result |=  rs ? _BIT(LCD_RS_REGISTER) : 0;
    result |=  rw ? _BIT(LCD_RW_REGISTER) : 0;
    result |=  en ? _BIT(LCD_EN_REGISTER) : 0;
    result |=  this->backlight ? _BIT(LCD_BACKLIGHT_REGISTER ) : 0;

    cmd = 0x0f & (cmd >> (msb ? 4 : 0));

    result |=  cmd & _BIT(0) ? _BIT(LCD_B4_REGISTER) : 0;
    result |=  cmd & _BIT(1) ? _BIT(LCD_B5_REGISTER) : 0;
    result |=  cmd & _BIT(2) ? _BIT(LCD_B6_REGISTER) : 0;
    result |=  cmd & _BIT(3) ? _BIT(LCD_B7_REGISTER) : 0;


    return result;
}


unsigned char SerialLCD::getData()
{
    if(this->queueLength == 0)
        return 0;

    unsigned char data = this->queue[this->queueCursor];
    this->queueLength--;
    this->queueCursor = (this->queueCursor + 1) % this->queueSize;

    return data;
}

void SerialLCD::moveCursorHome (){
    this->addCommand(LCD_CURSOR_HOME, false, false);
}

void SerialLCD::moveCursorRight (){
        this->addCommand(
                     LCD_SET_SHIFT   |
                     LCD_CURSOR_MOVE |
                     LCD_SHIFT_RIGHT
                     ,
                     false,
                     false
                    );
}

void SerialLCD::moveCursorLeft (){
    this->addCommand(
                     LCD_SET_SHIFT   |
                     LCD_CURSOR_MOVE |
                     LCD_SHIFT_LEFT
                     ,
                     false,
                     false
                    );
}

void SerialLCD::moveCursor(bool bottomLine, unsigned char position)
{
    this->addCommand(
                     LCD_SET_DD_RAM_ADDRESS  |
                     (bottomLine ? 0x40 : 0) |
                     position,
                     false,
                     false
                    );
}

void SerialLCD::clearDisplay (){
    this->addCommand(LCD_CLEAR_DISPLAY, false, false);
}

void SerialLCD::shiftDisplayLeft (){
        this->addCommand(
                     LCD_SET_SHIFT     |
                     LCD_DISPLAY_SHIFT |
                     LCD_SHIFT_LEFT
                     ,
                     false,
                     false
                    );
}

void SerialLCD::shiftDisplayRight (){
        this->addCommand(
                     LCD_SET_SHIFT     |
                     LCD_DISPLAY_SHIFT |
                     LCD_SHIFT_RIGHT
                     ,
                     false,
                     false
                    );
}

void SerialLCD::controlDisplay (
                                bool displayOn,
                                bool cursorOn,
                                bool cursorBlinking
                               )
{
    this->addCommand(
                     LCD_SET_DISPLAY |
                     (displayOn      ? LCD_DISPLAY_ON  : LCD_DISPLAY_OFF ) |
                     (cursorOn       ? LCD_CURSOR_ON   : LCD_CURSOR_OFF  ) |
                     (cursorBlinking ? LCD_BLINKING_ON : LCD_BLINKING_OFF),
                     false,
                     false
                    );
}

bool SerialLCD::writeCharacter(unsigned char _c){
    if (this->queueLength > this->queueSize)
        return false;

    addCommand(_c, true, false);

    return true;
}

bool SerialLCD::addGraphic(unsigned char id, unsigned char * graphic)
{
    if(id > 7)
        return false;

    unsigned char addressCmd = SET_CG_RAM_ADDRESS | (id << 3);
    this->addCommand(addressCmd, false, false);

    unsigned char i = 8;

    while(i--){
        this->writeCharacter(graphic[7 - i]);
    }

    this->addCommand(SET_DD_RAM_ADDRESS, false, false);
    this->moveCursorHome();

    return true;
}

void SerialLCD::setUpDisplay(bool _8bit, bool twoLines, bool sndFont)
{
    this->addCommand(
                     LCD_FUNCTION_SET |
                     (_8bit    ? LCD_8_BIT_MODE   : LCD_4_BIT_MODE) |
                     (twoLines ? LCD_TWO_LINE     : LCD_ONE_LINE  ) |
                     (sndFont  ? LCD_FONT_SET_TWO : LCD_FONT_SET_ONE),
                     false,
                     false
                    );
}