#include "../utilities/utilities.h"
#include "../vga/vga.h"
#include "../pci/pci.h"
#include "biosPrint.h"

void main(void){
  /*====== VGA INIT ======*/
  vgaInit();
  biosPrint();
  appendl("\xFE VGA",NORMAL);
  append(" ON",SUCCESS);


  /*====== PCI INIT ======*/
  PCIinit();
  appendl("\xFE PCI DEVICES",NORMAL);
  append(" MAPPED",SUCCESS);
}
