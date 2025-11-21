#include "../utilities/utilities.h"
#include "../vga/vga.h"

/*====== BIOS TITLE ======*/
#define BIOSROWS 4
#define CENTERBIOS 25
const char * biosString[BIOSROWS]={
  "  ____   ___   ___   ____   ",
  " | __ ) |_ _| / _ \\ / ___|  ",
  " |  _ \\  | | | (_) |\\___ \\  ",
  " |____/ |___| \\___/ |____/  "
};

/* ************ *
 *  BIOS MAIN   *
 * ************ */

void main(void){
  uint32_t i;

  /*====== VGA INIT ======*/
  vgaInit();
  for(i=0;i<BIOSROWS;i++)
    print(i,CENTERBIOS,biosString[i],WHITEONBLACK);
  
  i++;
  print(++i,0,"\xFE VGA ",WHITEONBLACK);
  print(i,6,"ON",GREENONBLACK);

  /*
    Rest of The hardware that has to be intialized
   */

}
