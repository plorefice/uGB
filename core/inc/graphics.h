//
//  graphics.h
//  GBEmu
//
//  Created by Pietro Lorefice on 17/05/15.
//
//

#ifndef _CORE__GRAPHICS_H_
#define _CORE__GRAPHICS_H_

#include "types.h"

typedef struct {
	byte VRAM[0x2000];
} graphics;


void graphics_init(graphics * ppu);

#endif /* _CORE__GRAPHICS_H_ */