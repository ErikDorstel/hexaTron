#define gpioEnc0A  9
#define gpioEnc0B 24
#define gpioEnc1A 25
#define gpioEnc1B 28
#define gpioButtonLeft 6
#define gpioButtonRight 29

volatile struct encStruct { uint8_t seqIndex[2]; uint8_t nextCW[2]; uint8_t nextCCW[2]; int32_t value[2]; } enc;
const uint8_t encSequence[6]={1,0,2,3,1,0};

void enc0ISR() {
  uint8_t encInput=(digitalRead(gpioEnc0A)<<1)+digitalRead(gpioEnc0B);

  if (encInput==enc.nextCW[0]) {
    if (enc.seqIndex[0]<4) { enc.seqIndex[0]+=1; } else { enc.seqIndex[0]=1; }
    enc.nextCW[0]=encSequence[enc.seqIndex[0]+1];
    enc.nextCCW[0]=encSequence[enc.seqIndex[0]-1];
    enc.value[0]+=1; } else

  if (encInput==enc.nextCCW[0]) {
    if (enc.seqIndex[0]>1) { enc.seqIndex[0]-=1; } else { enc.seqIndex[0]=4; }
    enc.nextCW[0]=encSequence[enc.seqIndex[0]+1];
    enc.nextCCW[0]=encSequence[enc.seqIndex[0]-1];
    enc.value[0]-=1; } }

void enc1ISR() {
  uint8_t encInput=(digitalRead(gpioEnc1A)<<1)+digitalRead(gpioEnc1B);

  if (encInput==enc.nextCW[1]) {
    if (enc.seqIndex[1]<4) { enc.seqIndex[1]+=1; } else { enc.seqIndex[1]=1; }
    enc.nextCW[1]=encSequence[enc.seqIndex[1]+1];
    enc.nextCCW[1]=encSequence[enc.seqIndex[1]-1];
    enc.value[1]+=1; } else

  if (encInput==enc.nextCCW[1]) {
    if (enc.seqIndex[1]>1) { enc.seqIndex[1]-=1; } else { enc.seqIndex[1]=4; }
    enc.nextCW[1]=encSequence[enc.seqIndex[1]+1];
    enc.nextCCW[1]=encSequence[enc.seqIndex[1]-1];
    enc.value[1]-=1; } }

void initEncoder() {
  pinMode(gpioButtonLeft,INPUT_PULLUP); pinMode(gpioButtonRight,INPUT_PULLUP);
  pinMode(gpioEnc0A,INPUT_PULLUP); pinMode(gpioEnc0B,INPUT_PULLUP);
  attachInterrupt(gpioEnc0A,enc0ISR,CHANGE); attachInterrupt(gpioEnc0B,enc0ISR,CHANGE);
  pinMode(gpioEnc1A,INPUT_PULLUP); pinMode(gpioEnc1B,INPUT_PULLUP);
  attachInterrupt(gpioEnc1A,enc1ISR,CHANGE); attachInterrupt(gpioEnc1B,enc1ISR,CHANGE);
  for (uint8_t encIndex=0;encIndex<2;encIndex++) {
    enc.value[encIndex]=0; enc.seqIndex[encIndex]=encSequence[3];
    enc.nextCW[encIndex]=encSequence[4]; enc.nextCCW[encIndex]=encSequence[2]; } }
