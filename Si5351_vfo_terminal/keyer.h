#ifndef _keyer_h
#define _keyer_h

#include <arduino.h>

// Copyright (c) dk2jk
class Keyer {

public:
  enum EKeyer{
    ruhelage,
    tx_verzoegerung_am_anfang,
    dit_ein,
    dit_zwischenraum,
    dah_ein,
    dah_zwischenraum,
    tx_verzoegerung_am_ende,
  }; 

//constructor..
  Keyer(  unsigned char wpm= 12, 
          unsigned char delay_start=100, 
          unsigned char delay_end=100);
  void run();
  void update_wpm(unsigned char wpm);
  unsigned char  delay_start;
  unsigned char  delay_end; 
  unsigned char  t_dit;
  bool txEin;
  bool txFreigabe;
  EKeyer state; 
private:
  bool dit_paddle();  
  bool dah_paddle();
  bool punkt_speicher;
};

#endif

