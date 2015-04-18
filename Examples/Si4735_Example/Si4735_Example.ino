/*
* Si4735 Example Sketch
* Written by Ryan Owens for SparkFun Electronics
*
* This example sketch illustrates how to use some of the basic commands in the
* Si4735 Library. The sketch will start the Si4735 in FM mode and tune to 97.3 MHz.
* The user can use a serial terminal to send characters (which correspond to basic commands) to the radio.
* The Si4735_SerialExample sketch allows you to send your own commands
* from the serial port. More information on the Si4735 chip can be found in the datasheet.
*
* HARDWARE SETUP:
* This sketch assumes you are using the Si4735 Shield from SparkFun Electronics. 
* The shield should be plugged into an Arduino Main Board (Uno, Duemillinove or similar).
*
* USING THE SKETCH:
* Once you've plugged the Si4735 Shield into your Arduino board, connect the Arduino to your computer
* and select the corresponding board and COM port from the Tools menu and upload the sketch. After
* the sketch has been updated, open the serial terminal using a 9600 bps baud speed. The sketch accepts
* single character commands (just enter the character and press 'send'). Here is a list of the acceptable commands:
* 8 - Increase the volume
* 2 - Decrease the volume
* 4 - Seek down to the next channel
* 6 - Seek up to the next channel
* m - Mute the radio
* u - Unmute the radio
* s - Get the current status of the Si4735 (learn more about the status response in the datasheet)
* r - Get the response of the latest command from the Si4735 (learn more about the command responses in the datasheet).
*
* NOTES:
* This sketch uses the Si4735 in FM mode. Other modes are AM, SW and LW. Check out the datasheet for more information on these
* modes. All of the functions in the library will work regardless of which mode is being used; however the user must indicate
* which mode is to be used in the begin() function. See the library documentation for more information.
*/

#include <Wire.h>
#include <ADG2128.h>
#include <TPA0172.h>
#include <PCF8575.h> // Required for PCF8575

// Define port expander I/O methods for RST and SEN pins.
// MUST be defined prior to si4707.h include file!
// Undefined, these will default to Standard Arduino methods.
#define IOMODE(...) expander1.pinMode(__VA_ARGS__)
#define IOWRITE(...) expander1.digitalWrite(__VA_ARGS__)

//Add the Si4735 Library to the sketch.
#include <Si4735.h>

PCF8575 expander;
PatchBay Matrix; 
TPA Amp;

//Create an instance of the Si4735 named radio.
Si4735 radio;

void setup()
{
  //Create a serial connection
  Serial.begin(9600);
  
  expander.begin(0x21);
  Amp.begin(0x6E,0x0B,0x00);
  Matrix.begin(0x70);
  
  Matrix.set(X0,Y0,ON);
  Matrix.set(X1,Y1,ON);

  //Amp.get_registers();

  //Initialize the radio to the FM mode (Possible modes are AM, FM, SW, LW).
  //The mode will set the proper bandwidth. Ensure that the antenna
  //switch on the shield is configured for the desired mode.
  Serial.println("1");
  radio.begin();
  Serial.println("2");
  initSi4735();
  Serial.println("3");
  radio.setMode(FM);
  Serial.println("4");
  //Set the FM Frequency to 97.3 MHz
  radio.tuneFrequency(9990);
  Serial.println("Ready");
}

void loop()
{       
  //Wait until a character comes in on the Serial port.
  if(Serial.available()>0){
    //Decide what to do based on the character received.
    switch(Serial.read()){
      //If we get the number 8, turn the volume up.
      case '8':radio.volumeUp();
        break;
      //If we get the number 2, turn the volume down.
      case '2':radio.volumeDown();
        break;
      //If we get the number 4, seek down to the next channel in the current bandwidth (wrap to the top when the bottom is reached).
      case '4':radio.seekDown();
        break;
      //If we get the number 6, seek up to the next channel in the current bandwidth (wrap to the bottom when the top is reached).
      case '6':radio.seekUp();
        break;
      //If we get the letter m, mute the radio.
      case 'M':
      case 'm':radio.mute();
        break;
      //If we get the letter u, unmute the radio.
      case 'U':
      case 'u':radio.unmute();
        break;
      //If we get the letter s, print the current status of the radio.
      case 'S':
      case 's':Serial.print((unsigned char)radio.getStatus(), HEX);
      default:
        break;
    }
  }   
}

void initSi4735()
{
  // Set initial pin value: RST (Active-low reset)
  
  expander.pinMode(13, OUTPUT);  // Reset
  expander.digitalWrite(13, LOW);  // Keep the SI4735 in reset
  
  
  delay(10);
  // Set initial pin values: SDIO and SCLK (serial data and clock lines)
  // Wire.begin() will take care of this
  Wire.begin();
  delay(10);  // Short delay before we take reset up
  
  // Raise RST, SCLK must not rise within 300ns before RST rises
  expander.digitalWrite(13, HIGH);
  delay(10);  // Give Si4707 a little time to reset
  
  expander.pinMode(3, INPUT_PULLUP);  // GPIO2
  
  return;    
}
