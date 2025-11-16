#include "vga.h"
#include "../utilities/utilities.h"

/* ************* *
 *     VGA IO    * 
 * ************* */

/*read the register indesed by AddrReg (for CRTC GCR SR)*/
uint8_t inDataAddrb(uint8_t index, uint16_t AddrReg){
  outb(index,AddrReg);
  return inb(AddrReg+1);
}

/*Read ACR, disable PAS in the read and than restore the original value*/
uint8_t readACR(uint8_t index){
  uint8_t orig, v;

  clearFlipflop();
  /*orig = inb(ACRREAD);*/
  outb(index, ACRWRITE);
  v = inb(ACRREAD);/*
  clearFlipflop();
  outb(orig, ACRWRITE);*/
  return v;
}

/*write ACR, disable PAS in the write and than restore the original value*/
void writeACR(uint8_t value, uint8_t index){
  uint8_t orig;

  clearFlipflop();
  /*orig = inb(ACRREAD);*/
  outb(index, ACRWRITE);
  outb(value, ACRWRITE);
  /*outb(orig, ACRWRITE);*/
}

/* ******************* *
 *    VGA UTILITIES    * 
 * ******************* */

/*
 * true -> Ram Access palette
 * flase -> Display Access palette
 */
void accessPalette(bool enable){
  clearFlipflop();
  if(enable){
    outb(inb(ACRREAD) & 0x20, ACRWRITE);/*0-> RAM*/
  }else{
    outb(inb(ACRREAD) | 0x20, ACRWRITE);/*1-> Display*/
  }
}

/*Fill the screen with blank spaces*/
void clearScreen(void) {
  uint32_t i;
  for (i = 0; i < COLS * ROWS; i++){
    videoMemory[i*2] = 0x20; 
    videoMemory[i*2+1]=0x07;
  }
}

/*Enable/Disable crtc write protection on 0-7 registers*/
static void CRTCProtection(bool enable){
  if(!enable){
    maskReg(0x80,0,EHBR,ADDRCRTC);
    maskReg(0,0x80,LPHR,ADDRCRTC);
  }else{
    maskReg(0x80,0,EHBR,ADDRCRTC);
    maskReg(0x80,0,LPHR,ADDRCRTC);
    maskReg(0,0x80,EHBR,ADDRCRTC);
  }
}

/*Set screen on/off*/
void screen(bool enable){
  if(enable)
    maskReg(0,0x20,CLOCKINGMODE,ADDRSR);/* 0 -> Screen on*/
  else
    maskReg(0x20,0,CLOCKINGMODE,ADDRSR);
}

static uint8_t *getAddress(void){
  uint8_t mr =  inDataAddrb(MISCELLANEOUSGCR,ADDRGCR);
  mr = ( mr>>2 ) & 3;
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
  outDataAddrb(p,READMAPSELECT,ADDRGCR); /*Read plane*/
  outDataAddrb(mask,MAPMASK,ADDRSR); /*Write plane*/
}

static void getFontAccess(void){
    outDataAddrb(0x01, 0x00, ADDRSR);
    outDataAddrb(0x04, 0x02, ADDRSR);
    outDataAddrb(0x07, 0x04, ADDRSR);
    outDataAddrb(0x03, 0x00, ADDRSR);
    outDataAddrb(0x02, 0x04, ADDRGCR);
    outDataAddrb(0x00, 0x05, ADDRGCR);
    outDataAddrb(0x00, 0x06, ADDRGCR);
}

static void releaseFontAccess(void){
    uint16_t v;
    outDataAddrb(0x01, 0x00, ADDRSR);
    outDataAddrb(0x03, 0x02, ADDRSR);
    outDataAddrb(0x03, 0x04, ADDRSR);
    outDataAddrb(0x03, 0x00, ADDRSR);
    v = (inb(MOR) & 0x01) ? 0x0e : 0x0a;
    outDataAddrb(v, 0x06, ADDRGCR);
    outDataAddrb(0x00, 0x04, ADDRGCR);
    outDataAddrb(0x10, 0x05, ADDRGCR);
}


/* ******************* *
 *       VGA INIT      * 
 * ******************* */

static void writeFont(void){
  uint8_t sr2, sr4, gc4, gc5, gc6;
  uint8_t *src=font8x16;
  uint32_t i;
  uint8_t *mem;
  /* FIXME: */
  /*save ragister modified by setPlane() and disable odd/even and chain four */
  
  sr2=inDataAddrb(MAPMASK,ADDRSR); /*0x02*/
 
  sr4=inDataAddrb(MEMORYMODE,ADDRSR);/*0x04*/
  maskReg(0x04,0,MEMORYMODE,ADDRSR);/* 1 -> chain four, to disable it*/
  
  gc4=inDataAddrb(READMAPSELECT,ADDRGCR);

  gc5=inDataAddrb(MODEREGISTER,ADDRGCR);
  maskReg(0,0x10,MODEREGISTER,ADDRGCR);/* 0 -> odd/even register */

  gc6=inDataAddrb(MISCELLANEOUSGCR,ADDRGCR);
  maskReg(0,0x02,MISCELLANEOUSGCR,ADDRGCR);/* 0 -> Chain Odd-even register */

  setPlane(2);

  mem=getAddress(); /*get the plane address*/

  /*getFontAccess();*/

  /*fonts are 16*8 bit but vga reserve a 32*8 space for each font charchter*/
  for(i = 0; i < 256; i++){
    memcpy((uint8_t *)(mem + (i * 32)), src, FONTHEIGHT);
		src += FONTHEIGHT;
	}

  /*releaseFontAccess();*/

  /*Restore registers*/
  outb(MAPMASK, ADDRSR);
  outb(sr2, DATASR);
  outb(MEMORYMODE, ADDRSR);
  outb(sr4, DATASR);
  outb(READMAPSELECT, ADDRGCR);
  outb(gc4, DATAGCR);
	outb(MODEREGISTER, ADDRGCR);
	outb(gc5, DATAGCR);
	outb(MISCELLANEOUSGCR, ADDRGCR);
	outb(gc6, DATAGCR);
}

/* FIXME: for debug purpose*/
/*static void dump(void){
  uint32_t i=0;
  for(i=0;i<NUMSR;i++)
    inFromDataAddrRegister(i,ADDRSR,DATASR);

 
  CRTCProtection(false);


  for(i=0;i<NUMCRTC;i++)
    inFromDataAddrRegister(i,ADDRCRTC,DATACRTC);


  for(i=0;i<NUMGCR;i++)
    inFromDataAddrRegister(i,ADDRGCR,DATAGCR);


  for(i=0;i<NUMACR;i++)
    inFromSameDataAddrRegister(i, ACR);


  clearFlipflop();
  outb(0x20,ACR);

  CRTCProtection(true);
}*/

void vgaInit(void){
  uint32_t i;
  uint8_t *regs=textMode80x25;

  /*======= LOAD PALETTE =========*/
  outb(0xff,DACMASK);
  outb(0,WRITEADDRDAC);
  for(i=0; i<VGAPALETTELENGHT ;i++)
    outb(pal_vga[i],DATADAC);


  /*======== DISABLE DISAPLAY & UNLOCK CRTC========*/
  screen(false);

  /*======== LOAD RREGISTERS ========*/
  
  /*Attribute Controller Registers*/
  for(i=0;i<NUMACR;i++)
    writeACR(*(regs++),i);
  
  /*Sequencer registers*/
  for(i=0;i<NUMSR;i++)
    outDataAddrb(*(regs++),i,ADDRSR);

  /*Graphics Controller Registers*/
  for(i=0;i<NUMGCR;i++)
    outDataAddrb(*(regs++),i,ADDRGCR);

  /*General Regiser*/
  outb(*(regs++),MOR);

  /*Disable crtc protection*/
  CRTCProtection(false);
  /*regs[EHBR]|= 0x80;
  regs[LPHR]&= ~0x80;

  /*CRTC register*/
  for(i=0;i<NUMCRTC;i++)
    outDataAddrb(*(regs++),i,ADDRCRTC);

  /*Lock 16-color palette and unblank display*/
  accessPalette(false);

  /*======== CLEAR SCREEN ========*/
  clearScreen();

  /*======== LOAD FONTS ========*/
  writeFont();

  /*======== EANBLE DSIPLAY & LOCK CRTC========*/
  screen(true);

  videoMemory[0] = 'A';
  videoMemory[1] = 0x07;

  videoMemory[2] = 'B';
  videoMemory[3] = 0x07;
}





