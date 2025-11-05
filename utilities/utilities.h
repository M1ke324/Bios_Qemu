#ifndef UTILITIES32
#define UTILITIES32

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

extern void outToRegisterB(uint8_t data, uint16_t reg);
extern void outToRegisterL(uint32_t data, uint16_t port);

extern uint8_t inFromRegistersB(uint32_t reg);
extern uint32_t inFromRegistersL(uint32_t reg);

void *memcpy(void *dest, void *src, uint32_t  count);

#endif
