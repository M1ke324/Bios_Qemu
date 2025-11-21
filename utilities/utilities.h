#ifndef UTILITIES32
#define UTILITIES32

/*====== TYPES ======*/
#define NULL ((void*)0)

typedef enum {
  false,
  true
} bool;

/*stdint.h style*/
typedef char int8_t;
typedef short int16_t;
typedef long int32_t;
typedef long long int64_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;
typedef unsigned long long uint64_t;
typedef uint32_t size_t;
typedef uint32_t uintptr_t;

/*====== IO ======*/
extern void outb(uint8_t data, uint16_t port);
extern void outw(uint16_t data, uint16_t port);
extern void outl(uint32_t data, uint16_t port);

extern uint8_t inb(uint16_t port);
extern uint16_t inw(uint16_t port);
extern uint32_t inl(uint16_t port);

/*===== MEM ======*/
void *memcpy(void *dest, void *src, uint32_t  count);

/*====== VIDEO  ======*/
extern volatile uint8_t * const videoMemory;

#define WHITEONBLACK 0x07
#define GREENONBLACK 0x0A

void print(int8_t cols, int8_t rows, const char *string, const uint8_t attribute);
void clearScreen(void);/*Fill the screen with blank spaces*/


#endif
