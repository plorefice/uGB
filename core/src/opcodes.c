//
//  opcodes.c
//  GBEmu
//
//  Created by Pietro Lorefice on 14/05/15.
//
//

#include "opcodes.h"
#include "processor.h"


static const char * mnemonics[] = {
	"NOP",
	"LC BC, d16",
	"LC (BC), A",

};



static void exec_INCH(processor * z80, r16 * reg)
{
	byte B = (*reg) >> 8;
	B += 1;
	reg_loadH(reg, B);

	z80->Z = (B == 0);
	z80->N = 0;
	z80->H = (B & 0x0F) == 0x00;
}

static void exec_INCL(processor * z80, r16 * reg)
{
	byte B = (*reg) & 0x00FF;
	B += 1;
	reg_loadL(reg, B);

	z80->Z = (B == 0);
	z80->N = 0;
	z80->H = (B & 0x0F) == 0x00;
}

static void exec_DECH(processor * z80, r16 * reg)
{
	byte B = (*reg) >> 8;
	B -= 1;
	reg_loadH(reg, B);

	z80->Z = (B == 0);
	z80->N = 1;
	z80->H = (B & 0x0F) == 0x0F;
}

static void exec_DECL(processor * z80, r16 * reg)
{
	byte B = (*reg) & 0x00FF;
	B -= 1;
	reg_loadL(reg, B);

	z80->Z = (B == 0);
	z80->N = 1;
	z80->H = (B & 0x0F) == 0x0F;
}

static void exec_ADDHL(processor * z80, r16 * reg, r16 op)
{
	int res = (*reg) + op;

	z80->N = 0;
	z80->H = (((*reg) ^ op ^ (res & 0xFFFF)) & 0x1000) ? 1 : 0;
	z80->C = (res & 0x10000) ? 1 : 0;

	*reg = (r16)(res & 0xFFFF);
}




static void exec_0x00 (processor * z80)
{
    /* NOP */
    return;
}

static void exec_0x01 (processor * z80)
{
    /* LD BC, d16 */
	reg_loadHL(&z80->BC, memory_r16(&z80->MEM, z80->PC));
	z80->PC += 2;
}

static void exec_0x02 (processor * z80)
{
    /* LD (BC), A */
    memory_w8(&z80->MEM, z80->BC, z80->AF >> 8);
}

static void exec_0x03(processor * z80)
{
	/* INC BC */
	z80->BC += 1;
}

static void exec_0x04(processor * z80)
{
	/* INC B */
	exec_INCH(z80, &z80->BC);
}

static void exec_0x05(processor * z80)
{
	/* DEC B */
	exec_DECH(z80, &z80->BC);
}

static void exec_0x06(processor * z80)
{
	/* LD B, d8 */
	reg_loadH(&z80->BC, memory_r8(&z80->MEM, z80->PC));
	z80->PC += 1;
}

static void exec_0x07(processor * z80)
{
	/* RLCA */
	byte A = z80->AF >> 8;
	proc_clrflg(z80);
	z80->C = (A & 0x80) ? 1 : 0;
	reg_loadH(&z80->AF, (A << 1) | (z80->C));
}

static void exec_0x08(processor * z80)
{
	/* LD (a16), SP */
	word addr = memory_r16(&z80->MEM, z80->PC);
	z80->PC += 2;
	memory_w16(&z80->MEM, addr, z80->SP);
}

static void exec_0x09(processor * z80)
{
	/* ADD HL, BC */
	exec_ADDHL(z80, &z80->HL, z80->BC);
}

static void exec_0x0A(processor * z80)
{
	/* LD A, (BC) */
	reg_loadH(&z80->AF, memory_r8(&z80->MEM, z80->BC));
}

static void exec_0x0B(processor * z80)
{
	/* DEC BC */
	z80->BC -= 1;
}

static void exec_0x0C(processor * z80)
{
	/* INC C */
	exec_INCL(z80, &z80->BC);
}

static void exec_0x0D(processor * z80)
{
	/* DEC C */
	exec_DECL(z80, &z80->BC);
}

static void exec_0x0E(processor * z80)
{
	/* LD C, d8 */
	reg_loadL(&z80->BC, memory_r8(&z80->MEM, z80->PC));
	z80->PC += 1;
}

static void exec_0x0F(processor * z80)
{
	/* RRCA */
	byte A = z80->AF >> 8;
	proc_clrflg(z80);
	z80->C = (A & 0x01) ? 1 : 0;
	reg_loadH(&z80->AF, (z80->C << 7) | (A >> 1));
}

static void exec_0x10(processor * z80)
{
	/* STOP */
	z80->PC += 1;
}

static void exec_0x11(processor * z80)
{
	/* LD DE, d16 */
	reg_loadHL(&z80->DE, memory_r16(&z80->MEM, z80->PC));
	z80->PC += 2;
}

static void exec_0x12(processor * z80)
{
	/* LD (DE), A */
	memory_w8(&z80->MEM, z80->DE, z80->AF >> 8);
}

static void exec_0x13(processor * z80)
{
	/* INC DE */
	z80->DE += 1;
}

static void exec_0x14(processor * z80)
{
	/* INC D */
	exec_INCH(z80, &z80->DE);
}

static void exec_0x15(processor * z80)
{
	/* DEC D */
	exec_DECH(z80, &z80->DE);
}

static void exec_0x16(processor * z80)
{
	/* LD D, d8 */
	reg_loadH(&z80->DE, memory_r8(&z80->MEM, z80->PC));
	z80->PC += 1;
}

static void exec_0x17(processor * z80)
{
	/* RLA */
	byte A = z80->AF >> 8;
	reg_loadH(&z80->AF, (A << 1) | (z80->C));

	proc_clrflg(z80);
	z80->C = (A & 0x80) ? 1 : 0;
}

static void exec_0x18(processor * z80)
{
	/* JR r8 */
	s8 dsp = memory_r8(&z80->MEM, z80->PC);
	z80->PC += 1;

	z80->PC += dsp;
}

static void exec_0x19(processor * z80)
{
	/* ADD HL, DE */
	exec_ADDHL(z80, &z80->HL, z80->DE);
}

static void exec_0x1A(processor * z80)
{
	/* LD A, (DE) */
	reg_loadH(&z80->AF, memory_r8(&z80->MEM, z80->DE));
}

static void exec_0x1B(processor * z80)
{
	/* DEC DE */
	z80->DE -= 1;
}

static void exec_0x1C(processor * z80)
{
	/* INC E */
	exec_INCL(z80, &z80->DE);
}

static void exec_0x1D(processor * z80)
{
	/* DEC E */
	exec_DECL(z80, &z80->DE);
}

static void exec_0x1E(processor * z80)
{
	/* LD E, d8 */
	reg_loadL(&z80->DE, memory_r8(&z80->MEM, z80->PC));
	z80->PC += 1;
}

static void exec_0x1F(processor * z80)
{
	/* RRA */
	byte A = z80->AF >> 8;
	reg_loadH(&z80->AF, (z80->C << 7) | (A >> 1));

	proc_clrflg(z80);
	z80->C = (A & 0x01) ? 1 : 0;
}

static void exec_0x20(processor * z80)
{
	/* JR NZ, r8 */
	if (!z80->Z)
		exec_0x18(z80); // JR r8
}

static void exec_0x21(processor * z80)
{
	/* LD HL, d16 */
	reg_loadHL(&z80->HL, memory_r16(&z80->MEM, z80->PC));
	z80->PC += 2;
}

static void exec_0x22(processor * z80)
{
	/* LD (HL+), A */
	memory_w8(&z80->MEM, z80->HL, z80->AF >> 8);
	z80->HL += 1;
}

static void exec_0x23(processor * z80)
{
	/* INC HL */
	z80->HL += 1;
}

static void exec_0x24(processor * z80)
{
	/* INC H */
	exec_INCH(z80, &z80->HL);
}

static void exec_0x25(processor * z80)
{
	/* DEC H */
	exec_DECH(z80, &z80->HL);
}

static void exec_0x26(processor * z80)
{
	/* LD H, d8 */
	reg_loadH(&z80->HL, memory_r8(&z80->MEM, z80->PC));
	z80->PC += 1;
}

static void exec_0x27(processor * z80)
{
	/* DAA */
	int a = z80->AF >> 8;

	if (!z80->N)
	{
		if (z80->H || (a & 0xF) > 9)
			a += 0x06;

		if (z80->C || a > 0x9F)
			a += 0x06;
	}
	else
	{
		if (z80->H)
			a = (a - 6) & 0xFF;

		if (z80->C)
			a -= 0x60;
	}

	z80->H = 0;
	z80->Z = 0;

	if ((a & 0x100) == 0x100)
		z80->C = 1;

	a &= 0xFF;

	if (a == 0)
		z80->Z = 1;

	reg_loadH(&z80->AF, (byte)a);
}

static void exec_0x28(processor * z80)
{
	/* JR Z, r8 */
	if (z80->Z)
		exec_0x18(z80);
}

static void exec_0x29(processor * z80)
{
	/* ADD HL, HL */
	exec_ADDHL(z80, &z80->HL, z80->HL);
}

static void exec_0x2A(processor * z80)
{
	/* LD A, (HL+) */
	reg_loadH(&z80->AF, memory_r8(&z80->MEM, z80->HL));
	z80->HL += 1;
}

static void exec_0x2B(processor * z80)
{
	/* DEC HL */
	z80->HL -= 1;
}

static void exec_0x2C(processor * z80)
{
	/* INC L */
	exec_INCL(z80, &z80->HL);
}

static void exec_0x2D(processor * z80)
{
	/* DEC L */
	exec_DECL(z80, &z80->HL);
}

static void exec_0x2E(processor * z80)
{
	/* LD L, d8 */
	reg_loadL(&z80->HL, memory_r8(&z80->MEM, z80->PC));
	z80->PC += 1;
}

static void exec_0x2F(processor * z80)
{
	/* CPL */
	byte B = z80->AF >> 8;
	reg_loadH(&z80->AF, ~B);

	z80->N = 1;
	z80->H = 1;
}



static void exec_0x30(processor * z80)
{
	/* JR NC, r8 */
	if (!z80->C)
		exec_0x18(z80); // JR r8
}

static void exec_0x31(processor * z80)
{
	/* LD SP, d16 */
	reg_loadHL(&z80->SP, memory_r16(&z80->MEM, z80->PC));
	z80->PC += 2;
}

static void exec_0x32(processor * z80)
{
	/* LD (HL-), A */
	memory_w8(&z80->MEM, z80->HL, z80->AF >> 8);
	z80->HL -= 1;
}

static void exec_0x33(processor * z80)
{
	/* INC SP */
	z80->SP += 1;
}

static void exec_0x34(processor * z80)
{
	/* INC (HL) */
	byte B = memory_r8(&z80->MEM, z80->HL);
	B += 1;
	memory_w8(&z80->MEM, z80->HL, B);

	z80->Z = (B == 0);
	z80->N = 0;
	z80->H = (B & 0x0F) == 0x00;
}

static void exec_0x35(processor * z80)
{
	/* DEC (HL) */
	byte B = memory_r8(&z80->MEM, z80->HL);
	B -= 1;
	memory_w8(&z80->MEM, z80->HL, B);

	z80->Z = (B == 0);
	z80->N = 1;
	z80->H = (B & 0x0F) == 0xF0;
}

static void exec_0x36(processor * z80)
{
	/* LD (HL), d8 */
	memory_w8(&z80->MEM, z80->HL, memory_r8(&z80->MEM, z80->PC));
	z80->PC += 1;
}

static void exec_0x37(processor * z80)
{
	/* SCF */
	z80->N = 0;
	z80->H = 0;
	z80->C = 1;
}

static void exec_0x38(processor * z80)
{
	/* JR C, r8 */
	if (z80->C)
		exec_0x18(z80);
}

static void exec_0x39(processor * z80)
{
	/* ADD HL, SP */
	exec_ADDHL(z80, &z80->HL, z80->SP);
}

static void exec_0x3A(processor * z80)
{
	/* LD A, (HL-) */
	reg_loadH(&z80->AF, memory_r8(&z80->MEM, z80->HL));
	z80->HL -= 1;
}

static void exec_0x3B(processor * z80)
{
	/* DEC SP */
	z80->SP -= 1;
}

static void exec_0x3C(processor * z80)
{
	/* INC A */
	exec_INCH(z80, &z80->AF);
}

static void exec_0x3D(processor * z80)
{
	/* DEC A */
	exec_DECH(z80, &z80->AF);
}

static void exec_0x3E(processor * z80)
{
	/* LD A, d8 */
	reg_loadH(&z80->AF, memory_r8(&z80->MEM, z80->PC));
	z80->PC += 1;
}

static void exec_0x3F(processor * z80)
{
	/* CCF */
	z80->N = 0;
	z80->H = 0;
	z80->C = (z80->C) ? 0 : 1;
}

static void exec_0x40(processor * z80)
{
	/* LD B, B */
	reg_loadH(&z80->BC, z80->BC >> 8);
}

static void exec_0x41(processor * z80)
{
	/* LD B, C */
	reg_loadH(&z80->BC, z80->BC & 0xFF);
}

static void exec_0x42(processor * z80)
{
	/* LD B, D */
	reg_loadH(&z80->BC, z80->DE >> 8);
}

static void exec_0x43(processor * z80)
{
	/* LD B, E */
	reg_loadH(&z80->BC, z80->DE & 0xFF);
}

static void exec_0x44(processor * z80)
{
	/* LD B, H */
	reg_loadH(&z80->BC, z80->HL >> 8);
}

static void exec_0x45(processor * z80)
{
	/* LD B, L */
	reg_loadH(&z80->BC, z80->HL & 0xFF);
}

static void exec_0x46(processor * z80)
{
	/* LD B, (HL) */
	reg_loadH(&z80->BC, memory_r8(&z80->MEM, z80->HL));
}

static void exec_0x47(processor * z80)
{
	/* LD B, A */
	reg_loadH(&z80->BC, z80->AF >> 8);
}

static void exec_0x48(processor * z80)
{
	/* LD C, B */
	reg_loadL(&z80->BC, z80->BC >> 8);
}

static void exec_0x49(processor * z80)
{
	/* LD C, C */
	reg_loadL(&z80->BC, z80->BC & 0xFF);
}

static void exec_0x4A(processor * z80)
{
	/* LD C, D */
	reg_loadL(&z80->BC, z80->DE >> 8);
}

static void exec_0x4B(processor * z80)
{
	/* LD C, E */
	reg_loadL(&z80->BC, z80->DE & 0xFF);
}

static void exec_0x4C(processor * z80)
{
	/* LD C, H */
	reg_loadL(&z80->BC, z80->HL >> 8);
}

static void exec_0x4D(processor * z80)
{
	/* LD C, L */
	reg_loadL(&z80->BC, z80->HL & 0xFF);
}

static void exec_0x4E(processor * z80)
{
	/* LD C, (HL) */
	reg_loadL(&z80->BC, memory_r8(&z80->MEM, z80->HL));
}

static void exec_0x4F(processor * z80)
{
	/* LD C, A */
	reg_loadL(&z80->BC, z80->AF >> 8);
}

static void exec_0x50(processor * z80)
{
	/* LD D, B */
	reg_loadH(&z80->DE, z80->BC >> 8);
}

static void exec_0x51(processor * z80)
{
	/* LD D, C */
	reg_loadH(&z80->DE, z80->BC & 0xFF);
}

static void exec_0x52(processor * z80)
{
	/* LD D, D */
	reg_loadH(&z80->DE, z80->DE >> 8);
}

static void exec_0x53(processor * z80)
{
	/* LD D, E */
	reg_loadH(&z80->DE, z80->DE & 0xFF);
}

static void exec_0x54(processor * z80)
{
	/* LD D, H */
	reg_loadH(&z80->DE, z80->HL >> 8);
}

static void exec_0x55(processor * z80)
{
	/* LD D, L */
	reg_loadH(&z80->DE, z80->HL & 0xFF);
}

static void exec_0x56(processor * z80)
{
	/* LD D, (HL) */
	reg_loadH(&z80->DE, memory_r8(&z80->MEM, z80->HL));
}

static void exec_0x57(processor * z80)
{
	/* LD D, A */
	reg_loadH(&z80->DE, z80->AF >> 8);
}

static void exec_0x58(processor * z80)
{
	/* LD E, B */
	reg_loadL(&z80->DE, z80->BC >> 8);
}

static void exec_0x59(processor * z80)
{
	/* LD E, C */
	reg_loadL(&z80->DE, z80->BC & 0xFF);
}

static void exec_0x5A(processor * z80)
{
	/* LD E, D */
	reg_loadL(&z80->DE, z80->DE >> 8);
}

static void exec_0x5B(processor * z80)
{
	/* LD E, E */
	reg_loadL(&z80->DE, z80->DE & 0xFF);
}

static void exec_0x5C(processor * z80)
{
	/* LD E, H */
	reg_loadL(&z80->DE, z80->HL >> 8);
}

static void exec_0x5D(processor * z80)
{
	/* LD E, L */
	reg_loadL(&z80->DE, z80->HL & 0xFF);
}

static void exec_0x5E(processor * z80)
{
	/* LD E, (HL) */
	reg_loadL(&z80->DE, memory_r8(&z80->MEM, z80->HL));
}

static void exec_0x5F(processor * z80)
{
	/* LD E, A */
	reg_loadL(&z80->DE, z80->AF >> 8);
}

static void exec_0x60(processor * z80)
{
	/* LD H, B */
	reg_loadH(&z80->HL, z80->BC >> 8);
}

static void exec_0x61(processor * z80)
{
	/* LD H, C */
	reg_loadH(&z80->HL, z80->BC & 0xFF);
}

static void exec_0x62(processor * z80)
{
	/* LD H, D */
	reg_loadH(&z80->HL, z80->DE >> 8);
}

static void exec_0x63(processor * z80)
{
	/* LD H, E */
	reg_loadH(&z80->HL, z80->DE & 0xFF);
}

static void exec_0x64(processor * z80)
{
	/* LD H, H */
	reg_loadH(&z80->HL, z80->HL >> 8);
}

static void exec_0x65(processor * z80)
{
	/* LD H, L */
	reg_loadH(&z80->HL, z80->HL & 0xFF);
}

static void exec_0x66(processor * z80)
{
	/* LD H, (HL) */
	reg_loadH(&z80->HL, memory_r8(&z80->MEM, z80->HL));
}

static void exec_0x67(processor * z80)
{
	/* LD H, A */
	reg_loadH(&z80->HL, z80->AF >> 8);
}

static void exec_0x68(processor * z80)
{
	/* LD L, B */
	reg_loadL(&z80->HL, z80->BC >> 8);
}

static void exec_0x69(processor * z80)
{
	/* LD L, C */
	reg_loadL(&z80->HL, z80->BC & 0xFF);
}

static void exec_0x6A(processor * z80)
{
	/* LD L, D */
	reg_loadL(&z80->HL, z80->DE >> 8);
}

static void exec_0x6B(processor * z80)
{
	/* LD L, E */
	reg_loadL(&z80->HL, z80->DE & 0xFF);
}

static void exec_0x6C(processor * z80)
{
	/* LD L, H */
	reg_loadL(&z80->HL, z80->HL >> 8);
}

static void exec_0x6D(processor * z80)
{
	/* LD L, L */
	reg_loadL(&z80->HL, z80->HL & 0xFF);
}

static void exec_0x6E(processor * z80)
{
	/* LD L, (HL) */
	reg_loadL(&z80->HL, memory_r8(&z80->MEM, z80->HL));
}

static void exec_0x6F(processor * z80)
{
	/* LD L, A */
	reg_loadL(&z80->HL, z80->AF >> 8);
}

static void exec_0x70(processor * z80)
{
	/* LD (HL), B */
	memory_w8(&z80->MEM, z80->HL, z80->BC >> 8);
}

static void exec_0x71(processor * z80)
{
	/* LD (HL), C */
	memory_w8(&z80->MEM, z80->HL, z80->BC & 0xFF);
}

static void exec_0x72(processor * z80)
{
	/* LD (HL), D */
	memory_w8(&z80->MEM, z80->HL, z80->DE >> 8);
}

static void exec_0x73(processor * z80)
{
	/* LD (HL), E */
	memory_w8(&z80->MEM, z80->HL, z80->DE & 0xFF);
}

static void exec_0x74(processor * z80)
{
	/* LD (HL), H */
	memory_w8(&z80->MEM, z80->HL, z80->HL >> 8);
}

static void exec_0x75(processor * z80)
{
	/* LD (HL), L */
	memory_w8(&z80->MEM, z80->HL, z80->HL & 0xFF);
}

static void exec_0x76(processor * z80)
{
	/* HALT */
	z80->hWFI = 1;
}

static void exec_0x77(processor * z80)
{
	/* LD (HL), A */
	memory_w8(&z80->MEM, z80->HL, z80->AF >> 8);
}

static void exec_0x78(processor * z80)
{
	/* LD A, B */
	reg_loadL(&z80->AF, z80->BC >> 8);
}

static void exec_0x79(processor * z80)
{
	/* LD A, C */
	reg_loadL(&z80->AF, z80->BC & 0xFF);
}

static void exec_0x7A(processor * z80)
{
	/* LD A, D */
	reg_loadL(&z80->AF, z80->DE >> 8);
}

static void exec_0x7B(processor * z80)
{
	/* LD A, E */
	reg_loadL(&z80->AF, z80->DE & 0xFF);
}

static void exec_0x7C(processor * z80)
{
	/* LD A, H */
	reg_loadL(&z80->AF, z80->HL >> 8);
}

static void exec_0x7D(processor * z80)
{
	/* LD A, L */
	reg_loadL(&z80->AF, z80->HL & 0xFF);
}

static void exec_0x7E(processor * z80)
{
	/* LD A, (HL) */
	reg_loadL(&z80->AF, memory_r8(&z80->MEM, z80->HL));
}

static void exec_0x7F(processor * z80)
{
	/* LD A, A */
	reg_loadL(&z80->AF, z80->AF >> 8);
}


static void exec_0xC3(processor * z80)
{
	/* JP a16 */
	z80->PC = memory_r16(&z80->MEM, z80->PC);
}


const dispatch_t dispatch[] = {
	exec_0x00,
	exec_0x01,
	exec_0x02,
	exec_0x03,
	exec_0x04,
	exec_0x05,
	exec_0x06,
	exec_0x07,
	exec_0x08,
	exec_0x09,
	exec_0x0A,
	exec_0x0B,
	exec_0x0C,
	exec_0x0D,
	exec_0x0E,
	exec_0x0F,
	exec_0x10,
	exec_0x11,
	exec_0x12,
	exec_0x13,
	exec_0x14,
	exec_0x15,
	exec_0x16,
	exec_0x17,
	exec_0x18,
	exec_0x19,
	exec_0x1A,
	exec_0x1B,
	exec_0x1C,
	exec_0x1D,
	exec_0x1E,
	exec_0x1F,
	exec_0x20,
	exec_0x21,
	exec_0x22,
	exec_0x23,
	exec_0x24,
	exec_0x25,
	exec_0x26,
	exec_0x27,
	exec_0x28,
	exec_0x29,
	exec_0x2A,
	exec_0x2B,
	exec_0x2C,
	exec_0x2D,
	exec_0x2E,
	exec_0x2F,
	exec_0x30,
	exec_0x31,
	exec_0x32,
	exec_0x33,
	exec_0x34,
	exec_0x35,
	exec_0x36,
	exec_0x37,
	exec_0x38,
	exec_0x39,
	exec_0x3A,
	exec_0x3B,
	exec_0x3C,
	exec_0x3D,
	exec_0x3E,
	exec_0x3F,
	exec_0x40,
	exec_0x41,
	exec_0x42,
	exec_0x43,
	exec_0x44,
	exec_0x45,
	exec_0x46,
	exec_0x47,
	exec_0x48,
	exec_0x49,
	exec_0x4A,
	exec_0x4B,
	exec_0x4C,
	exec_0x4D,
	exec_0x4E,
	exec_0x4F,
	exec_0x50,
	exec_0x51,
	exec_0x52,
	exec_0x53,
	exec_0x54,
	exec_0x55,
	exec_0x56,
	exec_0x57,
	exec_0x58,
	exec_0x59,
	exec_0x5A,
	exec_0x5B,
	exec_0x5C,
	exec_0x5D,
	exec_0x5E,
	exec_0x5F,
	exec_0x60,
	exec_0x61,
	exec_0x62,
	exec_0x63,
	exec_0x64,
	exec_0x65,
	exec_0x66,
	exec_0x67,
	exec_0x68,
	exec_0x69,
	exec_0x6A,
	exec_0x6B,
	exec_0x6C,
	exec_0x6D,
	exec_0x6E,
	exec_0x6F,
	exec_0x70,
	exec_0x71,
	exec_0x72,
	exec_0x73,
	exec_0x74,
	exec_0x75,
	exec_0x76,
	exec_0x77,
	exec_0x78,
	exec_0x79,
	exec_0x7A,
	exec_0x7B,
	exec_0x7C,
	exec_0x7D,
	exec_0x7E,
	exec_0x7F,
	0, //exec_0x80,
	0, //exec_0x81,
	0, //exec_0x82,
	0, //exec_0x83,
	0, //exec_0x84,
	0, //exec_0x85,
	0, //exec_0x86,
	0, //exec_0x87,
	0, //exec_0x88,
	0, //exec_0x89,
	0, //exec_0x8A,
	0, //exec_0x8B,
	0, //exec_0x8C,
	0, //exec_0x8D,
	0, //exec_0x8E,
	0, //exec_0x8F,
	0, //exec_0x90,
	0, //exec_0x91,
	0, //exec_0x92,
	0, //exec_0x93,
	0, //exec_0x94,
	0, //exec_0x95,
	0, //exec_0x96,
	0, //exec_0x97,
	0, //exec_0x98,
	0, //exec_0x99,
	0, //exec_0x9A,
	0, //exec_0x9B,
	0, //exec_0x9C,
	0, //exec_0x9D,
	0, //exec_0x9E,
	0, //exec_0x9F,
	0, //exec_0xA0,
	0, //exec_0xA1,
	0, //exec_0xA2,
	0, //exec_0xA3,
	0, //exec_0xA4,
	0, //exec_0xA5,
	0, //exec_0xA6,
	0, //exec_0xA7,
	0, //exec_0xA8,
	0, //exec_0xA9,
	0, //exec_0xAA,
	0, //exec_0xAB,
	0, //exec_0xAC,
	0, //exec_0xAD,
	0, //exec_0xAE,
	0, //exec_0xAF,
	0, //exec_0xB0,
	0, //exec_0xB1,
	0, //exec_0xB2,
	0, //exec_0xB3,
	0, //exec_0xB4,
	0, //exec_0xB5,
	0, //exec_0xB6,
	0, //exec_0xB7,
	0, //exec_0xB8,
	0, //exec_0xB9,
	0, //exec_0xBA,
	0, //exec_0xBB,
	0, //exec_0xBC,
	0, //exec_0xBD,
	0, //exec_0xBE,
	0, //exec_0xBF,
	0, //exec_0xC0,
	0, //exec_0xC1,
	0, //exec_0xC2,
	exec_0xC3,
	0, //exec_0xC4,
	0, //exec_0xC5,
	0, //exec_0xC6,
	0, //exec_0xC7,
	0, //exec_0xC8,
	0, //exec_0xC9,
	0, //exec_0xCA,
	0, //exec_0xCB,
	0, //exec_0xCC,
	0, //exec_0xCD,
	0, //exec_0xCE,
	0, //exec_0xCF,
	0, //exec_0xD0,
	0, //exec_0xD1,
	0, //exec_0xD2,
	0, //exec_0xD3,
	0, //exec_0xD4,
	0, //exec_0xD5,
	0, //exec_0xD6,
	0, //exec_0xD7,
	0, //exec_0xD8,
	0, //exec_0xD9,
	0, //exec_0xDA,
	0, //exec_0xDB,
	0, //exec_0xDC,
	0, //exec_0xDD,
	0, //exec_0xDE,
	0, //exec_0xDF,
	0, //exec_0xE0,
	0, //exec_0xE1,
	0, //exec_0xE2,
	0, //exec_0xE3,
	0, //exec_0xE4,
	0, //exec_0xE5,
	0, //exec_0xE6,
	0, //exec_0xE7,
	0, //exec_0xE8,
	0, //exec_0xE9,
	0, //exec_0xEA,
	0, //exec_0xEB,
	0, //exec_0xEC,
	0, //exec_0xED,
	0, //exec_0xEE,
	0, //exec_0xEF,
	0, //exec_0xF0,
	0, //exec_0xF1,
	0, //exec_0xF2,
	0, //exec_0xF3,
	0, //exec_0xF4,
	0, //exec_0xF5,
	0, //exec_0xF6,
	0, //exec_0xF7,
	0, //exec_0xF8,
	0, //exec_0xF9,
	0, //exec_0xFA,
	0, //exec_0xFB,
	0, //exec_0xFC,
	0, //exec_0xFD,
	0, //exec_0xFE,
	0, //exec_0xFF
};
