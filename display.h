#include <TFT_eSPI.h>
TFT_eSPI tft=TFT_eSPI();

#include <XPT2046_Touchscreen.h>
XPT2046_Touchscreen ts(0,5);

#include <Encoder.h>
Encoder knobLeft(24,9);
Encoder knobRight(28,25);
#define buttonLeft 6
#define buttonRight 29

const char *knobText[2][32]={{"Attack","Hold","Decay","Sustain","Release","Arpeggiator Mode","Arpeggiator Speed","Glissando Speed",\
  "VCO 1 Waveform","VCO 2 Waveform","VCO Ratio","VCO Add/Multiply","VCO 2 Shift","VCO Level","","",\
  "LFO VCO Waveform","LFO VCO Phase Start","LFO VCO Level","LFO VCO Frequency","VCF Bypass","VCF Lowpass","VCF Bandpass","VCF Highpass",\
  "LFO VCF Waveform","LFO VCF Phase Start","LFO VCF Level","LFO VCF Frequency","VCF Frequency","VCF Resonance","",""},\
  {"Record","Play"}};
uint8_t knobValue[32]={10,0,0,127,20,0,127,0,40,70,64,64,120,70,0,0,0,0,0,0,0,127,0,0,0,30,70,10,50,40,0,0};
uint8_t page=0,knob=0,oldPage=255,oldKnob=1;

void setAHDSR(bool clear=true) {
  static uint16_t oldx,oldax,oldhx,olddx,oldsx,oldsy;
  uint16_t x=knobValue[0]+knobValue[1]+knobValue[2]+25+knobValue[4];
  uint16_t ax=knobValue[0]*480/x;
  uint16_t hx=ax+knobValue[1]*480/x;
  uint16_t dx=hx+knobValue[2]*480/x;
  uint16_t sx=dx+25*480/x;
  uint16_t sy=300-knobValue[3]*200/127;
  if (oldx+oldsy==x+sy && clear) { return; }
  if (clear) {
    tft.drawWideLine(0,300,oldax,100,3,TFT_WHITE,TFT_WHITE);
    tft.drawWideLine(oldax,100,oldhx,100,3,TFT_WHITE,TFT_WHITE);
    tft.drawWideLine(oldhx,100,olddx,oldsy,3,TFT_WHITE,TFT_WHITE);
    tft.drawWideLine(olddx,oldsy,oldsx,oldsy,3,TFT_WHITE,TFT_WHITE);
    tft.drawWideLine(oldsx,oldsy,479,300,3,TFT_WHITE,TFT_WHITE);
    tft.drawLine(0,300,479,300,TFT_WHITE);
    tft.drawLine(oldax,95,oldax,305,TFT_WHITE);
    tft.drawLine(oldhx,95,oldhx,305,TFT_WHITE);
    tft.drawLine(olddx,oldsy-5,olddx,305,TFT_WHITE);
    tft.drawLine(oldsx,oldsy-5,oldsx,305,TFT_WHITE); }
  tft.drawWideLine(0,300,ax,100,3,TFT_SKYBLUE,TFT_WHITE);
  tft.drawWideLine(ax,100,hx,100,3,TFT_SKYBLUE,TFT_WHITE);
  tft.drawWideLine(hx,100,dx,sy,3,TFT_SKYBLUE,TFT_WHITE);
  tft.drawWideLine(dx,sy,sx,sy,3,TFT_SKYBLUE,TFT_WHITE);
  tft.drawWideLine(sx,sy,479,300,3,TFT_SKYBLUE,TFT_WHITE);
  tft.drawLine(0,300,479,300,TFT_BLACK);
  tft.drawLine(ax,95,ax,305,TFT_BLACK);
  tft.drawLine(hx,95,hx,305,TFT_BLACK);
  tft.drawLine(dx,sy-5,dx,305,TFT_BLACK);
  tft.drawLine(sx,sy-5,sx,305,TFT_BLACK);
  oldx=x; oldax=ax; oldhx=hx; olddx=dx; oldsx=sx; oldsy=sy; }

void setVCO(bool clear=true) {
  static uint8_t old8,old9,old10,old11; static uint16_t oldx;
  uint16_t x=knobValue[8]+knobValue[9]+knobValue[10]+knobValue[11];
  if (oldx==x && clear) { return; }
  if (clear) {
    for (uint16_t x=0;x<480;x++) {
      float y1=0;
      if ((old8&96)==0) { y1=sin(6.28*x/480); } else
      if ((old8&96)==32) { y1=(float)x/240; if (x>=240) { y1-=2; } } else
      if ((old8&96)==64) { y1=1; if (x>=240) { y1=-1; } } else
      if ((old8&96)==96) { y1=(float)x/120; if (x>=120) { y1=2-y1; } if (x>=360) { y1=-2-y1; } }
      float y2=0;
      if ((old9&96)==0) { y2=sin(6.28*x/480); } else
      if ((old9&96)==32) { y2=(float)x/240; if (x>=240) { y2-=2; } } else
      if ((old9&96)==64) { y2=1; if (x>=240) { y2=-1; } } else
      if ((old9&96)==96) { y2=(float)x/120; if (x>=120) { y2=2-y2; } if (x>=360) { y2=-2-y2; } }
      float ya=(y1*(1.0-((float)old10/127.0)))+(y2*((float)old10/127.0));
      float ym=(y1*y2);
      float ys=(ya*(1.0-((float)old11/127.0)))+(ym*((float)old11/127.0));
      tft.drawPixel(x,200-ys*100,TFT_WHITE);
      tft.drawPixel(x,201-ys*100,TFT_WHITE);
      tft.drawPixel(x,202-ys*100,TFT_WHITE); }
    tft.drawLine(0,200,479,200,TFT_WHITE); }
  for (uint16_t x=0;x<480;x++) {
    float y1=0;
    if ((knobValue[8]&96)==0) { y1=sin(6.28*x/480); } else
    if ((knobValue[8]&96)==32) { y1=(float)x/240; if (x>=240) { y1-=2; } } else
    if ((knobValue[8]&96)==64) { y1=1; if (x>=240) { y1=-1; } } else
    if ((knobValue[8]&96)==96) { y1=(float)x/120; if (x>=120) { y1=2-y1; } if (x>=360) { y1=-2-y1; } }
    float y2=0;
    if ((knobValue[9]&96)==0) { y2=sin(6.28*x/480); } else
    if ((knobValue[9]&96)==32) { y2=(float)x/240; if (x>=240) { y2-=2; } } else
    if ((knobValue[9]&96)==64) { y2=1; if (x>=240) { y2=-1; } } else
    if ((knobValue[9]&96)==96) { y2=(float)x/120; if (x>=120) { y2=2-y2; } if (x>=360) { y2=-2-y2; } }
    float ya=(y1*(1.0-((float)knobValue[10]/127.0)))+(y2*((float)knobValue[10]/127.0));
    float ym=(y1*y2);
    float ys=(ya*(1.0-((float)knobValue[11]/127)))+(ym*((float)knobValue[11]/127.0));
    tft.drawPixel(x,200-ys*100,TFT_SKYBLUE);
    tft.drawPixel(x,201-ys*100,TFT_SKYBLUE);
    tft.drawPixel(x,202-ys*100,TFT_SKYBLUE); }
  tft.drawLine(0,200,479,200,TFT_BLACK);
  oldx=x; old8=knobValue[8]; old9=knobValue[9]; old10=knobValue[10]; old11=knobValue[11]; }

void setDisplay(bool direct=false) {
  if (page>3) { page=0; }
  if (page==3) { knobValue[knob+8]=knobValue[knob+8]&0x7F; } else { knobValue[knob]=knobValue[knob]&0x7F; }
  if (oldPage!=page) {
    if (direct==false) { knob=0; oldKnob=1; } else { oldKnob=knob+1; }
    tft.fillScreen(TFT_WHITE);
    tft.setTextColor(TFT_BLACK,TFT_WHITE);
    if (page==0) {
      for (uint8_t i=0;i<32;i++) {
        tft.setCursor(i%4*120+3,i/4*40+9);
        tft.print(knobText[0][i]);
        tft.setCursor(i%4*120+3,i/4*40+22);
        if (knobValue[i]<10) { tft.print("  "); } else
        if (knobValue[i]<100) { tft.print(" "); }
        tft.print(knobValue[i]);
        tft.fillRect(i%4*120+28,i/4*40+24,knobValue[i]/2,3,TFT_BLACK);
        tft.fillRect(i%4*120+28+(knobValue[i]/2),i/4*40+24,63-(knobValue[i]/2),3,TFT_SKYBLUE); } }
    if (page==1) {
      for (uint8_t i=0;i<2;i++) {
        tft.setCursor(i%4*120+3,i/4*40+9);
        tft.print(knobText[1][i]); } }
    if (page==2) {
      for (uint8_t i=0;i<5;i++) {
        tft.setCursor(i%4*120+3,i/4*40+9);
        tft.print(knobText[0][i]);
        tft.setCursor(i%4*120+3,i/4*40+22);
        if (knobValue[i]<10) { tft.print("  "); } else
        if (knobValue[i]<100) { tft.print(" "); }
        tft.print(knobValue[i]);
        tft.fillRect(i%4*120+28,i/4*40+24,knobValue[i]/2,3,TFT_BLACK);
        tft.fillRect(i%4*120+28+(knobValue[i]/2),i/4*40+24,63-(knobValue[i]/2),3,TFT_SKYBLUE);
        setAHDSR(false); } }
    if (page==3) {
      for (uint8_t i=0;i<5;i++) {
        tft.setCursor(i%4*120+3,i/4*40+9);
        tft.print(knobText[0][i+8]);
        tft.setCursor(i%4*120+3,i/4*40+22);
        if (knobValue[i+8]<10) { tft.print("  "); } else
        if (knobValue[i+8]<100) { tft.print(" "); }
        tft.print(knobValue[i+8]);
        tft.fillRect(i%4*120+28,i/4*40+24,knobValue[i+8]/2,3,TFT_BLACK);
        tft.fillRect(i%4*120+28+(knobValue[i+8]/2),i/4*40+24,63-(knobValue[i+8]/2),3,TFT_SKYBLUE);
        setVCO(false); } } }
  if (page==0) {
    knob=knob&0x1F; oldKnob=oldKnob&0x1F;
    if (oldKnob!=knob) {
      tft.fillRoundRect(oldKnob%4*120,oldKnob/4*40,120,40,10,TFT_WHITE);
      tft.setTextColor(TFT_BLACK,TFT_WHITE);
      tft.setCursor(oldKnob%4*120+3,oldKnob/4*40+9);
      tft.print(knobText[0][oldKnob]);
      tft.setCursor(oldKnob%4*120+3,oldKnob/4*40+22);
      if (knobValue[oldKnob]<10) { tft.print("  "); } else
      if (knobValue[oldKnob]<100) { tft.print(" "); }
      tft.print(knobValue[oldKnob]);
      tft.fillRect(oldKnob%4*120+28,oldKnob/4*40+24,knobValue[oldKnob]/2,3,TFT_BLACK);
      tft.fillRect(oldKnob%4*120+28+(knobValue[oldKnob]/2),oldKnob/4*40+24,63-(knobValue[oldKnob]/2),3,TFT_SKYBLUE);
      tft.fillRoundRect(knob%4*120,knob/4*40,120,40,10,TFT_SKYBLUE);
      tft.setTextColor(TFT_BLACK,TFT_SKYBLUE);
      tft.setCursor(knob%4*120+3,knob/4*40+9);
      tft.print(knobText[0][knob]); }
    tft.setCursor(knob%4*120+3,knob/4*40+22);
    if (knobValue[knob]<10) { tft.print("  "); } else
    if (knobValue[knob]<100) { tft.print(" "); }
    tft.print(knobValue[knob]);
    tft.fillRect(knob%4*120+28,knob/4*40+24,knobValue[knob]/2,3,TFT_BLACK);
    tft.fillRect(knob%4*120+28+(knobValue[knob]/2),knob/4*40+24,63-(knobValue[knob]/2),3,TFT_WHITE); }
  if (page==1) {
    knob=knob&0x1; oldKnob=oldKnob&0x1;
    if (oldKnob!=knob) {
      tft.fillRoundRect(oldKnob%4*120,oldKnob/4*40,120,40,10,TFT_WHITE);
      tft.setTextColor(TFT_BLACK,TFT_WHITE);
      tft.setCursor(oldKnob%4*120+3,oldKnob/4*40+9);
      tft.print(knobText[1][oldKnob]);
      tft.fillRoundRect(knob%4*120,knob/4*40,120,40,10,TFT_SKYBLUE);
      tft.setTextColor(TFT_BLACK,TFT_SKYBLUE);
      tft.setCursor(knob%4*120+3,knob/4*40+9);
      tft.print(knobText[1][knob]); } }
  if (page==2) {
    if (knob>5) { knob=4; } else
    if (knob>4) { knob=0; }
    if (oldKnob>5) { oldKnob=4; } else
    if (oldKnob>4) { oldKnob=0; }
    if (oldKnob!=knob) {
      tft.fillRoundRect(oldKnob%4*120,oldKnob/4*40,120,40,10,TFT_WHITE);
      tft.setTextColor(TFT_BLACK,TFT_WHITE);
      tft.setCursor(oldKnob%4*120+3,oldKnob/4*40+9);
      tft.print(knobText[0][oldKnob]);
      tft.setCursor(oldKnob%4*120+3,oldKnob/4*40+22);
      if (knobValue[oldKnob]<10) { tft.print("  "); } else
      if (knobValue[oldKnob]<100) { tft.print(" "); }
      tft.print(knobValue[oldKnob]);
      tft.fillRect(oldKnob%4*120+28,oldKnob/4*40+24,knobValue[oldKnob]/2,3,TFT_BLACK);
      tft.fillRect(oldKnob%4*120+28+(knobValue[oldKnob]/2),oldKnob/4*40+24,63-(knobValue[oldKnob]/2),3,TFT_SKYBLUE);
      tft.fillRoundRect(knob%4*120,knob/4*40,120,40,10,TFT_SKYBLUE);
      tft.setTextColor(TFT_BLACK,TFT_SKYBLUE);
      tft.setCursor(knob%4*120+3,knob/4*40+9);
      tft.print(knobText[0][knob]); }
    tft.setCursor(knob%4*120+3,knob/4*40+22);
    if (knobValue[knob]<10) { tft.print("  "); } else
    if (knobValue[knob]<100) { tft.print(" "); }
    tft.print(knobValue[knob]);
    tft.fillRect(knob%4*120+28,knob/4*40+24,knobValue[knob]/2,3,TFT_BLACK);
    tft.fillRect(knob%4*120+28+(knobValue[knob]/2),knob/4*40+24,63-(knobValue[knob]/2),3,TFT_WHITE);
    setAHDSR(); }
  if (page==3) {
    if (knob>5) { knob=4; } else
    if (knob>4) { knob=0; }
    if (oldKnob>5) { oldKnob=4; } else
    if (oldKnob>4) { oldKnob=0; }
    if (oldKnob!=knob) {
      tft.fillRoundRect(oldKnob%4*120,oldKnob/4*40,120,40,10,TFT_WHITE);
      tft.setTextColor(TFT_BLACK,TFT_WHITE);
      tft.setCursor(oldKnob%4*120+3,oldKnob/4*40+9);
      tft.print(knobText[0][oldKnob+8]);
      tft.setCursor(oldKnob%4*120+3,oldKnob/4*40+22);
      if (knobValue[oldKnob+8]<10) { tft.print("  "); } else
      if (knobValue[oldKnob+8]<100) { tft.print(" "); }
      tft.print(knobValue[oldKnob+8]);
      tft.fillRect(oldKnob%4*120+28,oldKnob/4*40+24,knobValue[oldKnob+8]/2,3,TFT_BLACK);
      tft.fillRect(oldKnob%4*120+28+(knobValue[oldKnob+8]/2),oldKnob/4*40+24,63-(knobValue[oldKnob+8]/2),3,TFT_SKYBLUE);
      tft.fillRoundRect(knob%4*120,knob/4*40,120,40,10,TFT_SKYBLUE);
      tft.setTextColor(TFT_BLACK,TFT_SKYBLUE);
      tft.setCursor(knob%4*120+3,knob/4*40+9);
      tft.print(knobText[0][knob+8]); }
    tft.setCursor(knob%4*120+3,knob/4*40+22);
    if (knobValue[knob+8]<10) { tft.print("  "); } else
    if (knobValue[knob+8]<100) { tft.print(" "); }
    tft.print(knobValue[knob+8]);
    tft.fillRect(knob%4*120+28,knob/4*40+24,knobValue[knob+8]/2,3,TFT_BLACK);
    tft.fillRect(knob%4*120+28+(knobValue[knob+8]/2),knob/4*40+24,63-(knobValue[knob+8]/2),3,TFT_WHITE);
    setVCO(); }
  oldPage=page; oldKnob=knob; }

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

  ts.begin(SPI1);
  ts.setRotation(1);

  pinMode(buttonLeft,INPUT_PULLUP);
  pinMode(buttonRight,INPUT_PULLUP);

  for (uint8_t i=0;i<32;i++) { MIDIsetControl(0,i,knobValue[i]); }
  setDisplay(); }

void displayWorker() {
  setTFTBL();

  if (ts.touched()) {
    setTFTBL(true);
    TS_Point p=ts.getPoint(); }

  static uint64_t buttonLeftTimer;
  if (!digitalRead(buttonLeft) && millis()>=buttonLeftTimer) {
    buttonLeftTimer=millis()+500;
    setTFTBL(true); page+=1; setDisplay(); }

  static uint64_t buttonRightTimer;
  if (!digitalRead(buttonRight) && millis()>=buttonRightTimer) {
    buttonRightTimer=millis()+500;
    setTFTBL(true);
    if (page==0) { knobValue[knob]+=32; setDisplay(); MIDIsetControl(0,knob,knobValue[knob]); }
    if (page==1 && knob==0 && seq.recording) { stopRecordingSequence(); } else { startRecordingSequence(); }
    if (page==1 && knob==1 && seq.playing) { stopPlayingSequence(); } else { startPlayingSequence(); }
    if (page==2) { knobValue[knob]+=32; setDisplay(); MIDIsetControl(0,knob,knobValue[knob]); }
    if (page==3) { knobValue[knob+8]+=32; setDisplay(); MIDIsetControl(0,knob+8,knobValue[knob+8]); } }

  static long oldLeft=0,oldRight=0;
  long newLeft=knobLeft.read()/4;
  long newRight=knobRight.read()/4;
  if (oldLeft!=newLeft) { knob+=newLeft-oldLeft; setTFTBL(true); setDisplay(); }
  if (page==0 && oldRight!=newRight) { knobValue[knob]+=newRight-oldRight; setTFTBL(true); setDisplay(); MIDIsetControl(0,knob,knobValue[knob]); }
  if (page==2 && oldRight!=newRight) { knobValue[knob]+=newRight-oldRight; setTFTBL(true); setDisplay(); MIDIsetControl(0,knob,knobValue[knob]); }
  if (page==3 && oldRight!=newRight) { knobValue[knob+8]+=newRight-oldRight; setTFTBL(true); setDisplay(); MIDIsetControl(0,knob+8,knobValue[knob+8]); }
  oldLeft=newLeft; oldRight=newRight;

  if (ethConfigured) {
    if (Udp.parsePacket()==2) {
      char receiveBuffer[2];
      Udp.read(receiveBuffer,2);
      knob=receiveBuffer[0];
      if (receiveBuffer[1]==1) { knobValue[knob]+=32; } else
      if (receiveBuffer[1]==2) { knobValue[knob]+=1; } else
      if (receiveBuffer[1]==3) { knobValue[knob]-=1; }
      if (page==2 && knob<5) { setTFTBL(true); setDisplay(true); MIDIsetControl(0,knob,knobValue[knob]); } else
      if (page==3 && knob>7 && knob<13) { knob-=8; setTFTBL(true); setDisplay(true); MIDIsetControl(0,knob+8,knobValue[knob+8]); }
      else { page=0; setTFTBL(true); setDisplay(true); MIDIsetControl(0,knob,knobValue[knob]); } } } }
