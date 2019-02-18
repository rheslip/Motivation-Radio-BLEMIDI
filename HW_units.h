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
// hardware unit definitions for Motivation Radio Eurorack module based on ESP32 DEV board 

#ifndef MOTIVATION_RADIO_UNIT_H_
#define MOTIVATION_RADIO_UNIT_H_

#define NUM_GATE_OUTS 4 // number of gate outs on this device
#define NUM_CV_OUTS 4 // number of CV outs on this device  

// gate output modes
// NOTES_GATE mode converts MIDI note on and off messages on the specified MIDI channel to GATE on/off
// NOTE_TRIGGER outputs a trigger on a specific note number
// CC_GATE converts the specified CC messages on the specified MIDI channel to a GATE. CC < 64= GATE off, CC > 64 = GATE on
enum gatetype {NOTES_GATE,NOTE_TRIGGER,CC_GATE};

// gateinfo structure contains info about the gate outputs

struct gateinfo {
  int MIDIchannel;        // MIDI channel it responds to
  enum gatetype type;  // type ie responds to NOTE messages or CC messages
  int CC_NOTE_num;         // controller # for CC mode or note # for trigger mode
} gateout[NUM_GATE_OUTS] = {
// default for gates is note on/off, channels 1-4
  1,        // MIDI channel it responds to 
  NOTES_GATE,  // mode of operation
  20,       // CC # for CC type
  
  2,        // MIDI channel it responds to
  NOTES_GATE,  // mode of operation
  21,       // CC # for CC type
  
  3,        // MIDI channel it responds to
  NOTES_GATE,  // mode of operation
  22,       // CC # for CC type
  
  4,        // MIDI channel it responds to
  NOTES_GATE,  // mode of operation
  23       // CC # for CC type
};

// CV output modes
// NOTE_CV mode converts MIDI note on and off messages on the specified MIDI channel to note CV out
// CC_CV converts the specified CC messages on the specified MIDI channel to control CV out
enum cvtype {NOTE_CV,CC_CV};

// cvinfo structure contains info about the CV outputs

struct cvinfo {
  int MIDIchannel;        // MIDI channel it responds to
  enum cvtype type;  // type ie responds to NOTE messages or CC messages
  int CC_num;         // controller #
  float offset;           // DAC offset 
  float scale;            // DAC scale
} cvout[NUM_CV_OUTS] = {
  1,        // MIDI channel it responds to 
  NOTE_CV,  // mode of operation
  24,       // CC # for CC type
  0.0,      // DAC offset
  1.0,      // DAC scale factor
  
  2,        // MIDI channel it responds to
  NOTE_CV,  // mode of operation
  25,       // CC # for CC type
  0.0,      // DAC offset
  1.0,      // DAC scale factor
  
  3,        // MIDI channel it responds to
  NOTE_CV,  // mode of operation
  26,       // CC # for CC type
  0.0,      // DAC offset
  1.005,      // DAC scale factor
  
  4,        // MIDI channel it responds to
  NOTE_CV,  // mode of operation
  27,       // CC # for CC type
  0.0,      // DAC offset
  1.0      // DAC scale factor  
};

#endif  // MOTIVATION_RADIO_UNIT_H_
