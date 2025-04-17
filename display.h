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
  "VCO 1 Waveform","VCO 2 Waveform","VCO 1/2 Ratio","VCO 1/2 Add/Multiply","VCO 2 Shift","VCO 1+2 Level","","",\
  "LFO VCO Waveform","LFO VCO Phase Start","LFO VCO Level","LFO VCO Frequency","VCF Bypass","VCF Lowpass","VCF Bandpass","VCF Highpass",\
  "LFO VCF Waveform","LFO VCF Phase Start","LFO VCF Level","LFO VCF Frequency","VCF Frequency","VCF Resonance","",""};
int knobValue[32]={10,0,0,127,20,0,127,0,40,70,64,64,120,70,0,0,0,0,0,0,0,127,0,0,0,30,70,10,50,40,0,0};

void setDisplay(uint8_t newLeft) {
  for (byte i=0;i<32;i++) {
    if (i==newLeft) { tft.setTextColor(TFT_BLACK,TFT_WHITE); } else { tft.setTextColor(TFT_WHITE,TFT_BLACK); }
    tft.setCursor(i%4*120,i/4*35);
    tft.print(knobText[i]);
    tft.setTextColor(TFT_WHITE,TFT_BLACK);
    tft.setCursor(i%4*120,i/4*35+10);
    tft.print(knobValue[i]);
    tft.print("   "); } }

void initDisplay() {
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);

  ts.begin(SPI1);
  ts.setRotation(1);

  pinMode(buttonLeft,INPUT_PULLUP);
  pinMode(buttonRight,INPUT_PULLUP);

  for (int i=0;i<32;i++) { MIDIsetControl(0,i,knobValue[i]); }
  setDisplay(0); }

void displayWorker() {
  static uint64_t blTimer=millis()+60000UL;
  if (millis()>=blTimer) { digitalWrite(TFT_BL,!TFT_BACKLIGHT_ON); }

  if (ts.touched()) {
    blTimer=millis()+60000UL;
    digitalWrite(TFT_BL,TFT_BACKLIGHT_ON);
    TS_Point p=ts.getPoint(); }

  static uint64_t buttonLeftTimer;
  if (!digitalRead(buttonLeft) && millis()>=buttonLeftTimer) {
    buttonLeftTimer=millis()+500;
    if (seq.playing) { stopPlayingSequence(); } else { startPlayingSequence(); } }

  static uint64_t buttonRightTimer;
  if (!digitalRead(buttonRight) && millis()>=buttonRightTimer) {
    buttonRightTimer=millis()+500;
    if (seq.recording) { stopRecordingSequence(); } else { startRecordingSequence(); } }

  static int knob=0;
  static long oldLeft=0;
  static long oldRight=0;
  long newLeft=knobLeft.read()/4;
  long newRight=knobRight.read()/4;
  knob+=newLeft-oldLeft;
  knobValue[knob]+=newRight-oldRight;
  if (knob<0) { knob=31; }
  if (knob>31) { knob=0; }
  if (knobValue[knob]<0) { knobValue[knob]=127; }
  if (knobValue[knob]>127) { knobValue[knob]=0; }
  if (oldLeft!=newLeft || oldRight!=newRight) { blTimer=millis()+60000UL; digitalWrite(TFT_BL,TFT_BACKLIGHT_ON); setDisplay(knob); }
  if (oldRight!=newRight) { MIDIsetControl(0,knob,knobValue[knob]); }
  oldLeft=newLeft; oldRight=newRight;

  int packetSize=Udp.parsePacket();
  if (packetSize==2) {
    char receiveBuffer[2];
    Udp.read(receiveBuffer,2);
    knob=receiveBuffer[0]-1;
    if (receiveBuffer[1]==1) { knobValue[knob]+=1; }
    if (receiveBuffer[1]==2) { knobValue[knob]-=1; }
    if (knobValue[knob]<0) { knobValue[knob]=127; }
    if (knobValue[knob]>127) { knobValue[knob]=0; }
    blTimer=millis()+60000UL; digitalWrite(TFT_BL,TFT_BACKLIGHT_ON); setDisplay(knob);
    MIDIsetControl(0,knob,knobValue[knob]); } }
