#include "utilities.h"
#include "../vga/vga.h"/*VIDEOMEM*/

/*====== MEM ======*/

void *memcpy(void *dest, void *src, uint32_t  count){
  if(dest == NULL || src == NULL)
    return dest;

  uint8_t *d=(uint8_t *)dest;
  const uint8_t *s=(uint8_t *)src;
 
  while(count--)
    *d++ = *s++;

  return dest;
}

/*====== VIDEO ======*/

volatile uint8_t * const videoMemory = (volatile uint8_t *)VIDEOMEM;

/*Fill the screen with blank spaces*/
void clearScreen(void) {
  uint32_t i;
  for (i = 0; i < COLS * ROWS; i++){
    videoMemory[i*2] = 0x20;
    videoMemory[i*2+1] = 0x07;
  }
}

#define LASTLINEADDRESS (VIDEOMEM + COLS*2*(ROWS-1))
#define XADDRESS 0x08000
#define YADDRESS (XADDRESS+ sizeof(uint8_t))

uint8_t * const lastWriteX = (uint8_t *)XADDRESS;
uint8_t * const lastWriteY = (uint8_t *)YADDRESS;


/*like printf but without formatting
 *obviously need the string terminatrion carachter
 *the only special charachter that works is \n
 */
void print(int8_t rows, int8_t cols,const char *string, const uint8_t attribute){
  if(!string || rows >=ROWS || cols >=COLS)
    return;

  if(rows==-1 &&cols==-1){
    cols=*lastWriteX;
    rows=*lastWriteY;
  }
  if(rows==-1 && cols==0){
    rows=(*lastWriteY==ROWS-1)? 0:*lastWriteY+1;
    cols=0;
  }

  volatile uint8_t *vm = ((volatile uint8_t * ) VIDEOMEM ) + (cols + rows*COLS) * 2;

  while(*string != '\0'){
    if(*string == '\n'){
      string++;
      if(vm > (volatile uint8_t * ) LASTLINEADDRESS){
        vm=(volatile uint8_t * )VIDEOMEM;
        rows=cols=0;

      }else{
        vm +=(COLS - cols)*2;
        rows+=1;
        cols=0;
      }
      continue;/*in case of 2 \n\n or \n\0 much more common*/
    }

    *vm++ = *string++;
    *vm++ = attribute;
    if(++(cols) == COLS){
      cols=0;
      if(++(rows) == ROWS){
        rows=0;
        vm=(volatile uint8_t * )VIDEOMEM;
      }
    }
  }
  *lastWriteY=rows;
  *lastWriteX=cols;
}

void intToStr(uint32_t value, char *buffer, int base) {
    char *ptr = buffer, *ptr1 = buffer, tmp_char;

    if (base < 2 || base > 16) {
        *buffer = '\0'; 
        return;
    }

    /* Conversion*/
    do {
        int remainder = value % base;
        *ptr++ = (remainder < 10) ? (remainder + '0') : (remainder - 10 + 'A');
    } while (value /= base);

    *ptr-- = '\0';

    /* Inversion of the string*/
    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }
}
