//
//  opcodes.h
//  GBEmu
//
//  Created by Pietro Lorefice on 14/05/15.
//
//

#ifndef _CORE__OPCODES_H_
#define _CORE__OPCODES_H_

#include "types.h"
#include "processor.h"

typedef void(*dispatch_t)(processor *);

extern const dispatch_t dispatch[];

const char * opcode_name(opcode opc);

#endif /* _CORE__OPCODES_H_ */