//
//  cartridge.h
//  GBEmu
//
//  Created by Pietro Lorefice on 15/05/15.
//
//

#ifndef _CORE__CARTRIDGE_H_
#define _CORE__CARTRIDGE_H_

#include "types.h"

typedef struct {
	byte ROM_BNK_00[0x4000];
	byte ROM_BNK_01[0x4000];
} cartridge;

void cart_init(cartridge * cart);
void cart_load_bnk(cartridge * cart, byte * rom, word size, byte id);

#endif
