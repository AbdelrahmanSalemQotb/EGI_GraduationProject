#include "Arduino.h"
#include "nhd_0420d3z.h"


NhdDisplay::NhdDisplay(int rs,int en,int d4,int d5,int d6,int d7):
  _row(0),
  _col(0) {
  _serial_ptr = new LiquidCrystal(rs, en, d4, d5, d6, d7);
}

void NhdDisplay::begin(int baud,int x) {
  // Start Serial connection with default baud.
  _serial_ptr->begin(20,4);
}

void NhdDisplay::print(String st) {
  _serial_ptr->print(st);
}

void NhdDisplay::print(int val) {
  _serial_ptr->print(String(val));
}

void NhdDisplay::println(String st) {
  _serial_ptr->print(st);
}

void NhdDisplay::setBacklight(byte brightness) {

 
}

void NhdDisplay::clearDisplay() {

  // Send special command byte.
  _serial_ptr->clear();
  
}

void NhdDisplay::setCursor(byte cursor_position, byte line) {
    _serial_ptr->setCursor(cursor_position,line);
    _row = line;
    _col = cursor_position;
}

void NhdDisplay::underlineOn() {
  
  // Send special command byte.
  _serial_ptr->write(0xFE);
  // Send command to turn underline on.
  _serial_ptr->write(0x47);
}

void NhdDisplay::underlineOff() {

  // Send special command byte.
  _serial_ptr->write(0xFE);
  // Send command to turn underline off.
  _serial_ptr->write(0x48);
}

void NhdDisplay::blinkingOn() {
  _serial_ptr->blink();
}

void NhdDisplay::blinkingOff() {
  _serial_ptr->noBlink();
}

void NhdDisplay::moveCursorRight() {
if (_col % 19 == 0 && _row % 3 == 0)
{
	_col = _row = 0;
}
else if (_col % 20 == 0)
{
	_row++;
	_col = 0;
}
else
{
	_col++;
}
 _serial_ptr->setCursor(_col, _row);
}

void NhdDisplay::moveCursorLeft() {

  if (_col == _row == 0)
  {
	  _col = 19;
	  _row = 3;
  }
  else if (_col == 0)
  {
	  _row--;
	  _col = 19;
  }
  else
  {
	  _col++;
  }
   _serial_ptr->setCursor(_col, _row);
}

void NhdDisplay::backspace() {

 
  // Send command to move cursor left and remove.
  _serial_ptr->write(" ");
  this->moveCursorLeft();
}

void NhdDisplay::remove() {
 _serial_ptr->write(" ");
 _serial_ptr->setCursor(_row, _col);
  // Together these delete the current character.
  //moveCursorRight();
  //backspace();
}

String NhdDisplay::zeroPad(int in) {
  // This helper function ensures integers have 2 decimal places.
  
  // Make a temp char array to hold the 2 digits.
  char digit[2];

  // Pad with a 0 if integer is 1 digit.
  sprintf(digit, "%02d", in);

  return String(digit);
}
