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
// MIDI event handlers for Motivation Radio Eurorack module based on ESP32 DEV board 

#ifndef MOTIVATION_RADIO_MIDI_H_
#define MOTIVATION_RADIO_MIDI_H_

#define MIN_GATE_OFF_TIME 500 // minimum gate off time in uS. fix for apps that send very rapid note off-note on sequences

// MIDI command handlers

void HandleNoteOn(byte channel, byte note, byte velocity) {
  /*
  Serial.print("Serial Noteon ");
  Serial.print(channel);
   Serial.print(' ');  
   Serial.print(note);
   Serial.print(' ');  
   Serial.println(velocity); 
*/
  // handle channels set up for note to CV 
  for( int i=0 ; i<(NUM_CV_OUTS);++i) {  // scan through CV configs
    if ((cvout[i].MIDIchannel==channel) && (cvout[i].type==NOTE_CV)) {  // see if MIDI channel matches and type is note to CV 
       if ((note >= LOWEST_NOTE) &&( note<= HIGHEST_NOTE)) { // don't play notes out of DAC range
         CVout(i,MIDInote_to_DACvalue(note,i));  // set the CV 
       }
    }
  }

  for( int i=0 ; i<(NUM_GATE_OUTS);++i) {  // scan through gate configs
    if ((gateout[i].MIDIchannel==channel) && (gateout[i].type==NOTES_GATE)) {  // see if MIDI channel matches and type is note to gate on/off 
       if ((note >= LOWEST_NOTE) &&( note<= HIGHEST_NOTE)) { // don't play notes out of DAC range
         GATEout(i,1); //turn on the gate
       }
    }
    if ((gateout[i].MIDIchannel==channel) && (gateout[i].type==NOTE_TRIGGER)) {  // see if MIDI channel matches and type is note trigger
       if (note == gateout[i].CC_NOTE_num) { // trigger on a specific note
         GATEout(i,1); //turn on the gate
       }
    }
  }
}

void HandleNoteOff(byte channel, byte note, byte velocity) {
  for( int i=0 ; i<(NUM_GATE_OUTS);++i) {  // scan through HW unit configs
    if ((gateout[i].MIDIchannel==channel) && (gateout[i].type==NOTES_GATE)) {  // see if MIDI channel matches and type is note to gate on/off 
       if ((note >= LOWEST_NOTE) &&( note<= HIGHEST_NOTE)) { // don't play notes out of DAC range
         GATEout(i,0); //turn off the gate. 
         delayMicroseconds(MIN_GATE_OFF_TIME);  // kludgy fix for apps that send rapid note off-note on sequence
       }
    }
    if ((gateout[i].MIDIchannel==channel) && (gateout[i].type==NOTE_TRIGGER)) {  // see if MIDI channel matches and type is note trigger
       if (note == gateout[i].CC_NOTE_num) { // trigger on a specific note
         GATEout(i,0); //turn off the gate
         delayMicroseconds(MIN_GATE_OFF_TIME);
       }
    }
  }  
}


void HandleControlChange(byte channel, byte number, byte value) {
  // handle channels set up for CC to CV 
  for( int i=0 ; i<(NUM_CV_OUTS);++i) {  // scan through CV configs
    if ((cvout[i].MIDIchannel==channel) && (cvout[i].type==CC_CV)) {  // see if MIDI channel matches and type is CC to CV 
      CVout(i,value*32); // don't worry about CV accuracy - CC is only 7 bits anyway.
    }
  }

  for( int i=0 ; i<(NUM_GATE_OUTS);++i) {  // scan through gate configs
    if ((gateout[i].MIDIchannel==channel) && (gateout[i].type==CC_GATE)) {  // see if MIDI channel matches and type is CC to gate
       if (value< 64) GATEout(i,0); // CC < 64=gate low. 
       else GATEout(i,1); // CC > 64=gate high. 
    }
  }
}

#endif // MOTIVATION_RADIO_MIDI_H_
