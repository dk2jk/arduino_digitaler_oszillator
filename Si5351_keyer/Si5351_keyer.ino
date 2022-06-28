// https://github.com/etherkit/Si5351Arduino
// http://nt7s.com/2014/06/si5351-libraries-and-breakout-board/

#include "global_pins.h"
#include "si5351.h"
#include "keyer.h"

Keyer keyer(12); //wpm
Si5351 si5351;
bool ist_an= false;
long const frq_basis = 7032000;

void set_frequency(long frq)
{
  si5351.set_pll(SI5351_PLL_FIXED, SI5351_PLLA);
  si5351.set_freq(frq, SI5351_PLL_FIXED, SI5351_CLK0);
}

void setup()
{
  si5351.init(SI5351_CRYSTAL_LOAD_10PF);
  set_frequency (frq_basis);
  si5351.clock_enable(SI5351_CLK0, 0);
}

////////////////////////////////////////////////////////////////
void loop()
{ long offset=0;
  long frq;
  frq = frq_basis + offset;
  set_frequency (frq);
  keyer.update_wpm( map( analogRead(PIN_POTI), 0, 1023, 10, 30));

  keyer.run();
  if (  ist_an == false )
  {
    if ( keyer.txEin == true)
    { si5351.clock_enable(SI5351_CLK0, 1);
      ist_an = true;
    }
  }
  if (  ist_an == true)
  {
    if ( keyer.txEin == false)
    { si5351.clock_enable(SI5351_CLK0, 0);
      ist_an = false;
    }
  }
}

