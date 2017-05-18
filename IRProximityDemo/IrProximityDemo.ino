#include <TimerOne.h>

const uint8_t IrRxPin = 2;
const uint8_t IrTxTonePin = 3;
const uint8_t IrTxDataPin = 12;
const unsigned long IrTxTime = 5000;

unsigned int irTxFrequency = 38000;

// toggle the IrTxDataPin each time called
// this is attached to TimerOne interrupts,
// that occur every IrTxTime microseconds
void ping() {
  static bool pingState = false;
  static uint8_t highDelayCount = 0;
  const uint8_t HighDelay = 10;

  if (pingState) {
    pingState = false;
    digitalWrite(IrTxDataPin, LOW);
  }
  else {
    if (highDelayCount++ > HighDelay) {
      pingState = true;
      digitalWrite(IrTxDataPin, HIGH);    
      highDelayCount = 0;
    }
  }
  
}

void setup() {
  // configure the pins
  pinMode(IrRxPin, INPUT);
  pinMode(IrTxTonePin, OUTPUT);
  digitalWrite(IrTxDataPin, LOW);
  pinMode(IrTxDataPin, OUTPUT);

  tone(IrTxTonePin, irTxFrequency);

  // set up TimerOne timebase to IrTxTime microseconds
  Timer1.initialize(IrTxTime);
  // attach ping() function to the TimerOne interrupt
  Timer1.attachInterrupt(ping);

  Serial.begin(57600);
  Serial.println("Initialized");
  Serial.print("Enter Frequency: ");
    
}

void loop() {
  static String freqString = "";
  static uint8_t count = 0;

  while (Serial.available() > 0) {
    char c = Serial.read();
    if (isDigit(c)) {
      freqString += c;
    }
    else if (c == '\n') {
      Serial.println();
      Serial.print("Frequency: ");
      irTxFrequency = freqString.toInt();
      Serial.println(irTxFrequency);
      tone(IrTxTonePin, irTxFrequency);
      freqString = "";
    }
  }

  if (digitalRead(IrRxPin) == LOW) {
    Serial.print("Ping ");
    Serial.println(count++);
  }
}
