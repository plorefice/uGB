//
//  cartridge.c
//  GBEmu
//
//  Created by Pietro Lorefice on 15/05/15.
//
//

#include "cartridge.h"

#include <string.h>

void
cart_init(cartridge * cart)
{

}

void
cart_load_bnk(cartridge * cart, byte rom[], word size, byte id)
{
	switch (id)
	{
	case 0x00:
		memcpy(cart->ROM_BNK_00, rom, size);
		break;
	default:
		memcpy(cart->ROM_BNK_01, rom, size);
		break;
	}
}
