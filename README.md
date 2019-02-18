Bluetooth MIDI sketch for the Motivation Radio Eurorack module

Motivation Radio is a general purpose eurorack module based on the ESP32 with:

	4 Gate/Trigger inputs

	4 CV inputs -5V to +5V range, 12 bit resolution

	4 Gate/Trigger outputs 0-6V

	4 CV outputs 0-6.144V 12 bit resolution

	128x32 OLED display

	Encoder with switch

	MIDI in and out with 1/8 jacks - MIDI in works with either polarity (Korg or Arturia), MIDI out is MMA standard (Korg)

	Bluetooth and Wifi are supported by the ESP32 module


Motivation Radio is intended mainly for interfacing eurorack to other devices (e.g. iPad) wirelessly but it can do more!

Gate inputs - currently unused in this sketch

CV inputs - currently unused in this sketch

Gate outputs - configurable in the menu as Note Gates, Triggers, or CC controlled Gates (see menu description below)

CV outputs - configurable in the menu as Note Control Voltages or CC controlled Control Voltages (see menu description below)

Bluetooth - the Bluetooth MIDI device name is "MRAD1". You can easily change this in the sketch. See the Wiki for helpful tips on getting Bluetooth working with your iOS device.

Serial MIDI in - works exactly the same way as Bluetooth MIDI. You can use both but this can lead to confusion!

Serial MIDI out - everything on Bluetooth MIDI AND serial MIDI in is forwarded to serial MIDI out.


Configuration - the configuration menu allows you to set the module's gate and CV outputs up as MIDI to CV converters, MIDI percussion triggers or MIDI CC controllers. On powerup the module defaults to MIDI to CV converters on MIDI channels 1-4.

Gate and CV out pairs are configured as logical "Units" A-D in the menus. Physical unit A is the left most on the front panel, D is on right side of panel. Each unit has a configuration submenu:

Gate mode - press button to toggle between:
	GATE - High on MIDI NOTE ON, low on MIDI NOTE off message. Use this mode when you want a MIDI to CV converter.
	TRIG on NOTE - High on specified MIDI NOTE ON, low on specified MIDI NOTE off. Set up note in GATE NOTE/CC# menu below. This mode is typically used for percussion triggers.
	CC to GATE - High when MIDI CC value > 63, low when MIDI CC value <=63. This allows you to control the gate from a CC message. CC message number set up in GATE NOTE/CC# menu below.
Gate MIDI CH - MIDI channel for this gate. For MIDI to CV conversion you will want this on the same channel as the unit's CV output.
Gate NOTE/CC# - the specific note or CC number to use with TRIG on NOTE or CC to GATE modes

CV mode - press button to toggle between:
	NOTE to CV - MIDI note number to control voltage out. Use this mode when you want a MIDI to CV converter.
	CC to CV - MIDI CC message to control voltage out. CC message number set up in GATE NOTE/CC# menu below.
CV MIDI CH - MIDI channel for this CV. For MIDI to CV conversion you will want this on the same channel as the unit's Gate output.
CV CC# - the specific CC number to use with CC to CV mode


Software Dependendencies:

Adafruit Graphics library and the SSD1306 driver https://github.com/adafruit/Adafruit-GFX-Library https://github.com/adafruit/Adafruit_SSD1306

Arduino Menu system https://github.com/neu-rah/ArduinoMenu

Clickencoder (included in the sketch library directory)

compiled with Arduino 1.85 with ESP32 Arduino installed. Use "ESP32 DEV Module" as the board. 

More info in the Wiki: https://github.com/rheslip/Motivation-Radio-BLEMIDI/wiki

Rich Heslip Feb 2019

