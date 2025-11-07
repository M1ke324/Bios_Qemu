#include "vga.h"
#include "../utilities/utilities.h"

/*Fill the screen with blank spaces*/
void clearScreen(void) {
  uint32_t i;
  for (i = 0; i < COLS * ROWS; i++){
    videoMemory[i*2] = ' ';
    videoMemory[i*2+1]= 0x07;
  }
}

/*Enable/Disable crtc write protection on 0-7 registers*/
static void CRTCProtection(bool enable){
  uint8_t reg;
 
  if(!enable){
    outToRegisterB(EHBR,ADDRCRTC);
    reg=inFromRegistersB(DATACRTC);
    reg= reg | 0x80;
    outToRegisterB(reg, DATACRTC);

    outToRegisterB(LPHR,ADDRCRTC);
    reg=inFromRegistersB(DATACRTC);
    reg=reg & ~0x80;
    outToRegisterB(reg, DATACRTC);
  }else{
    outToRegisterB(EHBR,ADDRCRTC);
    reg=inFromRegistersB(DATACRTC);
    reg= reg | 0x80;
    outToRegisterB(reg, DATACRTC);

    outToRegisterB(LPHR,ADDRCRTC);
    reg=inFromRegistersB(DATACRTC);
    reg=reg | 0x80;
    outToRegisterB(reg, DATACRTC);

    outToRegisterB(EHBR,ADDRCRTC);
    reg=inFromRegistersB(DATACRTC);
    reg= reg & ~0x80;
    outToRegisterB(reg, DATACRTC);
  }
}

/*Set screen on/off*/
void screen(bool enable){
  outToRegisterB(CLOCKINGMODE,ADDRSR);
  if(enable)
    outToRegisterB( inFromRegistersB(DATASR) & 0xDF , DATASR); /* 0 -> Screen on*/
  else
    outToRegisterB( inFromRegistersB(DATASR) | 0x20 , DATASR);
}

static uint8_t *getAddress(void){
  uint8_t mr;
  outToRegisterB(MISCELLANEOUS,ADDRGCR);
  mr = inFromRegistersB(DATAGCR);
  mr >>= 2;
  mr &= 3;
  switch (mr) {
    case 0: /*passtrought*/
    case 1:
      return (uint8_t *) 0xA0000;
    case 2:
      return (uint8_t *) 0xB0000;
    case 3:
      return (uint8_t *) 0xB8000;
  }
  return NULL;
}


static void setPlane(uint8_t p){
  uint8_t mask;

  p&=3;
  mask= 1 << p;
  outToRegisterB(READMAPSELECT,ADDRGCR); /*Read plane*/
  outToRegisterB(p,DATAGCR);
 
  outToRegisterB(MAPMASK,ADDRSR); /*Write plane*/
  outToRegisterB(mask,DATASR);
}


static void writeFont(void){
  uint8_t SR2, SR4, GC4, GC5, GC6;
  uint8_t *src=font8x16;
  uint32_t i;

  uint8_t *mem;

  /*save ragister modified by setPlane() and disable odd/even and chain four */

  outToRegisterB(MAPMASK,ADDRSR);
  SR2=inFromRegistersB(DATASR);

  outToRegisterB(MEMORYMODE,ADDRSR);
  SR4=inFromRegistersB(DATASR);
  outToRegisterB(SR4 | 0x04, DATASR); /* 1 -> chain four, to disable it*/

  outToRegisterB(READMAPSELECT,ADDRGCR);
  GC4=inFromRegistersB(DATAGCR);

  outToRegisterB(MODEREGISTER,ADDRGCR);
  GC5=inFromRegistersB(DATAGCR);
  outToRegisterB(GC5 & ~0x10 ,DATAGCR); /* 0 -> odd/even register */

  outToRegisterB(MISCELLANEOUS,ADDRGCR);
  GC6=inFromRegistersB(DATAGCR);
  outToRegisterB( GC6 & ~0x02 ,DATAGCR); /* 0 -> Chain Odd-even register */

  setPlane(2);

  mem=getAddress(); /*get the plane address*/

  /*fonts are 16*8 bit but vga reserve a 32*8 space for each font charchter*/
  for(i = 0; i < 256; i++){
    memcpy((uint8_t *)(mem + (i * 32)), src, FONTHEIGHT);
		src += FONTHEIGHT;
	}

  /*Restore registers*/
  outToRegisterB(MAPMASK, ADDRSR);
  outToRegisterB(SR2, DATASR);
  outToRegisterB(MEMORYMODE, ADDRSR);
  outToRegisterB(SR4, DATASR);
  outToRegisterB(READMAPSELECT, ADDRGCR);
  outToRegisterB(GC4, DATAGCR);
	outToRegisterB(MODEREGISTER, ADDRGCR);
	outToRegisterB(GC5, DATAGCR);
	outToRegisterB(MISCELLANEOUS, ADDRGCR);
	outToRegisterB(GC6, DATAGCR);
}

/*for debug purpose*/
static void dump(void){
  uint32_t i=0;
  for(i=0;i<NUMSR;i++)
    inFromDataAddrRegister(i,ADDRSR,DATASR);

  /*Disable crtc protection*/
  CRTCProtection(false);

  /*CRTC register*/
  for(i=0;i<NUMCRTC;i++)
    inFromDataAddrRegister(i,ADDRCRTC,DATACRTC);

  /*Graphics Controller Registers*/
  for(i=0;i<NUMGCR;i++)
    inFromDataAddrRegister(i,ADDRGCR,DATAGCR);

  /*Attribute Controller Registers*/
  for(i=0;i<NUMACR;i++)
    inFromSameDataAddrRegister(i, ACR);

  /*Lock 16-color palette and unblank display*/
  CLEARFLIPFLOP();
  outToRegisterB(0x20,ACR);

  CRTCProtection(true);
}

void vgaInit(void){
  uint8_t i;
  uint8_t *regs=textMode80x25;

  /*======== DISABLE DISAPLAY & UNLOCK CRTC========*/
  screen(false);

  /*======== LOAD RREGISTERS ========*/
  
  /*General Regiser*/
  outToRegisterB(*(regs++),MOR);

  /*Sequencer register*/
  for(i=0;i<NUMSR;i++)
    outToDataAddrRegister(*(regs++),i,ADDRSR,DATASR);

  /*Disable crtc protection*/
  CRTCProtection(false);
  regs[EHBR]|= 0x80;
  regs[LPHR]&= ~0x80;

  /*CRTC register*/
  for(i=0;i<NUMCRTC;i++)
    outToDataAddrRegister(*(regs++),i,ADDRCRTC,DATACRTC);

  /*Graphics Controller Registers*/
  for(i=0;i<NUMGCR;i++)
    outToDataAddrRegister(*(regs++),i,ADDRGCR,DATAGCR);

  /*Attribute Controller Registers*/
  for(i=0;i<NUMACR;i++)
    outToSameDataAddrRegister(*(regs++),i, ACR);

  /*Lock 16-color palette and unblank display*/
  CLEARFLIPFLOP();
  outToRegisterB(0x20,ACR);

  /*======== CLEAR SCREEN ========*/
  clearScreen();

  /*======== LOAD FONTS ========*/
  writeFont();

  /*======== EANBLE DSIPLAY ========*/
  screen(true);

  videoMemory[0] = 0xdb;
  videoMemory[1] = 0x47;

  videoMemory[2] = 'B';
  videoMemory[3] = 0x07;


}





