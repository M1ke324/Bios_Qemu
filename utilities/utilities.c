#include "utilities.h"

void *memcpy(void *dest, void *src, uint32_t  count){
  if(dest == NULL || src == NULL)
    return dest;

  uint8_t *d=(uint8_t *)dest;
  const uint8_t *s=(uint8_t *)src;
  
  while(count--)
    *d++=*s++;

  return dest;
}

