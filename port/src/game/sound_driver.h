/*
 * sound_driver.h - names for the Capcom sound driver in PRG banks $06/$07
 * (the port's 16 KiB bank 3; docs/sound.md, port/tools/sound_model.py)
 *
 * sound_update (0F:FF72) maps banks $06 at $8000 and $07 at $A000, calls
 * sound_play ($8003) with each queued command and sound_tick ($8000) once
 * a frame. The routines are in sound_driver.c (entry points, helpers,
 * commands, the frame tick), sound_effect.c (the effect sequencer),
 * sound_music.c (the music sequencer), sound_voice.c (channel state:
 * key on/off, pitch, instrument, volume) and sound_output.c (envelope and
 * register output).
 *
 * Each routine keeps the 6502's registers in the lifted locals a/x/y and
 * the flags in n/z/c/v (mff.h, REGS_IN), so that every call and exit sees
 * exactly what the original leaves there.
 */
#ifndef SOUND_DRIVER_H
#define SOUND_DRIVER_H

#include "mff.h"
#include "state.h"

/* ---- 6502 flag helpers for the lifted locals (n, z, c set as the 6502 does) */
#define NZ(v_)       (n = (uint8_t)(v_) >> 7, z = (uint8_t)(v_) == 0)
#define CMP(r_, m_)  (c = (uint8_t)(r_) >= (uint8_t)(m_), NZ((uint8_t)((r_) - (m_))))

/* ---- channels
 * X: 3 pulse 1 ($4000), 2 pulse 2 ($4004), 1 triangle ($4008), 0 noise
 * ($400C); the register block is (3 - X) * 4. The effect uses X = 0-3, the
 * music X = $28 + channel, so the same arrays (SND_INSTRUMENT ... ) serve
 * both: the music's copies sit $28 bytes higher. */
#define SND_X_NOISE       0x00
#define SND_X_TRIANGLE    0x01
#define SND_X_PULSE2      0x02
#define SND_X_PULSE1      0x03
#define SND_X_MUSIC       0x28   /* ORA #$28 (06:8393) */
#define SND_REG_VOLUME    0x00   /* snd_reg_write's Y: register within the block */
#define SND_REG_PERIOD_LO 0x02
#define SND_REG_PERIOD_HI 0x03
#define SND_TRIANGLE_REGS 0x08   /* (3 - 1) * 4: the triangle's block */
#define SND_CONST_VOLUME  0x30   /* $4000/$4004/$400C bits 4-5: constant volume, no length counter */
#define SND_VOLUME_SILENT 0x30   /* the same at volume 0: silent */
#define SND_SWEEP_OFF     0x08   /* $4001/$4005: sweep off, negate set */
#define SND_CHANNELS_ON   0x0F   /* $4015: pulse 1, pulse 2, triangle, noise */
#define SND_PERIOD_HI_LEN 0x08   /* ORed into $4003/$4007/$400B: length index 1 */

/* The music sequencer's arrays, reached as base + X with X = $28 + channel:
 * state.h names them at the channel's own address ($0750 + channel ...). */
#define SND_SEQ_PTR_LO     (SND_MUSIC_PTR_LO - SND_X_MUSIC)      /* $0728,X */
#define SND_SEQ_PTR_HI     (SND_MUSIC_PTR_HI - SND_X_MUSIC)      /* $072C,X */
#define SND_SEQ_FLAGS      (SND_MUSIC_FLAGS - SND_X_MUSIC)       /* $0730,X */
#define SND_SEQ_TRANSPOSE  (SND_MUSIC_TRANSPOSE - SND_X_MUSIC)   /* $0734,X */
#define SND_SEQ_TICKS      (SND_MUSIC_TICKS - SND_X_MUSIC)       /* $0738,X */
#define SND_SEQ_GATE_RATIO (SND_MUSIC_GATE_RATIO - SND_X_MUSIC)  /* $073C,X */
#define SND_SEQ_GATE       (SND_MUSIC_GATE - SND_X_MUSIC)        /* $0740,X */
#define SND_SEQ_LOOPS      (SND_MUSIC_LOOPS - SND_X_MUSIC)       /* $0744,Y, Y = X + 4 * slot */
#define SND_MUSIC_RAM_SIZE 0x54  /* $0728-$077B, cleared by a music start */
#define SND_EFFECT_RAM_SIZE 0x28 /* $0700-$0727, cleared by an effect start */

/* scratch bytes, by the use each routine makes of them */
#define SND_PTR            SND_TMP          /* $C1/$C2: data pointer (snd_read) */
#define SND_PRODUCT_HI     SND_TMP          /* $C1: multiplicand in, product high out */
#define SND_PRODUCT_LO     (SND_TMP + 1)    /* $C2: product low */
#define SND_ARG            (SND_TMP + 2)    /* $C3: a command's argument */
#define SND_MULTIPLIER     (SND_TMP + 3)    /* $C4: snd_mul8's multiplier */
#define SND_REG_OFFSET     (SND_TMP + 3)    /* $C4: snd_reg_write's register */

/* ---- $C0 snd_flags */
#define SND_FLAG_BUSY     0x01  /* inside sound_play; a new effect row this frame */
#define SND_FLAG_PAUSED   0x02  /* music paused ($F3, $F4) */

/* $CF snd_effect_channels: bit 0 pulse 1 ... bit 3 noise; during the
 * channel loop of sound_tick, bit 7 = the effect owns the current channel */
#define SND_OWNED         0x80
/* $CC snd_fade_speed */
#define SND_FADE_IN       0x80

/* ---- $0704,X snd_env */
#define SND_ENV_PHASE     0x07  /* 0 attack, 1 decay, 2 sustain, 3 release, 4 off */
#define SND_PHASE_ATTACK  0
#define SND_PHASE_DECAY   1
#define SND_PHASE_SUSTAIN 2
#define SND_PHASE_RELEASE 3
#define SND_PHASE_OFF     4
#define SND_ENV_NEW_INSTR 0x08  /* the instrument just changed: reset the LFO */
#define SND_ENV_SLIDING   0x20
#define SND_ENV_LFO_ODD   0x40  /* LFO quarters 1 and 3: the wave runs down */
#define SND_ENV_LFO_NEG   0x80  /* LFO quarters 2 and 3: the vibrato subtracts */
#define SND_ENV_LFO_STEP  0x40  /* one LFO quarter */

/* ---- $0730,X music flags (SND_SEQ_FLAGS) */
#define SND_MF_OCTAVE     0x07
#define SND_MF_OCTAVE_UP2 0x08
#define SND_MF_DOTTED     0x10
#define SND_MF_TRIPLET    0x20
#define SND_MF_TIE        0x40
#define SND_MF_TIED       0x80  /* the note being read is tied to the previous one */
#define SND_MF_SET_KEEP   0x97  /* command $04 ANDs the flags with this */

/* ---- commands (sound_play's A) */
#define SND_CMD_SPECIAL      0xF0   /* $F0-$FF: A & 7 through snd_special's table */
#define SND_CMD_STOP_ALL     0xF0
#define SND_CMD_STOP_EFFECT  0xF1
#define SND_CMD_STOP_MUSIC   0xF2
#define SND_CMD_PAUSE        0xF3
#define SND_CMD_RESUME       0xF4
#define SND_CMD_FADE_IN      0xF5   /* $F5-$F7 run in the interpreter: no */
#define SND_CMD_FADE_OUT     0xF6   /* lifted routine (never used) */
#define SND_CMD_PITCH_SCALE  0xF7
#define SND_EFFECT_LOOPS     0x80   /* an effect's first byte: bit 7 loop, bits 0-6 priority */

/* ---- music stream opcodes ($00-$18; notes $20-$FF) */
#define SND_OP_TRIPLET     0x00
#define SND_OP_TIE         0x01
#define SND_OP_DOT         0x02
#define SND_OP_OCTAVE_UP2  0x03
#define SND_OP_SET_FLAGS   0x04   /* the first one with an argument */
#define SND_OP_TEMPO       0x05
#define SND_OP_GATE        0x06
#define SND_OP_VOLUME      0x07
#define SND_OP_INSTRUMENT  0x08
#define SND_OP_OCTAVE      0x09
#define SND_OP_TRANSPOSE   0x0A
#define SND_OP_CH_TRANSPOSE 0x0B
#define SND_OP_DETUNE      0x0C
#define SND_OP_PORTAMENTO  0x0D
#define SND_OP_LOOP0       0x0E   /* $0E-$11: loop slot 0-3 */
#define SND_OP_BREAK0      0x12   /* $12-$15: break out of loop slot 0-3 */
#define SND_OP_JUMP        0x16
#define SND_OP_END         0x17
#define SND_OP_DUTY        0x18
#define SND_NOTE           0x20   /* $20-$FF: bits 5-7 length code, 0-4 pitch (0 = rest) */
#define SND_NOTE_PITCH     0x1F

/* ---- effect rows */
#define SND_ROW_END        0x80   /* header bit 7 */
#define SND_ROW_LOOP       0x01   /* header bit 0: cc hh ll follow */
#define SND_ROW_GATE       0x02   /* header bit 1: gg follows */
#define SND_ROW_TRANSPOSE  0x04   /* header bit 2: tt follows */
#define SND_NOTE_HOLD      0x80   /* an effect note with bit 7: keep the pitch */
#define SND_SLIDE_UP_TARGET   0x54  /* note 83 + 1: where a rising effect slide ends */
#define SND_SLIDE_DOWN_TARGET 0x0A  /* note 9 + 1: where a falling one ends */

/* ---- tables in bank $06 */
#define SND_TBL_CMD_COUNT    0x8A40  /* entries in the command table ($36) */
#define SND_TBL_INSTR_HI     0x8A41  /* the instrument table's address, high */
#define SND_TBL_INSTR_LO     0x8A42  /*   and low ($8AAF; 8 bytes each) */
#define SND_TBL_CMD_PTR_HI   0x8A43  /* + 2 * cmd: big-endian data pointers */
#define SND_TBL_CMD_PTR_LO   0x8A44
#define SND_TBL_LEN_TRIPLET  0x8915  /* + code - 1: 2, 4, ... 128 ticks */
#define SND_TBL_LEN_NORMAL   0x891C  /* + code - 1: 3, 6, ... 192 ticks */
#define SND_TBL_OCTAVE_BASE  0x8923  /* + (flags & $0F): first note of the octave */
#define SND_TBL_RATE         0x8933  /* envelope steps, 32 entries */
#define SND_TBL_OCT_THRESH   0x8953  /* pseudo-float octave thresholds 0, 7, ... */
#define SND_TBL_PERIOD_LO    0x8959  /* + 2 * (note + 1): pitch, low */
#define SND_TBL_PERIOD_HI    0x895A  /*   and high byte */

/* the jump tables snd_jump_inline reads after its JSR (for reference) */
#define SND_TBL_SPECIAL      0x81B5  /* $F0-$F7 */
#define SND_TBL_FIELDS       0x8329  /* effect channel fields 0-4 */
#define SND_TBL_MUSIC_CMD    0x84A7  /* music opcodes $00-$18 */
#define SND_TBL_ENVELOPE     0x86C7  /* envelope phases 0-4 */

/* instrument bytes ((snd_instr_ptr),Y) */
#define SND_INS_ATTACK    0
#define SND_INS_DECAY     1
#define SND_INS_SUSTAIN   2
#define SND_INS_RELEASE   3
#define SND_INS_LFO       4   /* bits 0-6 speed, bit 7 reset at every note */
#define SND_INS_VIBRATO   5
#define SND_INS_TREMOLO   6
#define SND_INS_NOISE     7   /* ORed into $400E */

/* routines another one runs into (the 6502 falls through or branches
 * there; the lifted copies of their code became C calls) */
void func_81C8_b3(void);
void func_81D4_b3(void);
void func_81F1_b3(void);
void func_8326_b3(void);
void func_83CD_b3(void);
void func_847E_b3(void);
void func_8491_b3(void);
void func_855A_b3(void);
void func_8636_b3(void);
void func_8644_b3(void);
void func_8684_b3(void);

#endif
