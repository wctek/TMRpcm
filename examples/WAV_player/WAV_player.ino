/* modified by A.Sardo for Piccolino - March 2015 */

#include <SD.h>                      // need to include the SD library
#define SD_ChipSelectPin 5           // Piccolino SD Card
#include <TMRpcm.h>                  //  also need to include this library...
#include <SPI.h>

TMRpcm tmrpcm;   // create an object for use in this sketch

unsigned long time = 0;

void setup(){

  tmrpcm.speakerPin = 9; 
  //Complimentary Output or Dual Speakers:
  //pinMode(10,OUTPUT); Pin pairs: 9,10 
  
  Serial.begin(9600);
  
  if (!SD.begin(SD_ChipSelectPin)) {  // see if the card is present and can be initialized:
    Serial.println("SD fail");  
    return;   // don't do anything more if not

  }
  else{   
    Serial.println("SD ok");   
  }
  tmrpcm.play("500miles.wav"); //the sound file "music" will play each time the arduino powers up, or is reset
  
  Serial.println("\nOptions");
  Serial.println("m : play");
  Serial.println("p : pause");
  Serial.println("s : stop");
  Serial.println("? : is music playing?");
  Serial.println("= : volume up");
  Serial.println("- : volume down");
  Serial.println("0 : quality low");
  Serial.println("1 : quality high\n");  
}



void loop(){  

  //do something to show that music is playing in the background without halting execution of main loop
  if(tmrpcm.isPlaying()) {
    if(millis() - time > 1000 ) {      
      Serial.print(":P ");
      time = millis();    
    }
  } else
      if(time) {
        Serial.println();
        time=0;
      }

if(!tmrpcm.isPlaying())
  pinMode(9,INPUT); // turn off noise!

  if(Serial.available()){    
    switch(Serial.read()){
    case 'm': pinMode(9,OUTPUT); tmrpcm.play("500miles.wav"); break;
    case 'p': tmrpcm.pause(); break;
    case '?': if(tmrpcm.isPlaying()){ Serial.println("A wav file is being played");} else Serial.println("No wav playing"); break;
    case 's': tmrpcm.stopPlayback(); break;
    case '=': tmrpcm.volume(1); break;
    case '-': tmrpcm.volume(0); break;
    case '0': tmrpcm.quality(0); break;
    case '1': tmrpcm.quality(1); break;
    default: break;
    }
  }

}