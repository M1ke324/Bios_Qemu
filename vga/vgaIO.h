#ifndef VGAIO
#define VGAIO
#include "../utilities/utilities.h"

/*
 * vgaIO files are an utilities for the vga directory,
 * the header is not meant to be included outside.
 * The vga.h header has this scope,
 * this is just a collection of preprocessor directive
 */

/* ************ *
 * VGA REGISTER * 
 * ************ */

/*======= General Regisers =======*/
#define MOR 0x3C2
#define FC 0x3DA
#define IS1 FC

/*======= Sequencer Registers =======*/
#define ADDRSR 0x3C4
#define DATASR 0x3C5

#define NUMSR 5

#define CLOCKINGMODE 1
#define MAPMASK 0x02 
#define MEMORYMODE 0x04

/*======= CRTC Registers ========*/
#define ADDRCRTC 0x3D4
#define DATACRTC 0x3D5

#define NUMCRTC 25

#define EHBR 0x03 /*End Horizontal Blanking Register*/
#define LPHR 0x11 /*Light Pen High register*/

/*======= Graphics Controller Registers =======*/
#define ADDRGCR 0x3CE
#define DATAGCR 0x3CF

#define NUMGCR 9

#define READMAPSELECT  0x04
#define MODEREGISTER 0x05
#define MISCELLANEOUSGCR 0x06

/*======= Attribute Controller Registers =======*/

#define ACRWRITE  0x3C0
#define ACRREAD   0x3C1

#define NUMACR 21

/*======= DAC Registers=======*/

#define WRITEADDRDAC 0x3C8
#define DATADAC 0x3C9
#define READADDRDAC 0x3C7
#define DACMASK 0x3c6

/* ************* *
 *     VGA IO    * 
 * ************* */

/*write the value in the register indexed by AddrReg (for CRTC GCR SR)*/
#define outDataAddrb(value, index, AddrReg) \
  outw( (value << 8) | index, AddrReg)

/*for CRTC GCR SR*/
#define maskReg(set, reset, index ,AddrReg) \
  outb( (inDataAddrb(index, AddrReg) & ~reset ) | set , AddrReg+1 )

#define clearFlipflop() \
  inb(IS1)

uint8_t inDataAddrb(uint8_t index, uint16_t AddrReg);/*read the register indexed by AddrReg (for CRTC GCR SR)*/
uint8_t readACR(uint8_t index);/*Read ACR, does reset PAS bit in the read, unless in index PAS bit is enabled*/
void writeACR(uint8_t value, uint8_t index);/*write ACR, does reset PAS bit in the write, unless in index PAS bit is enabled*/

#endif
