/*
 * enemy.h - what the boss and stage-object files share (PLAN_DECOMP.md P4):
 * the object behaviours of ids $90-$9F (8 KiB bank $03) and $70-$7F (bank
 * $0B). objects_update maps the id's bank at $A000 and enters through the
 * sixteen JMPs at $B300 + 3 * (id & $0F) (docs/objects.md, "Update").
 *
 * The scaffold names these routines after its 16 KiB banks: func_XXXX_b1 is
 * bank $03 ($02/$03 as one 16 KiB bank) and func_XXXX_b5 bank $0B
 * ($0A/$0B), both at $A000-$BFFF. The two banks hold different code at the
 * same addresses (03:B331 and 0B:B331 have nothing in common).
 *
 *   id    bank:routine  file              what
 *   $70   0B:B331       enemy_drums.c     bonus stage: its controller (slot $0F)
 *   $71   0B:B331       enemy_drums.c     (same routine; no code makes the id)
 *   $72   0B:B5D7       enemy_drums.c     bonus stage: a drum
 *   $74   0B:B696       boss_belger.c     Belger, round 5's boss
 *   $7B   0B:BD7F       boss_belger_end.c after Belger: the closing scene
 *   $7D   0B:BD7F       boss_belger_end.c (same routine; $9B becomes it)
 *   $90   03:B3F4       boss_abigail.c    Abigail, round 3's boss
 *   $91   03:BB91       enemy_drums.c     a rolling drum (round 1, second area)
 *   $94   03:BBCB       enemy_drums.c     the drum dispenser (invisible)
 *   $95   03:BD46       enemy_drums.c     a smashed drum: debris and the item
 *   $9B   03:BF38       boss_belger_end.c Belger down: waits, becomes $7D
 *   $9C   03:BFBE       boss_belger_end.c closing scene
 *   $9D   03:BF95       boss_belger.c     Belger's shot
 *   $9E   03:BF48       boss_belger_end.c closing scene
 *   $9F   03:BED2       boss_belger.c     Belger killed (0C:8211)
 *   the other ids of both tables JMP to an RTS (enemy_tables.c).
 *
 * The routines started as port/tools/lift.py output and keep its contract
 * (mff.h, obj_defs.h): A/X/Y and the flags live in the locals of REGS_IN
 * and meet g_cpu at every call and exit, and each path charges the cycles
 * the 6502 spent, with mff_cycles, before every call and ROM read it made.
 * A flag is computed only where a call or an exit can read it
 * (port/tools/exit_use.py). "Never executed" means no route and no CDL run
 * reached the path (the P0 map, runs/cdl); "no route" that only the nine
 * routes of port/tests/routes miss it.
 */
#ifndef MFF_ENEMY_H
#define MFF_ENEMY_H

#include "obj_defs.h"

/* the scaffold's 16 KiB bank numbers of the two behaviour banks */
#define BANK_03     1
#define BANK_0B     5

/* ---- calls and jumps inside a behaviour bank ----
 * Through the dispatcher, as the lifted code made them: a JSR pushes
 * site + 2 (in the $A000 window) and dispatches; a JMP charges 3 cycles,
 * 2 more at the target, and leaves. */
#define CALL_IN(site, target, bank) do { \
        SYNC_OUT; \
        if (!mff_call_at((uint16_t)(((site) + 2) & 0x1FFF), target, bank)) return; \
        SYNC_IN; \
    } while (0)
#define JMP_IN(pc, target, bank) do { \
        mff_cycles(pc, 3); \
        SYNC_OUT; \
        nes_cpu_instruction_boundary(target, 2); \
        call_by_address_tail(target, bank); \
        return; \
    } while (0)
/* the same JMPs with their 3 cycles already charged with the code before */
#define GO_IN(target, bank) do { \
        SYNC_OUT; \
        nes_cpu_instruction_boundary(target, 2); \
        call_by_address_tail(target, bank); \
        return; \
    } while (0)
#define GO_FIXED(target, fn) do { \
        SYNC_OUT; \
        nes_cpu_instruction_boundary(target, 2); \
        { uint16_t swb_ = g_code_window_base; g_code_window_base = 0xE000; fn(); g_code_window_base = swb_; } \
        return; \
    } while (0)

/* ---- names ---- */

/* the boss-talk object: the invisible id $6E in slot $0D starts each boss
 * talk (obj_props.c); the bosses stand untouchable while it lives
 * (01:A116, 03:B40E, 0B:B6B8) */
#define OBJ_BOSS_TALK   0x6E
#define BOSS_TALK_ID    (OBJ_ID + 0x0D)

/* obj_kind ($05B0) of the bosses here (docs/objects.md) */
#define KIND_ABIGAIL    0x09
#define KIND_BELGER     0x0B

/* bank $0F's table of obj_dir pad bits by the 16-way direction index of
 * 0F:FB5B (clockwise from up: 0 up, 4 right, 8 down, 12 left) */
#define DIR_BY_INDEX    0xFBF0

/* player_id ($34) */
#define PLAYER_CODY     0x00
#define PLAYER_GUY      0x01

/* obj_dir ($0460), pad bits (DIR_RIGHT, DIR_LEFT in obj_defs.h) */
#define DIR_DOWN        0x04
#define DIR_UP          0x08

/* obj_flags ($0560) bits (docs/objects.md) */
#define OBJF_FACE_RIGHT 0x40    /* the art faces left: mirrored */
#define OBJF_WORLD      0x10    /* obj_x/obj_x_hi are world coordinates */
#define OBJF_HIDDEN     0x04    /* neither drawn nor hit */

/* obj_attack ($0440): bit 7 = out, bits 0-4 the reach box */
#define ATTACK_OUT      0x80

/* the sprite bank registers (chr_banks+2..+5 = MMC3 R2-R5) */
#define CHR_BANK2       (CHR_BANKS + 2)
#define CHR_BANK4       (CHR_BANKS + 4)

/* game_action ($30) values these behaviours test or set (docs/ram_map.md) */
#define ACT_IDLE        0x00
#define ACT_JUMP        0x01
#define ACT_PUNCH       0x03
#define ACT_GRAB        0x04
#define ACT_FALL        0x06
#define ACT_HELD        0x0A    /* the player held by a boss (Abigail's grab) */
#define ACT_SKIP_AREA   0x0E    /* ends the area through the game's own code */
#define ACT_SPECIAL     0x17
#define ACT_LYING       0x1F
#define ACT_GRAPPLE_HIT 0x21
#define ACT_FALL_INVULN 0x25

/* ---- bank-0F routines these files call (beyond obj_defs.h) ---- */
void func_F03F(void);   /* player_face_right: slot 0's obj_flags |= $40 */
void func_F048(void);   /* player_face_left: slot 0's obj_flags &= ~$40 */
void func_F1BF(void);   /* obj_move_down: y += obj_vy */
void func_F1DF(void);   /* obj_move_up: y -= obj_vy */
void func_F34A(void);   /* sound_queue_push (music): A = command */
void func_E954(void);   /* hud_level_gauge */

#endif
