// Copyright 2019 Rich Heslip
//
// Author: Rich Heslip 
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// 
// See http://creativecommons.org/licenses/MIT/ for more information.
//
// -----------------------------------------------------------------------------
//
// Bluetooth MIDI handlers for Motivation Radio Eurorack module based on ESP32 DEV board 

#ifndef MOTIVATION_RADIO_BT_MIDI_H_
#define MOTIVATION_RADIO_BT_MIDI_H_

// bluetooth MIDI stuff

#define SERVICE_UUID        "03b80e5a-ede8-4b33-a751-6ce34ec4c700"
#define CHARACTERISTIC_UUID "7772e5db-3868-4112-a1a9-f2669d106bf3"

BLECharacteristic *pCharacteristic;
bool deviceConnected = false;


class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

// decode Bluetooth MIDI commands and dispatch to MIDI handlers

void MIDIaction(byte command, byte data1, byte data2, byte channel) {
      if ((command==0x90) && (data2==0)) command=0x80; // sometimes note on with velocity 0 is used as note off - if so change the command to note off
        
        switch (command) { // midi command dispatch
          case 0x90:    // note on msg
            HandleNoteOn(channel+1,data1,data2);  // we add 1 to the channel number because BTMIDI channels are 0-15 vs 1-16 for serial MIDI library
            break;  
          case 0x80:    // note off msg
            HandleNoteOff(channel+1,data1,data2);
            break;
          case 0xB0:
            HandleControlChange(channel+1,data1,data2);
            break;
        }
    MIDI.send((midi::MidiType)command,data1,data2,(midi::Channel)(channel+1)); // forward BTMIDI messages to serial midi out. 
    // channel+1 because serial MIDI lib uses channels 1-16
 /*   
   Serial.print((midi::MidiType)command,HEX);
   Serial.print(' ');  
   Serial.print(data1);  
   Serial.print(' ');  
   Serial.print(data2);
   Serial.print(' ');  
   Serial.println((midi::Channel)channel);   
 */ 
}

// Decodes the BLE characteristics and calls MIDIaction if the packet contains actionable MIDI data
// https://learn.sparkfun.com/tutorials/midi-ble-tutorial

void BLEMidiReceive(uint8_t *buffer, uint8_t bufferSize)
{
  /*
    The general form of a MIDI message follows:

    n-byte MIDI Message
      Byte 0            MIDI message Status byte, Bit 7 is Set to 1.
      Bytes 1 to n-1    MIDI message Data bytes, if n > 1. Bit 7 is Set to 0
    There are two types of MIDI messages that can appear in a single packet: full MIDI messages and
    Running Status MIDI messages. Each is encoded differently.
    A full MIDI message is simply the MIDI message with the Status byte included.
    A Running Status MIDI message is a MIDI message with the Status byte omitted. Running Status
    MIDI messages may only be placed in the data stream if the following criteria are met:
    1.  The original MIDI message is 2 bytes or greater and is not a System Common or System
        Real-Time message.
    2.  The omitted Status byte matches the most recently preceding full MIDI message�s Status
        byte within the same BLE packet.
    In addition, the following rules apply with respect to Running Status:
    1.  A Running Status MIDI message is allowed within the packet after at least one full MIDI
        message.
    2.  Every MIDI Status byte must be preceded by a timestamp byte. Running Status MIDI
        messages may be preceded by a timestamp byte. If a Running Status MIDI message is not
        preceded by a timestamp byte, the timestamp byte of the most recently preceding message
        in the same packet is used.
    3.  System Common and System Real-Time messages do not cancel Running Status if
        interspersed between Running Status MIDI messages. However, a timestamp byte must
        precede the Running Status MIDI message that follows.
    4.  The end of a BLE packet does cancel Running Status.
    In the MIDI 1.0 protocol, System Real-Time messages can be sent at any time and may be
    inserted anywhere in a MIDI data stream, including between Status and Data bytes of any other
    MIDI messages. In the MIDI BLE protocol, the System Real-Time messages must be deinterleaved
    from other messages � except for System Exclusive messages.
  */
  byte   channel;
  byte  command;

  //Pointers used to search through payload.
  byte lPtr = 0;
  byte rPtr = 0;
  //Decode first packet -- SHALL be "Full MIDI message"
  lPtr = 2; //Start at first MIDI status -- SHALL be "MIDI status"
  //While statement contains incrementing pointers and breaks when buffer size exceeded.
  while (1) {
    //lastStatus used to capture runningStatus
    byte lastStatus = buffer[lPtr];
    if ( (buffer[lPtr] < 0x80) ) {
      //Status message not present, bail
      return;
    }
    //command = MIDI.getTypeFromStatusByte(lastStatus);
    //channel = MIDI.getChannelFromStatusByte(lastStatus);
    command = lastStatus & 0xf0;
    channel = lastStatus & 0x0f;    
    //Point to next non-data byte
    rPtr = lPtr;
    while ( (buffer[rPtr + 1] < 0x80) && (rPtr < (bufferSize - 1)) ) {
      rPtr++;
    }
    //look at l and r pointers and decode by size.
    if ( rPtr - lPtr < 1 ) {
      //Time code or system
      MIDIaction(command, 0, 0, channel);
    } else if ( rPtr - lPtr < 2 ) {
      MIDIaction(command, buffer[lPtr + 1], 0, channel);
    } else if ( rPtr - lPtr < 3 ) {
      MIDIaction(command, buffer[lPtr + 1], buffer[lPtr + 2], channel);
    } else {
      //Too much data
      //If not System Common or System Real-Time, send it as running status
      switch ( buffer[lPtr] & 0xF0 )
      {
        case 0x80:
        case 0x90:
        case 0xA0:
        case 0xB0:
        case 0xE0:
          for (int i = lPtr; i < rPtr; i = i + 2) {
            MIDIaction(command, buffer[i + 1], buffer[i + 2], channel);
          }
          break;
        case 0xC0:
        case 0xD0:
          for (int i = lPtr; i < rPtr; i = i + 1) {
            MIDIaction(command, buffer[i + 1], 0, channel);
          }
          break;
        default:
          break;
      }
    }
    //Point to next status
    lPtr = rPtr + 2;
    if (lPtr >= bufferSize) {
      //end of packet
      return;
    }
  }
}
// incoming midi packet handler

class MyCharacteristicCallback : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* pChar){
      std::string rxValue = pCharacteristic->getValue();
      if (rxValue.length() > 0) {
          BLEMidiReceive((uint8_t *)(rxValue.c_str()), rxValue.length());
          //Serial.print("BLE Received %2d bytes", rxValue.length());  
      }
    }
};



#endif  // MOTIVATION_RADIO_BT_MIDI_H_
