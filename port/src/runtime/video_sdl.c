/*
 * video_sdl.c - a window to play in (built with MFF_SDL; without it the
 * port runs headless only)
 */
#include "machine.h"
#include <stdio.h>

#ifndef MFF_HAVE_SDL

int video_open(int scale) {
    (void)scale;
    fprintf(stderr, "[mff] this build has no window (configure with -DMFF_SDL=ON)\n");
    return -1;
}
void video_frame(const uint32_t *fb) { (void)fb; }
void video_poll_input(void) {}

#endif
