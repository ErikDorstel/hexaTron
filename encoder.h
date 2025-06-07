#define gpioEnc0A  9
#define gpioEnc0B 24
#define gpioEnc1A 25
#define gpioEnc1B 28
#define gpioButtonLeft 6
#define gpioButtonRight 29

volatile struct encStruct { uint8_t seqIndex[2]; uint8_t nextCW[2]; uint8_t nextCCW[2]; int32_t value[2]; } enc;
const uint8_t encSequence[6]={1,0,2,3,1,0};

void enc0ISR() {
  uint8_t encIndex=0; uint8_t encInput=0;
  if (digitalRead(gpioEnc0A)) { encInput|=2; }
  if (digitalRead(gpioEnc0B)) { encInput|=1; }

  if (encInput==enc.nextCW[encIndex]) {
    if (enc.seqIndex[encIndex]<4) { enc.seqIndex[encIndex]+=1; } else { enc.seqIndex[encIndex]=1; }
    enc.nextCW[encIndex]=encSequence[enc.seqIndex[encIndex]+1];
    enc.nextCCW[encIndex]=encSequence[enc.seqIndex[encIndex]-1];
    enc.value[encIndex]+=1; } else

  if (encInput==enc.nextCCW[encIndex]) {
    if (enc.seqIndex[encIndex]>1) { enc.seqIndex[encIndex]-=1; } else { enc.seqIndex[encIndex]=4; }
    enc.nextCW[encIndex]=encSequence[enc.seqIndex[encIndex]+1];
    enc.nextCCW[encIndex]=encSequence[enc.seqIndex[encIndex]-1];
    enc.value[encIndex]-=1; } }

void enc1ISR() {
  uint8_t encIndex=1; uint8_t encInput=0;
  if (digitalRead(gpioEnc1A)) { encInput|=2; }
  if (digitalRead(gpioEnc1B)) { encInput|=1; }

  if (encInput==enc.nextCW[encIndex]) {
    if (enc.seqIndex[encIndex]<4) { enc.seqIndex[encIndex]+=1; } else { enc.seqIndex[encIndex]=1; }
    enc.nextCW[encIndex]=encSequence[enc.seqIndex[encIndex]+1];
    enc.nextCCW[encIndex]=encSequence[enc.seqIndex[encIndex]-1];
    enc.value[encIndex]+=1; } else

  if (encInput==enc.nextCCW[encIndex]) {
    if (enc.seqIndex[encIndex]>1) { enc.seqIndex[encIndex]-=1; } else { enc.seqIndex[encIndex]=4; }
    enc.nextCW[encIndex]=encSequence[enc.seqIndex[encIndex]+1];
    enc.nextCCW[encIndex]=encSequence[enc.seqIndex[encIndex]-1];
    enc.value[encIndex]-=1; } }

void initEncoder() {
  pinMode(gpioButtonLeft,INPUT_PULLUP); pinMode(gpioButtonRight,INPUT_PULLUP);
  pinMode(gpioEnc0A,INPUT_PULLUP); pinMode(gpioEnc0B,INPUT_PULLUP);
  attachInterrupt(gpioEnc0A,enc0ISR,CHANGE); attachInterrupt(gpioEnc0B,enc0ISR,CHANGE);
  pinMode(gpioEnc1A,INPUT_PULLUP); pinMode(gpioEnc1B,INPUT_PULLUP);
  attachInterrupt(gpioEnc1A,enc1ISR,CHANGE); attachInterrupt(gpioEnc1B,enc1ISR,CHANGE);
  for (uint8_t encIndex=0;encIndex<2;encIndex++) {
    enc.value[encIndex]=0; enc.seqIndex[encIndex]=encSequence[3];
    enc.nextCW[encIndex]=encSequence[4]; enc.nextCCW[encIndex]=encSequence[2]; } }
