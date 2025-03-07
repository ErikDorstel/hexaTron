void startPlayingSequence() {
  if (seq.recording) { sequence[seq.position].note=255; seq.recording=false; }
  seq.position=0;
  seq.playing=true;
  seq.recording=false;
  seq.baseTimer=millis(); }

void stopPlayingSequence() {
  if (seq.playing) { seq.playing=false; }
  if (seq.recording) { sequence[seq.position].note=255; seq.recording=false; } }

void startRecordingSequence() {
  if (seq.recording) { sequence[seq.position].note=255; seq.recording=false; }
  seq.position=0;
  seq.playing=false;
  seq.recording=true;
  seq.baseTimer=millis(); }

void stopRecordingSequence() {
  if (seq.playing) { seq.playing=false; }
  if (seq.recording) { sequence[seq.position].note=255; seq.recording=false; } }

void sequenceWorker() {
  if (!seq.playing) { return; }
  if (sequence[seq.position].note==255) { stopPlayingSequence(); return; }
  if (millis()>=sequence[seq.position].timer+seq.baseTimer) {
    if (sequence[seq.position].note<128) { MIDIsetNoteOn(0,sequence[seq.position].note,sequence[seq.position].velocity); }
    else { MIDIsetNoteOff(0,sequence[seq.position].note-128,sequence[seq.position].velocity); }
    seq.position++; } }
