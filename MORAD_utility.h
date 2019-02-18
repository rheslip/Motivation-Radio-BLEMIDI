
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
// utility functions for Motivation Radio Eurorack module based on ESP32 DEV board 

// CVout - send a 12 bit value to the DAC
// there are 2 DACs, each with 2 channels of output
// channels 0,1 on first chip, 2,3 on 2nd chip
// channel = channel number 0-3
// value - 12 bit DAC value to write

#ifndef MOTIVATION_RADIO_UTILITY_H_
#define MOTIVATION_RADIO_UTILITY_H_

void CVout(int channel, unsigned value) {
  if (channel &2) digitalWrite(DAC1_CS, LOW); // assert correct chip select
  else digitalWrite(DAC0_CS, LOW);
  byte data = value >> 8;  // get to 8 bits of DAC value
  data = data & B00001111; // mask off 4 msbs which are control
  if (channel &1) data = data | B10110000; //2nd DAC channel, 2.048v mode
  else data = data | B00110000; //1st DAC channel, 2.048v mode
  SPI.transfer(data); // send  control bits and 4 MSBs of DAC value
  data = value;
  SPI.transfer(data);  // send low 8 bits of DAC value
  if (channel &2) digitalWrite(DAC1_CS, HIGH);
  else digitalWrite(DAC0_CS, HIGH);
}

// CVin - read MCP3204 12 bit ADC 
// input voltage of -5v = 0, +5V in = 4095
// channel 0-3

unsigned CVin(int channel) {
  uint8_t cmd=0x6 | channel >>2; // start bit, single ended mode
  uint16_t cmd2 = (channel << 14); // high 2 bits of channel number
  digitalWrite(ADC_CS, LOW);  
  SPI.transfer(cmd);
  int result =SPI.transfer16(cmd2) & 0xfff;
  digitalWrite(ADC_CS, HIGH); 
  result=-((result-4096)+1); // invert the result
  return result;
}

// write value to gate/trigger output 0 -3
//  

void GATEout(int channel, bool value) {
  switch (channel) {
    case 0:
       digitalWrite(GATEout_0,value); // 
       break;
    case 1:
       digitalWrite(GATEout_1,value); // 
       break;
    case 2:
       digitalWrite(GATEout_2,value); // 
       break;
    case 3:
       digitalWrite(GATEout_3,value); // 
       break;
  }
}

// convert MIDI note number to a DAC value
// conversion is DAC specific because each one has a slightly different scale and offset

unsigned MIDInote_to_DACvalue(byte notenumber, int cvnumber) {
  return (unsigned)(notenumber-LOWEST_NOTE)*(float)NOTE_STEP*(float)cvout[cvnumber].scale;
}


#endif // MOTIVATION_RADIO_UTILITY_H_
