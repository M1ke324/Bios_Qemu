#ifndef UTILITIES32
#define UTILITIES32

/*====== TYPES ======*/

#define NULL ((void*)0)

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long uint64_t;
typedef uint32_t size_t;

typedef enum {
  false,
  true
} bool;

/*====== IO ======*/
extern void outb(uint8_t data, uint16_t reg);
extern void outw(uint16_t data, uint16_t reg);
extern void outl(uint32_t data, uint16_t reg);

extern uint8_t inb(uint16_t reg);
extern uint32_t inl(uint16_t reg);

/*====== MEM ======*/
void *memcpy(void *dest, void *src, uint32_t  count);

#endif
