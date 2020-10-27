#include <MIDI.h>
#include <Arduino_DebugUtils.h>

// Create and bind the MIDI interface to the default hardware Serial port
MIDI_CREATE_DEFAULT_INSTANCE();

// uncomment this for debugging via the LED
//#define MYDEBUG 1

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(19200);
  Debug.setDebugLevel(DBG_VERBOSE);
  Debug.print(DBG_WARNING, "starting the CC filter");
  blinkShortly();
  MIDI.begin(MIDI_CHANNEL_OMNI);  // Listen to all incoming messages
  MIDI.turnThruOff();
  MIDI.setHandleControlChange(ccFunc);
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

void ccFunc(byte channel, byte number, byte value) {
  // re-map the value from 127 to 0 for HB MP100 -> Headrush GB
  if (value == 127) {
    // down
    MIDI.sendControlChange(number, 127, channel);
    // up
    MIDI.sendControlChange(number, 0, channel);
  } else {
    // pass through
    MIDI.sendControlChange(number, value, channel);
  }
  
#ifdef MYDEBUG
  blinkNumberInReverseOrder(channel);
  blinkNumberInReverseOrder(number);
  blinkNumberInReverseOrder(value);
#else
  blinkShortly();
#endif
}

void blinkShortly() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);
}

// use when debugging
void blinkNumberInReverseOrder(byte number) {
  int delayMs = 300;
  while (number > 0) {
    byte lastDigit = number % 10;
    number = number / 10;

    if (lastDigit == 0) {
      delayMs = 100;
      lastDigit = 10;
    } else {
      delayMs = 300;
    }

    for (byte i = 0; i < lastDigit; i++) {
      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(delayMs);                       // wait for a second
      digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
      delay(delayMs);
    }
    delay(1000);
  }
  delay(3000);
}

void loop() {
  MIDI.read();
}
