#pragma once

#include <wchar.h>

#include "state.h"

// Shows a short branded splash screen ("fruitchip" + subtitle) and then
// calls on_continue once it times out or the user presses a button to skip it.
// Used right before actually handing off execution (e.g. to OSDSYS or a game/app ELF).
void scene_switch_to_boot_splash(struct state *state, const wchar_t *subtitle, void (*on_continue)(struct state *state));
