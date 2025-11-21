#include "vga.h"
#include "dataVga.h" /*value for registers, font & palette*/
#include "vgaIO.h" /*To read/write easly vga register*/
#include "../utilities/utilities.h" /*memcpy & in/out function*/

/* ********************** *
 *    STATIC UTILITIES    *
 * ********************** */

/*
 * true -> Ram Access palette
 * flase -> Display Access palette
 */
static void accessPalette(bool enable){
  clearFlipflop();
  if(enable){
    outb(inb(ACRREAD) & 0x20, ACRWRITE);/*0-> RAM*/
  }else{
    outb(inb(ACRREAD) | 0x20, ACRWRITE);/*1-> Display*/
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

static void setPlane(uint8_t p){
  uint8_t mask;
  p&=3;
  mask= 1 << p;
  outDataAddrb(p,READMAPSELECT,ADDRGCR); /*Read plane*/
  outDataAddrb(mask,MAPMASK,ADDRSR); /*Write plane*/
}

/* ********************** *
 *   PUBLIC  UTILITIES    *
 * ********************** */

/*Set screen on/off*/
void screen(bool enable){
  if(enable)
    maskReg(0,0x20,CLOCKINGMODE,ADDRSR);/* 0 -> Screen on*/
  else
    maskReg(0x20,0,CLOCKINGMODE,ADDRSR);
}

/* ******************* *
 *       VGA INIT      * 
 * ******************* */

static void writeFont(void){
  uint8_t sr2, sr4, gc4, gc5, gc6;
  uint8_t *src=font8x16;
  uint32_t i;

  /*save ragister modified by setPlane() and disable odd/even and chain four */
  
  sr2=inDataAddrb(MAPMASK,ADDRSR);
 
  sr4=inDataAddrb(MEMORYMODE,ADDRSR);
  maskReg(0x04,0,MEMORYMODE,ADDRSR);/* 1 -> chain four, to disable it*/
  
  gc4=inDataAddrb(READMAPSELECT,ADDRGCR);

  gc5=inDataAddrb(MODEREGISTER,ADDRGCR);
  maskReg(0,0x10,MODEREGISTER,ADDRGCR);/* 0 -> odd/even register */

  gc6=inDataAddrb(MISCELLANEOUSGCR,ADDRGCR);
  maskReg(0,0x02,MISCELLANEOUSGCR,ADDRGCR);/* 0 -> Chain Odd-even register */

  setPlane(2);

  /*fonts are 16*8 bit but vga reserve a 32*8 space for each font charchter*/
  for(i = 0; i < 256; i++){
    memcpy((uint8_t *)(VIDEOMEM + (i * 32)), src, FONTHEIGHT);
		src += FONTHEIGHT;
	}

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

void vgaInit(void){
  uint32_t i;
  uint8_t *regs=textMode80x25;

  /*======= LOAD PALETTE =========*/
  outb(0xff,DACMASK);
  outb(0,WRITEADDRDAC);
  for(i=0; i<NDATAPALETTE;i++)
    outb(palette[i],DATADAC);

  /*======== DISABLE DISAPLAY ========*/
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
  regs[EHBR]|= 0x80;
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
  CRTCProtection(true);
}





