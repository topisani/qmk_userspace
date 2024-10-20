#include <stdint.h>
#include "action.h"
#include "action_layer.h"
#include "action_tapping.h"
#include "action_util.h"
#include "color.h"
#include "debug.h"
#include "keyboard.h"
#include "keycodes.h"
#include "keymap_us.h"
#include "modifiers.h"
#include "process_combo.h"
#include "caps_word.h"
#include "keycodes.h"
#include "quantum.h"
#include "quantum_keycodes.h"
#include "timer.h"
#include "topisani.h"
#include "send_string.h"

#ifdef RGBLIGHT_ENABLE
#    include "rgblight/rgblight.h"

void housekeeping_task_user() {
    if (!is_keyboard_master())
        return;
    uint8_t layer = get_highest_layer(layer_state);
    uint8_t val   = 0xFF;
    HSV     hsv   = {0x4B, 0x1a, val};

    switch (layer) {
        case _OSL:
            break;
        case _NAV:
            hsv = (HSV){0x94, 0xFF, val};
            break;
        case _MOUSE:
            hsv = (HSV){0x64, 0xFD, val};
            break;
        case _SYS:
            hsv = (HSV){0x40, 0xED, val};
            break;
    }
    
    HSV rhsv = hsv;
    
    if (is_caps_word_on()) {
        rhsv = (HSV){0xDC, 0xA0, val};
    }

    rgblight_sethsv_noeeprom(rhsv.h, rhsv.s, rhsv.v);
    // rgblight_sethsv_at(hsv.h, hsv.s, hsv.v, 0);
}

void keyboard_post_init_user(void) {
    rgblight_enable_noeeprom();
    HSV hsv = {0x4B, 0x1a, 0xff};
    rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
    rgblight_sethsv_noeeprom(hsv.h, hsv.s, hsv.v);
    // debug_enable=true;
    // debug_keyboard=true;
}
#else
static layer_state_t layer_state_set_user_rgb(layer_state_t state) {
    return state;
}
#endif

layer_state_t layer_state_set_user(layer_state_t state) {
    uint8_t layer = get_highest_layer(state);

    // Always return to the base layer
    state = 1 << layer;
    return state;
}

int tmux_prefix_mode = 0;

static void send_tmux_prefix(void) {
    if (get_mods() & MOD_MASK_CTRL)
        SEND_STRING(" ");
    else
        SEND_STRING(SS_LCTL(" "));
}

bool tmux_prefix_process(uint16_t keycode, keyrecord_t *record) {
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
            return false;
        }
    }
    return true;
}

/* -------- Number Mode -------- */
static bool     _num_mode_active = false;
static uint16_t nav_mode_timer;

// Turn nav mode on. To be called from a custom keycode.
bool num_mode_enable(keyrecord_t *record) {
    if (record->event.pressed) {
        layer_on(_OSL);
        nav_mode_timer = timer_read();
    } else {
        if (_num_mode_active && timer_elapsed(nav_mode_timer) < TAPPING_TERM * 2) {
            _num_mode_active = false;
        } else if (timer_elapsed(nav_mode_timer) < TAPPING_TERM) {
            // Tapping enables layer mode
            _num_mode_active = true;
        } else {
            // Holding treats as a normal LT
            layer_off(_OSL);
        }
    }
    return false;
}

// Turn number mode off.
void num_mode_disable(void) {
    if (_num_mode_active) {
        _num_mode_active = false;
        layer_off(_OSL);
    }
}

bool num_mode_process(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case TKC_NAV:
            num_mode_enable(record);
            return false;
        // Assess if we should exit layermode or continue processing normally.
        case OLALT:
        case OLSFT:
        case OLCTL:
        case OLGUI:
        // case KC_HOME:
        // case KC_END:
        case KC_PAGE_UP:
        case KC_PAGE_DOWN:
        case KC_RIGHT:
        case KC_LEFT:
        case KC_DOWN:
        case KC_UP:
            break;
        default:
            // All other keys disable the layer mode on keyup.
            if (!record->event.pressed && _num_mode_active) {
                num_mode_disable();
            }
            break;
    }
    return true;
}

// MODLOCK ////////////////////////////////////////////////////////////////////
//
// When MODLOCK is pressed, all currently held modifiers are locked active until
// MODLOCK is released.

static uint8_t  modlock_mask  = 0;
static uint16_t modlock_timer = 0;

static bool modlock_process_record(uint16_t keycode, keyrecord_t *record) {
    add_weak_mods(modlock_mask);
    switch (keycode) {
        case MODLOCK: {
            if (record->event.pressed) {
                // modlock_mask = get_mods();
                add_weak_mods(modlock_mask);
                modlock_timer = timer_read();
            } else {
                del_weak_mods(modlock_mask);
                modlock_mask = 0;
                if (timer_elapsed(modlock_timer) < TAPPING_TERM) {
                    add_oneshot_mods(modlock_mask);
                }
            }
            return false;
        } break;
        case QK_MOD_TAP...QK_MOD_TAP_MAX: {
            if (record->event.pressed) {
                //modlock_mask |= QK_MOD_TAP_GET_MODS() ;
            }
        } break;
    }
    return true;
}

// CALLBACKS //////////////////////////////////////////////////////////////////

void post_process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case OLGUI: {
            // clear oneshot layer when gui mod is pressed
            if (record->event.pressed && get_oneshot_layer() != 0) {
                clear_oneshot_layer_state(ONESHOT_OTHER_KEY_PRESSED);
            }
        } break;
    }
}

// const key_override_t key_override_par =
//     ko_make_basic(MOD_MASK_SHIFT, KC_LPRN, KC_RPRN);  // Shift . is ?
// const key_override_t key_override_brc =
//     ko_make_basic(MOD_MASK_SHIFT, KC_LBRC, KC_RBRC); // Shift , is !
// const key_override_t key_override_cbr =
//     ko_make_basic(MOD_MASK_SHIFT, KC_LCBR, KC_RCBR);  // Shift - is =

// const key_override_t** key_overrides = (const key_override_t*[]){
//     &key_override_par,
//     &key_override_brc,
//     &key_override_cbr,
//     NULL
// };

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // uint8_t mod_state = get_mods();
    if (!num_mode_process(keycode, record)) return false;
    if (!tmux_prefix_process(keycode, record)) return false;
    if (!modlock_process_record(keycode, record)) return false;
    switch (keycode) {
        case PANIC: {
            clear_oneshot_mods();
            clear_mods();
            if (get_oneshot_layer() != 0) {
                clear_oneshot_layer_state(ONESHOT_OTHER_KEY_PRESSED);
            }
            layer_move(0);
            caps_word_off();
            swap_hands_off();
            return false;
        }
        case KC_BSPC: {
            // shift+backspace = delete
            // static bool delkey_registered;
            // if (record->event.pressed) {
            //     if (mod_state & MOD_MASK_SHIFT) {
            //         del_mods(MOD_MASK_SHIFT);
            //         register_code(KC_DEL);
            //         delkey_registered = true;
            //         set_mods(mod_state);
            //         return false;
            //     }
            // } else {
            //     if (delkey_registered) {
            //         unregister_code(KC_DEL);
            //         delkey_registered = false;
            //         return false;
            //     }
            // }
            // Let QMK process the KC_BSPC keycode as usual outside of shift
            return true;
        }
    }
    return true;
}

bool caps_word_press_user(uint16_t keycode) {
    switch (keycode) {
        // Keycodes that continue Caps Word, with shift applied.
        case KC_A ... KC_Z:
            add_weak_mods(MOD_BIT(KC_LSFT)); // Apply shift to next key.
            return true;

        // Keycodes that continue Caps Word, without shifting.
        case KC_1 ... KC_0:
        case KC_BSPC:
        case KC_DEL:
        case KC_UNDS:
            return true;

        case KC_MINS:
        default:
            return false; // Deactivate Caps Word.
    }
}

bool get_custom_auto_shifted_key(uint16_t keycode, keyrecord_t *record) {
    if (IS_QK_MOD_TAP(keycode)) {
        keycode = QK_MOD_TAP_GET_TAP_KEYCODE(keycode);
    }
    switch (keycode) {
        case DK_AA:
        case DK_AE:
        case DK_OE:
        case KC_A ... KC_Z:
        case KC_QUOTE:
        case KC_DOT:
        case KC_COMMA:
        case KC_SLASH:
        case KC_1 ... KC_0:
            return true;
        default:
            return false;
    }
}
