/*
 A new Rick Roll: Play a well known theme song (Axel F) and beat a desk with a solenoid.
 By: Nathan Seidle
 SparkFun Electronics
 Date: August 2nd, 2014
 License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).
 
 This plays the Axel F theme and taps a solenoid at the same time.
 
 */

#include "pitches.h"

int line1[] = {
  NOTE_D4, 0, NOTE_F4, NOTE_D4, 0, NOTE_D4, NOTE_G4, NOTE_D4, NOTE_C4,
  NOTE_D4, 0, NOTE_A4, NOTE_D4, 0, NOTE_D4, NOTE_AS4, NOTE_A4, NOTE_F4,
  NOTE_D4, NOTE_A4, NOTE_D5, NOTE_D4, NOTE_C4, 0, NOTE_C4, NOTE_A3, NOTE_E4, NOTE_D4,
  0};

int line1_durations[] = {
  8, 8, 6, 16, 16, 16, 8, 8, 8, 
  8, 8, 6, 16, 16, 16, 8, 8, 8,
  8, 8, 8, 16, 16, 16, 16, 8, 8, 2,
  2};
//9, 9, 10, 2 = 29 total things


boolean playing = false;

//This controls how fast the song will play
//1 is slow. 2 is ok. 2.5 is really fast.
float playSpeed = 2;

int brightness = 0; //How bright the LED is
int fadeAmount = 5; //How many points to fade the LED by

//GPIO declarations
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
byte torpedo = 3; //This controls the solenoid. First avail channel on MOSFET shield
byte button = 7; //Any GPIO will do

byte audioPin = 10; //This is the audio. tone() doesn't care which pin.
byte audioShutdown = 6; //This turns the audio off while we're pulsing the LED

byte LED_low = 8; //Low side of the LED
byte LED = 9; //High side of LED. 
//I wasn't sure which was the anode/cathode so I hooked them both to GPIOs so I can fix 
//in software

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

void setup()
{
  pinMode(torpedo, OUTPUT);
  digitalWrite(torpedo, LOW);

  pinMode(audioShutdown, OUTPUT);
  digitalWrite(audioShutdown, LOW); //Turn off the audio

  pinMode(LED_low, OUTPUT);
  digitalWrite(LED_low, LOW); //Ground the cathode of the LED

  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW); //Turn off the LED for now

  pinMode(button, INPUT_PULLUP);
}

void loop()
{
  if(digitalRead(button) == LOW)
  {
    delay(100); //Small debounce
    while(digitalRead(button) == LOW) ; //Wait for user to stop pressing button

    if(playing == true)
    {
      playing = false; //Stop playing
    }
    else 
    {
      playing = true;
      doIt(); //Start the party
    }
  }

  analogWrite(LED, brightness);

  brightness += fadeAmount;

  if(brightness == 0 || brightness == 255) fadeAmount = -fadeAmount;

  for(int x = 0 ; x < 50 ; x++)
  {
    if(digitalRead(button) == LOW) break; //Ignore fading if you press the button
    delay(1);
  }
}

void doIt()
{
  digitalWrite(audioShutdown, HIGH); //Turn on the amplifier

  for (int thisNote = 0 ; thisNote < 29 ; thisNote++)
  {
    long startTime = millis(); //Used to time solenoid duration

    if(line1[thisNote] > 0)
    {
      digitalWrite(torpedo, HIGH); //Fire the torpedoes

      //delay(25); //Pretty short and soft

      delay(40); //Pretty good, solid
      digitalWrite(torpedo, LOW);

      if(digitalRead(button) == LOW) return;
    }

    int noteDuration = 1000/line1_durations[thisNote];
    tone(audioPin, line1[thisNote], noteDuration * playSpeed);

    if(digitalRead(button) == LOW) return;

    int pauseBetweenNotes = noteDuration * playSpeed;
    delay(pauseBetweenNotes);

    if(digitalRead(button) == LOW) return;

    noTone(audioPin); //Stop the tone playing
  }

  digitalWrite(audioShutdown, LOW); //Turn off the amplifier

}


