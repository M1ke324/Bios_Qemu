#include "vgaIO.h"
#include "../utilities/utilities.h" /* uint* &in/out*/

/*read the register indexed by AddrReg (for CRTC GCR SR)*/
uint8_t inDataAddrb(uint8_t index, uint16_t AddrReg){
  outb(index,AddrReg);
  return inb(AddrReg+1);
}

/*Read ACR, does reset PAS bit in the read, unless in index PAS bit is enabled*/
uint8_t readACR(uint8_t index){
  clearFlipflop();
  outb(index, ACRWRITE);
  return inb(ACRREAD);
}

/*write ACR, does reset PAS bit in the write, unless in index PAS bit is enabled*/
void writeACR(uint8_t value, uint8_t index){
  clearFlipflop();
  outb(index, ACRWRITE);
  outb(value, ACRWRITE);
}
