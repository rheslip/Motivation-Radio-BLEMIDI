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
// DAC definitions for Motivation Radio Eurorack module based on ESP32 DEV board 

#ifndef MOTIVATION_RADIO_DAC_H_
#define MOTIVATION_RADIO_DAC_H_


// DAC uses 2.048v vref and is amplified 3x ie 6.144V full scale 
// the DACs/amps are not perfect and really should be calibrated in software
// 6v is 6 octaves so each note step is 4000/(6*12)=55.5555 counts. if we use 56 there is some error but its OK for a test
#define DAC_BITS 12
#define DAC_RANGE 4096
#define NOTE_STEP 55.5555
#define LOWEST_NOTE 24 // make C1 the lowest MIDI note ie 0V output
#define HIGHEST_NOTE 96 // highest MIDI note C7 ie 6V output

#endif  // MOTIVATION_RADIO_DAC_H_