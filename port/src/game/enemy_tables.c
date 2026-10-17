/*
 * enemy_tables.c - the behaviour tables of banks $03 and $0B: sixteen
 * JMPs each at $B300 + 3 * (id & $0F), where objects_update enters for
 * ids $90-$9F (bank $03) and $70-$7F (bank $0B) once it has mapped the
 * bank at $A000 (docs/objects.md, "Update"), and the RTS at $B330 the
 * unused ids point to. enemy.h lists who each id is.
 *
 * Each JMP is 3 cycles, and 2 more at the target (the scaffold keeps every
 * target a function of its own); the registers pass through untouched.
 */
#include "enemy.h"

static void behavior_jmp(uint16_t pc, uint16_t target, int bank)
{
    mff_cycles(pc, 3);
    nes_cpu_instruction_boundary(target, 2);
    call_by_address_tail(target, bank);
}

/* the RTS at $B330 (both banks) */
static void behavior_none(uint16_t pc)
{
    mff_cycles(pc, 6);
    mff_rts();
}

/* bank $03, ids $90-$9F */
void func_B300_b1(void) { behavior_jmp(0xB300, 0xB3F4, BANK_03); }  /* $90 Abigail */
void func_B303_b1(void) { behavior_jmp(0xB303, 0xBB91, BANK_03); }  /* $91 rolling drum */
void func_B306_b1(void) { behavior_jmp(0xB306, 0xB330, BANK_03); }  /* $92 none */
void func_B309_b1(void) { behavior_jmp(0xB309, 0xB330, BANK_03); }  /* $93 none */
void func_B30C_b1(void) { behavior_jmp(0xB30C, 0xBBCB, BANK_03); }  /* $94 drum dispenser */
void func_B30F_b1(void) { behavior_jmp(0xB30F, 0xBD46, BANK_03); }  /* $95 smashed drum */
void func_B312_b1(void) { behavior_jmp(0xB312, 0xB330, BANK_03); }  /* $96 none */
void func_B315_b1(void) { behavior_jmp(0xB315, 0xB330, BANK_03); }  /* $97 none */
void func_B318_b1(void) { behavior_jmp(0xB318, 0xB330, BANK_03); }  /* $98 none */
void func_B31B_b1(void) { behavior_jmp(0xB31B, 0xB330, BANK_03); }  /* $99 none */
void func_B31E_b1(void) { behavior_jmp(0xB31E, 0xB330, BANK_03); }  /* $9A none */
void func_B321_b1(void) { behavior_jmp(0xB321, 0xBF38, BANK_03); }  /* $9B Belger down */
void func_B324_b1(void) { behavior_jmp(0xB324, 0xBFBE, BANK_03); }  /* $9C closing scene */
void func_B327_b1(void) { behavior_jmp(0xB327, 0xBF95, BANK_03); }  /* $9D Belger's shot */
void func_B32A_b1(void) { behavior_jmp(0xB32A, 0xBF48, BANK_03); }  /* $9E closing scene */
void func_B32D_b1(void) { behavior_jmp(0xB32D, 0xBED2, BANK_03); }  /* $9F Belger killed */
void func_B330_b1(void) { behavior_none(0xB330); }

/* bank $0B, ids $70-$7F */
void func_B300_b5(void) { behavior_jmp(0xB300, 0xB331, BANK_0B); }  /* $70 bonus stage */
void func_B303_b5(void) { behavior_jmp(0xB303, 0xB331, BANK_0B); }  /* $71 (same) */
void func_B306_b5(void) { behavior_jmp(0xB306, 0xB5D7, BANK_0B); }  /* $72 bonus drum */
void func_B309_b5(void) { behavior_jmp(0xB309, 0xB330, BANK_0B); }  /* $73 none */
void func_B30C_b5(void) { behavior_jmp(0xB30C, 0xB696, BANK_0B); }  /* $74 Belger */
void func_B30F_b5(void) { behavior_jmp(0xB30F, 0xB330, BANK_0B); }  /* $75 none */
void func_B312_b5(void) { behavior_jmp(0xB312, 0xB330, BANK_0B); }  /* $76 none */
void func_B315_b5(void) { behavior_jmp(0xB315, 0xB330, BANK_0B); }  /* $77 none */
void func_B318_b5(void) { behavior_jmp(0xB318, 0xB330, BANK_0B); }  /* $78 none */
void func_B31B_b5(void) { behavior_jmp(0xB31B, 0xB330, BANK_0B); }  /* $79 none */
void func_B31E_b5(void) { behavior_jmp(0xB31E, 0xB330, BANK_0B); }  /* $7A none */
void func_B321_b5(void) { behavior_jmp(0xB321, 0xBD7F, BANK_0B); }  /* $7B closing scene */
void func_B324_b5(void) { behavior_jmp(0xB324, 0xB330, BANK_0B); }  /* $7C none */
void func_B327_b5(void) { behavior_jmp(0xB327, 0xBD7F, BANK_0B); }  /* $7D closing scene */
void func_B32A_b5(void) { behavior_jmp(0xB32A, 0xB330, BANK_0B); }  /* $7E none */
void func_B32D_b5(void) { behavior_jmp(0xB32D, 0xB330, BANK_0B); }  /* $7F none */
void func_B330_b5(void) { behavior_none(0xB330); }
