const bool logging=true;

#include "synthesizer.h"
#include "display.h"
#include "eth.h"

void setup() {
  if (logging) { Serial.begin(115200); }

  initSynth();
  initDisplay();
  initEth(); }

void loop() { synthWorker(); displayWorker(); ethWorker(); }
