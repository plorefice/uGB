//
//  processor.h
//  GBEmu
//
//  Created by Pietro Lorefice on 14/05/15.
//
//

#ifndef _CORE__PROCESSOR_H_
#define _CORE__PROCESSOR_H_

#include "types.h"

#include "memory.h"

typedef struct {
    r16 AF, BC, DE, HL, SP, PC;  ///< 16-bit registers
} processor;

#endif /* _CORE__PROCESSOR_H_ */
