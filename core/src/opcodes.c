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
	"LD BC,nn",
	"LD (BC),A",
	"INC BC",
	"INC B",
	"DEC B",
	"LD B,n",
	"RLCA",
	"LD (nn),SP",
	"ADD HL,BC",
	"LD A,(BC)",
	"DEC BC",
	"INC C",
	"DEC C",
	"LD C,n",
	"RRCA",

	"STOP",
	"LD DE,nn",
	"LD (DE),A",
	"INC DE",
	"INC D",
	"DEC D",
	"LD D,n",
	"RLA",
	"JR n",
	"ADD HL,DE",
	"LD A,(DE)",
	"DEC DE",
	"INC E",
	"DEC E",
	"LD E,n",
	"RRA",

	"JR NZ,n",
	"LD HL,nn",
	"LD (HLI),A",
	"INC HL",
	"INC H",
	"DEC H",
	"LD H,n",
	"DAA",
	"JR Z,n",
	"ADD HL,HL",
	"LD A,(HLI)",
	"DEC HL",
	"INC L",
	"DEC L",
	"LD L,n",
	"CPL",

	"JR NC,n",
	"LD SP,nn",
	"LD (HLD),A",
	"INC SP",
	"INC (HL)",
	"DEC (HL)",
	"LD (HL),n",
	"SCF",
	"JR C,n",
	"ADD HL,SP",
	"LD A,(HLD)",
	"DEC SP",
	"INC A",
	"DEC A",
	"LDA,n",
	"CCF",

	"LD B,B",
	"LD B,C",
	"LD B,D",
	"LD B,E",
	"LD B,H",
	"LD B,L",
	"LD B,(HL)",
	"LD B,A",
	"LD C,B",
	"LD C,C",
	"LD C,D",
	"LD C,E",
	"LD C,H",
	"LD C,L",
	"LD C,(HL)",
	"LD C,A",

	"LD D,B",
	"LD D,C",
	"LD D,D",
	"LD D,E",
	"LD D,H",
	"LD D,L",
	"LD D,(HL)",
	"LD D,A",
	"LD E,B",
	"LD E,C",
	"LD E,D",
	"LD E,E",
	"LD E,H",
	"LD E,L",
	"LD E,(HL)",
	"LD E,A",

	"LD H,B",
	"LD H,C",
	"LD H,D",
	"LD H,E",
	"LD H,H",
	"LD H,L",
	"LD H,(HL)",
	"LD H,A",
	"LD L,B",
	"LD L,C",
	"LD L,D",
	"LD L,E",
	"LD L,H",
	"LD L,L",
	"LD L,(HL)",
	"LD L,A",

	"LD (HL),B",
	"LD (HL),C",
	"LD (HL),D",
	"LD (HL),E",
	"LD (HL),H",
	"LD (HL),L",
	"HALT",
	"LD (HL),A",
	"LD A,B",
	"LD A,C",
	"LD A,D",
	"LD A,E",
	"LD A,H",
	"LD A,L",
	"LD A,(HL)",
	"LD A,A",

	"ADD A,B",
	"ADD A,C",
	"ADD A,D",
	"ADD A,E",
	"ADD A,H",
	"ADD A,L",
	"ADD A,(HL)",
	"ADD A,A",
	"ADC A,B",
	"ADC A,C",
	"ADC A,D",
	"ADC A,E",
	"ADC A,H",
	"ADC A,L",
	"ADC A,(HL)",
	"ADC A,A",

	"SUB B",
	"SUB C",
	"SUB D",
	"SUB E",
	"SUB H",
	"SUB L",
	"SUB (HL)",
	"SUB A",
	"SBC A,B",
	"SBC A,C",
	"SBC A,D",
	"SBC A,E",
	"SBC A,H",
	"SBC A,L",
	"SBC A,(HL)",
	"SBC A,A",

	"AND B",
	"AND C",
	"AND D",
	"AND E",
	"AND H",
	"AND L",
	"AND (HL)",
	"AND A",
	"XOR B",
	"XOR C",
	"XOR D",
	"XOR E",
	"XOR H",
	"XOR L",
	"XOR (HL)",
	"XOR A",

	"OR B",
	"OR C",
	"OR D",
	"OR E",
	"OR H",
	"OR L",
	"OR (HL)",
	"OR A",
	"CP B",
	"CP C",
	"CP D",
	"CP E",
	"CP H",
	"CP L",
	"CP (HL)",
	"CP A",

	"RET NZ",
	"POP BC",
	"JP NZ,nn",
	"JP nn",
	"CALL NZ,nn",
	"PUSH BC",
	"ADD A,n",
	"RST ",
	"RET Z",
	"RET",
	"JP Z,nn",
	"cb opcode",
	"CALL Z,nn",
	"CALL nn",
	"ADC A,n",
	"RST 0x08",

	"RET NC",
	"POP DE",
	"JP NC,nn",
	"unused opcode",
	"CALL NC,nn",
	"PUSH DE",
	"SUB n",
	"RST 0x10",
	"RET C",
	"RETI",
	"JP C,nn",
	"unused opcode",
	"CALL C,nn",
	"unused opcode",
	"SBC A,n",
	"RST 0x18",

	"LD (0xFF00+n),A",
	"POP HL",
	"LD (0xFF00+C),A",
	"unused opcode",
	"unused opcode",
	"PUSH HL",
	"AND n",
	"RST 0x20",
	"ADD SP,n",
	"JP (HL)",
	"LD (nn),A",
	"unused opcode",
	"unused opcode",
	"unused opcode",
	"XOR n",
	"RST 0x28",

	"LD A,(0xFF00+n)",
	"POP AF",
	"LD A,(0xFF00+C)",
	"DI",
	"unused opcode",
	"PUSH AF",
	"OR n",
	"RST 0x30",
	"LD HL,SP",
	"LD SP,HL",
	"LD A,(nn)",
	"EI",
	"unused opcode",
	"unused opcode",
	"CP n",
	"RST 0x38"
};

const char * opcode_name(opcode opc)
{
	return mnemonics[opc];
}

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
	z80->H = (((*reg) ^ op ^ (res & 0xFFFF)) & 0x1000) != 0;
	z80->C = (res & 0x10000) != 0;

	reg_loadHL(reg, res & 0xFFFF);
}

static void exec_ADDH(processor * z80, r16 * reg, r8 op)
{
	r8 A = (*reg) >> 8;
	int res = A + op;

	z80->Z = ((res & 0xFF) == 0);
	z80->N = 0;
	z80->H = (((A & 0x0F) + (op & 0x0F)) > 0x0F);
	z80->C = (res > 0xFF);

	reg_loadH(reg, res & 0xFF);
}

static void exec_ADCH(processor * z80, r16 * reg, r8 op)
{
	r8 A = (*reg) >> 8;
	int res = A + op + z80->C;

	z80->Z = ((res & 0xFF) == 0);
	z80->N = 0;
	z80->H = (((A & 0x0F) + (op & 0x0F) + z80->C) > 0x0F);
	z80->C = (res > 0xFF);

	reg_loadH(reg, res & 0xFF);
}

static void exec_SUBH(processor * z80, r16 * reg, r8 op)
{
	r8 A = (*reg) >> 8;
	int res = A - op;

	z80->Z = ((res & 0xFF) == 0);
	z80->N = 1;
	z80->H = (((A & 0x0F) - (op & 0x0F)) < 0);
	z80->C = (res < 0);

	reg_loadH(reg, res & 0xFF);
}

static void exec_SBCH(processor * z80, r16 * reg, r8 op)
{
	r8 A = (*reg) >> 8;
	int res = A - op - z80->C;

	z80->Z = ((res & 0xFF) == 0);
	z80->N = 1;
	z80->H = (((A & 0x0F) - (op & 0x0F) - z80->C) < 0);
	z80->C = (res < 0);

	reg_loadH(reg, res & 0xFF);
}

static void exec_ANDH(processor * z80, r16 * reg, r8 op)
{
	r8 A = (*reg) >> 8;
	r8 res = A & op;

	z80->Z = (res == 0);
	z80->N = 0;
	z80->H = 1;
	z80->C = 0;

	reg_loadH(reg, res);
}

static void exec_XORH(processor * z80, r16 * reg, r8 op)
{
	r8 A = (*reg) >> 8;
	r8 res = A ^ op;

	z80->Z = (res == 0);
	z80->N = 0;
	z80->H = 0;
	z80->C = 0;

	reg_loadH(reg, res);
}

static void exec_ORH(processor * z80, r16 * reg, r8 op)
{
	r8 A = (*reg) >> 8;
	r8 res = A | op;

	z80->Z = (res == 0);
	z80->N = 0;
	z80->H = 0;
	z80->C = 0;

	reg_loadH(reg, res);
}

static void exec_CPH(processor * z80, r16 * reg, r8 op)
{
	r8 A = (*reg) >> 8;
	r8 res = A - op;

	z80->Z = (res == 0);
	z80->N = 1;
	z80->H = (((A & 0x0F) - (op & 0x0F)) < 0);
	z80->C = (res < 0);
}

static void exec_CALL(processor * z80)
{
	memory_w16(z80->MEM, z80->SP - 2, z80->PC);
	z80->PC = memory_r16(z80->MEM, z80->PC);
	z80->SP -= 2;
}

static void exec_RET(processor * z80)
{
	z80->PC = memory_r16(z80->MEM, z80->SP);
	z80->SP += 2;
}



static void exec_0x00 (processor * z80)
{
    /* NOP */

	z80->gClk += 4;
}

static void exec_0x01 (processor * z80)
{
    /* LD BC, d16 */
	reg_loadHL(&z80->BC, memory_r16(z80->MEM, z80->PC));
	z80->PC += 2;

	z80->gClk += 12;
}

static void exec_0x02 (processor * z80)
{
    /* LD (BC), A */
    memory_w8(z80->MEM, z80->BC, z80->AF >> 8);

	z80->gClk += 8;
}

static void exec_0x03(processor * z80)
{
	/* INC BC */
	z80->BC += 1;

	z80->gClk += 8;
}

static void exec_0x04(processor * z80)
{
	/* INC B */
	exec_INCH(z80, &z80->BC);

	z80->gClk += 4;
}

static void exec_0x05(processor * z80)
{
	/* DEC B */
	exec_DECH(z80, &z80->BC);

	z80->gClk += 4;
}

static void exec_0x06(processor * z80)
{
	/* LD B, d8 */
	reg_loadH(&z80->BC, memory_r8(z80->MEM, z80->PC));
	z80->PC += 1;

	z80->gClk += 8;
}

static void exec_0x07(processor * z80)
{
	/* RLCA */
	byte A = z80->AF >> 8;
	proc_clrflg(z80);
	z80->C = (A & 0x80) ? 1 : 0;
	reg_loadH(&z80->AF, (A << 1) | (z80->C));

	z80->gClk += 4;
}

static void exec_0x08(processor * z80)
{
	/* LD (a16), SP */
	word addr = memory_r16(z80->MEM, z80->PC);
	z80->PC += 2;
	memory_w16(z80->MEM, addr, z80->SP);

	z80->gClk += 20;
}

static void exec_0x09(processor * z80)
{
	/* ADD HL, BC */
	exec_ADDHL(z80, &z80->HL, z80->BC);

	z80->gClk += 8;
}

static void exec_0x0A(processor * z80)
{
	/* LD A, (BC) */
	reg_loadH(&z80->AF, memory_r8(z80->MEM, z80->BC));

	z80->gClk += 8;
}

static void exec_0x0B(processor * z80)
{
	/* DEC BC */
	z80->BC -= 1;

	z80->gClk += 8;
}

static void exec_0x0C(processor * z80)
{
	/* INC C */
	exec_INCL(z80, &z80->BC);

	z80->gClk += 4;
}

static void exec_0x0D(processor * z80)
{
	/* DEC C */
	exec_DECL(z80, &z80->BC);

	z80->gClk += 4;
}

static void exec_0x0E(processor * z80)
{
	/* LD C, d8 */
	reg_loadL(&z80->BC, memory_r8(z80->MEM, z80->PC));
	z80->PC += 1;

	z80->gClk += 8;
}

static void exec_0x0F(processor * z80)
{
	/* RRCA */
	byte A = z80->AF >> 8;
	proc_clrflg(z80);
	z80->C = (A & 0x01) ? 1 : 0;
	reg_loadH(&z80->AF, (z80->C << 7) | (A >> 1));

	z80->gClk += 4;
}

static void exec_0x10(processor * z80)
{
	/* STOP */
	z80->PC += 1;

	z80->gClk += 4;
}

static void exec_0x11(processor * z80)
{
	/* LD DE, d16 */
	reg_loadHL(&z80->DE, memory_r16(z80->MEM, z80->PC));
	z80->PC += 2;

	z80->gClk += 12;
}

static void exec_0x12(processor * z80)
{
	/* LD (DE), A */
	memory_w8(z80->MEM, z80->DE, z80->AF >> 8);

	z80->gClk += 8;
}

static void exec_0x13(processor * z80)
{
	/* INC DE */
	z80->DE += 1;

	z80->gClk += 8;
}

static void exec_0x14(processor * z80)
{
	/* INC D */
	exec_INCH(z80, &z80->DE);

	z80->gClk += 4;
}

static void exec_0x15(processor * z80)
{
	/* DEC D */
	exec_DECH(z80, &z80->DE);

	z80->gClk += 4;
}

static void exec_0x16(processor * z80)
{
	/* LD D, d8 */
	reg_loadH(&z80->DE, memory_r8(z80->MEM, z80->PC));
	z80->PC += 1;

	z80->gClk += 8;
}

static void exec_0x17(processor * z80)
{
	/* RLA */
	byte A = z80->AF >> 8;
	reg_loadH(&z80->AF, (A << 1) | (z80->C));

	proc_clrflg(z80);
	z80->C = (A & 0x80) ? 1 : 0;

	z80->gClk += 4;
}

static void exec_0x18(processor * z80)
{
	/* JR r8 */
	s8 dsp = memory_r8(z80->MEM, z80->PC);
	z80->PC += 1;

	z80->PC += dsp;

	z80->gClk += 12;
}

static void exec_0x19(processor * z80)
{
	/* ADD HL, DE */
	exec_ADDHL(z80, &z80->HL, z80->DE);

	z80->gClk += 8;
}

static void exec_0x1A(processor * z80)
{
	/* LD A, (DE) */
	reg_loadH(&z80->AF, memory_r8(z80->MEM, z80->DE));

	z80->gClk += 8;
}

static void exec_0x1B(processor * z80)
{
	/* DEC DE */
	z80->DE -= 1;

	z80->gClk += 8;
}

static void exec_0x1C(processor * z80)
{
	/* INC E */
	exec_INCL(z80, &z80->DE);
	z80->gClk += 4;
}

static void exec_0x1D(processor * z80)
{
	/* DEC E */
	exec_DECL(z80, &z80->DE);

	z80->gClk += 4;
}

static void exec_0x1E(processor * z80)
{
	/* LD E, d8 */
	reg_loadL(&z80->DE, memory_r8(z80->MEM, z80->PC));
	z80->PC += 1;

	z80->gClk += 8;
}

static void exec_0x1F(processor * z80)
{
	/* RRA */
	byte A = z80->AF >> 8;
	reg_loadH(&z80->AF, (z80->C << 7) | (A >> 1));

	proc_clrflg(z80);
	z80->C = (A & 0x01) ? 1 : 0;

	z80->gClk += 4;
}

static void exec_0x20(processor * z80)
{
	/* JR NZ, r8 */
	if (!z80->Z)
		exec_0x18(z80); // JR r8
	else
	{
		z80->PC += 1;
		z80->gClk += 8;
	}

}

static void exec_0x21(processor * z80)
{
	/* LD HL, d16 */
	reg_loadHL(&z80->HL, memory_r16(z80->MEM, z80->PC));
	z80->PC += 2;

	z80->gClk += 12;
}

static void exec_0x22(processor * z80)
{
	/* LD (HL+), A */
	memory_w8(z80->MEM, z80->HL, z80->AF >> 8);
	z80->HL += 1;

	z80->gClk += 8;
}

static void exec_0x23(processor * z80)
{
	/* INC HL */
	z80->HL += 1;

	z80->gClk += 8;
}

static void exec_0x24(processor * z80)
{
	/* INC H */
	exec_INCH(z80, &z80->HL);

	z80->gClk += 4;
}

static void exec_0x25(processor * z80)
{
	/* DEC H */
	exec_DECH(z80, &z80->HL);

	z80->gClk += 4;
}

static void exec_0x26(processor * z80)
{
	/* LD H, d8 */
	reg_loadH(&z80->HL, memory_r8(z80->MEM, z80->PC));
	z80->PC += 1;

	z80->gClk += 8;
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

	z80->gClk += 4;
}

static void exec_0x28(processor * z80)
{
	/* JR Z, r8 */
	if (z80->Z)
		exec_0x18(z80);
	else
	{
		z80->PC += 1;
		z80->gClk += 8;
	}
}

static void exec_0x29(processor * z80)
{
	/* ADD HL, HL */
	exec_ADDHL(z80, &z80->HL, z80->HL);

	z80->gClk += 8;
}

static void exec_0x2A(processor * z80)
{
	/* LD A, (HL+) */
	reg_loadH(&z80->AF, memory_r8(z80->MEM, z80->HL));
	z80->HL += 1;

	z80->gClk += 8;
}

static void exec_0x2B(processor * z80)
{
	/* DEC HL */
	z80->HL -= 1;

	z80->gClk += 8;
}

static void exec_0x2C(processor * z80)
{
	/* INC L */
	exec_INCL(z80, &z80->HL);

	z80->gClk += 4;
}

static void exec_0x2D(processor * z80)
{
	/* DEC L */
	exec_DECL(z80, &z80->HL);

	z80->gClk += 4;
}

static void exec_0x2E(processor * z80)
{
	/* LD L, d8 */
	reg_loadL(&z80->HL, memory_r8(z80->MEM, z80->PC));
	z80->PC += 1;

	z80->gClk += 8;
}

static void exec_0x2F(processor * z80)
{
	/* CPL */
	byte B = z80->AF >> 8;
	reg_loadH(&z80->AF, ~B);

	z80->N = 1;
	z80->H = 1;

	z80->gClk += 4;
}

static void exec_0x30(processor * z80)
{
	/* JR NC, r8 */
	if (!z80->C)
		exec_0x18(z80); // JR r8
	else
	{
		z80->PC += 1;
		z80->gClk += 8;
	}
}

static void exec_0x31(processor * z80)
{
	/* LD SP, d16 */
	reg_loadHL(&z80->SP, memory_r16(z80->MEM, z80->PC));
	z80->PC += 2;

	z80->gClk += 12;
}

static void exec_0x32(processor * z80)
{
	/* LD (HL-), A */
	memory_w8(z80->MEM, z80->HL, z80->AF >> 8);
	z80->HL -= 1;

	z80->gClk += 8;
}

static void exec_0x33(processor * z80)
{
	/* INC SP */
	z80->SP += 1;

	z80->gClk += 8;
}

static void exec_0x34(processor * z80)
{
	/* INC (HL) */
	byte B = memory_r8(z80->MEM, z80->HL);
	B += 1;
	memory_w8(z80->MEM, z80->HL, B);

	z80->Z = (B == 0);
	z80->N = 0;
	z80->H = (B & 0x0F) == 0x00;

	z80->gClk += 12;
}

static void exec_0x35(processor * z80)
{
	/* DEC (HL) */
	byte B = memory_r8(z80->MEM, z80->HL);
	B -= 1;
	memory_w8(z80->MEM, z80->HL, B);

	z80->Z = (B == 0);
	z80->N = 1;
	z80->H = (B & 0x0F) == 0xF0;

	z80->gClk += 12;
}

static void exec_0x36(processor * z80)
{
	/* LD (HL), d8 */
	memory_w8(z80->MEM, z80->HL, memory_r8(z80->MEM, z80->PC));
	z80->PC += 1;

	z80->gClk += 12;
}

static void exec_0x37(processor * z80)
{
	/* SCF */
	z80->N = 0;
	z80->H = 0;
	z80->C = 1;

	z80->gClk += 4;
}

static void exec_0x38(processor * z80)
{
	/* JR C, r8 */
	if (z80->C)
		exec_0x18(z80);
	else
	{
		z80->PC += 1;
		z80->gClk += 8;
	}
}

static void exec_0x39(processor * z80)
{
	/* ADD HL, SP */
	exec_ADDHL(z80, &z80->HL, z80->SP);

	z80->gClk += 8;
}

static void exec_0x3A(processor * z80)
{
	/* LD A, (HL-) */
	reg_loadH(&z80->AF, memory_r8(z80->MEM, z80->HL));
	z80->HL -= 1;

	z80->gClk += 8;
}

static void exec_0x3B(processor * z80)
{
	/* DEC SP */
	z80->SP -= 1;

	z80->gClk += 8;
}

static void exec_0x3C(processor * z80)
{
	/* INC A */
	exec_INCH(z80, &z80->AF);

	z80->gClk += 4;
}

static void exec_0x3D(processor * z80)
{
	/* DEC A */
	exec_DECH(z80, &z80->AF);

	z80->gClk += 4;
}

static void exec_0x3E(processor * z80)
{
	/* LD A, d8 */
	reg_loadH(&z80->AF, memory_r8(z80->MEM, z80->PC));
	z80->PC += 1;

	z80->gClk += 8;
}

static void exec_0x3F(processor * z80)
{
	/* CCF */
	z80->N = 0;
	z80->H = 0;
	z80->C = (z80->C) ? 0 : 1;

	z80->gClk += 4;
}

static void exec_0x40(processor * z80)
{
	/* LD B, B */
	reg_loadH(&z80->BC, z80->BC >> 8);

	z80->gClk += 4;
}

static void exec_0x41(processor * z80)
{
	/* LD B, C */
	reg_loadH(&z80->BC, z80->BC & 0xFF);

	z80->gClk += 4;
}

static void exec_0x42(processor * z80)
{
	/* LD B, D */
	reg_loadH(&z80->BC, z80->DE >> 8);

	z80->gClk += 4;
}

static void exec_0x43(processor * z80)
{
	/* LD B, E */
	reg_loadH(&z80->BC, z80->DE & 0xFF);

	z80->gClk += 4;
}

static void exec_0x44(processor * z80)
{
	/* LD B, H */
	reg_loadH(&z80->BC, z80->HL >> 8);

	z80->gClk += 4;
}

static void exec_0x45(processor * z80)
{
	/* LD B, L */
	reg_loadH(&z80->BC, z80->HL & 0xFF);

	z80->gClk += 4;
}

static void exec_0x46(processor * z80)
{
	/* LD B, (HL) */
	reg_loadH(&z80->BC, memory_r8(z80->MEM, z80->HL));

	z80->gClk += 8;
}

static void exec_0x47(processor * z80)
{
	/* LD B, A */
	reg_loadH(&z80->BC, z80->AF >> 8);

	z80->gClk += 4;
}

static void exec_0x48(processor * z80)
{
	/* LD C, B */
	reg_loadL(&z80->BC, z80->BC >> 8);

	z80->gClk += 4;
}

static void exec_0x49(processor * z80)
{
	/* LD C, C */
	reg_loadL(&z80->BC, z80->BC & 0xFF);

	z80->gClk += 4;
}

static void exec_0x4A(processor * z80)
{
	/* LD C, D */
	reg_loadL(&z80->BC, z80->DE >> 8);

	z80->gClk += 4;
}

static void exec_0x4B(processor * z80)
{
	/* LD C, E */
	reg_loadL(&z80->BC, z80->DE & 0xFF);

	z80->gClk += 4;
}

static void exec_0x4C(processor * z80)
{
	/* LD C, H */
	reg_loadL(&z80->BC, z80->HL >> 8);

	z80->gClk += 4;
}

static void exec_0x4D(processor * z80)
{
	/* LD C, L */
	reg_loadL(&z80->BC, z80->HL & 0xFF);

	z80->gClk += 4;
}

static void exec_0x4E(processor * z80)
{
	/* LD C, (HL) */
	reg_loadL(&z80->BC, memory_r8(z80->MEM, z80->HL));

	z80->gClk += 8;
}

static void exec_0x4F(processor * z80)
{
	/* LD C, A */
	reg_loadL(&z80->BC, z80->AF >> 8);

	z80->gClk += 4;
}

static void exec_0x50(processor * z80)
{
	/* LD D, B */
	reg_loadH(&z80->DE, z80->BC >> 8);

	z80->gClk += 4;
}

static void exec_0x51(processor * z80)
{
	/* LD D, C */
	reg_loadH(&z80->DE, z80->BC & 0xFF);

	z80->gClk += 4;
}

static void exec_0x52(processor * z80)
{
	/* LD D, D */
	reg_loadH(&z80->DE, z80->DE >> 8);

	z80->gClk += 4;
}

static void exec_0x53(processor * z80)
{
	/* LD D, E */
	reg_loadH(&z80->DE, z80->DE & 0xFF);

	z80->gClk += 4;
}

static void exec_0x54(processor * z80)
{
	/* LD D, H */
	reg_loadH(&z80->DE, z80->HL >> 8);

	z80->gClk += 4;
}

static void exec_0x55(processor * z80)
{
	/* LD D, L */
	reg_loadH(&z80->DE, z80->HL & 0xFF);

	z80->gClk += 4;
}

static void exec_0x56(processor * z80)
{
	/* LD D, (HL) */
	reg_loadH(&z80->DE, memory_r8(z80->MEM, z80->HL));

	z80->gClk += 8;
}

static void exec_0x57(processor * z80)
{
	/* LD D, A */
	reg_loadH(&z80->DE, z80->AF >> 8);

	z80->gClk += 4;
}

static void exec_0x58(processor * z80)
{
	/* LD E, B */
	reg_loadL(&z80->DE, z80->BC >> 8);

	z80->gClk += 4;
}

static void exec_0x59(processor * z80)
{
	/* LD E, C */
	reg_loadL(&z80->DE, z80->BC & 0xFF);

	z80->gClk += 4;
}

static void exec_0x5A(processor * z80)
{
	/* LD E, D */
	reg_loadL(&z80->DE, z80->DE >> 8);

	z80->gClk += 4;
}

static void exec_0x5B(processor * z80)
{
	/* LD E, E */
	reg_loadL(&z80->DE, z80->DE & 0xFF);

	z80->gClk += 4;
}

static void exec_0x5C(processor * z80)
{
	/* LD E, H */
	reg_loadL(&z80->DE, z80->HL >> 8);

	z80->gClk += 4;
}

static void exec_0x5D(processor * z80)
{
	/* LD E, L */
	reg_loadL(&z80->DE, z80->HL & 0xFF);
	
	z80->gClk += 4;
}

static void exec_0x5E(processor * z80)
{
	/* LD E, (HL) */
	reg_loadL(&z80->DE, memory_r8(z80->MEM, z80->HL));

	z80->gClk += 8;
}

static void exec_0x5F(processor * z80)
{
	/* LD E, A */
	reg_loadL(&z80->DE, z80->AF >> 8);

	z80->gClk += 4;
}

static void exec_0x60(processor * z80)
{
	/* LD H, B */
	reg_loadH(&z80->HL, z80->BC >> 8);

	z80->gClk += 4;
}

static void exec_0x61(processor * z80)
{
	/* LD H, C */
	reg_loadH(&z80->HL, z80->BC & 0xFF);

	z80->gClk += 4;
}

static void exec_0x62(processor * z80)
{
	/* LD H, D */
	reg_loadH(&z80->HL, z80->DE >> 8);

	z80->gClk += 4;
}

static void exec_0x63(processor * z80)
{
	/* LD H, E */
	reg_loadH(&z80->HL, z80->DE & 0xFF);

	z80->gClk += 4;
}

static void exec_0x64(processor * z80)
{
	/* LD H, H */
	reg_loadH(&z80->HL, z80->HL >> 8);

	z80->gClk += 4;
}

static void exec_0x65(processor * z80)
{
	/* LD H, L */
	reg_loadH(&z80->HL, z80->HL & 0xFF);

	z80->gClk += 4;
}

static void exec_0x66(processor * z80)
{
	/* LD H, (HL) */
	reg_loadH(&z80->HL, memory_r8(z80->MEM, z80->HL));

	z80->gClk += 8;
}

static void exec_0x67(processor * z80)
{
	/* LD H, A */
	reg_loadH(&z80->HL, z80->AF >> 8);

	z80->gClk += 4;
}

static void exec_0x68(processor * z80)
{
	/* LD L, B */
	reg_loadL(&z80->HL, z80->BC >> 8);

	z80->gClk += 4;
}

static void exec_0x69(processor * z80)
{
	/* LD L, C */
	reg_loadL(&z80->HL, z80->BC & 0xFF);

	z80->gClk += 4;
}

static void exec_0x6A(processor * z80)
{
	/* LD L, D */
	reg_loadL(&z80->HL, z80->DE >> 8);

	z80->gClk += 4;
}

static void exec_0x6B(processor * z80)
{
	/* LD L, E */
	reg_loadL(&z80->HL, z80->DE & 0xFF);

	z80->gClk += 4;
}

static void exec_0x6C(processor * z80)
{
	/* LD L, H */
	reg_loadL(&z80->HL, z80->HL >> 8);

	z80->gClk += 4;
}

static void exec_0x6D(processor * z80)
{
	/* LD L, L */
	reg_loadL(&z80->HL, z80->HL & 0xFF);

	z80->gClk += 4;
}

static void exec_0x6E(processor * z80)
{
	/* LD L, (HL) */
	reg_loadL(&z80->HL, memory_r8(z80->MEM, z80->HL));

	z80->gClk += 8;
}

static void exec_0x6F(processor * z80)
{
	/* LD L, A */
	reg_loadL(&z80->HL, z80->AF >> 8);

	z80->gClk += 4;
}

static void exec_0x70(processor * z80)
{
	/* LD (HL), B */
	memory_w8(z80->MEM, z80->HL, z80->BC >> 8);

	z80->gClk += 8;
}

static void exec_0x71(processor * z80)
{
	/* LD (HL), C */
	memory_w8(z80->MEM, z80->HL, z80->BC & 0xFF);

	z80->gClk += 8;
}

static void exec_0x72(processor * z80)
{
	/* LD (HL), D */
	memory_w8(z80->MEM, z80->HL, z80->DE >> 8);

	z80->gClk += 8;
}

static void exec_0x73(processor * z80)
{
	/* LD (HL), E */
	memory_w8(z80->MEM, z80->HL, z80->DE & 0xFF);

	z80->gClk += 8;
}

static void exec_0x74(processor * z80)
{
	/* LD (HL), H */
	memory_w8(z80->MEM, z80->HL, z80->HL >> 8);

	z80->gClk += 8;
}

static void exec_0x75(processor * z80)
{
	/* LD (HL), L */
	memory_w8(z80->MEM, z80->HL, z80->HL & 0xFF);

	z80->gClk += 8;
}

static void exec_0x76(processor * z80)
{
	/* HALT */
	z80->hWFI = 1;

	z80->gClk += 4;
}

static void exec_0x77(processor * z80)
{
	/* LD (HL), A */
	memory_w8(z80->MEM, z80->HL, z80->AF >> 8);

	z80->gClk += 8;
}

static void exec_0x78(processor * z80)
{
	/* LD A, B */
	reg_loadH(&z80->AF, z80->BC >> 8);

	z80->gClk += 4;
}

static void exec_0x79(processor * z80)
{
	/* LD A, C */
	reg_loadH(&z80->AF, z80->BC & 0xFF);

	z80->gClk += 4;
}

static void exec_0x7A(processor * z80)
{
	/* LD A, D */
	reg_loadH(&z80->AF, z80->DE >> 8);

	z80->gClk += 4;
}

static void exec_0x7B(processor * z80)
{
	/* LD A, E */
	reg_loadH(&z80->AF, z80->DE & 0xFF);

	z80->gClk += 4;
}

static void exec_0x7C(processor * z80)
{
	/* LD A, H */
	reg_loadH(&z80->AF, z80->HL >> 8);

	z80->gClk += 4;
}

static void exec_0x7D(processor * z80)
{
	/* LD A, L */
	reg_loadH(&z80->AF, z80->HL & 0xFF);

	z80->gClk += 4;
}

static void exec_0x7E(processor * z80)
{
	/* LD A, (HL) */
	reg_loadH(&z80->AF, memory_r8(z80->MEM, z80->HL));

	z80->gClk += 8;
}

static void exec_0x7F(processor * z80)
{
	/* LD A, A */
	reg_loadH(&z80->AF, z80->AF >> 8);

	z80->gClk += 4;
}

static void exec_0x80(processor * z80)
{
	/* ADD A, B */
	exec_ADDH(z80, &z80->AF, z80->BC >> 8);

	z80->gClk += 4;
}

static void exec_0x81(processor * z80)
{
	/* ADD A, C */
	exec_ADDH(z80, &z80->AF, z80->BC & 0xFF);

	z80->gClk += 4;
}

static void exec_0x82(processor * z80)
{
	/* ADD A, D */
	exec_ADDH(z80, &z80->AF, z80->DE >> 8);

	z80->gClk += 4;
}

static void exec_0x83(processor * z80)
{
	/* ADD A, E */
	exec_ADDH(z80, &z80->AF, z80->DE & 0xFF);

	z80->gClk += 4;
}

static void exec_0x84(processor * z80)
{
	/* ADD A, H */
	exec_ADDH(z80, &z80->AF, z80->HL >> 8);

	z80->gClk += 4;
}

static void exec_0x85(processor * z80)
{
	/* ADD A, L */
	exec_ADDH(z80, &z80->AF, z80->HL & 0xFF);

	z80->gClk += 4;
}

static void exec_0x86(processor * z80)
{
	/* ADD A, (HL) */
	exec_ADDH(z80, &z80->AF, memory_r8(z80->MEM, z80->HL));

	z80->gClk += 8;
}

static void exec_0x87(processor * z80)
{
	/* ADD A, A */
	exec_ADDH(z80, &z80->AF, z80->AF >> 8);

	z80->gClk += 4;
}

static void exec_0x88(processor * z80)
{
	/* ADC A, B */
	exec_ADCH(z80, &z80->AF, z80->BC >> 8);

	z80->gClk += 4;
}

static void exec_0x89(processor * z80)
{
	/* ADC A, C */
	exec_ADCH(z80, &z80->AF, z80->BC & 0xFF);

	z80->gClk += 4;
}

static void exec_0x8A(processor * z80)
{
	/* ADC A, D */
	exec_ADCH(z80, &z80->AF, z80->DE >> 8);

	z80->gClk += 4;
}

static void exec_0x8B(processor * z80)
{
	/* ADC A, E */
	exec_ADCH(z80, &z80->AF, z80->DE & 0xFF);

	z80->gClk += 4;
}

static void exec_0x8C(processor * z80)
{
	/* ADC A, H */
	exec_ADCH(z80, &z80->AF, z80->HL >> 8);

	z80->gClk += 4;
}

static void exec_0x8D(processor * z80)
{
	/* ADC A, L */
	exec_ADCH(z80, &z80->AF, z80->HL & 0xFF);

	z80->gClk += 4;
}

static void exec_0x8E(processor * z80)
{
	/* ADC A, (HL) */
	exec_ADCH(z80, &z80->AF, memory_r8(z80->MEM, z80->HL));

	z80->gClk += 8;
}

static void exec_0x8F(processor * z80)
{
	/* ADC A, A */
	exec_ADCH(z80, &z80->AF, z80->AF >> 8);

	z80->gClk += 4;
}

static void exec_0x90(processor * z80)
{
	/* SUB A, B */
	exec_SUBH(z80, &z80->AF, z80->BC >> 8);

	z80->gClk += 4;
}

static void exec_0x91(processor * z80)
{
	/* SUB A, C */
	exec_SUBH(z80, &z80->AF, z80->BC & 0xFF);

	z80->gClk += 4;
}

static void exec_0x92(processor * z80)
{
	/* SUB A, D */
	exec_SUBH(z80, &z80->AF, z80->DE >> 8);

	z80->gClk += 4;
}

static void exec_0x93(processor * z80)
{
	/* SUB A, E */
	exec_SUBH(z80, &z80->AF, z80->DE & 0xFF);

	z80->gClk += 4;
}

static void exec_0x94(processor * z80)
{
	/* SUB A, H */
	exec_SUBH(z80, &z80->AF, z80->HL >> 8);

	z80->gClk += 4;
}

static void exec_0x95(processor * z80)
{
	/* SUB A, L */
	exec_SUBH(z80, &z80->AF, z80->HL & 0xFF);

	z80->gClk += 4;
}

static void exec_0x96(processor * z80)
{
	/* SUB A, (HL) */
	exec_SUBH(z80, &z80->AF, memory_r8(z80->MEM, z80->HL));

	z80->gClk += 8;
}

static void exec_0x97(processor * z80)
{
	/* SUB A, A */
	exec_SUBH(z80, &z80->AF, z80->AF >> 8);

	z80->gClk += 4;
}

static void exec_0x98(processor * z80)
{
	/* SBC A, B */
	exec_SBCH(z80, &z80->AF, z80->BC >> 8);

	z80->gClk += 4;
}

static void exec_0x99(processor * z80)
{
	/* SBC A, C */
	exec_SBCH(z80, &z80->AF, z80->BC & 0xFF);

	z80->gClk += 4;
}

static void exec_0x9A(processor * z80)
{
	/* SBC A, D */
	exec_SBCH(z80, &z80->AF, z80->DE >> 8);

	z80->gClk += 4;
}

static void exec_0x9B(processor * z80)
{
	/* SBC A, E */
	exec_SBCH(z80, &z80->AF, z80->DE & 0xFF);

	z80->gClk += 4;
}

static void exec_0x9C(processor * z80)
{
	/* SBC A, H */
	exec_SBCH(z80, &z80->AF, z80->HL >> 8);

	z80->gClk += 4;
}

static void exec_0x9D(processor * z80)
{
	/* SBC A, L */
	exec_SBCH(z80, &z80->AF, z80->HL & 0xFF);

	z80->gClk += 4;
}

static void exec_0x9E(processor * z80)
{
	/* SBC A, (HL) */
	exec_SBCH(z80, &z80->AF, memory_r8(z80->MEM, z80->HL));

	z80->gClk += 8;
}

static void exec_0x9F(processor * z80)
{
	/* SBC A, A */
	exec_SBCH(z80, &z80->AF, z80->AF >> 8);

	z80->gClk += 4;
}

static void exec_0xA0(processor * z80)
{
	/* AND B */
	exec_ANDH(z80, &z80->AF, z80->BC >> 8);

	z80->gClk += 4;
}

static void exec_0xA1(processor * z80)
{
	/* AND C */
	exec_ANDH(z80, &z80->AF, z80->BC & 0xFF);

	z80->gClk += 4;
}

static void exec_0xA2(processor * z80)
{
	/* AND D */
	exec_ANDH(z80, &z80->AF, z80->DE >> 8);

	z80->gClk += 4;
}

static void exec_0xA3(processor * z80)
{
	/* AND E */
	exec_ANDH(z80, &z80->AF, z80->DE & 0xFF);

	z80->gClk += 4;
}

static void exec_0xA4(processor * z80)
{
	/* AND H */
	exec_ANDH(z80, &z80->AF, z80->HL >> 8);

	z80->gClk += 4;
}

static void exec_0xA5(processor * z80)
{
	/* AND L */
	exec_ANDH(z80, &z80->AF, z80->HL & 0xFF);

	z80->gClk += 4;
}

static void exec_0xA6(processor * z80)
{
	/* AND (HL) */
	exec_ANDH(z80, &z80->AF, memory_r8(z80->MEM, z80->HL));

	z80->gClk += 8;
}

static void exec_0xA7(processor * z80)
{
	/* AND A */
	exec_ANDH(z80, &z80->AF, z80->AF >> 8);

	z80->gClk += 4;
}

static void exec_0xA8(processor * z80)
{
	/* XOR B */
	exec_XORH(z80, &z80->AF, z80->BC >> 8);

	z80->gClk += 4;
}

static void exec_0xA9(processor * z80)
{
	/* XOR C */
	exec_XORH(z80, &z80->AF, z80->BC & 0xFF);

	z80->gClk += 4;
}

static void exec_0xAA(processor * z80)
{
	/* XOR D */
	exec_XORH(z80, &z80->AF, z80->DE >> 8);

	z80->gClk += 4;
}

static void exec_0xAB(processor * z80)
{
	/* XOR E */
	exec_XORH(z80, &z80->AF, z80->DE & 0xFF);

	z80->gClk += 4;
}

static void exec_0xAC(processor * z80)
{
	/* XOR H */
	exec_XORH(z80, &z80->AF, z80->HL >> 8);

	z80->gClk += 4;
}

static void exec_0xAD(processor * z80)
{
	/* XOR L */
	exec_XORH(z80, &z80->AF, z80->HL & 0xFF);

	z80->gClk += 4;
}

static void exec_0xAE(processor * z80)
{
	/* XOR (HL) */
	exec_XORH(z80, &z80->AF, memory_r8(z80->MEM, z80->HL));

	z80->gClk += 8;
}

static void exec_0xAF(processor * z80)
{
	/* XOR A */
	exec_XORH(z80, &z80->AF, z80->AF >> 8);

	z80->gClk += 4;
}

static void exec_0xB0(processor * z80)
{
	/* OR B */
	exec_ORH(z80, &z80->AF, z80->BC >> 8);

	z80->gClk += 4;
}

static void exec_0xB1(processor * z80)
{
	/* OR C */
	exec_ORH(z80, &z80->AF, z80->BC & 0xFF);

	z80->gClk += 4;
}

static void exec_0xB2(processor * z80)
{
	/* OR D */
	exec_ORH(z80, &z80->AF, z80->DE >> 8);

	z80->gClk += 4;
}

static void exec_0xB3(processor * z80)
{
	/* OR E */
	exec_ORH(z80, &z80->AF, z80->DE & 0xFF);

	z80->gClk += 4;
}

static void exec_0xB4(processor * z80)
{
	/* OR H */
	exec_ORH(z80, &z80->AF, z80->HL >> 8);

	z80->gClk += 4;
}

static void exec_0xB5(processor * z80)
{
	/* OR L */
	exec_ORH(z80, &z80->AF, z80->HL & 0xFF);

	z80->gClk += 4;
}

static void exec_0xB6(processor * z80)
{
	/* OR (HL) */
	exec_ORH(z80, &z80->AF, memory_r8(z80->MEM, z80->HL));

	z80->gClk += 8;
}

static void exec_0xB7(processor * z80)
{
	/* OR A */
	exec_ORH(z80, &z80->AF, z80->AF >> 8);

	z80->gClk += 4;
}

static void exec_0xB8(processor * z80)
{
	/* CP B */
	exec_CPH(z80, &z80->AF, z80->BC >> 8);

	z80->gClk += 4;
}

static void exec_0xB9(processor * z80)
{
	/* CP C */
	exec_CPH(z80, &z80->AF, z80->BC & 0xFF);

	z80->gClk += 4;
}

static void exec_0xBA(processor * z80)
{
	/* CP D */
	exec_CPH(z80, &z80->AF, z80->DE >> 8);

	z80->gClk += 4;
}

static void exec_0xBB(processor * z80)
{
	/* CP E */
	exec_CPH(z80, &z80->AF, z80->DE & 0xFF);

	z80->gClk += 4;
}

static void exec_0xBC(processor * z80)
{
	/* CP H */
	exec_CPH(z80, &z80->AF, z80->HL >> 8);

	z80->gClk += 4;
}

static void exec_0xBD(processor * z80)
{
	/* CP L */
	exec_CPH(z80, &z80->AF, z80->HL & 0xFF);

	z80->gClk += 4;
}

static void exec_0xBE(processor * z80)
{
	/* CP (HL) */
	exec_CPH(z80, &z80->AF, memory_r8(z80->MEM, z80->HL));

	z80->gClk += 8;
}

static void exec_0xBF(processor * z80)
{
	/* CP A */
	exec_CPH(z80, &z80->AF, z80->AF >> 8);

	z80->gClk += 4;
}

static void exec_0xC0(processor * z80)
{
	/* RET NZ */
	if (!z80->Z)
	{
		exec_RET(z80);
		z80->gClk += 12;
	}

	z80->gClk += 8;
}

static void exec_0xC1(processor * z80)
{
	/* POP BC */
	reg_loadHL(&z80->BC, memory_r16(z80->MEM, z80->SP));
	z80->SP += 2;

	z80->gClk += 12;
}

static void exec_0xC2(processor * z80)
{
	/* JP NZ, a16 */
	if (!z80->Z)
	{
		z80->PC = memory_r16(z80->MEM, z80->PC);
		z80->gClk += 4;
	}
	else
		z80->PC += 2;

	z80->gClk += 12;
}

static void exec_0xC3(processor * z80)
{
	/* JP a16 */
	z80->PC = memory_r16(z80->MEM, z80->PC);

	z80->gClk += 16;
}

static void exec_0xC4(processor * z80)
{
	/* CALL NZ, a16 */
	if (!z80->Z)
	{
		exec_CALL(z80);
		z80->gClk += 12;
	}
	else
		z80->PC += 2;

	z80->gClk += 12;
}

static void exec_0xC5(processor * z80)
{
	/* PUSH BC */
	memory_w16(z80->MEM, z80->SP - 2, z80->BC);
	z80->SP -= 2;

	z80->gClk += 16;
}

static void exec_0xC6(processor * z80)
{
	/* ADD A, d8 */
	exec_ADDH(z80, &z80->AF, memory_r8(z80->MEM, z80->PC));
	z80->PC += 1;

	z80->gClk += 8;
}

static void exec_0xC7(processor * z80)
{
	/* RST 00h */
	memory_w16(z80->MEM, z80->SP - 2, z80->PC);
	reg_loadHL(&z80->PC, 0x0000);
	z80->SP -= 2;

	z80->gClk += 16;
}

static void exec_0xC8(processor * z80)
{
	/* RET Z */
	if (z80->Z)
	{
		exec_RET(z80);
		z80->gClk += 12;
	}

	z80->gClk += 8;
}

static void exec_0xC9(processor * z80)
{
	/* RET */
	exec_RET(z80);

	z80->gClk += 16;
}

static void exec_0xCA(processor * z80)
{
	/* JP Z, a16 */
	if (z80->Z)
	{
		z80->PC = memory_r16(z80->MEM, z80->PC);
		z80->gClk += 4;
	}
	else
		z80->PC += 2;

	z80->gClk += 12;
}

static void exec_0xCB(processor * z80)
{
	/* Prefix CB */
	// TODO: CB
	z80->PC += 1;

	z80->gClk += 4;
}

static void exec_0xCC(processor * z80)
{
	/* CALL Z, a16 */
	if (z80->Z)
	{
		exec_CALL(z80);
		z80->gClk += 12;
	}
	else
		z80->PC += 2;

	z80->gClk += 12;
}

static void exec_0xCD(processor * z80)
{
	/* CALL a16 */
	exec_CALL(z80);

	z80->gClk += 24;
}

static void exec_0xCE(processor * z80)
{
	/* ADC A, d8 */
	exec_ADCH(z80, &z80->AF, memory_r8(z80->MEM, z80->PC));
	z80->PC += 1;

	z80->gClk += 8;
}

static void exec_0xCF(processor * z80)
{
	/* RST 08h */
	memory_w16(z80->MEM, z80->SP - 2, z80->PC);
	reg_loadHL(&z80->PC, 0x0008);
	z80->SP -= 2;

	z80->gClk += 16;
}

static void exec_0xD0(processor * z80)
{
	/* RET NZ */
	if (!z80->C)
	{
		exec_RET(z80);
		z80->gClk += 12;
	}

	z80->gClk += 8;
}

static void exec_0xD1(processor * z80)
{
	/* POP DE */
	reg_loadHL(&z80->DE, memory_r16(z80->MEM, z80->SP));
	z80->SP += 2;

	z80->gClk += 12;
}

static void exec_0xD2(processor * z80)
{
	/* JP NC, a16 */
	if (!z80->C)
	{
		z80->PC = memory_r16(z80->MEM, z80->PC);
		z80->gClk += 4;
	}
	else
		z80->PC += 2;

	z80->gClk += 12;
}

static void exec_0xD4(processor * z80)
{
	/* CALL NC, a16 */
	if (!z80->C)
	{
		exec_CALL(z80);
		z80->gClk += 12;
	}
	else
		z80->PC += 2;

	z80->gClk += 12;
}

static void exec_0xD5(processor * z80)
{
	/* PUSH DE */
	memory_w16(z80->MEM, z80->SP - 2, z80->DE);
	z80->SP -= 2;

	z80->gClk += 16;
}

static void exec_0xD6(processor * z80)
{
	/* SUB d8 */
	exec_SUBH(z80, &z80->AF, memory_r8(z80->MEM, z80->PC));
	z80->PC += 1;

	z80->gClk += 8;
}

static void exec_0xD7(processor * z80)
{
	/* RST 10h */
	memory_w16(z80->MEM, z80->SP - 2, z80->PC);
	reg_loadHL(&z80->PC, 0x0010);
	z80->SP -= 2;

	z80->gClk += 16;
}

static void exec_0xD8(processor * z80)
{
	/* RET C */
	if (z80->C)
	{
		exec_RET(z80);
		z80->gClk += 12;
	}

	z80->gClk += 8;
}

static void exec_0xD9(processor * z80)
{
	/* RETI */
	exec_RET(z80);
	z80->IRQE = 1;

	z80->gClk += 16;
}

static void exec_0xDA(processor * z80)
{
	/* JP C, a16 */
	if (z80->C)
	{
		z80->PC = memory_r16(z80->MEM, z80->PC);
		z80->gClk += 4;
	}
	else
		z80->PC += 2;

	z80->gClk += 12;
}

static void exec_0xDC(processor * z80)
{
	/* CALL C, a16 */
	if (z80->C)
	{
		exec_CALL(z80);
		z80->gClk += 12;
	}
	else
		z80->PC += 2;

	z80->gClk += 12;
}

static void exec_0xDE(processor * z80)
{
	/* SBC A, d8 */
	exec_SBCH(z80, &z80->AF, memory_r8(z80->MEM, z80->PC));
	z80->PC += 1;

	z80->gClk += 8;
}

static void exec_0xDF(processor * z80)
{
	/* RST 18h */
	memory_w16(z80->MEM, z80->SP - 2, z80->PC);
	reg_loadHL(&z80->PC, 0x0018);
	z80->SP -= 2;

	z80->gClk += 16;
}

static void exec_0xE0(processor * z80)
{
	/* LDH (a8), A */
	memory_w8(z80->MEM, 0xFF00 + memory_r8(z80->MEM, z80->PC), z80->AF >> 8);
	z80->PC += 1;

	z80->gClk += 12;
}

static void exec_0xE1(processor * z80)
{
	/* POP HL */
	reg_loadHL(&z80->HL, memory_r16(z80->MEM, z80->SP));
	z80->SP += 2;

	z80->gClk += 12;
}

static void exec_0xE2(processor * z80)
{
	/* LD (C), A */
	memory_w8(z80->MEM, 0xFF00 + (z80->BC & 0xFF), z80->AF >> 8);

	z80->gClk += 8;
}

static void exec_0xE5(processor * z80)
{
	/* PUSH HL */
	memory_w16(z80->MEM, z80->SP - 2, z80->HL);
	z80->SP -= 2;

	z80->gClk += 16;
}

static void exec_0xE6(processor * z80)
{
	/* AND d8 */
	exec_ANDH(z80, &z80->AF, memory_r8(z80->MEM, z80->PC));
	z80->PC += 1;

	z80->gClk += 8;
}

static void exec_0xE7(processor * z80)
{
	/* RST 20h */
	memory_w16(z80->MEM, z80->SP - 2, z80->PC);
	reg_loadHL(&z80->PC, 0x0020);
	z80->SP -= 2;

	z80->gClk += 16;
}

static void exec_0xE8(processor * z80)
{
	/* ADD SP, r8 */
	exec_ADDHL(z80, &z80->SP, memory_r8(z80->MEM, z80->PC));
	z80->PC += 1;

	z80->gClk += 16;
}

static void exec_0xE9(processor * z80)
{
	/* JP (HL) */
	z80->PC = z80->HL;

	z80->gClk += 4;
}

static void exec_0xEA(processor * z80)
{
	/* LD (a16), A */
	memory_w8(z80->MEM, memory_r16(z80->MEM, z80->PC), z80->AF >> 8);
	z80->PC += 2;

	z80->gClk += 16;
}

static void exec_0xEE(processor * z80)
{
	/* XOR d8 */
	exec_XORH(z80, &z80->AF, memory_r8(z80->MEM, z80->PC));
	z80->PC += 1;

	z80->gClk += 8;
}

static void exec_0xEF(processor * z80)
{
	/* RST 28h */
	memory_w16(z80->MEM, z80->SP - 2, z80->PC);
	reg_loadHL(&z80->PC, 0x0028);
	z80->SP -= 2;

	z80->gClk += 16;
}

static void exec_0xF0(processor * z80)
{
	/* LDH A, (a8) */
	byte a8 = memory_r8(z80->MEM, z80->PC);
	z80->PC += 1;

	reg_loadH(&z80->AF, memory_r8(z80->MEM, 0xFF00 + a8));

	z80->gClk += 12;
}

static void exec_0xF1(processor * z80)
{
	/* POP AF */
	reg_loadHL(&z80->AF, memory_r16(z80->MEM, z80->SP));
	z80->SP += 2;

	z80->Z = (z80->AF >> 7) & 0x01;
	z80->N = (z80->AF >> 6) & 0x01;
	z80->H = (z80->AF >> 5) & 0x01;
	z80->C = (z80->AF >> 4) & 0x01;

	z80->gClk += 12;
}

static void exec_0xF2(processor * z80)
{
	/* LD A, (C) */
	reg_loadH(&z80->AF, memory_r8(z80->MEM, 0xFF00 + (z80->BC & 0xFF)));

	z80->gClk += 8;
}

static void exec_0xF3(processor * z80)
{
	/* DI */
	z80->IRQE = 0;

	z80->gClk += 4;
}

static void exec_0xF5(processor * z80)
{
	/* PUSH AF */
	z80->AF = (z80->AF & 0xFF00) | (z80->Z << 7) | (z80->N << 6) | (z80->H << 5) | (z80->C << 4);

	memory_w16(z80->MEM, z80->SP - 2, z80->AF);
	z80->SP -= 2;

	z80->gClk += 16;
}

static void exec_0xF6(processor * z80)
{
	/* OR d8 */
	exec_ORH(z80, &z80->AF, memory_r8(z80->MEM, z80->PC));
	z80->PC += 1;

	z80->gClk += 8;
}

static void exec_0xF7(processor * z80)
{
	/* RST 30h */
	memory_w16(z80->MEM, z80->SP - 2, z80->PC);
	reg_loadHL(&z80->PC, 0x0030);
	z80->SP -= 2;

	z80->gClk += 16;
}

static void exec_0xF8(processor * z80)
{
	/* LD HL, SP+r8 */
	byte d8 = memory_r8(z80->MEM, z80->PC);
	word addr = z80->SP + d8;
	z80->PC += 1;

	z80->Z = 0;
	z80->N = 0;
	z80->H = ((z80->SP ^ d8 ^ addr) & 0x10) == 0x10;
	z80->C = ((z80->SP ^ d8 ^ addr) & 0x100) == 0x100;

	reg_loadHL(&z80->HL, memory_r16(z80->MEM, addr));

	z80->gClk += 12;
}

static void exec_0xF9(processor * z80)
{
	/* LD SP, HL */
	z80->SP = z80->HL;

	z80->gClk += 8;
}

static void exec_0xFA(processor * z80)
{
	/* LD A, (a16) */
	word a16 = memory_r16(z80->MEM, z80->PC);
	z80->PC += 2;

	reg_loadH(&z80->AF, memory_r8(z80->MEM, a16));

	z80->gClk += 16;
}

static void exec_0xFB(processor * z80)
{
	/* EI */
	z80->IRQE = 1;

	z80->gClk += 4;
}

static void exec_0xFE(processor * z80)
{
	/* CP d8 */
	exec_CPH(z80, &z80->AF, memory_r8(z80->MEM, z80->PC));
	z80->PC += 1;

	z80->gClk += 8;
}

static void exec_0xFF(processor * z80)
{
	/* RST 38h */
	memory_w16(z80->MEM, z80->SP - 2, z80->PC);
	reg_loadHL(&z80->PC, 0x0038);
	z80->SP -= 2;

	z80->gClk += 16;
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
	exec_0x80,
	exec_0x81,
	exec_0x82,
	exec_0x83,
	exec_0x84,
	exec_0x85,
	exec_0x86,
	exec_0x87,
	exec_0x88,
	exec_0x89,
	exec_0x8A,
	exec_0x8B,
	exec_0x8C,
	exec_0x8D,
	exec_0x8E,
	exec_0x8F,
	exec_0x90,
	exec_0x91,
	exec_0x92,
	exec_0x93,
	exec_0x94,
	exec_0x95,
	exec_0x96,
	exec_0x97,
	exec_0x98,
	exec_0x99,
	exec_0x9A,
	exec_0x9B,
	exec_0x9C,
	exec_0x9D,
	exec_0x9E,
	exec_0x9F,
	exec_0xA0,
	exec_0xA1,
	exec_0xA2,
	exec_0xA3,
	exec_0xA4,
	exec_0xA5,
	exec_0xA6,
	exec_0xA7,
	exec_0xA8,
	exec_0xA9,
	exec_0xAA,
	exec_0xAB,
	exec_0xAC,
	exec_0xAD,
	exec_0xAE,
	exec_0xAF,
	exec_0xB0,
	exec_0xB1,
	exec_0xB2,
	exec_0xB3,
	exec_0xB4,
	exec_0xB5,
	exec_0xB6,
	exec_0xB7,
	exec_0xB8,
	exec_0xB9,
	exec_0xBA,
	exec_0xBB,
	exec_0xBC,
	exec_0xBD,
	exec_0xBE,
	exec_0xBF,
	exec_0xC0,
	exec_0xC1,
	exec_0xC2,
	exec_0xC3,
	exec_0xC4,
	exec_0xC5,
	exec_0xC6,
	exec_0xC7,
	exec_0xC8,
	exec_0xC9,
	exec_0xCA,
	exec_0xCB,
	exec_0xCC,
	exec_0xCD,
	exec_0xCE,
	exec_0xCF,
	exec_0xD0,
	exec_0xD1,
	exec_0xD2,
	0,
	exec_0xD4,
	exec_0xD5,
	exec_0xD6,
	exec_0xD7,
	exec_0xD8,
	exec_0xD9,
	exec_0xDA,
	0,
	exec_0xDC,
	0,
	exec_0xDE,
	exec_0xDF,
	exec_0xE0,
	exec_0xE1,
	exec_0xE2,
	0,
	0,
	exec_0xE5,
	exec_0xE6,
	exec_0xE7,
	exec_0xE8,
	exec_0xE9,
	exec_0xEA,
	0,
	0,
	0,
	exec_0xEE,
	exec_0xEF,
	exec_0xF0,
	exec_0xF1,
	exec_0xF2,
	exec_0xF3,
	0,
	exec_0xF5,
	exec_0xF6,
	exec_0xF7,
	exec_0xF8,
	exec_0xF9,
	exec_0xFA,
	exec_0xFB,
	0,
	0,
	exec_0xFE,
	exec_0xFF
};
