
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
// definitions for Arduino Menu System
// https://github.com/neu-rah/ArduinoMenu/tree/master/src/menuIO

//using namespace Menu;

#define gfxWidth 128
#define gfxHeight 32
#define fontX 6    // font width in pixels
#define fontY 10  // vertical text spacing
#define MAX_DEPTH 2 // max menu depth?
#define textScale 1

// save settings to eeprom - not yet implemented
result  savesetup(void) {
  return proceed;
}


// Gate A select mode
TOGGLE(gateout[0].type,subMenu_GATE_A_TYPE,"GATE_A: ",doNothing,noEvent,wrapStyle,
  VALUE("GATE",NOTES_GATE,doNothing,noEvent),
  VALUE("TRIG on NOTE",NOTE_TRIGGER,doNothing,noEvent),
  VALUE("CC to GATE",CC_GATE,doNothing,noEvent)
);

// Gate B select mode
TOGGLE(gateout[1].type,subMenu_GATE_B_TYPE,"GATE_B: ",doNothing,noEvent,wrapStyle,
  VALUE("GATE",NOTES_GATE,doNothing,noEvent),
  VALUE("TRIG on NOTE",NOTE_TRIGGER,doNothing,noEvent),
  VALUE("CC to GATE",CC_GATE,doNothing,noEvent)
);

// Gate C select mode
TOGGLE(gateout[2].type,subMenu_GATE_C_TYPE,"GATE_C: ",doNothing,noEvent,wrapStyle,
  VALUE("GATE",NOTES_GATE,doNothing,noEvent),
  VALUE("TRIG on NOTE",NOTE_TRIGGER,doNothing,noEvent),
  VALUE("CC to GATE",CC_GATE,doNothing,noEvent)
);

// Gate D select mode
TOGGLE(gateout[3].type,subMenu_GATE_D_TYPE,"GATE_D: ",doNothing,noEvent,wrapStyle,
  VALUE("GATE",NOTES_GATE,doNothing,noEvent),
  VALUE("TRIG on NOTE",NOTE_TRIGGER,doNothing,noEvent),
  VALUE("CC to GATE",CC_GATE,doNothing,noEvent)
);

// CV A select mode
TOGGLE(cvout[0].type,subMenu_CV_A_TYPE,"CV_A: ",doNothing,noEvent,wrapStyle,
  VALUE("NOTE to CV",NOTE_CV,doNothing,noEvent),
  VALUE("CC to CV",CC_CV,doNothing,noEvent)
);

// CV B select mode
TOGGLE(cvout[1].type,subMenu_CV_B_TYPE,"CV_B: ",doNothing,noEvent,wrapStyle,
  VALUE("NOTE to CV",NOTE_CV,doNothing,noEvent),
  VALUE("CC to CV",CC_CV,doNothing,noEvent)
);

// CV C select mode
TOGGLE(cvout[2].type,subMenu_CV_C_TYPE,"CV_C: ",doNothing,noEvent,wrapStyle,
  VALUE("NOTE to CV",NOTE_CV,doNothing,noEvent),
  VALUE("CC to CV",CC_CV,doNothing,noEvent)
);

// CV D select mode
TOGGLE(cvout[3].type,subMenu_CV_D_TYPE,"CV_D: ",doNothing,noEvent,wrapStyle,
  VALUE("NOTE to CV",NOTE_CV,doNothing,noEvent),
  VALUE("CC to CV",CC_CV,doNothing,noEvent)
);



// RH I had to add submenus for each unit setup
// the menu macros can't handle really large menu definitions

MENU(subMenu_UNIT_A_SETUP,"UNIT A",doNothing,noEvent,wrapStyle,
  SUBMENU(subMenu_GATE_A_TYPE),
  FIELD(gateout[0].MIDIchannel,"GATE_A MIDI CH ","",1,16,1,0,doNothing,noEvent,wrapStyle),
  FIELD(gateout[0].CC_NOTE_num,"GATE_A NOTE/CC# ","",0,127,1,0,doNothing,noEvent,wrapStyle),
  
  SUBMENU(subMenu_CV_A_TYPE),
  FIELD(cvout[0].MIDIchannel,"CV_A MIDI CH ","",1,16,1,0,doNothing,noEvent,wrapStyle),
  FIELD(cvout[0].CC_num,"CV_A CC# ","",0,127,1,0,doNothing,noEvent,wrapStyle),
  EXIT("<Back")
);

MENU(subMenu_UNIT_B_SETUP,"UNIT B",doNothing,noEvent,wrapStyle,
  SUBMENU(subMenu_GATE_B_TYPE),
  FIELD(gateout[1].MIDIchannel,"GATE_B MIDI CH ","",1,16,1,0,doNothing,noEvent,wrapStyle),
  FIELD(gateout[1].CC_NOTE_num,"GATE_B NOTE/CC# ","",0,127,1,0,doNothing,noEvent,wrapStyle),
  
  SUBMENU(subMenu_CV_B_TYPE),
  FIELD(cvout[1].MIDIchannel,"CV_B MIDI CH ","",1,16,1,0,doNothing,noEvent,wrapStyle),
  FIELD(cvout[1].CC_num,"CV_B CC# ","",0,127,1,0,doNothing,noEvent,wrapStyle),
  EXIT("<Back")
);

MENU(subMenu_UNIT_C_SETUP,"UNIT C",doNothing,noEvent,wrapStyle,
  SUBMENU(subMenu_GATE_C_TYPE),
  FIELD(gateout[2].MIDIchannel,"GATE_C MIDI CH ","",1,16,1,0,doNothing,noEvent,wrapStyle),
  FIELD(gateout[2].CC_NOTE_num,"GATE_C NOTE/CC# ","",0,127,1,0,doNothing,noEvent,wrapStyle),
  
  SUBMENU(subMenu_CV_C_TYPE),
  FIELD(cvout[2].MIDIchannel,"CV_C MIDI CH ","",1,16,1,0,doNothing,noEvent,wrapStyle),
  FIELD(cvout[2].CC_num,"CV_C CC# ","",0,127,1,0,doNothing,noEvent,wrapStyle),
  EXIT("<Back")
);

MENU(subMenu_UNIT_D_SETUP,"UNIT D",doNothing,noEvent,wrapStyle,
  SUBMENU(subMenu_GATE_D_TYPE),
  FIELD(gateout[3].MIDIchannel,"GATE_D MIDI CH ","",1,16,1,0,doNothing,noEvent,wrapStyle),
  FIELD(gateout[3].CC_NOTE_num,"GATE_D NOTE/CC# ","",0,127,1,0,doNothing,noEvent,wrapStyle),

  SUBMENU(subMenu_CV_D_TYPE),
  FIELD(cvout[3].MIDIchannel,"CV_D MIDI CH ","",1,16,1,0,doNothing,noEvent,wrapStyle),
  FIELD(cvout[3].CC_num,"CV_D CC# ","",0,127,1,0,doNothing,noEvent,wrapStyle), 
  EXIT("<Back")
);

// save setup not implemented yet
//   OP("Save Setup",savesetup,enterEvent), 

MENU(mainMenu,"        SETUP",doNothing,noEvent,wrapStyle,
  SUBMENU(subMenu_UNIT_A_SETUP),  
  SUBMENU(subMenu_UNIT_B_SETUP),  
  SUBMENU(subMenu_UNIT_C_SETUP),  
  SUBMENU(subMenu_UNIT_D_SETUP),       
  EXIT("<Exit Setup Menu")
 );
 
  // define menu colors --------------------------------------------------------
//  {{disabled normal,disabled selected},{enabled normal,enabled selected, enabled editing}}
//monochromatic color table
const colorDef<uint16_t> colors[] MEMMODE={
  {{WHITE,BLACK},{WHITE,BLACK,BLACK}},//bgColor
  {{BLACK,WHITE},{BLACK,WHITE,WHITE}},//fgColor
  {{BLACK,WHITE},{BLACK,WHITE,WHITE}},//valColor
  {{BLACK,WHITE},{BLACK,WHITE,WHITE}},//unitColor
  {{BLACK,WHITE},{WHITE,WHITE,WHITE}},//cursorColor
  {{BLACK,WHITE},{WHITE,BLACK,BLACK}},//titleColor
};

//the encoder button is a keyboard with only one key 
keyMap encBtn_map[]={{-ENC_SW,defaultNavCodes[enterCmd].ch}};  //negative pin numbers use internal pull-up, switch is on when low
keyIn<1> encButton(encBtn_map);   //1 is the number of keys

// menu system is driven by the encoder stream
MENU_INPUTS(in,&encStream);

MENU_OUTPUTS(out,MAX_DEPTH
  ,ADAGFX_OUT(display,colors,fontX,fontY,{0,0,gfxWidth/fontX,gfxHeight/fontY})
  ,NONE //must have 2 items at least if using this macro
);

NAVROOT(nav,mainMenu,MAX_DEPTH,in,out);


