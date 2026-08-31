#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include "components/font.h"
#include "scene/boot_splash.h"
#include "scene/superscene.h"
#include "constants.h"
#include "utils.h"

// How long the splash stays up before continuing on its own, in frames (~60fps)
#define BOOT_SPLASH_DURATION_FRAMES 90

typedef struct boot_splash_state {
    wchar_t *text;
    void (*on_continue)(struct state *state);
    u32 frame_count;
} boot_splash_state_t;

static boot_splash_state_t splash_state;

static void proceed(struct state *state)
{
    void (*on_continue)(struct state *state) = splash_state.on_continue;

    free(splash_state.text);
    splash_state.text = NULL;
    splash_state.on_continue = NULL;
    splash_state.frame_count = 0;

    superscene_pop_scene();

    if (on_continue)
        on_continue(state);
}

static void scene_input_handler_boot_splash(struct state *state, int input)
{
    // Any button press skips the splash immediately
    if (input)
        proceed(state);
}

static void scene_paint_handler_boot_splash(struct state *state)
{
    float y = (state->gs->Height / 2.0) - (font_text_block_height(splash_state.text) / 2.0);
    font_print_centered(state->gs, y, 1.0, FG, splash_state.text);

    splash_state.frame_count++;
    if (splash_state.frame_count >= BOOT_SPLASH_DURATION_FRAMES)
    {
        proceed(state);
        return;
    }

    state->repaint = true;
}

void scene_switch_to_boot_splash(struct state *state, const wchar_t *subtitle, void (*on_continue)(struct state *state))
{
    wchar_t buf[128];
    snwprintf(buf, 128, L"fruitchip\n\n%ls", subtitle);

    splash_state.text = wcsdup(buf);
    splash_state.on_continue = on_continue;
    splash_state.frame_count = 0;

    scene_t scene;
    scene_init(&scene);
    scene.input_handler = scene_input_handler_boot_splash;
    scene.paint_handler = scene_paint_handler_boot_splash;
    superscene_push_scene(scene);
    state->repaint = true;
}
