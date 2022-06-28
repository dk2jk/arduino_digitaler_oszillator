// https://github.com/etherkit/Si5351Arduino
// http://nt7s.com/2014/06/si5351-libraries-and-breakout-board/

#include "si5351.h"
#define  PIN_POTI  A0
Si5351   si5351;
unsigned long serial_input_number;
unsigned long neueFrequenz = 7032000;

void setup()
{ Serial.begin(9600);
  serial_input_number = 0;
  si5351.init(SI5351_CRYSTAL_LOAD_10PF);
  si5351.set_pll(SI5351_PLL_FIXED, SI5351_PLLA);  // 900 MHz
  si5351.clock_enable(SI5351_CLK0, 1);            // CLK0 freigeben
}

////////////////////////////////////////////////////////////////
void loop()
{ //frequenz vom Terminal
  parser();
  if ( neueFrequenz != 0 )
  {
    //frequenz neu einstellen:
    si5351.set_freq(neueFrequenz, SI5351_PLL_FIXED, SI5351_CLK0);
    neueFrequenz = 0;
  }
}

void parser() {
  char c;
  //Check for character
  if (Serial.available() > 0) {
    c = Serial.read();
    switch (c) {
      case 'a': //option a
      case 'A':
        if ( serial_input_number > 0xfffffffeL )
        {
          serial_input_number = 0;
          Serial.print("*** ");
        }
        Serial.print("erkannt: ");
        Serial.println(serial_input_number);
        neueFrequenz = serial_input_number;
        serial_input_number = 0;
        break;
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':  // eingabezahl ansammeln
        serial_input_number = serial_input_number * 10 + (c - '0');
        break;
      case 'v':
        Serial.println("serial parser 1.0");
        serial_input_number = 0;
        break;
    }
    Serial.flush();
  }
}

