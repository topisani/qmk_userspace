#include "action.h"
#include "action_layer.h"
#include "action_util.h"
#include "keycodes.h"
#include "keymap_us.h"
#include "modifiers.h"
#include "process_combo.h"
#include "caps_word.h"
#include "keycodes.h"
#include "send_string.h"

layer_state_t layer_state_set_user(layer_state_t state) {
    uint8_t layer = get_highest_layer(state);

    // Always return to the base layer
    state = 1 << layer;
    return state;
}

void oneshot_layer_changed_user(uint8_t layer) {
    if (layer == _OSL) {
        // entering the osl should always exit other layers
        layer_move(_OSL);
    }
}

int tmux_prefix_mode = 0;

static void send_tmux_prefix(void) {
    SEND_STRING(SS_LCTL(" "));
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    uint8_t mod_state = get_mods();
    if (tmux_prefix_mode && record->event.pressed) {
        send_tmux_prefix();
        tmux_prefix_mode = 2;
    }
    switch (keycode) {
        case TMUX: {
            if (record->event.pressed) {
                tmux_prefix_mode = 1;
            } else {
                if (tmux_prefix_mode == 1) {
                    send_tmux_prefix();
                }
                tmux_prefix_mode = 0;
            } 
        }
        case KC_BSPC: {
            // shift+backspace = delete
            static bool delkey_registered;
            if (record->event.pressed) {
                if (mod_state & MOD_MASK_SHIFT) {
                    del_mods(MOD_MASK_SHIFT);
                    register_code(KC_DEL);
                    delkey_registered = true;
                    set_mods(mod_state);
                    return false;
                }
            } else {
                if (delkey_registered) {
                    unregister_code(KC_DEL);
                    delkey_registered = false;
                    return false;
                }
            }
            // Let QMK process the KC_BSPC keycode as usual outside of shift
            return true;
        }
    }
    return true;
}
