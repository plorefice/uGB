//
// File: types.h
//
// Created by: Pietro Lorefice, May 2015
//

#ifndef _ARCH__TYPES_H_
#define _ARCH__TYPES_H_

#include <stdint.h>

/* Logic types */
typedef uint8_t bool;

#define true  1
#define false 0

/* Arithmetic types */
typedef uint8_t  byte;
typedef uint16_t word;

typedef uint8_t  u8;
typedef uint16_t u16;

typedef int8_t   s8;
typedef int16_t  s16;

/* Register types */
typedef uint8_t  bit;
typedef uint8_t  r8;
typedef uint16_t r16;

/* Instruction types */
typedef uint8_t  opcode;

typedef uint32_t clock;


#endif /* _ARCH__TYPES_H_ */
