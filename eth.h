#include <NativeEthernet.h>
#include <NativeEthernetUdp.h>
EthernetUDP Udp;

static bool ethConfigured=false;

void initEth() {
  byte mac[]={0x00,0xAA,0xBB,0xCC,0xDE,0x01};
  Ethernet.begin(mac,100,100); }

void ethWorker() {
  static uint64_t ethTimer;
  if (millis()>=ethTimer) {
    ethTimer=millis()+1000;
    if (Ethernet.linkStatus()==LinkON && (!ethConfigured)) {
      byte mac[]={0x00,0xAA,0xBB,0xCC,0xDE,0x01};
      IPAddress ip(10,0,0,1);
      IPAddress net(255,255,255,0);
      Ethernet.begin(mac,ip);
      Ethernet.setSubnetMask(net);
      Udp.begin(4242);
      ethConfigured=true; } } }
