// https://github.com/etherkit/Si5351Arduino
// http://nt7s.com/2014/06/si5351-libraries-and-breakout-board/

#include "si5351.h"

Si5351 si5351;


long frq = 7035000;    // start frequency in Hz
long frq_old;

void setup()
{
  si5351.init(SI5351_CRYSTAL_LOAD_10PF);
  // Set CLK0 to output x MHz with a fixed PLL frequency
  si5351.set_pll(SI5351_PLL_FIXED, SI5351_PLLA);
  si5351.set_freq(frq, SI5351_PLL_FIXED, SI5351_CLK0);
}

void loop()
{
  set_frequency (7035000);
  delay(22);
  set_frequency (7035750);
  delay(22);
}

void set_frequency(long frq)
{
  si5351.set_freq(frq, SI5351_PLL_FIXED, SI5351_CLK0);
}

