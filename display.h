#include <TFT_eSPI.h>
TFT_eSPI tft=TFT_eSPI();

#include <XPT2046_Touchscreen.h>
XPT2046_Touchscreen ts(0,5);

#include <Encoder.h>
Encoder knobLeft(24,9);
Encoder knobRight(28,25);
#define buttonLeft 6
#define buttonRight 29

const char *knobText[32]={"Attack","Hold","Decay","Sustain","Release","Arpeggiator Mode","Arpeggiator Speed","Glissando Speed",\
  "VCO 1 Waveform","VCO 2 Waveform","VCO Ratio","VCO Add/Multiply","VCO 2 Shift","VCO Level","","",\
  "LFO VCO Waveform","LFO VCO Phase Start","LFO VCO Level","LFO VCO Frequency","VCF Bypass","VCF Lowpass","VCF Bandpass","VCF Highpass",\
  "LFO VCF Waveform","LFO VCF Phase Start","LFO VCF Level","LFO VCF Frequency","VCF Frequency","VCF Resonance","",""};
uint8_t knobValue[32]={10,0,0,127,20,0,127,0,40,70,64,64,120,70,0,0,0,0,0,0,0,127,0,0,0,30,70,10,50,40,0,0};

void setDisplay(uint8_t knob) {
  static uint8_t oldKnob;
  if (oldKnob!=knob) {
    tft.fillRoundRect(oldKnob%4*120,oldKnob/4*40,120,40,10,TFT_WHITE);
    tft.setTextColor(TFT_BLACK,TFT_WHITE);
    tft.setCursor(oldKnob%4*120+3,oldKnob/4*40+9);
    tft.print(knobText[oldKnob]);
    tft.setCursor(oldKnob%4*120+3,oldKnob/4*40+22);
    tft.print(knobValue[oldKnob]); tft.print("  ");
    tft.fillRoundRect(knob%4*120,knob/4*40,120,40,10,TFT_SKYBLUE);
    tft.setTextColor(TFT_BLACK,TFT_SKYBLUE);
    tft.setCursor(knob%4*120+3,knob/4*40+9);
    tft.print(knobText[knob]); }
  tft.setCursor(knob%4*120+3,knob/4*40+22);
  tft.print(knobValue[knob]); tft.print("  ");
  oldKnob=knob; }

void setTFTBL(bool blActive=false) {
  static uint64_t blTimer=millis()+60000UL;
  static uint8_t blValue=255;
  if (blActive) { blTimer=millis()+60000UL; blValue=255; analogWrite(TFT_BL,blValue); }
  if (millis()>=blTimer) {
    if (blValue<10) { blValue=0; } else { blValue-=10; blTimer=millis()+50; }
    analogWrite(TFT_BL,blValue); } }

void initDisplay() {
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_WHITE);

  ts.begin(SPI1);
  ts.setRotation(1);

  pinMode(buttonLeft,INPUT_PULLUP);
  pinMode(buttonRight,INPUT_PULLUP);

  for (uint8_t i=0;i<32;i++) { MIDIsetControl(0,i,knobValue[i]); setDisplay(i); }
  setDisplay(0); }

void displayWorker() {
  setTFTBL();

  if (ts.touched()) {
    setTFTBL(true);
    TS_Point p=ts.getPoint(); }

  static uint64_t buttonLeftTimer;
  if (!digitalRead(buttonLeft) && millis()>=buttonLeftTimer) {
    buttonLeftTimer=millis()+500;
    if (seq.playing) { stopPlayingSequence(); } else { startPlayingSequence(); } }

  static uint64_t buttonRightTimer;
  if (!digitalRead(buttonRight) && millis()>=buttonRightTimer) {
    buttonRightTimer=millis()+500;
    if (seq.recording) { stopRecordingSequence(); } else { startRecordingSequence(); } }

  static uint8_t knob=0;
  static long oldLeft=0;
  static long oldRight=0;
  long newLeft=knobLeft.read()/4;
  long newRight=knobRight.read()/4;
  knob+=newLeft-oldLeft;
  knobValue[knob]+=newRight-oldRight;
  knob=knob&0x1F;
  knobValue[knob]=knobValue[knob]&0x7F;
  if (oldLeft!=newLeft || oldRight!=newRight) { setTFTBL(true); setDisplay(knob); }
  if (oldRight!=newRight) { MIDIsetControl(0,knob,knobValue[knob]); }
  oldLeft=newLeft; oldRight=newRight;

  if (ethConfigured) {
    if (Udp.parsePacket()==2) {
      char receiveBuffer[2];
      Udp.read(receiveBuffer,2);
      knob=receiveBuffer[0];
      if (receiveBuffer[1]==1) { knobValue[knob]+=32; } else
      if (receiveBuffer[1]==2) { knobValue[knob]+=1; } else
      if (receiveBuffer[1]==3) { knobValue[knob]-=1; }
      knobValue[knob]=knobValue[knob]&0x7F;
      setTFTBL(true);
      setDisplay(knob); MIDIsetControl(0,knob,knobValue[knob]); } } }
