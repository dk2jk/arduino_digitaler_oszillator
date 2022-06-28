#include "keyer.h"
#include <arduino.h>
#include "timer.h"

#include "global_pins.h"

// Copyright (c) dk2jk
//Class
Timer    timer;

//constructor..
Keyer::Keyer( unsigned char  wpm,
unsigned char xdelay_start, unsigned char xdelay_end)
{


  pinMode(PIN_KEYER_DID, INPUT_PULLUP);
  pinMode(PIN_KEYER_DAH, INPUT_PULLUP);

  //default-werte
  delay_start = xdelay_start;
  delay_end   = xdelay_end; 
  update_wpm( wpm);
  state = ruhelage;
}

void Keyer::update_wpm(unsigned char wpm)
{
  t_dit  = 1200/wpm;
}


void Keyer::run(){
  switch( state)
  {
  case ruhelage:
    // warten, bis taste betaetigt wird  
    //  wenn ja, so startet die einschaltverzoegerung.
    //  tx wird freigegeben, aber noch nicht getastet
    if( dit_paddle() ||  dah_paddle() ) { // ereignis 1: dit- oder dah-taste betaetigt
      txFreigabe=1; 
      timer.interval(delay_start);
      state = tx_verzoegerung_am_anfang;
    }
    break;

  case tx_verzoegerung_am_anfang:  
    // einschaltverzoegerung abwarten
    // dann prüfen, ob dit oder dah taste gedrueckt wurde
    // falls taste zu kurz betaetigt wurde, wieder
    // in ruhelage zurueckkehren
    if( timer.event() ) {
      if (dit_paddle()){  // ereignis 1: dit taste betaetigt
        timer.interval(t_dit);
        txEin=1;
        state = dit_ein;
      }       
      else if (dah_paddle() ) { // ereignis 2: dah taste betaetigt
        timer.interval( 3 * (int) t_dit );
        txEin=1;
        state = dah_ein;
      }
      else { // ereignis 3: keine taste betaetigt
        txFreigabe=0;
        state = ruhelage;
      }
      break;

    case dit_ein: 
      // sender ist getastet fuer 1 zeiteinheit
      if ( timer.event() ) { // ereignis 1: zeit ist um
        txEin=0;
        timer.interval( t_dit );
        state = dit_zwischenraum; 
      }
      break;

    case dit_zwischenraum: 
      // sender ist aus fuer 1 zeiteinheit  
      // dadurch, dass jetzt dah-taste zuerst abgefragt wird,
      // kommen beim druecken beider tasten wechselnd dits und dahs heraus
      if ( timer.event() ) {
        if (dah_paddle()) {// ereignis 1: dah-taste gedrueckt
          timer.interval( 3 * t_dit );
          txEin=1;
          state = dah_ein;
        }
        else  if (dit_paddle()){// ereignis 2: dit-taste gedrueckt
          timer.interval(t_dit);
          txEin=1;
          state = dit_ein;
        }
        else{ // ereignis 3: keine taste gedrückt
          timer.interval(delay_end);
          state= tx_verzoegerung_am_ende;
        }
      } 
      break;

    case dah_ein:
      // sender ist getastet fuer 3 zeiteinheiten
      if ( timer.event() ) {  // ereignis 1: zeit ist um
        txEin=0;
        timer.interval( t_dit );
        state = dah_zwischenraum; 
      }
      break;

    case dah_zwischenraum:
      // sender ist aus fuer 1 zeiteinheit  
      // dadurch, dass jetzt die dit-taste zuerst abgefragt wird,
      // kommen beim druecken beider tasten wechselnd dits und dahs heraus
      punkt_speicher = dit_paddle();
      if ( timer.event() ) {
        if ( dit_paddle() || punkt_speicher ) {
          punkt_speicher = 0;
          timer.interval(t_dit );
          txEin=1;
          state = dit_ein;
        }
        else if (dah_paddle()) {
          timer.interval( 3 * t_dit );
          txEin=1;
          state = dah_ein;
        }
        else{ //keine taste gedrückt
          timer.interval(delay_end);
          state= tx_verzoegerung_am_ende;
        }
      }
      break;

    case tx_verzoegerung_am_ende:
      // der sender ist aus, jedoch noch freigegeben
      // falls in der  ausschaltverzoegerung eine taste
      // betaetigt wird, bleibt der sender freigegeben
      // und fast sofort wieder mit dit oder dahs weitergemacht
      if ( timer.event() ) { // ereignis 1: nichts mehr los...
        //ganz aus
        txFreigabe=0; 

        state= ruhelage;
      }
      else {  
        if ( dit_paddle()|| dah_paddle()){ // ereignis 2: eine taste gedrueckt
          timer.interval(1);//sehr klein
          state = tx_verzoegerung_am_anfang;
        }
      }
      break;
    }                               
  }
  

}

bool Keyer::dit_paddle(){
  return (digitalRead(PIN_KEYER_DID) == LOW);
}

bool Keyer::dah_paddle(){
  return (digitalRead(PIN_KEYER_DAH) == LOW) ;
}























