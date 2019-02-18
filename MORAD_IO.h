
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
// I/O pin definitions for Motivation Radio Eurorack module based on ESP32 DEV board 

#ifndef MOTIVATION_RADIO_H_
#define MOTIVATION_RADIO_H_

// SPI pins
#define MISO 12
#define MOSI 13
#define SCLK 14

// DAC chip select pins
#define DAC0_CS 33
#define DAC1_CS 32

// ADC chip select pin
#define ADC_CS 15

// I2C pins
#define SDA 21
#define SCL 22

// GATE output pins
#define GATEout_0 25
#define GATEout_1 26
#define GATEout_2 4
#define GATEout_3 27

// GATE input pins 
#define GATEin_0 36
#define GATEin_1 39
#define GATEin_2 34
#define GATEin_3 35

// encoder pins
#define ENC_A   5
#define ENC_B   18
#define ENC_SW  19

// MIDI serial port pins
#define MIDIRX 16
#define MIDITX 17

#endif // MOTIVATION_RADIO_H_
