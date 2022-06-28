// https://github.com/etherkit/Si5351Arduino
// http://nt7s.com/2014/06/si5351-libraries-and-breakout-board/

#include "si5351.h" 
#define  PIN_POTI  A0 
Si5351   si5351;

void setup()
{ si5351.init(SI5351_CRYSTAL_LOAD_10PF);
  si5351.set_pll(SI5351_PLL_FIXED, SI5351_PLLA);  // 900 MHz
  si5351.clock_enable(SI5351_CLK0, 1);            // CLK0 freigeben
}
void loop()
{ //frequenz vom Poti ablesen:
  long frequenz = map( analogRead(PIN_POTI), 0, 1023, 7000000, 7040000);
  //frequenz neu einstellen:
  si5351.set_freq(frequenz, SI5351_PLL_FIXED, SI5351_CLK0);
}

