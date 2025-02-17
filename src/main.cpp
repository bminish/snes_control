#include <Arduino.h>

// Function declarations
unsigned short getButtons(const unsigned short nButtons,
                          const unsigned short erclock,
                          const unsigned short latch,
                          const unsigned short data);
void showDiff(const unsigned short curr, const unsigned short prev);

unsigned short curr, prev;

// Button setup
const unsigned short nButtons = 12;
const unsigned short erclock = 19; // blue
const unsigned short latch = 18;    // yellow
const unsigned short data = 5;     // red
// ground -> black
// 5v -> white

// Names of buttons
const char *name[] = {"B",      "Y",     "Select", "Start",
                      "Up",     "Down",  "Left",   "Right",
                      "A",      "X",     "L",      "R"};

unsigned short getButtons(const unsigned short nButtons,
                          const unsigned short erclock,
                          const unsigned short latch,
                          const unsigned short data) {
  // Refresh data
  digitalWrite(latch, HIGH);
  digitalWrite(latch, LOW);

  unsigned short buttons = 0;
  unsigned short swap = 1;
  for (unsigned short i = 0; i < nButtons; ++i) {
    // Switch to next button
    digitalWrite(erclock, LOW);

    // Check if pressed
    if (digitalRead(data) == LOW) {
      // Toggle bit if read
      buttons |= swap;
    }
    swap <<= 1;

    digitalWrite(erclock, HIGH);
  }
  return buttons;
}

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  pinMode(erclock, OUTPUT); // erclock (blue)
  pinMode(latch, OUTPUT);   // latch (yellow)
  pinMode(data, INPUT_PULLDOWN);     // data (red)

  // Get initial state
  curr = getButtons(nButtons, erclock, latch, data);
  prev = curr;
}

void loop() {
  curr = getButtons(nButtons, erclock, latch, data);
  if (curr != prev) {
    // If different, print changes
    showDiff(curr, prev);
    prev = curr;
  }
}

void showDiff(const unsigned short curr, const unsigned short prev) {
  // Bitwise XOR to find differences
  unsigned short diff = curr ^ prev;

  // Print buttons that changed state
  for (unsigned short i = 0; i < nButtons; ++i) {
    if (diff & (1 << i)) {
      Serial.print(name[i]);
      Serial.print(": ");
      if (curr & (1 << i))
        Serial.println("pressed");
      else
        Serial.println("released");
    }
  }
}
