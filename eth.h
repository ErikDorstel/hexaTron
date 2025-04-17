#include <NativeEthernet.h>
#include <NativeEthernetUdp.h>
EthernetUDP Udp;

void initEth() {
  byte mac[]={0x00,0xAA,0xBB,0xCC,0xDE,0x01};
  IPAddress ip(10,0,0,1);
  IPAddress net(255,255,255,0);
  Ethernet.begin(mac,ip);
  Ethernet.setSubnetMask(net);
  Udp.begin(4242); }

void ethWorker() {
  static uint64_t ethTimer;
  if (millis()>=ethTimer) {
    ethTimer=millis()+10000;
    if (Ethernet.linkStatus() == LinkON) { Ethernet.maintain(); } } }
