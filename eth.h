#include <NativeEthernet.h>

void initEth() {
  byte mac[]={0x00,0xAA,0xBB,0xCC,0xDE,0x02};
  Ethernet.begin(mac,1000); }

void ethWorker() {
  static uint64_t ethTimer;
  if (millis()>=ethTimer) {
    ethTimer=millis()+10000;
    if (Ethernet.linkStatus() == LinkON) { Ethernet.maintain(); } } }
