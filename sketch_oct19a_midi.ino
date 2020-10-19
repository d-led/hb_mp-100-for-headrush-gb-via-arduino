#include <MIDI.h>
#include <Arduino_DebugUtils.h>

// Create and bind the MIDI interface to the default hardware Serial port
MIDI_CREATE_DEFAULT_INSTANCE();

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(19200);
  Debug.print(DBG_WARNING, "starting the CC filter");
  delay(500);
  MIDI.begin(MIDI_CHANNEL_OMNI);  // Listen to all incoming messages
  MIDI.setHandleControlChange(ccFunc);
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

void ccFunc(byte channel, byte number, byte value) {
  Debug.setDebugLevel(DBG_VERBOSE);
  MIDI.sendControlChange(number, value, channel);
  blinkShortly();
  //  blinkNumberInReverseOrder(channel);
  //  blinkNumberInReverseOrder(number);
  //  blinkNumberInReverseOrder(value);
}

void blinkShortly() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(300);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);
}

void blinkNumberInReverseOrder(byte number) {
  while (number > 0) {
    byte lastDigit = number % 10;
    number = number / 10;
    for (byte i = 0; i < lastDigit; i++) {
      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(300);                       // wait for a second
      digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
      delay(300);
    }
    delay(1000);
  }
  delay(3000);
}

void loop() {
  MIDI.read();
}
