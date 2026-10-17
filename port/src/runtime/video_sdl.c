/*
 * video_sdl.c - a window to play in (built with -DMFF_SDL=ON; without it
 * the port runs headless only)
 *
 * The picture of each frame is shown as it is made (frame.c), and the
 * game is paced to the NES's frame rate, 60.0988 per second. Pad 1 reads
 * the keyboard and the first game controller:
 *
 *   NES        keyboard              controller
 *   A          X                     B or Y (right, top)
 *   B          Z                     A or X (bottom, left)
 *   Start      Enter                 Start
 *   Select     Right Shift           Back
 *   D-pad      arrows                d-pad or left stick
 *
 * Tab runs as fast as it can while held, F11 or Alt+Enter toggles full
 * screen, Esc quits. A keyed route (MFF_INPUT) or the bot (MFF_BOT) still
 * takes over the pad.
 */
#include "machine.h"
#include <stdio.h>
#include <stdlib.h>

#ifndef MFF_HAVE_SDL

int video_open(int scale) {
    (void)scale;
    fprintf(stderr, "[mff] this build has no window (configure with -DMFF_SDL=ON)\n");
    return -1;
}
void video_frame(const uint32_t *fb) { (void)fb; }
void video_poll_input(void) {}

#else

#define SDL_MAIN_HANDLED
#include <SDL.h>

/* NTSC: 39,375,000 / 655,171 frames a second */
#define FRAME_NUM 655171.0
#define FRAME_DEN 39375000.0

static SDL_Window *s_win;
static SDL_Renderer *s_ren;
static SDL_Texture *s_tex;
static SDL_GameController *s_pad;
static double s_next;                   /* when the next frame is due, in counter ticks */
static int s_fast;

static void quit(void) {
    hooks_exit_log();
    SDL_Quit();
    exit(0);
}

int video_open(int scale) {
    SDL_SetMainReady();
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_TIMER) < 0) {
        fprintf(stderr, "[mff] SDL_Init: %s\n", SDL_GetError());
        return -1;
    }
    if (scale < 1) scale = 1;
    s_win = SDL_CreateWindow("Mighty Final Fight", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                             256 * scale, 240 * scale, SDL_WINDOW_RESIZABLE);
    if (!s_win) {
        fprintf(stderr, "[mff] SDL_CreateWindow: %s\n", SDL_GetError());
        return -1;
    }
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
    s_ren = SDL_CreateRenderer(s_win, -1, SDL_RENDERER_ACCELERATED);
    if (!s_ren) s_ren = SDL_CreateRenderer(s_win, -1, SDL_RENDERER_SOFTWARE);
    if (!s_ren) {
        fprintf(stderr, "[mff] SDL_CreateRenderer: %s\n", SDL_GetError());
        return -1;
    }
    SDL_RenderSetLogicalSize(s_ren, 256, 240);
    s_tex = SDL_CreateTexture(s_ren, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 256, 240);
    if (!s_tex) {
        fprintf(stderr, "[mff] SDL_CreateTexture: %s\n", SDL_GetError());
        return -1;
    }
    for (int i = 0; i < SDL_NumJoysticks() && !s_pad; i++)
        if (SDL_IsGameController(i)) s_pad = SDL_GameControllerOpen(i);
    s_next = (double)SDL_GetPerformanceCounter();
    return 0;
}

static int pressed(SDL_GameControllerButton b) {
    return s_pad && SDL_GameControllerGetButton(s_pad, b);
}

void video_poll_input(void) {
    SDL_Event ev;
    const Uint8 *k;
    uint8_t b = 0;
    int ax = 0, ay = 0;
    while (SDL_PollEvent(&ev)) {
        switch (ev.type) {
        case SDL_QUIT:
            quit();
            break;
        case SDL_KEYDOWN:
            if (ev.key.keysym.sym == SDLK_ESCAPE) quit();
            if (ev.key.keysym.sym == SDLK_TAB) s_fast = 1;
            if (ev.key.keysym.sym == SDLK_F11 ||
                (ev.key.keysym.sym == SDLK_RETURN && (ev.key.keysym.mod & KMOD_ALT))) {
                int full = SDL_GetWindowFlags(s_win) & SDL_WINDOW_FULLSCREEN;
                SDL_SetWindowFullscreen(s_win, full ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP);
            }
            break;
        case SDL_KEYUP:
            if (ev.key.keysym.sym == SDLK_TAB) s_fast = 0;
            break;
        case SDL_CONTROLLERDEVICEADDED:
            if (!s_pad) s_pad = SDL_GameControllerOpen(ev.cdevice.which);
            break;
        case SDL_CONTROLLERDEVICEREMOVED:
            if (s_pad && SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(s_pad)) == ev.cdevice.which) {
                SDL_GameControllerClose(s_pad);
                s_pad = NULL;
            }
            break;
        }
    }
    k = SDL_GetKeyboardState(NULL);
    if (s_pad) {
        ax = SDL_GameControllerGetAxis(s_pad, SDL_CONTROLLER_AXIS_LEFTX);
        ay = SDL_GameControllerGetAxis(s_pad, SDL_CONTROLLER_AXIS_LEFTY);
    }
    if (k[SDL_SCANCODE_X] || pressed(SDL_CONTROLLER_BUTTON_B) || pressed(SDL_CONTROLLER_BUTTON_Y)) b |= 0x80;
    if (k[SDL_SCANCODE_Z] || pressed(SDL_CONTROLLER_BUTTON_A) || pressed(SDL_CONTROLLER_BUTTON_X)) b |= 0x40;
    if (k[SDL_SCANCODE_RSHIFT] || pressed(SDL_CONTROLLER_BUTTON_BACK)) b |= 0x20;
    if ((k[SDL_SCANCODE_RETURN] && !(SDL_GetModState() & KMOD_ALT)) || pressed(SDL_CONTROLLER_BUTTON_START))
        b |= 0x10;
    if (k[SDL_SCANCODE_UP] || pressed(SDL_CONTROLLER_BUTTON_DPAD_UP) || ay < -16000) b |= 0x08;
    if (k[SDL_SCANCODE_DOWN] || pressed(SDL_CONTROLLER_BUTTON_DPAD_DOWN) || ay > 16000) b |= 0x04;
    if (k[SDL_SCANCODE_LEFT] || pressed(SDL_CONTROLLER_BUTTON_DPAD_LEFT) || ax < -16000) b |= 0x02;
    if (k[SDL_SCANCODE_RIGHT] || pressed(SDL_CONTROLLER_BUTTON_DPAD_RIGHT) || ax > 16000) b |= 0x01;
    if ((b & 0x0C) == 0x0C) b &= (uint8_t)~0x0C;    /* a pad cannot press opposite directions */
    if ((b & 0x03) == 0x03) b &= (uint8_t)~0x03;
    g_controller1_buttons = b;
}

void video_frame(const uint32_t *fb) {
    double freq = (double)SDL_GetPerformanceFrequency(), period = freq * FRAME_NUM / FRAME_DEN, now;
    SDL_UpdateTexture(s_tex, NULL, fb, 256 * (int)sizeof(uint32_t));
    SDL_RenderClear(s_ren);
    SDL_RenderCopy(s_ren, s_tex, NULL, NULL);
    SDL_RenderPresent(s_ren);
    s_next += period;
    now = (double)SDL_GetPerformanceCounter();
    if (s_fast || now > s_next + 4 * period) {      /* fast, or fell behind: no catching up */
        s_next = now;
        return;
    }
    while (now < s_next) {
        double ms = (s_next - now) * 1000.0 / freq;
        if (ms > 2.0) SDL_Delay((Uint32)(ms - 1.0));
        now = (double)SDL_GetPerformanceCounter();
    }
}

#endif
