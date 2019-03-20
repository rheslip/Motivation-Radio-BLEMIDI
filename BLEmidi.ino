/*
 Bluetooth Low Energy MIDI interface for Motivation Radio Eurorack ESP32 module
 R Heslip Nov 2018 

 This code implements MIDI over Bluetooth Low Energy

 The code creates a BLE MIDI service and characteristic, advertises the service and waits for a client connection
 it converts note on/off messages to gate outs and control voltages
 gate outs and CV outs are configurable in the menu. default is MIDI note to gate and CV:
 MIDI channel 1 = GATE0 +CV0
 MIDI channel 2 = GATE1 +CV1
 MIDI channel 3 = GATE2 +CV2
 MIDI channel 4 = GATE3 +CV3

 gate outs and CV outs will also respond to MIDI CC (continuous controller messages) if configured for that

 Motivation Radio with the BLEMIDI sketch is intended for use with iPhone and iPad apps - sequencers, controllers etc
 at this point the sketch is MIDI receive only since it will generally be used as a MIDI to CV and GATE converter
 
 TBD:
 implement more MIDI commands e.g. pitch bend
 add a menu system for configuration
 
 Dec 1/18 - incorporated BLEmidi decoder from the Pedalino project which resolved problem of notes getting dropped
 Dec 29/18 - code ported from prototype HW to Motivation Radio
 dec 30/18 - added serial midi handlers. units can be controlled by either BTMIDI or serial MIDI (both is not a good idea!) 
  BTMIDI in and serial MIDI in are both forwarded to serial MIDI out. this allows units to be cascaded for more CV and GATE outputs
  added CC message handler and the beginnings of a tweekable configuration
 Feb 16/19 - code restructure, added configuration menus                                
*/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <Wire.h>
#include "SPI.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "MIDI.h"
#include <menu.h>
#include <menuIO/clickEncoderIn.h>
#include <menuIO/keyIn.h>
#include <menuIO/chainStream.h>
#include <menuIO/serialOut.h>
#include <menuIO/adafruitGfxOut.h>
#include <menuIO/serialIn.h>
#include "MORAD_IO.h"
#include "DAC.h"
#include "HW_units.h"
#include "MORAD_utility.h"

//#include "menusystem.h"

void maindisplay(void); // function prototype needed for BT connect and disconnect

// create display device
#define OLED_RESET -1  // unused port - to keep the compiler happy
Adafruit_SSD1306 display(OLED_RESET);

struct SerialMIDISettings : public midi::DefaultSettings
{
  static const long BaudRate = 31250;
};
// must use HardwareSerial for extra UARTs
HardwareSerial MIDISerial(2);

// instantiate the serial MIDI library
MIDI_CREATE_CUSTOM_INSTANCE(HardwareSerial, MIDISerial, MIDI, SerialMIDISettings);

#include "MIDI_handlers.h"
#include "BT_MIDI_handlers.h"

// encoder 
ClickEncoder clickEncoder(ENC_A,ENC_B,ENC_SW,4); // divide by 4 works best with my encoder
ClickEncoderStream encStream(clickEncoder,1);
  
// interrupt timer defs
#define ENC_TIMER_MICROS 1000 // 1khz for encoder
hw_timer_t * timer0 = NULL;

// encoder timer interrupt handler at 1khz

void ICACHE_RAM_ATTR encTimer(){
  clickEncoder.service();    // check the encoder inputs
}

#include "menusystem.h"  // has to follow encoder instance and HW_units.h

void maindisplay(void) {
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextColor(WHITE);
  display.println("    MORAD BLEMIDI");
  display.println();
  if (!deviceConnected) display.println("  BT Not Connected");
  else display.println("    BT Connected");
  display.println("Press for Setup Menu");
  display.display();
}

//when menu is suspended
result idle(menuOut& o,idleEvent e) {
  maindisplay();
  return proceed;
}

void setup() {
  Serial.begin(115200);
  
  pinMode(DAC0_CS, OUTPUT);
  pinMode(DAC1_CS, OUTPUT);
  pinMode(ADC_CS, OUTPUT);  
  digitalWrite(DAC0_CS, HIGH);
  digitalWrite(DAC1_CS, HIGH);
  digitalWrite(ADC_CS, HIGH);
  pinMode(GATEout_0, OUTPUT);
  pinMode(GATEout_1, OUTPUT);
  pinMode(GATEout_2, OUTPUT);
  pinMode(GATEout_3, OUTPUT);
  
  pinMode(GATEin_0, INPUT);
  pinMode(GATEin_0, INPUT);
  pinMode(GATEin_0, INPUT);
  pinMode(GATEin_0, INPUT);

  SPI.begin(SCLK,MISO,MOSI,DAC0_CS); // we actually use a CS pin for each DAC
  SPI.setBitOrder(MSBFIRST);
  SPI.setFrequency(2000000); // ADC max clock 2 Mhz 
    
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.clearDisplay();
  display.println("   MORAD BLEMIDI");
  display.println();
  display.println("    Mar 19/2019");
  display.display();
  delay(5000);

//  Set up serial MIDI port
  MIDISerial.begin(31250, SERIAL_8N1, MIDIRX,MIDITX ); // midi port

  // set up serial MIDI library callbacks
  MIDI.setHandleNoteOn(HandleNoteOn);  // Put only the name of the function
  MIDI.setHandleNoteOff(HandleNoteOff);
  MIDI.setHandleControlChange(HandleControlChange);
  // Initiate serial MIDI communications, listen to all channels
  MIDI.begin(MIDI_CHANNEL_OMNI);

// set up BLEMIDI
  BLEDevice::init("MRAD1");  // NOTE - iOS does not like BT devices with long names
   Serial.println("BLE device created");   
  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
     Serial.println("BLE server created");  
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
 // BLEService *pService = pServer->createService(BLEUUID(SERVICE_UUID));
  BLEService* pService = pServer->createService("03b80e5a-ede8-4b33-a751-6ce34ec4c700");
  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic("7772e5db-3868-4112-a1a9-f2669d106bf3", 
                    BLECharacteristic::PROPERTY_READ   |
                    BLECharacteristic::PROPERTY_NOTIFY |
                    BLECharacteristic::PROPERTY_WRITE_NR
        );
  // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml

  pCharacteristic->setCallbacks(new MyCharacteristicCallback());
  // Create a BLE Descriptor
  pCharacteristic->addDescriptor(new BLE2902());

  // Start the service
  pService->start();
//  pCharacteristic->setValue(midiPacket, 5); // storage for incoming packet, length in bytes
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->addServiceUUID(pService->getUUID());
  pAdvertising->start();
  // Start advertising
 // pServer->getAdvertising()->start();

  // timer for encoder sampling
  timer0 = timerBegin(0, 80, true);
  timerAttachInterrupt(timer0, &encTimer, true);
  // Set alarm to trigger ISR (value in microseconds).
  // Repeat the alarm (third parameter)
  timerAlarmWrite(timer0, ENC_TIMER_MICROS, true);
  timerAlarmEnable(timer0);

    // set up the menu system
  nav.idleTask=idle;//point a function to be used when menu is suspended
  nav.idleOn(); // start up in idle state
  nav.navRoot::timeOut=30; // inactivity timeout

  maindisplay(); 
}

bool connectedstatus=false;  // flag to keep track of status display

void loop() {
  
  MIDI.read();  // do serial MIDI
  
  //do menus on a need to draw basis:
  nav.doInput();
  if (nav.changed(0)) {//only draw if changed
    nav.doOutput();
    display.display();
  }  
  if (deviceConnected != connectedstatus) { // check for BT connect or disconnect and update the main status display
    if (nav.sleepTask) maindisplay();  // update the main display only if menus are idle. if we are in the menus it will update on exit anyway
    connectedstatus=deviceConnected;
  }

}
