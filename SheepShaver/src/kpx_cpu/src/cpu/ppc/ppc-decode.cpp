/*
 *  ppc-decode.cpp - PowerPC instructions decoder
 *
 *  Kheperix (C) 2003 Gwenole Beauchesne
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "cpu/ppc/ppc-cpu.hpp"
#include "cpu/ppc/ppc-bitfields.hpp"
#include "cpu/ppc/ppc-operands.hpp"
#include "cpu/ppc/ppc-operations.hpp"
#include "cpu/ppc/ppc-instructions.hpp"

#define DEBUG 0
#include "debug.h"

// Enable specialized instruction decoders?
#define SPECIALIZED_DECODERS 0

#if SPECIALIZED_DECODERS
#define SPD(X) X
#else
#define SPD(X) NULL
#endif

#define EXECUTE_0(HANDLER) \
&powerpc_cpu::execute_##HANDLER

#define EXECUTE_1(HANDLER, ARG1) \
&powerpc_cpu::execute_##HANDLER<ARG1>

#define EXECUTE_2(HANDLER, ARG1, ARG2) \
&powerpc_cpu::execute_##HANDLER<ARG1, ARG2>

#define EXECUTE_3(HANDLER, ARG1, ARG2, ARG3) \
&powerpc_cpu::execute_##HANDLER<ARG1, ARG2, ARG3>

#define EXECUTE_4(HANDLER, ARG1, ARG2, ARG3, ARG4) \
&powerpc_cpu::execute_##HANDLER<ARG1, ARG2, ARG3, ARG4>

#define EXECUTE_7(HANDLER, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7) \
&powerpc_cpu::execute_##HANDLER<ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7>

#define EXECUTE_ADDITION(RA, RB, RC, CA, OE, Rc) \
&powerpc_cpu::execute_addition<operand_##RA, operand_##RB, operand_##RC, CA, OE, Rc>

#define EXECUTE_GENERIC_ARITH(OP, RD, RA, RB, RC, OE, Rc) \
&powerpc_cpu::execute_generic_arith<op_##OP, operand_##RD, operand_##RA, operand_##RB, operand_##RC, OE, Rc>

#define EXECUTE_BRANCH(PC, BO, DP, AA, LK) \
&powerpc_cpu::execute_branch<operand_##PC, BO, operand_##DP, AA, LK>

#define EXECUTE_COMPARE(RB, CT) \
&powerpc_cpu::execute_compare<operand_##RB, CT>

#define EXECUTE_CR_OP(OP) \
&powerpc_cpu::execute_cr_op<op_##OP>

#define EXECUTE_FP_ARITH(FP, OP, RD, RA, RB, RC, Rc, FPSCR) \
&powerpc_cpu::execute_fp_arith<FP, op_##OP, operand_fp_##RD, operand_fp_##RA, operand_fp_##RB, operand_fp_##RC, Rc, FPSCR>

#define EXECUTE_LOADSTORE(OP, RA, RB, LD, SZ, UP, RX) \
&powerpc_cpu::execute_loadstore<op_##OP, operand_##RA, operand_##RB, LD, SZ, UP, RX>

#define EXECUTE_LOADSTORE_MULTIPLE(RA, DP, LD) \
&powerpc_cpu::execute_loadstore_multiple<operand_##RA, operand_##DP, LD>

#define EXECUTE_LOAD_STRING(RA, IM, NB) \
&powerpc_cpu::execute_load_string<operand_##RA, IM, operand_##NB>

#define EXECUTE_STORE_STRING(RA, IM, NB) \
&powerpc_cpu::execute_store_string<operand_##RA, IM, operand_##NB>

#define EXECUTE_SHIFT(OP, RD, RA, SH, SO, CA, Rc) \
&powerpc_cpu::execute_shift<op_##OP, operand_##RD, operand_##RA, operand_##SH, op_##SO, CA, Rc>

#define EXECUTE_FP_LOADSTORE(RA, RB, LD, DB, UP) \
&powerpc_cpu::execute_fp_loadstore<operand_##RA, operand_##RB, LD, DB, UP>

#define _DECODE_ADDITION(RA, RB, RC, CA) \
powerpc_cpu::decode_addition<operand_##RA, operand_##RB, operand_##RC, CA>

#define DECODE_ADDITION(RA, RB, RC, CA) \
SPD(&_DECODE_ADDITION(RA, RB, RC, CA))

#define _DECODE_RLWINM(RA, RS) \
powerpc_cpu::decode_rlwinm<operand_##RA, operand_##RS>

#define DECODE_RLWINM(RA, RS) \
SPD(&_DECODE_RLWINM(RA, RS))

const powerpc_cpu::instr_info_t powerpc_cpu::powerpc_ii_table[] = {
	{ "invalid",
	  EXECUTE_0(illegal),
	  NULL,
	  PPC_I(INVALID),
	  INVALID_form, 0, 0, CFLOW_TRAP
	},
	{ "add",
	  EXECUTE_ADDITION(RA, RB, NONE, CA_BIT_0, OE_BIT_G, RC_BIT_G),
	  DECODE_ADDITION(RA, RB, NONE, CA_BIT_0),
	  PPC_I(ADD),
	  XO_form, 31, 266, CFLOW_NORMAL
	},
	{ "addc",
	  EXECUTE_ADDITION(RA, RB, NONE, CA_BIT_1, OE_BIT_G, RC_BIT_G),
	  DECODE_ADDITION(RA, RB, NONE, CA_BIT_1),
	  PPC_I(ADDC),
	  XO_form, 31,  10, CFLOW_NORMAL
	},
	{ "adde",
	  EXECUTE_ADDITION(RA, RB, XER_CA, CA_BIT_1, OE_BIT_G, RC_BIT_G),
	  DECODE_ADDITION(RA, RB, XER_CA, CA_BIT_1),
	  PPC_I(ADDE),
	  XO_form, 31, 138, CFLOW_NORMAL
	},
	{ "addi",
	  EXECUTE_ADDITION(RA_or_0, SIMM, NONE, CA_BIT_0, OE_BIT_0, RC_BIT_0),
	  NULL,
	  PPC_I(ADDI),
	  D_form, 14, 0, CFLOW_NORMAL
	},
	{ "addic",
	  EXECUTE_ADDITION(RA, SIMM, NONE, CA_BIT_1, OE_BIT_0, RC_BIT_0),
	  NULL,
	  PPC_I(ADDIC),
	  D_form, 12, 0, CFLOW_NORMAL
	},
	{ "addic.",
	  EXECUTE_ADDITION(RA, SIMM, NONE, CA_BIT_1, OE_BIT_0, RC_BIT_1),
	  NULL,
	  PPC_I(ADDIC_),
	  D_form, 13, 0, CFLOW_NORMAL
	},
	{ "addis",
	  EXECUTE_ADDITION(RA_or_0, SIMM_shifted, NONE, CA_BIT_0, OE_BIT_0, RC_BIT_0),
	  NULL,
	  PPC_I(ADDIS),
	  D_form, 15, 0, CFLOW_NORMAL
	},
	{ "addme",
	  EXECUTE_ADDITION(RA, MINUS_ONE, XER_CA, CA_BIT_1, OE_BIT_G, RC_BIT_G),
	  DECODE_ADDITION(RA, MINUS_ONE, XER_CA, CA_BIT_1),
	  PPC_I(ADDME),
	  XO_form, 31, 234, CFLOW_NORMAL
	},
	{ "addze",
	  EXECUTE_ADDITION(RA, ZERO, XER_CA, CA_BIT_1, OE_BIT_G, RC_BIT_G),
	  DECODE_ADDITION(RA, ZERO, XER_CA, CA_BIT_1),
	  PPC_I(ADDZE),
	  XO_form, 31, 202, CFLOW_NORMAL
	},
	{ "and",
	  EXECUTE_GENERIC_ARITH(and, RA, RS, RB, NONE, OE_BIT_0, RC_BIT_G),
	  NULL,
	  PPC_I(AND),
	  X_form, 31, 28, CFLOW_NORMAL
	},
	{ "andc",
	  EXECUTE_GENERIC_ARITH(andc, RA, RS, RB, NONE, OE_BIT_0, RC_BIT_G),
	  NULL,
	  PPC_I(ANDC),
	  X_form, 31, 60, CFLOW_NORMAL
	},
	{ "andi.",
	  EXECUTE_GENERIC_ARITH(and, RA, RS, UIMM, NONE, OE_BIT_0, RC_BIT_1),
	  NULL,
	  PPC_I(ANDI),
	  D_form, 28, 0, CFLOW_NORMAL
	},
	{ "andis.",
	  EXECUTE_GENERIC_ARITH(and, RA, RS, UIMM_shifted, NONE, OE_BIT_0, RC_BIT_1),
	  NULL,
	  PPC_I(ANDIS),
	  D_form, 29, 0, CFLOW_NORMAL
	},
	{ "b",
	  EXECUTE_BRANCH(PC, immediate_value<BO_MAKE(0,0,0,0)>, LI, AA_BIT_G, LK_BIT_G),
	  NULL,
	  PPC_I(B),
	  I_form, 18, 0, CFLOW_BRANCH
	},
	{ "bc",
	  EXECUTE_BRANCH(PC, operand_BO, BD, AA_BIT_G, LK_BIT_G),
	  NULL,
	  PPC_I(BC),
	  B_form, 16, 0, CFLOW_BRANCH
	},
	{ "bcctr",
	  EXECUTE_BRANCH(CTR, operand_BO, ZERO, AA_BIT_0, LK_BIT_G),
	  NULL,
	  PPC_I(BCCTR),
	  XL_form, 19, 528, CFLOW_BRANCH
	},
	{ "bclr",
	  EXECUTE_BRANCH(LR, operand_BO, ZERO, AA_BIT_0, LK_BIT_G),
	  NULL,
	  PPC_I(BCLR),
	  XL_form, 19, 16, CFLOW_BRANCH
	},
	{ "cmp",
	  EXECUTE_COMPARE(RB, int32),
	  NULL,
	  PPC_I(CMP),
	  X_form, 31, 0, CFLOW_NORMAL
	},
	{ "cmpi",
	  EXECUTE_COMPARE(SIMM, int32),
	  NULL,
	  PPC_I(CMPI),
	  D_form, 11, 0, CFLOW_NORMAL
	},
	{ "cmpl",
	  EXECUTE_COMPARE(RB, uint32),
	  NULL,
	  PPC_I(CMPL),
	  X_form, 31, 32, CFLOW_NORMAL
	},
	{ "cmpli",
	  EXECUTE_COMPARE(UIMM, uint32),
	  NULL,
	  PPC_I(CMPLI),
	  D_form, 10, 0, CFLOW_NORMAL
	},
	{ "cntlzw",
	  EXECUTE_GENERIC_ARITH(cntlzw, RA, RS, NONE, NONE, OE_BIT_0, RC_BIT_G),
	  NULL,
	  PPC_I(CNTLZW),
	  X_form, 31, 26, CFLOW_NORMAL
	},
	{ "crand",
	  EXECUTE_CR_OP(and),
	  NULL,
	  PPC_I(CRAND),
	  XL_form, 19, 257, CFLOW_NORMAL
	},
	{ "crandc",
	  EXECUTE_CR_OP(andc),
	  NULL,
	  PPC_I(CRANDC),
	  XL_form, 19, 129, CFLOW_NORMAL
	},
	{ "creqv",
	  EXECUTE_CR_OP(eqv),
	  NULL,
	  PPC_I(CREQV),
	  XL_form, 19, 289, CFLOW_NORMAL
	},
	{ "crnand",
	  EXECUTE_CR_OP(nand),
	  NULL,
	  PPC_I(CRNAND),
	  XL_form, 19, 225, CFLOW_NORMAL
	},
	{ "crnor",
	  EXECUTE_CR_OP(nor),
	  NULL,
	  PPC_I(CRNOR),
	  XL_form, 19, 33, CFLOW_NORMAL
	},
	{ "cror",
	  EXECUTE_CR_OP(or),
	  NULL,
	  PPC_I(CROR),
	  XL_form, 19, 449, CFLOW_NORMAL
	},
	{ "crorc",
	  EXECUTE_CR_OP(orc),
	  NULL,
	  PPC_I(CRORC),
	  XL_form, 19, 417, CFLOW_NORMAL
	},
	{ "crxor",
	  EXECUTE_CR_OP(xor),
	  NULL,
	  PPC_I(CRXOR),
	  XL_form, 19, 193, CFLOW_NORMAL
	},
	{ "dcba",
	  EXECUTE_0(nop),
	  NULL,
	  PPC_I(DCBA),
	  X_form, 31, 758, CFLOW_NORMAL
	},
	{ "dcbf",
	  EXECUTE_0(nop),
	  NULL,
	  PPC_I(DCBF),
	  X_form, 31, 86, CFLOW_NORMAL
	},
	{ "dcbi",
	  EXECUTE_0(nop),
	  NULL,
	  PPC_I(DCBI),
	  X_form, 31, 470, CFLOW_NORMAL
	},
	{ "dcbst",
	  EXECUTE_0(nop),
	  NULL,
	  PPC_I(DCBST),
	  X_form, 31, 54, CFLOW_NORMAL
	},
	{ "dcbt",
	  EXECUTE_0(nop),
	  NULL,
	  PPC_I(DCBT),
	  X_form, 31, 278, CFLOW_NORMAL
	},
	{ "dcbtst",
	  EXECUTE_0(nop),
	  NULL,
	  PPC_I(DCBTST),
	  X_form, 31, 246, CFLOW_NORMAL
	},
	{ "dcbz",
	  EXECUTE_2(dcbz, operand_RA_or_0, operand_RB),
	  NULL,
	  PPC_I(DCBZ),
	  X_form, 31, 1014, CFLOW_NORMAL
	},
	{ "divw",
	  EXECUTE_3(divide, true, OE_BIT_G, RC_BIT_G),
	  NULL,
	  PPC_I(DIVW),
	  XO_form, 31, 491, CFLOW_NORMAL
	},
	{ "divwu",
	  EXECUTE_3(divide, false, OE_BIT_G, RC_BIT_G),
	  NULL,
	  PPC_I(DIVWU),
	  XO_form, 31, 459, CFLOW_NORMAL
	},
	{ "eciwx",
	  EXECUTE_0(nop),
	  NULL,
	  PPC_I(ECIWX),
	  X_form, 31, 310, CFLOW_NORMAL
	},
	{ "ecowx",
	  EXECUTE_0(nop),
	  NULL,
	  PPC_I(ECOWX),
	  X_form, 31, 438, CFLOW_NORMAL
	},
	{ "eieio",
	  EXECUTE_0(nop),
	  NULL,
	  PPC_I(EIEIO),
	  X_form, 31, 854, CFLOW_NORMAL
	},
	{ "eqv",
	  EXECUTE_GENERIC_ARITH(eqv, RA, RS, RB, NONE, OE_BIT_0, RC_BIT_G),
	  NULL,
	  PPC_I(EQV),
	  X_form, 31, 284, CFLOW_NORMAL
	},
	{ "extsb",
	  EXECUTE_GENERIC_ARITH(sign_extend_8_32, RA, RS, NONE, NONE, OE_BIT_0, RC_BIT_G),
	  NULL,
	  PPC_I(EXTSB),
	  X_form, 31, 954, CFLOW_NORMAL
	},
	{ "extsh",
	  EXECUTE_GENERIC_ARITH(sign_extend_16_32, RA, RS, NONE, NONE, OE_BIT_0, RC_BIT_G),
	  NULL, 
	  PPC_I(EXTSH),
	  X_form, 31, 922, CFLOW_NORMAL
	},
	{ "fabs",
	  EXECUTE_FP_ARITH(double, fabs, RD, RB, NONE, NONE, RC_BIT_G, false),
	  NULL,
	  PPC_I(FABS),
	  X_form, 63, 264, CFLOW_NORMAL
	},
	{ "fadd",
	  EXECUTE_FP_ARITH(double, fadd, RD, RA, RB, NONE, RC_BIT_G, true),
	  NULL,
	  PPC_I(FADD),
	  A_form, 63, 21, CFLOW_NORMAL
	},
	{ "fadds",
	  EXECUTE_FP_ARITH(float, fadd, RD, RA, RB, NONE, RC_BIT_G, true),
	  NULL,
	  PPC_I(FADDS),
	  A_form, 59, 21, CFLOW_NORMAL
	},
	{ "fcmpo",
	  EXECUTE_1(fp_compare, true),
	  NULL,
	  PPC_I(FCMPO),
	  X_form, 63, 32, CFLOW_NORMAL
	},
	{ "fcmpu",
	  EXECUTE_1(fp_compare, false),
	  NULL,
	  PPC_I(FCMPU),
	  X_form, 63, 0, CFLOW_NORMAL
	},
	{ "fctiw",
	  EXECUTE_2(fp_int_convert, operand_FPSCR_RN, RC_BIT_G),
	  NULL,
	  PPC_I(FCTIW),
	  X_form, 63, 14, CFLOW_NORMAL
	},
	{ "fctiwz",
	  EXECUTE_2(fp_int_convert, operand_ONE, RC_BIT_G),
	  NULL,
	  PPC_I(FCTIWZ),
	  X_form, 63, 15, CFLOW_NORMAL
	},
	{ "fdiv",
	  EXECUTE_FP_ARITH(double, fdiv, RD, RA, RB, NONE, RC_BIT_G, true),
	  NULL,
	  PPC_I(FDIV),
	  A_form, 63, 18, CFLOW_NORMAL
	},
	{ "fdivs",
	  EXECUTE_FP_ARITH(float, fdiv, RD, RA, RB, NONE, RC_BIT_G, true),
	  NULL,
	  PPC_I(FDIVS),
	  A_form, 59, 18, CFLOW_NORMAL
	},
	{ "fmadd",
	  EXECUTE_FP_ARITH(double, fmadd, RD, RA, RC, RB, RC_BIT_G, true),
	  NULL,
	  PPC_I(FMADD),
	  A_form, 63, 29, CFLOW_NORMAL
	},
	{ "fmadds",
	  EXECUTE_FP_ARITH(float, fmadd, RD, RA, RC, RB, RC_BIT_G, true),
	  NULL,
	  PPC_I(FMADDS),
	  A_form, 59, 29, CFLOW_NORMAL
	},
	{ "fmr",
	  EXECUTE_FP_ARITH(double, fnop, RD, RB, NONE, NONE, RC_BIT_G, false),
	  NULL,
	  PPC_I(FMR),
	  X_form, 63, 72, CFLOW_NORMAL
	},
	{ "fmsub",
	  EXECUTE_FP_ARITH(double, fmsub, RD, RA, RC, RB, RC_BIT_G, true),
	  NULL,
	  PPC_I(FMSUB),
	  A_form, 63, 28, CFLOW_NORMAL
	},
	{ "fmsubs",
	  EXECUTE_FP_ARITH(float, fmsub, RD, RA, RC, RB, RC_BIT_G, true),
	  NULL,
	  PPC_I(FMSUBS),
	  A_form, 59, 28, CFLOW_NORMAL
	},
	{ "fmul",
	  EXECUTE_FP_ARITH(double, fmul, RD, RA, RC, NONE, RC_BIT_G, true),
	  NULL,
	  PPC_I(FMUL),
	  A_form, 63, 25, CFLOW_NORMAL
	},
	{ "fmuls",
	  EXECUTE_FP_ARITH(float, fmul, RD, RA, RC, NONE, RC_BIT_G, true),
	  NULL,
	  PPC_I(FMULS),
	  A_form, 59, 25, CFLOW_NORMAL
	},
	{ "fnabs",
	  EXECUTE_FP_ARITH(double, fnabs, RD, RB, NONE, NONE, RC_BIT_G, false),
	  NULL,
	  PPC_I(FNABS),
	  X_form, 63, 136, CFLOW_NORMAL
	},
	{ "fneg",
	  EXECUTE_FP_ARITH(double, fneg, RD, RB, NONE, NONE, RC_BIT_G, false),
	  NULL,
	  PPC_I(FNEG),
	  X_form, 63, 40, CFLOW_NORMAL
	},
	{ "fnmadd",
	  EXECUTE_FP_ARITH(double, fnmadd, RD, RA, RC, RB, RC_BIT_G, true),
	  NULL,
	  PPC_I(FNMADD),
	  A_form, 63, 31, CFLOW_NORMAL
	},
	{ "fnmadds",
	  EXECUTE_FP_ARITH(float, fnmadd, RD, RA, RC, RB, RC_BIT_G, true),
	  NULL,
	  PPC_I(FNMADDS),
	  A_form, 59, 31, CFLOW_NORMAL
	},
	{ "fnmsub",
	  EXECUTE_FP_ARITH(double, fnmsub, RD, RA, RC, RB, RC_BIT_G, true),
	  NULL,
	  PPC_I(FNMSUB),
	  A_form, 63, 30, CFLOW_NORMAL
	},
	{ "fnmsubs",
	  EXECUTE_FP_ARITH(float, fnmsub, RD, RA, RC, RB, RC_BIT_G, true),
	  NULL,
	  PPC_I(FNMSUBS),
	  A_form, 59, 30, CFLOW_NORMAL
	},
	{ "frsp",
	  EXECUTE_1(fp_round, RC_BIT_G),
	  NULL,
	  PPC_I(FRSP),
	  X_form, 63, 12, CFLOW_NORMAL
	},
	{ "fsub",
	  EXECUTE_FP_ARITH(double, fsub, RD, RA, RB, NONE, RC_BIT_G, true),
	  NULL,
	  PPC_I(FSUB),
	  A_form, 63, 20, CFLOW_NORMAL
	},
	{ "fsubs",
	  EXECUTE_FP_ARITH(float, fsub, RD, RA, RB, NONE, RC_BIT_G, true),
	  NULL,
	  PPC_I(FSUBS),
	  A_form, 59, 20, CFLOW_NORMAL
	},
	{ "icbi",
	  EXECUTE_2(icbi, operand_RA_or_0, operand_RB),
	  NULL,
	  PPC_I(ICBI),
	  X_form, 31, 982, CFLOW_NORMAL
	},
	{ "isync",
	  EXECUTE_0(isync),
	  NULL,
	  PPC_I(ISYNC),
	  X_form, 19, 150, CFLOW_NORMAL
	},
	{ "lbz",
	  EXECUTE_LOADSTORE(nop, RA_or_0, D, true, 1, false, false),
	  NULL,
	  PPC_I(LBZ),
	  D_form, 34, 0, CFLOW_NORMAL
	},
	{ "lbzu",
	  EXECUTE_LOADSTORE(nop, RA, D, true, 1, true, false),
	  NULL,
	  PPC_I(LBZU),
	  D_form, 35, 0, CFLOW_NORMAL
	},
	{ "lbzux",
	  EXECUTE_LOADSTORE(nop, RA, RB, true, 1, true, false),
	  NULL,
	  PPC_I(LBZUX),
	  X_form, 31, 119, CFLOW_NORMAL
	},
	{ "lbzx",
	  EXECUTE_LOADSTORE(nop, RA_or_0, RB, true, 1, false, false),
	  NULL,
	  PPC_I(LBZX),
	  X_form, 31, 87, CFLOW_NORMAL
	},
	{ "lfd",
	  EXECUTE_FP_LOADSTORE(RA_or_0, D, true, true, false),
	  NULL,
	  PPC_I(LFD),
	  D_form, 50, 0, CFLOW_NORMAL
	},
	{ "lfdu",
	  EXECUTE_FP_LOADSTORE(RA, D, true, true, true),
	  NULL,
	  PPC_I(LFDU),
	  D_form, 51, 0, CFLOW_NORMAL
	},
	{ "lfdux",
	  EXECUTE_FP_LOADSTORE(RA, RB, true, true, true),
	  NULL,
	  PPC_I(LFDUX),
	  X_form, 31, 631, CFLOW_NORMAL
	},
	{ "lfdx",
	  EXECUTE_FP_LOADSTORE(RA_or_0, RB, true, true, false),
	  NULL,
	  PPC_I(LFDX),
	  X_form, 31, 599, CFLOW_NORMAL
	},
	{ "lfs",
	  EXECUTE_FP_LOADSTORE(RA_or_0, D, true, false, false),
	  NULL,
	  PPC_I(LFS),
	  D_form, 48, 0, CFLOW_NORMAL
	},
	{ "lfsu",
	  EXECUTE_FP_LOADSTORE(RA, D, true, false, true),
	  NULL,
	  PPC_I(LFSU),
	  D_form, 49, 0, CFLOW_NORMAL
	},
	{ "lfsux",
	  EXECUTE_FP_LOADSTORE(RA, RB, true, false, true),
	  NULL,
	  PPC_I(LFSUX),
	  X_form, 31, 567, CFLOW_NORMAL
	},
	{ "lfsx",
	  EXECUTE_FP_LOADSTORE(RA_or_0, RB, true, false, false),
	  NULL,
	  PPC_I(LFSX),
	  X_form, 31, 535, CFLOW_NORMAL
	},
	{ "lha",
	  EXECUTE_LOADSTORE(sign_extend_16_32, RA_or_0, D, true, 2, false, false),
	  NULL,
	  PPC_I(LHA),
	  D_form, 42, 0, CFLOW_NORMAL
	},
	{ "lhau",
	  EXECUTE_LOADSTORE(sign_extend_16_32, RA, D, true, 2, true, false),
	  NULL,
	  PPC_I(LHAU),
	  D_form, 43, 0, CFLOW_NORMAL
	},
	{ "lhaux",
	  EXECUTE_LOADSTORE(sign_extend_16_32, RA, RB, true, 2, true, false),
	  NULL,
	  PPC_I(LHAUX),
	  X_form, 31, 375, CFLOW_NORMAL
	},
	{ "lhax",
	  EXECUTE_LOADSTORE(sign_extend_16_32, RA_or_0, RB, true, 2, false, false),
	  NULL,
	  PPC_I(LHAX),
	  X_form, 31, 343, CFLOW_NORMAL
	},
	{ "lhbrx",
	  EXECUTE_LOADSTORE(nop, RA_or_0, RB, true, 2, false, true),
	  NULL,
	  PPC_I(LHBRX),
	  X_form, 31, 790, CFLOW_NORMAL
	},
	{ "lhz",
	  EXECUTE_LOADSTORE(nop, RA_or_0, D, true, 2, false, false),
	  NULL,
	  PPC_I(LHZ),
	  D_form, 40, 0, CFLOW_NORMAL
	},
	{ "lhzu",
	  EXECUTE_LOADSTORE(nop, RA, D, true, 2, true, false),
	  NULL,
	  PPC_I(LHZU),
	  D_form, 41, 0, CFLOW_NORMAL
	},
	{ "lhzux",
	  EXECUTE_LOADSTORE(nop, RA, RB, true, 2, true, false),
	  NULL,
	  PPC_I(LHZUX),
	  X_form, 31, 311, CFLOW_NORMAL
	},
	{ "lhzx",
	  EXECUTE_LOADSTORE(nop, RA_or_0, RB, true, 2, false, false),
	  NULL,
	  PPC_I(LHZX),
	  X_form, 31, 279, CFLOW_NORMAL
	},
	{ "lmw",
	  EXECUTE_LOADSTORE_MULTIPLE(RA_or_0, D, true),
	  NULL,
	  PPC_I(LMW),
	  D_form, 46, 0, CFLOW_NORMAL
	},
	{ "lswi",
	  EXECUTE_LOAD_STRING(RA_or_0, true, NB),
	  NULL,
	  PPC_I(LSWI),
	  X_form, 31, 597, CFLOW_NORMAL
	},
	{ "lswx",
	  EXECUTE_LOAD_STRING(RA_or_0, false, XER_COUNT),
	  NULL,
	  PPC_I(LSWX),
	  X_form, 31, 533, CFLOW_NORMAL
	},
	{ "lwarx",
	  EXECUTE_1(lwarx, operand_RA_or_0),
	  NULL,
	  PPC_I(LWARX),
	  X_form, 31, 20, CFLOW_NORMAL
	},
	{ "lwbrx",
	  EXECUTE_LOADSTORE(nop, RA_or_0, RB, true, 4, false, true),
	  NULL,
	  PPC_I(LWBRX),
	  X_form, 31, 534, CFLOW_NORMAL
	},
	{ "lwz",
	  EXECUTE_LOADSTORE(nop, RA_or_0, D, true, 4, false, false),
	  NULL,
	  PPC_I(LWZ),
	  D_form, 32, 0, CFLOW_NORMAL
	},
	{ "lwzu",
	  EXECUTE_LOADSTORE(nop, RA, D, true, 4, true, false),
	  NULL,
	  PPC_I(LWZU),
	  D_form, 33, 0, CFLOW_NORMAL
	},
	{ "lwzux",
	  EXECUTE_LOADSTORE(nop, RA, RB, true, 4, true, false),
	  NULL,
	  PPC_I(LWZUX),
	  X_form, 31, 55, CFLOW_NORMAL
	},
	{ "lwzx",
	  EXECUTE_LOADSTORE(nop, RA_or_0, RB, true, 4, false, false),
	  NULL,
	  PPC_I(LWZX),
	  X_form, 31, 23, CFLOW_NORMAL
	},
	{ "mcrf",
	  EXECUTE_0(mcrf),
	  NULL,
	  PPC_I(MCRF),
	  XL_form, 19, 0, CFLOW_NORMAL
	},
	{ "mcrfs",
	  EXECUTE_0(mcrfs),
	  NULL,
	  PPC_I(MCRFS),
	  X_form, 63, 64, CFLOW_NORMAL
	},
	{ "mfcr",
	  EXECUTE_GENERIC_ARITH(nop, RD, CR, NONE, NONE, OE_BIT_0, RC_BIT_0),
	  NULL,
	  PPC_I(MFCR),
	  X_form, 31, 19, CFLOW_NORMAL
	},
	{ "mffs",
	  EXECUTE_1(mffs, RC_BIT_G),
	  NULL,
	  PPC_I(MFFS),
	  X_form, 63, 583, CFLOW_NORMAL
	},
	{ "mfmsr",
	  EXECUTE_0(mfmsr),
	  NULL,
	  PPC_I(MFMSR),
	  X_form, 31, 83, CFLOW_NORMAL
	},
	{ "mfspr",
	  EXECUTE_1(mfspr, operand_SPR),
	  NULL,
	  PPC_I(MFSPR),
	  XFX_form, 31, 339, CFLOW_NORMAL
	},
	{ "mftb",
	  EXECUTE_1(mftbr, operand_TBR),
	  NULL,
	  PPC_I(MFTB),
	  XFX_form, 31, 371, CFLOW_NORMAL
	},
	{ "mtcrf",
	  EXECUTE_0(mtcrf),
	  NULL,
	  PPC_I(MTCRF),
	  XFX_form, 31, 144, CFLOW_NORMAL
	},
	{ "mtfsb0",
	  EXECUTE_2(mtfsb, immediate_value<0>, RC_BIT_G),
	  NULL,
	  PPC_I(MTFSB0),
	  X_form, 63, 70, CFLOW_NORMAL
	},
	{ "mtfsb1",
	  EXECUTE_2(mtfsb, immediate_value<1>, RC_BIT_G),
	  NULL,
	  PPC_I(MTFSB1),
	  X_form, 63, 38, CFLOW_NORMAL
	},
	{ "mtfsf",
	  EXECUTE_3(mtfsf, operand_FM, operand_fp_dw_RB, RC_BIT_G),
	  NULL,
	  PPC_I(MTFSF),
	  XFL_form, 63, 711, CFLOW_NORMAL
	},
	{ "mtfsfi",
	  EXECUTE_2(mtfsfi, operand_IMM, RC_BIT_G),
	  NULL,
	  PPC_I(MTFSFI),
	  X_form, 63, 134, CFLOW_NORMAL
	},
	{ "mtspr",
	  EXECUTE_1(mtspr, operand_SPR),
	  NULL,
	  PPC_I(MTSPR),
	  XFX_form, 31, 467, CFLOW_NORMAL
	},
	{ "mulhw",
	  EXECUTE_4(multiply, true, true, OE_BIT_0, RC_BIT_G),
	  NULL,
	  PPC_I(MULHW),
	  XO_form, 31, 75, CFLOW_NORMAL
	},
	{ "mulhwu",
	  EXECUTE_4(multiply, true, false, OE_BIT_0, RC_BIT_G),
	  NULL,
	  PPC_I(MULHWU),
	  XO_form, 31, 11, CFLOW_NORMAL
	},
	{ "mulli",
	  EXECUTE_GENERIC_ARITH(smul, RD, RA, SIMM, NONE, OE_BIT_0, RC_BIT_0),
	  NULL,
	  PPC_I(MULLI),
	  D_form, 7, 0, CFLOW_NORMAL
	},
	{ "mullw",
	  EXECUTE_4(multiply, false, true, OE_BIT_G, RC_BIT_G),
	  NULL,
	  PPC_I(MULLW),
	  XO_form, 31, 235, CFLOW_NORMAL
	},
	{ "nand",
	  EXECUTE_GENERIC_ARITH(nand, RA, RS, RB, NONE, OE_BIT_0, RC_BIT_G),
	  NULL, 
	  PPC_I(NAND),
	  X_form, 31, 476, CFLOW_NORMAL
	},
	{ "neg",
	  EXECUTE_GENERIC_ARITH(neg, RD, RA, NONE, NONE, OE_BIT_G, RC_BIT_G),
	  NULL, 
	  PPC_I(NEG),
	  XO_form, 31, 104, CFLOW_NORMAL
	},
	{ "nor",
	  EXECUTE_GENERIC_ARITH(nor, RA, RS, RB, NONE, OE_BIT_0, RC_BIT_G),
	  NULL,
	  PPC_I(NOR),
	  XO_form, 31, 124, CFLOW_NORMAL
	},
	{ "or",
	  EXECUTE_GENERIC_ARITH(or, RA, RS, RB, NONE, OE_BIT_0, RC_BIT_G),
	  NULL,
	  PPC_I(OR),
	  XO_form, 31, 444, CFLOW_NORMAL
	},
	{ "orc",
	  EXECUTE_GENERIC_ARITH(orc, RA, RS, RB, NONE, OE_BIT_0, RC_BIT_G),
	  NULL,
	  PPC_I(ORC),
	  XO_form, 31, 412, CFLOW_NORMAL
	},
	{ "ori",
	  EXECUTE_GENERIC_ARITH(or, RA, RS, UIMM, NONE, OE_BIT_0, RC_BIT_0),
	  NULL,
	  PPC_I(ORI),
	  D_form, 24, 0, CFLOW_NORMAL
	},
	{ "oris",
	  EXECUTE_GENERIC_ARITH(or, RA, RS, UIMM_shifted, NONE, OE_BIT_0, RC_BIT_0),
	  NULL,
	  PPC_I(ORIS),
	  D_form, 25, 0, CFLOW_NORMAL
	},
	{ "rlwimi",
	  EXECUTE_3(rlwimi, operand_SH, operand_MASK, RC_BIT_G),
	  NULL,
	  PPC_I(RLWIMI),
	  M_form, 20, 0, CFLOW_NORMAL
	},
	{ "rlwinm",
	  EXECUTE_GENERIC_ARITH(ppc_rlwinm, RA, RS, SH, MASK, OE_BIT_0, RC_BIT_G),
	  DECODE_RLWINM(RA, RS),
	  PPC_I(RLWINM),
	  M_form, 21, 0, CFLOW_NORMAL
	},
	{ "rlwnm",
	  EXECUTE_GENERIC_ARITH(ppc_rlwnm, RA, RS, RB, MASK, OE_BIT_0, RC_BIT_G),
	  NULL,
	  PPC_I(RLWNM),
	  M_form, 23, 0, CFLOW_NORMAL
	},
	{ "sc",
	  EXECUTE_0(syscall),
	  NULL,
	  PPC_I(SC),
	  SC_form, 17, 0, CFLOW_NORMAL
	},
	{ "slw",
	  EXECUTE_SHIFT(shll, RA, RS, RB, andi<0x3f>, CA_BIT_0, RC_BIT_G),
	  NULL,
	  PPC_I(SLW),
	  X_form, 31, 24, CFLOW_NORMAL
	},
	{ "sraw",
	  EXECUTE_SHIFT(shra, RA, RS, RB, andi<0x3f>, CA_BIT_1, RC_BIT_G),
	  NULL,
	  PPC_I(SRAW),
	  X_form, 31, 792, CFLOW_NORMAL
	},
	{ "srawi",
	  EXECUTE_SHIFT(shra, RA, RS, SH, andi<0x1f>, CA_BIT_1, RC_BIT_G),
	  NULL,
	  PPC_I(SRAWI),
	  X_form, 31, 824, CFLOW_NORMAL
	},
	{ "srw",
	  EXECUTE_SHIFT(shrl, RA, RS, RB, andi<0x3f>, CA_BIT_0, RC_BIT_G),
	  NULL,
	  PPC_I(SRW),
	  X_form, 31, 536, CFLOW_NORMAL
	},
	{ "stb",
	  EXECUTE_LOADSTORE(nop, RA_or_0, D, false, 1, false, false),
	  NULL,
	  PPC_I(STB),
	  D_form, 38, 0, CFLOW_NORMAL
	},
	{ "stbu",
	  EXECUTE_LOADSTORE(nop, RA, D, false, 1, true, false),
	  NULL,
	  PPC_I(STBU),
	  D_form, 39, 0, CFLOW_NORMAL
	},
	{ "stbux",
	  EXECUTE_LOADSTORE(nop, RA, RB, false, 1, true, false),
	  NULL,
	  PPC_I(STBUX),
	  X_form, 31, 247, CFLOW_NORMAL
	},
	{ "stbx",
	  EXECUTE_LOADSTORE(nop, RA_or_0, RB, false, 1, false, false),
	  NULL,
	  PPC_I(STBX),
	  X_form, 31, 215, CFLOW_NORMAL
	},
	{ "stfd",
	  EXECUTE_FP_LOADSTORE(RA_or_0, D, false, true, false),
	  NULL,
	  PPC_I(STFD),
	  D_form, 54, 0, CFLOW_NORMAL
	},
	{ "stfdu",
	  EXECUTE_FP_LOADSTORE(RA, D, false, true, true),
	  NULL,
	  PPC_I(STFDU),
	  D_form, 55, 0, CFLOW_NORMAL
	},
	{ "stfdux",
	  EXECUTE_FP_LOADSTORE(RA, RB, false, true, true),
	  NULL,
	  PPC_I(STFDUX),
	  X_form, 31, 759, CFLOW_NORMAL
	},
	{ "stfdx",
	  EXECUTE_FP_LOADSTORE(RA_or_0, RB, false, true, false),
	  NULL,
	  PPC_I(STFDX),
	  X_form, 31, 727, CFLOW_NORMAL
	},
	{ "stfs",
	  EXECUTE_FP_LOADSTORE(RA_or_0, D, false, false, false),
	  NULL,
	  PPC_I(STFS),
	  D_form, 52, 0, CFLOW_NORMAL
	},
	{ "stfsu",
	  EXECUTE_FP_LOADSTORE(RA, D, false, false, true),
	  NULL,
	  PPC_I(STFSU),
	  D_form, 53, 0, CFLOW_NORMAL
	},
	{ "stfsux",
	  EXECUTE_FP_LOADSTORE(RA, RB, false, false, true),
	  NULL,
	  PPC_I(STFSUX),
	  X_form, 31, 695, CFLOW_NORMAL
	},
	{ "stfsx",
	  EXECUTE_FP_LOADSTORE(RA_or_0, RB, false, false, false),
	  NULL,
	  PPC_I(STFSX),
	  X_form, 31, 663, CFLOW_NORMAL
	},
	{ "sth",
	  EXECUTE_LOADSTORE(nop, RA_or_0, D, false, 2, false, false),
	  NULL,
	  PPC_I(STH),
	  D_form, 44, 0, CFLOW_NORMAL
	},
	{ "sthbrx",
	  EXECUTE_LOADSTORE(nop, RA_or_0, RB, false, 2, false, true),
	  NULL,
	  PPC_I(STHBRX),
	  X_form, 31, 918, CFLOW_NORMAL
	},
	{ "sthu",
	  EXECUTE_LOADSTORE(nop, RA, D, false, 2, true, false),
	  NULL,
	  PPC_I(STHU),
	  D_form, 45, 0, CFLOW_NORMAL
	},
	{ "sthux",
	  EXECUTE_LOADSTORE(nop, RA, RB, false, 2, true, false),
	  NULL,
	  PPC_I(STHUX),
	  X_form, 31, 439, CFLOW_NORMAL
	},
	{ "sthx",
	  EXECUTE_LOADSTORE(nop, RA_or_0, RB, false, 2, false, false),
	  NULL,
	  PPC_I(STHX),
	  X_form, 31, 407, CFLOW_NORMAL
	},
	{ "stmw",
	  EXECUTE_LOADSTORE_MULTIPLE(RA_or_0, D, false),
	  NULL,
	  PPC_I(STMW),
	  D_form, 47, 0, CFLOW_NORMAL
	},
	{ "stswi",
	  EXECUTE_STORE_STRING(RA_or_0, true, NB),
	  NULL,
	  PPC_I(STSWI),
	  X_form, 31, 725, CFLOW_NORMAL
	},
	{ "stswx",
	  EXECUTE_STORE_STRING(RA_or_0, false, XER_COUNT),
	  NULL,
	  PPC_I(STSWX),
	  X_form, 31, 661, CFLOW_NORMAL
	},
	{ "stw",
	  EXECUTE_LOADSTORE(nop, RA_or_0, D, false, 4, false, false),
	  NULL,
	  PPC_I(STW),
	  D_form, 36, 0, CFLOW_NORMAL
	},
	{ "stwbrx",
	  EXECUTE_LOADSTORE(nop, RA_or_0, RB, false, 4, false, true),
	  NULL,
	  PPC_I(STWBRX),
	  X_form, 31, 662, CFLOW_NORMAL
	},
	{ "stwcx.",
	  EXECUTE_1(stwcx, operand_RA_or_0),
	  NULL,
	  PPC_I(STWCX),
	  X_form, 31, 150, CFLOW_NORMAL
	},
	{ "stwu",
	  EXECUTE_LOADSTORE(nop, RA, D, false, 4, true, false),
	  NULL,
	  PPC_I(STWU),
	  D_form, 37, 0, CFLOW_NORMAL
	},
	{ "stwux",
	  EXECUTE_LOADSTORE(nop, RA, RB, false, 4, true, false),
	  NULL,
	  PPC_I(STWUX),
	  X_form, 31, 183, CFLOW_NORMAL
	},
	{ "stwx",
	  EXECUTE_LOADSTORE(nop, RA_or_0, RB, false, 4, false, false),
	  NULL,
	  PPC_I(STWX),
	  X_form, 31, 151, CFLOW_NORMAL
	},
	{ "subf",
	  EXECUTE_ADDITION(RA_compl, RB, ONE, CA_BIT_0, OE_BIT_G, RC_BIT_G),
	  DECODE_ADDITION(RA_compl, RB, ONE, CA_BIT_0),
	  PPC_I(SUBF),
	  XO_form, 31, 40, CFLOW_NORMAL
	},
	{ "subfc",
	  EXECUTE_ADDITION(RA_compl, RB, ONE, CA_BIT_1, OE_BIT_G, RC_BIT_G),
	  DECODE_ADDITION(RA_compl, RB, ONE, CA_BIT_1),
	  PPC_I(SUBFC),
	  XO_form, 31, 8, CFLOW_NORMAL
	},
	{ "subfe",
	  EXECUTE_ADDITION(RA_compl, RB, XER_CA, CA_BIT_1, OE_BIT_G, RC_BIT_G),
	  DECODE_ADDITION(RA_compl, RB, XER_CA, CA_BIT_1),
	  PPC_I(SUBFE),
	  XO_form, 31, 136, CFLOW_NORMAL
	},
	{ "subfic",
	  EXECUTE_ADDITION(RA_compl, SIMM, ONE, CA_BIT_1, OE_BIT_0, RC_BIT_0),
	  NULL,
	  PPC_I(SUBFIC),
	  D_form, 8, 0, CFLOW_NORMAL
	},
	{ "subfme",
	  EXECUTE_ADDITION(RA_compl, XER_CA, MINUS_ONE, CA_BIT_1, OE_BIT_G, RC_BIT_G),
	  DECODE_ADDITION(RA_compl, XER_CA, MINUS_ONE, CA_BIT_1),
	  PPC_I(SUBFME),
	  XO_form, 31, 232, CFLOW_NORMAL
	},
	{ "subfze",
	  EXECUTE_ADDITION(RA_compl, XER_CA, ZERO, CA_BIT_1, OE_BIT_G, RC_BIT_G),
	  DECODE_ADDITION(RA_compl, XER_CA, ZERO, CA_BIT_1),
	  PPC_I(SUBFZE),
	  XO_form, 31, 200, CFLOW_NORMAL
	},
	{ "sync",
	  EXECUTE_0(nop),
	  NULL,
	  PPC_I(SYNC),
	  X_form, 31, 598, CFLOW_NORMAL
	},
	{ "xor",
	  EXECUTE_GENERIC_ARITH(xor, RA, RS, RB, NONE, OE_BIT_0, RC_BIT_G),
	  NULL,
	  PPC_I(XOR),
	  X_form, 31, 316, CFLOW_NORMAL
	},
	{ "xori",
	  EXECUTE_GENERIC_ARITH(xor, RA, RS, UIMM, NONE, OE_BIT_0, RC_BIT_0),
	  NULL,
	  PPC_I(XORI),
	  D_form, 26, 0, CFLOW_NORMAL
	},
	{ "xoris",
	  EXECUTE_GENERIC_ARITH(xor, RA, RS, UIMM_shifted, NONE, OE_BIT_0, RC_BIT_0),
	  NULL,
	  PPC_I(XORIS),
	  D_form, 27, 0, CFLOW_NORMAL
	}
};

#ifndef PPC_NO_STATIC_II_INDEX_TABLE
powerpc_cpu::ii_index_t powerpc_cpu::ii_index_table[II_INDEX_TABLE_SIZE];
std::vector<powerpc_cpu::instr_info_t> powerpc_cpu::ii_table;
#endif

void powerpc_cpu::init_decoder()
{
#ifndef PPC_NO_STATIC_II_INDEX_TABLE
	static bool initialized = false;
	if (initialized)
		return;
	initialized = true;
#endif

	const int ii_count = sizeof(powerpc_ii_table)/sizeof(powerpc_ii_table[0]);
	D(bug("PowerPC decode table has %d entries\n", ii_count));
	assert(ii_count < (1 << (8 * sizeof(ii_index_t))));
	ii_table.reserve(ii_count);

	for (int i = 0; i < ii_count; i++) {
		const instr_info_t * ii = &powerpc_ii_table[i];
		init_decoder_entry(ii);
	}
}

void powerpc_cpu::init_decoder_entry(const instr_info_t * ii)
{
	ii_table.push_back(*ii);
	const ii_index_t ii_index = ii_table.size() - 1;

	assert((ii->format == INVALID_form && ii_index == 0) ||
		   (ii->format != INVALID_form && ii_index != 0) );

	switch (ii->format) {
	case INVALID_form:
		// Initialize all index table
		for (int i = 0; i < II_INDEX_TABLE_SIZE; i++)
			ii_index_table[i] = ii_index;
		break;

	case B_form:
	case D_form:
	case I_form:
	case M_form:
		// Primary opcode only
		for (int j = 0; j < 1024; j++)
			ii_index_table[make_ii_index(ii->opcode, j)] = ii_index;
		break;

	case SC_form:
		// Primary opcode only, with reserved bits
		ii_index_table[make_ii_index(ii->opcode, 1)] = ii_index;
		break;

	case X_form:
	case XL_form:
	case XFX_form:
	case XFL_form:
		// Extended opcode in bits 21..30
		ii_index_table[make_ii_index(ii->opcode, ii->xo)] = ii_index;
		break;

	case XO_form:
		// Extended opcode in bits 22..30, with OE bit 21
		ii_index_table[make_ii_index(ii->opcode,          ii->xo)] = ii_index;
		ii_index_table[make_ii_index(ii->opcode, 1 << 9 | ii->xo)] = ii_index;
		break;

	case A_form:
		// Extended opcode in bits 26..30
		for (int j = 0; j < 32; j++)
			ii_index_table[make_ii_index(ii->opcode, (j << 5) | ii->xo)] = ii_index;
		break;

	default:
		fprintf(stderr, "Unhandled form %d\n", ii->format);
		abort();
		break;
	}
}

#if SPECIALIZED_DECODERS

#define DEFINE_TEMPLATE_1(T1)					class T1
#define DEFINE_TEMPLATE_2(T1, T2)				class T1, class T2
#define DEFINE_TEMPLATE_3(T1, T2, T3)			class T1, class T2, class T3
#define DEFINE_TEMPLATE_4(T1, T2, T3, T4)		class T1, class T2, class T3, class T4
#define DEFINE_TEMPLATE_5(T1, T2, T3, T4, T5)	class T1, class T2, class T3, class T4, class T5

#ifndef GENEXEC
#define DEFINE_TEMPLATE(NAME, NARGS, ARGLIST)	\
template< DEFINE_TEMPLATE_##NARGS ARGLIST > powerpc_cpu::execute_fn powerpc_cpu::decode_##NAME(uint32 opcode)
#endif

DEFINE_TEMPLATE(addition, 4, (RA, RB, RC, CA))
{
	if (OE_field::test(opcode)) {
		if (Rc_field::test(opcode))
			return &powerpc_cpu::execute_addition< RA, RB, RC, CA, OE_BIT_1, RC_BIT_1 >;
		else
			return &powerpc_cpu::execute_addition< RA, RB, RC, CA, OE_BIT_1, RC_BIT_0 >;
	}
	else {
		if (Rc_field::test(opcode))
			return &powerpc_cpu::execute_addition< RA, RB, RC, CA, OE_BIT_0, RC_BIT_1 >;
		else
			return &powerpc_cpu::execute_addition< RA, RB, RC, CA, OE_BIT_0, RC_BIT_0 >;
	}
	abort();
	return NULL;
}

#define IMPL_DECODE_ADDITION(RA, RB, RC, CA) \
template powerpc_cpu::execute_fn _DECODE_ADDITION(RA, RB, RC, CA)(uint32)

IMPL_DECODE_ADDITION(RA, RB, NONE, CA_BIT_0);
IMPL_DECODE_ADDITION(RA, RB, NONE, CA_BIT_1);
IMPL_DECODE_ADDITION(RA, RB, XER_CA, CA_BIT_1);
IMPL_DECODE_ADDITION(RA, MINUS_ONE, XER_CA, CA_BIT_1);
IMPL_DECODE_ADDITION(RA, ZERO, XER_CA, CA_BIT_1);
IMPL_DECODE_ADDITION(RA_compl, RB, ONE, CA_BIT_0);
IMPL_DECODE_ADDITION(RA_compl, RB, ONE, CA_BIT_1);
IMPL_DECODE_ADDITION(RA_compl, RB, XER_CA, CA_BIT_1);
IMPL_DECODE_ADDITION(RA_compl, XER_CA, MINUS_ONE, CA_BIT_1);
IMPL_DECODE_ADDITION(RA_compl, XER_CA, ZERO, CA_BIT_1);

DEFINE_TEMPLATE(rlwinm, 2, (RA, RS))
{
	int SH = SH_field::extract(opcode);
	int MB = MB_field::extract(opcode);
	int ME = ME_field::extract(opcode);
	int Rc = Rc_field::extract(opcode);
#define _(SH, MB, ME, Rc) (((SH) << 11) | ((MB) << 6) | ((ME) << 1) | (Rc))
	switch (_(SH, MB, ME, Rc)) {
#define CASE_SHIFT_1(SH) \
case _((SH), 0, 31 - (SH), 0): return EXECUTE_7(generic_arith, op_shll, RA, RS, immediate_value<SH>, operand_NONE, OE_BIT_0, RC_BIT_0); \
case _((SH), 0, 31 - (SH), 1): return EXECUTE_7(generic_arith, op_shll, RA, RS, immediate_value<SH>, operand_NONE, OE_BIT_0, RC_BIT_1);
#define CASE_SHIFT_2(SH)  CASE_SHIFT_1(SH); CASE_SHIFT_1(SH+1)
#define CASE_SHIFT_4(SH)  CASE_SHIFT_2(SH); CASE_SHIFT_2(SH+2)
#define CASE_SHIFT_8(SH)  CASE_SHIFT_4(SH); CASE_SHIFT_4(SH+4)
#define CASE_SHIFT_16(SH) CASE_SHIFT_8(SH); CASE_SHIFT_8(SH+8)
		CASE_SHIFT_16(0);
		CASE_SHIFT_16(16);
	}
#undef _
	if (Rc)
		return EXECUTE_GENERIC_ARITH(ppc_rlwinm, RA, RS, SH, MASK, OE_BIT_0, RC_BIT_1);
	else
		return EXECUTE_GENERIC_ARITH(ppc_rlwinm, RA, RS, SH, MASK, OE_BIT_0, RC_BIT_0);
}

#define IMPL_DECODE_RLWINM(RA, RS) \
template powerpc_cpu::execute_fn _DECODE_RLWINM(RA, RS)(uint32)

IMPL_DECODE_RLWINM(RA, RS);

#endif /* SPECIALIZED_DECODERS */