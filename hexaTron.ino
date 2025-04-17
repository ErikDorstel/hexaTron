const bool logging=true;

#include "synthesizer.h"
#include "sequencer.h"
#include "eth.h"
#include "display.h"

void setup() {
  if (logging) { Serial.begin(115200); }

  initSynth();
  initEth();
  initDisplay(); }

void loop() { synthWorker(); ethWorker(); displayWorker(); sequenceWorker(); }
