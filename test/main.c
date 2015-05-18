//
//  main.c
//  GBEmu
//
//  Created by Pietro Lorefice on 15/05/15.
//
//

#include "processor.h"
#include "cartridge.h"
#include "graphics.h"

#include <stdio.h>
#include <sys/stat.h>

processor z80;
memory mem;
graphics ppu;
cartridge cart;

int main(int argc, char * argv[])
{
	FILE * f;
	struct stat fs;
	size_t fsize, byteRead;

	byte cart_content[0x4000];

	proc_init(&z80);
    memory_init(&mem);
    graphics_init(&ppu);
    cart_init(&cart);
    
    z80.MEM = &mem;
    mem.VRAM = &ppu.VRAM[0];

	/* load cart */
	proc_load_cart(&z80, &cart);

	/* load rom */
	f = fopen("cpu_instrs.gb", "rb");
	fstat(fileno(f), &fs);
	fsize = fs.st_size;

	while (fsize > 0)
	{
		static byte bnkId = 0;

		byteRead = fread(cart_content, 1, 0x4000, f);
		cart_load_bnk(&cart, cart_content, byteRead, bnkId++);

		fsize -= byteRead;
	}

	while (1)
	{
		proc_fetch(&z80);
	}

	return 0;
}