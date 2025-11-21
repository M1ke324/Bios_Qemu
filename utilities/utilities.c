#include "utilities.h"
#include "../vga/vga.h"/*VIDEOMEM*/

volatile uint8_t * const videoMemory = (volatile uint8_t *)VIDEOMEM;

void *memcpy(void *dest, void *src, uint32_t  count){
  if(dest == NULL || src == NULL)
    return dest;

  uint8_t *d=(uint8_t *)dest;
  const uint8_t *s=(uint8_t *)src;
 
  while(count--)
    *d++ = *s++;

  return dest;
}

/*Fill the screen with blank spaces*/
void clearScreen(void) {
  uint32_t i;
  for (i = 0; i < COLS * ROWS; i++){
    videoMemory[i*2] = 0x20;
    videoMemory[i*2+1] = 0x07;
  }
}

#define LASTLINEADDRESS (VIDEOMEM + COLS*2*(ROWS-1))

/*like printf but without formatting
 *obviously need the string terminatrion carachter
 *the only special charachter that works is \n
 */
void print(uint8_t rows, uint8_t cols,const char *string, const uint8_t attribute){
  if(string==NULL || rows >=ROWS || cols >=COLS)
    return;

  volatile uint8_t *vm = ((volatile uint8_t * ) VIDEOMEM ) + (cols + rows*COLS) * 2;

  while(*string != '\0'){

    if(*string == '\n'){
      string++;
      if(vm > (volatile uint8_t * ) LASTLINEADDRESS)
        vm=(volatile uint8_t * )VIDEOMEM;
      else
        vm += (COLS*2) - (((uintptr_t)(vm) - VIDEOMEM) % (COLS*2));/*next line address calculation*/
    }

    *vm++ = *string++;
    *vm++ = attribute;
  }

}


