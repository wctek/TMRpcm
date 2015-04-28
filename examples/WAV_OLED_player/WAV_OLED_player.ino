/* modified by A.Sardo for Piccolino - March 2015 */

#include <SD.h>                      // need to include the SD library
#define SD_ChipSelectPin 5           // Piccolino SD Card
#include <TMRpcm.h>                  //  also need to include this library...
#include <SPI.h>
#include <Wire.h>
#include <Piccolino_RAM.h>
#include <Piccolino_OLED_SRAM.h>
#include "WAV_OLED_player.h"

TMRpcm tmrpcm;   // create an object for use in this sketch
Piccolino_OLED_SRAM display;

unsigned long time = 0;

void setup(){

  display.begin();

  tmrpcm.speakerPin = 9; 
  //Complimentary Output for Dual Speakers:
  //pinMode(10,OUTPUT); Pin pairs: 9,10 
  
  Serial.begin(9600);
  
  if (!SD.begin(SD_ChipSelectPin)) {  // see if the card is present and can be initialized:
    Serial.println(F("SD fail"));  
    return;   // don't do anything more if not

  }
  else{   
    Serial.println(F("SD ok"));   
  }
 
  Serial.println(F("\nOptions"));
  Serial.println(F("m : play"));
  Serial.println(F("p : pause"));
  Serial.println(F("s : stop"));
  Serial.println(F("? : is music playing?"));
  Serial.println(F("= : volume up"));
  Serial.println(F("- : volume down"));
  Serial.println(F("0 : quality low"));
  Serial.println(F("1 : quality high\n"));  

  drawBitmap(0,0,img1,64,62);
  display.setTextSize(1);
  display.setCursor(68,20);
  display.print("PLAYING");
  display.setTextSize(2);
  display.setCursor(60,30);
  display.print("BANJO");
  display.update();
  
  tmrpcm.play("banjo.wav"); //the sound file "music" will play each time the arduino powers up, or is reset

}


void drawBitmap(int16_t x, int16_t y, const unsigned char *bitmap, int16_t w, int16_t h) {
  int16_t i, j, byteWidth = (w + 7) / 8;
  for(j=0; j<h; j++) {
    for(i=0; i<w; i++ ) {
      if(pgm_read_byte(bitmap + j * byteWidth + i / 8) & (128 >> (i & 7))) {
        display.drawPixel(x+i, y+j, WHITE);
      }
    }
  }
}

void loop(){  


  char f;
  
  //do something to show that music is playing in the background without halting execution of main loop
  if(tmrpcm.isPlaying()) {
    if(millis() - time > 1000 ) {      

      time = millis();    
    }
  } else
      if(time) {
        Serial.println();
        time=0;
      }

if(!tmrpcm.isPlaying()) {
  tmrpcm.stopPlayback();
  display.clear();
  drawBitmap(0,0,img1,64,62);
  display.update();
  pinMode(9,INPUT); // turn off noise!
}

  if(Serial.available()){    
    switch(Serial.read()){
    case 'm': pinMode(9,OUTPUT); tmrpcm.play("banjo.wav"); break;
    case 'p': tmrpcm.pause(); break;
    case '?': if(tmrpcm.isPlaying()){ Serial.println(F("A wav file is being played"));} else Serial.println(F("No wav playing")); break;
    case 's': tmrpcm.stopPlayback(); break;
    case '=': tmrpcm.volume(1); break;
    case '-': tmrpcm.volume(0); break;
    case '0': tmrpcm.quality(0); break;
    case '1': tmrpcm.quality(1); break;
    default: break;
    }
  }
}
