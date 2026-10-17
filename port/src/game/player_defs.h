/*
 * player_defs.h - names and helpers shared by the player files: the player's
 * states in the fixed bank ($D000-$DFFF, player_state_* handlers and their
 * helpers) and its actions in bank $00 (player_action_dispatch).
 *
 * The routines keep the register model of the lifted code (mff.h): A/X/Y and
 * the flags live in locals (REGS_IN) and meet g_cpu at every call and exit,
 * because a P4 replacement leaves registers, flags, RAM, S and cycles exactly
 * as the 6502 code does. The macros spell the 6502 operations that set flags
 * and the call and exit forms the generator emits.
 *
 * Tables indexed by player_id hold three bytes: Cody, Guy, Haggar.
 */
#ifndef PLAYER_DEFS_H
#define PLAYER_DEFS_H

#include "mff.h"
#include "state.h"

/* ---- flags ---- */

/* N and Z of a value, as a load, AND, INC... leaves them */
#define SET_NZ(r) (n = (uint8_t)(r) >> 7, z = (uint8_t)(r) == 0)
/* CMP/CPX/CPY: C = reg >= operand, N and Z of reg - operand */
#define COMPARE(r, val) (m = (uint8_t)(val), c = (r) >= m, SET_NZ((r) - m))
/* ADC and SBC on A (binary mode), with C and V */
#define ADC(val) (m = (uint8_t)(val), w = a + m + c, v = (~(a ^ m) & (a ^ w) & 0x80) != 0, \
                  c = w > 0xFF, a = (uint8_t)w, SET_NZ(a))
#define SBC(val) (m = (uint8_t)(val), w = a - m - !c, v = ((a ^ m) & (a ^ w) & 0x80) != 0, \
                  c = w >= 0, a = (uint8_t)w, SET_NZ(a))

/* ---- calls and exits, as the generator emits them ---- */

/* the return address a JSR at `site` pushes, less the code window */
#define RP(site) ((uint16_t)(((site) + 2) & 0x1FFF))
/* JSR at `site`: registers out, push the return address, run the callee
 * (dispatched by address with `bank`, or a C function run in the $E000
 * window), and leave at once if S came back unbalanced (the callee left
 * through the scheduler, a task switch unwinding the C frames) */
#define JSR_AT(site, target, bank) \
    do { SYNC_OUT; if (!mff_call_at(RP(site), target, bank)) return; SYNC_IN; } while (0)
#define JSR_FN(site, fn) \
    do { SYNC_OUT; if (!mff_call_fn(RP(site), fn, 0xE000)) return; SYNC_IN; } while (0)
/* RTS: registers out, the caller's return address popped into g_rts_target */
#define RTS() do { SYNC_OUT; mff_rts(); return; } while (0)
/* JMP to another routine: the target is charged 2 cycles, then dispatched
 * (bank -1 = as mapped) or run as a C function in the $E000 window */
#define JMP_TAIL(target, bank) \
    do { SYNC_OUT; nes_cpu_instruction_boundary(target, 2); call_by_address_tail(target, bank); return; } while (0)
#define JMP_FN(target, fn) \
    do { SYNC_OUT; nes_cpu_instruction_boundary(target, 2); \
         { uint16_t swb_ = g_code_window_base; g_code_window_base = 0xE000; fn(); g_code_window_base = swb_; } \
         return; } while (0)
/* JMP (zp): the target read from zero page, charged 2 cycles, dispatched */
#define JMP_IND(zp) \
    do { uint16_t jt_; SYNC_OUT; jt_ = nes_read16zp(zp); nes_cpu_instruction_boundary(jt_, 2); \
         call_by_address_tail(jt_, -1); return; } while (0)
/* a branch to another routine's entry: dispatched with no extra charge */
#define BRANCH_TAIL(target, bank) \
    do { SYNC_OUT; call_by_address_tail((uint16_t)(g_code_window_base + ((target) & 0x1FFF)), bank); return; } while (0)
/* JSR into task_wait_frame or task_wait_frames: the scheduler's resume
 * changes S across the yield, so (as the generator does) no balance check */
#define JSR_YIELD(site, fn) \
    do { SYNC_OUT; mff_call_fn(RP(site), fn, 0xE000); SYNC_IN; } while (0)
/* the 6502 code runs on into the entry of another routine: the same code,
 * reached as a C call (no JSR, nothing pushed) */
#define FALL_INTO(fn) do { SYNC_OUT; fn(); return; } while (0)

/* ---- characters (player_id) ---- */
#define PLAYER_CODY   0
#define PLAYER_GUY    1
#define PLAYER_HAGGAR 2

/* ---- joypad bits (joy_pressed $14, joy_held $16) ---- */
#define PAD_A      0x80
#define PAD_B      0x40
#define PAD_SELECT 0x20
#define PAD_START  0x10
#define PAD_UP     0x08
#define PAD_DOWN   0x04
#define PAD_LEFT   0x02
#define PAD_RIGHT  0x01

/* ---- game_action ($30): the player_state handler that runs ---- */
#define ACT_IDLE       0x00   /* player_state_idle */
#define ACT_JUMP       0x01   /* player_state_jump */
#define ACT_LAND       0x02   /* player_state_land: a few frames after a jump */
#define ACT_ATTACK     0x03   /* player_state_punch */
#define ACT_GRAB       0x04   /* player_state_grab */
#define ACT_HIT        0x05   /* player_state_hit */
#define ACT_KNOCKDOWN  0x06   /* player_state_knockdown */
#define ACT_SELECT     0x07   /* player_state_transition: continued, to the select screen */
#define ACT_CONTINUE   0x09   /* player_state_continue: the continue screen */
#define ACT_HOLD       0x0A   /* player_state_0A: a boss in bank $03 holds the player */
#define ACT_THROW      0x0B   /* player_state_throw: Cody's throw, the end of the others,
                                and the death blink */
#define ACT_HITPOS     0x0C   /* player_state_hitpos: after a combo hit */
#define ACT_NEXT_AREA  0x0E   /* player_state_transition: the area is over */
#define ACT_EARTHQUAKE 0x0F   /* the HeadSmasher has landed: the screen shakes */
/* from here on handled in bank $00 (player_action_dispatch); names from Data
 * Crystal where the code agrees */
#define ACT_HEADSMASHER   0x12   /* Haggar: A while holding an enemy */
#define ACT_GUY_THROW     0x13   /* Guy: B while holding an enemy */
#define ACT_CODY_SLASH    0x14   /* Cody's combo finisher */
#define ACT_HAGGAR_THROW  0x15   /* Haggar's back throw: B while holding */
#define ACT_LARIAT        0x16   /* Haggar's moving special (action_lariat, $86FF) */
#define ACT_SPECIAL       0x17   /* the A+B special (Data Crystal: special power) */
#define ACT_LYING         0x1F   /* down after a knockdown, getting up */
#define ACT_GRAB_KNEE     0x21   /* the knee: B with no direction while holding */
#define ACT_PIT_FALL      0x24   /* falling into a pit (player_pit_fall, $CFD1) */
#define ACT_DROP_IN       0x25   /* a new life drops in */

/* ---- background probe ($10 after obj_fall/obj_move_*) ---- */
#define PROBE_PIT      0x40   /* attribute of a pit tile (and up) */

/* ---- sound effects (sound_queue_push_sfx, $F34C) ---- */
#define SFX_SWING      0x17   /* an attack goes out (attack_swing_sound, $F430) */
#define SFX_JUMP       0x1B
#define SFX_LAND       0x1C   /* a jump lands */
#define SFX_KNOCKDOWN  0x28   /* the player is knocked down */
#define SFX_THROW      0x33

/* ---- obj_state ---- */
#define STATE_LIVE     0x80

/* ---- player_no_hit ---- */
#define NO_HIT         0x80   /* bit 7: enemy attacks are not tested */

/* ---- obj_flags: bit 7 on screen, 6 flipped (facing right: obj_face_right
 * $F090 sets it), 4 world coordinates ---- */
#define FLAGS_FLIP         0x40
#define FLAGS_FACING_LEFT  0x90
#define FLAGS_FACING_RIGHT 0xD0

/* ---- the screen edge a self-scrolling camera keeps the player in ---- */
#define SCREEN_RIGHT_LIMIT 0xF0   /* obj_x - scroll_x */

/* ---- per-character tables in the fixed bank (3 bytes each) ---- */
/* animation ids to set (player_set_anim, $F294) */
#define ANIM_STAND          0xE1E3   /* 01 02 03 (player_stand, $CEAA) */
#define ANIM_WALK           0xE1E6   /* 04 05 06 */
#define ANIM_JUMP           0xE1E9   /* 07 08 09 */
#define ANIM_JUMP_MOVING    0xE1EC   /* 0A 0B 0C: jump with left/right */
#define ANIM_JUMP_KICK      0xE1FE   /* 1C 1D 1E */
#define ANIM_KNOCKED_DOWN   0xE20D   /* 2B 2C 2D */
#define ANIM_GET_UP         0xE210   /* 2E 2F 30 */
#define ANIM_FALL           0xE219   /* 37 38 39: falling (a jump's descent, a pit) */
#define ANIM_JUMP_BACK      0xE21C   /* 3A 3B 3C: moving away from the facing side */
/* CHR banks for the sprite (player_set_chr, $F440) */
#define CHR_STAND           0xE22B   /* 00 04 08: standing and walking */
#define CHR_JUMP            0xE22E   /* 01 05 12 */
#define CHR_JUMP_KICK       0xE243   /* 02 06 13 */
#define CHR_KNOCKED_DOWN    0xE255   /* 0D 15 12 */
#define CHR_GET_UP          0xE258   /* 00 04 0B */
/* walking speed (player_state_idle sets it each frame, 0E:CAA2) */
#define WALK_VX_FRAC        0xE1C1   /* 4C 80 19 */
#define WALK_VX             0xE1C4   /* 01 01 01 */
#define WALK_VY_FRAC        0xE1C7   /* 00 33 CC */
#define WALK_VY             0xE1CA   /* 01 01 00 */
/* jump speed in x when moving against the facing */
#define JUMP_BACK_VX_FRAC   0xE1CD   /* 80 B3 33 */
#define JUMP_BACK_VX        0xE1D0   /* 01 01 01 */
/* animation ids to compare with obj_anim */
#define IS_WALKING          0xE273   /* 04 05 06 */
#define IS_JUMP_KICK_MOVING 0xE276   /* 1F 20 21 */
#define IS_JUMP_KICK        0xE279   /* 1C 1D 1E */
#define IS_JUMP_MOVING      0xE27F   /* 0A 0B 0C */
#define IS_JUMP             0xE282   /* 07 08 09 */
/* the first attack of a combo: what player_stand_anim restarts from, and
 * with $E28B/$E28E the animations in which the punch state turns */
#define COMBO_FIRST_ANIM    0xE288   /* 0D 0E 0F */
#define TURN_ANIM_2         0xE28B   /* 16 17 18 */
#define TURN_ANIM_3         0xE28E   /* 45 46 47 */
/* animations in which A+B does not start the special (Cody has none) */
#define NO_SPECIAL_ANIM_1   0xE29D   /* 00 14 15 */
#define NO_SPECIAL_ANIM_2   0xE2A0   /* 00 5B 15 */
/* game_action of the power move (forward + B from a higher level) */
#define POWER_ACTION        0xFCCE   /* 23 27 10 */

/* ---- routines elsewhere ---- */
void func_F000(void);   /* obj_face_right ($F090), then $F003 */
void func_F051(void);   /* obj_face_left ($F099), then $F054 */
void func_F003(void);   /* steps slot X right by vx and probes the background
                           (Y = probe point); C set = blocked */
void func_F03F(void);   /* player_face_right: obj_flags[0] |= $40 */
void func_F048(void);   /* player_face_left: obj_flags[0] &= ~$40 */
void func_F090(void);   /* obj_face_right: obj_flags[X] |= $40 */
void func_F054(void);   /* steps slot X left, as $F003 */
void func_F099(void);   /* obj_face_left: obj_flags[X] &= ~$40 */
void func_F0A2(void);   /* steps slot X down by vy and probes; C set = blocked */
void func_F0F7(void);   /* steps slot X up, as $F0A2 */
void func_F119(void);   /* obj_fall: slot X falls by obj_vy with gravity; C set
                           on the floor, $10 = the probed tile's attribute */
void func_F183(void);   /* obj_move_right: x += vx (slot X) */
void func_F1A1(void);   /* obj_move_left: x -= vx (slot X) */
void func_F1BF(void);   /* obj_move_down: y += vy (slot X) */
void func_F1DF(void);   /* obj_move_up: y -= vy (slot X) */
void func_F1FD(void);   /* obj_fall: slot X falls by vy with gravity, no floor probe */
void func_F288(void);   /* obj_set_anim: obj_anim[X] = A, frame and timer 0 */
void func_F26E(void);   /* the start of a fall: obj_vy = -$40/256 for the player */
void func_F294(void);   /* player_set_anim: obj_anim[0] = A, frame and timer 0 */
void func_F2A0(void);   /* obj_spawn: slot Y with animation A */
void func_F34A(void);   /* sound_queue_push: queue sound command A (music too) */
void func_F34C(void);   /* sound_queue_push_sfx: queue sound A */
void func_F428(void);   /* attack_start: attack_live $1E, attack_id 0, swing sound */
void func_F430(void);   /* attack_swing_sound: sound $17 */
void func_F436(void);   /* attack_end: attack_live = 0 */
void func_F43B(void);   /* attack_begin: attack_live = $1E */
void func_F440(void);   /* player_set_chr: chr_banks+2 = A (the player's sprite bank) */
void func_FF2A(void);   /* task_wait_frames: X frames */
void func_FF31(void);   /* task_wait_frame */
void func_FF4D(void);   /* bank_switch: $8000/$A000 <- bank_request/+1 */

/* ---- the player's routines (player_*.c) ---- */
void func_D00E(void);
void func_D03F(void);
void func_D18E(void);
void func_D242(void);
void func_D2B7(void);
void func_D30F(void);
void func_D346(void);
void func_D39E(void);
void func_D3B3(void);
void func_D438(void);
void func_D45A(void);
void func_D45E(void);
void func_D477(void);
void func_D47D(void);
void func_D4CF(void);
void func_D4E5(void);
void func_D4E6(void);
void func_D4F2(void);
void func_D50F(void);
void func_D522(void);
void func_D55D(void);
void func_D55E(void);
void func_D567(void);
void func_D570(void);
void func_D58D(void);
void func_D5B7(void);
void func_D5F3(void);
void func_D61F(void);
void func_D641(void);
void func_D6D4(void);
void func_D6DF(void);
void func_D6E0(void);
void func_D70A(void);
void func_D777(void);
void func_D79A(void);
void func_D7A3(void);
void func_D7B8(void);
void func_D7F2(void);
void func_D82E(void);
void func_D83B(void);
void func_D886(void);
void func_D88C(void);
void func_D89A(void);
void func_D8A8(void);
void func_D8B7(void);
void func_D8E5(void);
void func_D93C(void);
void func_D986(void);
void func_D9A3(void);
void func_D9E9(void);
void func_D9FA(void);
void func_DA3F(void);
void func_DB03(void);
void func_DB2C(void);
void func_DB6C(void);
void func_DBD4(void);
void func_DBFD(void);
void func_DC29(void);
void func_DC6C(void);
void func_DCDD(void);
void func_DD6A(void);
void func_DD7F(void);
void func_DD8E(void);
void func_DDB7(void);
void func_DDC1(void);
void func_DDC4(void);
void func_DDD3(void);
void func_DDE2(void);
void func_DDFF(void);
void func_DE0E(void);
void func_DE33(void);
void func_DE5E(void);
void func_DF50(void);
void func_DFE9(void);

/* bank $00 */
void func_8000_b0(void);
void func_8065_b0(void);
void func_8066_b0(void);
void func_807E_b0(void);
void func_8090_b0(void);
void func_8193_b0(void);
void func_821D_b0(void);
void func_82AF_b0(void);
void func_8300_b0(void);
void func_83A1_b0(void);
void func_83DC_b0(void);
void func_8429_b0(void);
void func_8479_b0(void);
void func_8497_b0(void);
void func_849C_b0(void);
void func_8540_b0(void);
void func_8541_b0(void);
void func_85AA_b0(void);
void func_85BF_b0(void);
void func_85F9_b0(void);
void func_85FE_b0(void);
void func_863E_b0(void);
void func_8643_b0(void);
void func_86FF_b0(void);
void func_87AA_b0(void);
void func_87E2_b0(void);
void func_87F7_b0(void);
void func_8832_b0(void);
void func_884E_b0(void);
void func_8864_b0(void);
void func_889F_b0(void);
void func_88AA_b0(void);
void func_8B00_b0(void);
void func_8B15_b0(void);
void func_8B98_b0(void);
void func_8BB1_b0(void);
void func_8C38_b0(void);
void func_8D8D_b0(void);
void func_8DD1_b0(void);
void func_8E3E_b0(void);
void func_8FAF_b0(void);
void func_90AC_b0(void);
void func_90CB_b0(void);
void func_92E1_b0(void);
void func_9297_b0(void);
void func_93EF_b0(void);
void func_9410_b0(void);
void func_943F_b0(void);
void func_945E_b0(void);
void func_94F2_b0(void);
void func_94F7_b0(void);
void func_9565_b0(void);
void func_958B_b0(void);
void func_95BA_b0(void);
void func_95FA_b0(void);
void func_9610_b0(void);
void func_9615_b0(void);
void func_961A_b0(void);
void func_961F_b0(void);
void func_963D_b0(void);
void func_96EF_b0(void);
void func_97A9_b0(void);
void func_97C8_b0(void);
void func_9819_b0(void);
void func_984F_b0(void);
void func_987A_b0(void);

#endif
