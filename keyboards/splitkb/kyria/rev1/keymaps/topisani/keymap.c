/* Copyright 2019 Thomas Baart <thomas@splitkb.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "action.h"
#include "action_layer.h"
#include "action_util.h"
#include "keycodes.h"
#include "keymap_us.h"
#include "modifiers.h"
#include "process_combo.h"
#include "caps_word.h"
#include "process_tap_dance.h"

#include QMK_KEYBOARD_H

enum layers {
    _COLEMAK_DH = 0,
    _OSL,
    _NAV,
    _MOUSE,
    _FUNCTION,
};

enum tapdance_idxs {
    TD_OSL_PANIC,
};

#define OLALT OSM(MOD_LALT)
#define OLCTL OSM(MOD_LCTL)
#define ORSFT OSM(MOD_RSFT)
#define OLSFT OSM(MOD_LSFT)
#define ORALT OSM(MOD_RALT)
#define OLGUI OSM(MOD_LGUI)
#define L_OSL OSL(_OSL)


#define HG(K) MT(MOD_LGUI, KC_##K)
#define HA(K) MT(MOD_LALT, KC_##K)
#define HS(K) MT(MOD_LSFT, KC_##K)
#define HC(K) MT(MOD_LCTL, KC_##K)
#define TABN C(KC_PGUP)
#define TABP C(KC_PGDN)
#define TMUX C(KC_SPC)

// clang-format off
/*
kakoune code to highlight and format keymap:
addhl -override window/qmk regex _______|\bKC_|XXXXXXX|\.\.\.\.\.\.\. 0:comment
reg @ <percent>s<space>=<space>LAYOUT36<ret>j<a-i>is<space><ret>dXs.<backspace>,<ret>&<gt><gt>i<space><right><esc>i<space><esc><left><left><esc>
*/
#define LAYOUT36(K1, K2, K3, K4, K5, NO5,NO6, K6, K7, K8, K9, K10, K11, K12, K13, K14, K15, NO15,NO16, K16, K17, K18, K19, K20, K21, K22, K23, K24, K25, NO25,NO26, K26, K27, K28, K29, K30, NO1, NO2, NO3, K31, K32, K33, K34, K35, K36, NO4, NO7, NO8) \
    LAYOUT(                                                                                                                        \
     XXXXXXX , K1  ,  K2    ,  K3   ,  K4  , K5  ,                                     K6  , K7  , K8      , K9  , K10 , XXXXXXX , \
     XXXXXXX , K11 ,  K12   ,  K13  ,  K14 , K15 ,                                     K16 , K17 , K18     , K19 , K20 , XXXXXXX , \
     XXXXXXX , K21 ,  K22   ,  K23  ,  K24 , K25 , XXXXXXX ,XXXXXXX,XXXXXXX, XXXXXXX , K26 , K27 , K28     , K29 , K30 , XXXXXXX , \
                            XXXXXXX ,  K31 , K32 ,     K33 ,XXXXXXX,XXXXXXX,     K34 , K35 , K36 , XXXXXXX                         \
    )

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[_COLEMAK_DH] = LAYOUT36(
        KC_Q    , KC_W    , HG(F)   , KC_P    , KC_B    , ....... , ....... , KC_J    , KC_L    , HG(U)   , KC_Y    , KC_BSPC , 
        KC_A    , HA(R)   , HS(S)   , HC(T)   , KC_G    , ....... , ....... , KC_M    , HC(N)   , HS(E)   , HA(I)   , KC_O    , 
        KC_Z    , KC_X    , KC_C    , KC_D    , KC_V    , ....... , ....... , KC_K    , KC_H    , KC_COMM , KC_DOT  , KC_SLSH , 
        ....... , ....... , ....... , XXXXXXX , QK_REP  , KC_SPC  , L_OSL   , QK_AREP , XXXXXXX , ....... , ....... , .......
    ),
	[_OSL] = LAYOUT36(
        KC_GRV  , KC_7    , KC_8    , KC_9    , KC_0    , ....... , ....... , KC_PLUS , KC_LBRC , KC_MINS , KC_RBRC , KC_BSPC , 
        KC_ESC  , KC_4    , KC_5    , KC_6    , XXXXXXX , ....... , ....... , KC_EQL  , KC_COLN , TO(2)   , KC_QUOT , KC_ENT  , 
        KC_TAB  , KC_1    , KC_2    , KC_3    , XXXXXXX , ....... , ....... , KC_GRV  , KC_SCLN , KC_UNDS , KC_BSLS , TMUX    , 
        ....... , ....... , ....... , _______ , _______ , OLSFT   , _______ , XXXXXXX , KC_RCTL , ....... , ....... , .......
    ),
	[_NAV] = LAYOUT36(
        KC_Q    , KC_7    , KC_8    , KC_9    , KC_0    , ....... , ....... , TO(4)   , KC_PGUP , KC_RCTL , KC_PGDN , _______ , 
        OLGUI   , KC_4    , KC_5    , KC_6    , XXXXXXX , ....... , ....... , XXXXXXX , KC_LEFT , KC_UP   , KC_RGHT , TO(3)   , 
        TMUX    , KC_1    , KC_2    , KC_3    , XXXXXXX , ....... , ....... , XXXXXXX , KC_HOME , KC_DOWN , KC_END  , TMUX    , 
        ....... , ....... , ....... , _______ , _______ , TO(0)   , L_OSL   , _______ , _______ , ....... , ....... , .......
    ),
	[_MOUSE] = LAYOUT36(
        XXXXXXX , KC_ACL0 , KC_ACL1 , KC_ACL2 , XXXXXXX , ....... , ....... , TABP    , KC_WH_U , KC_BTN1 , KC_WH_D , TABN    , 
        XXXXXXX , KC_BTN3 , KC_BTN2 , KC_BTN1 , XXXXXXX , ....... , ....... , KC_BTN4 , KC_MS_L , KC_MS_U , KC_MS_R , KC_BTN5 , 
        XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , ....... , ....... , XXXXXXX , KC_WH_L , KC_MS_D , KC_WH_R , XXXXXXX , 
        ....... , ....... , ....... , _______ , _______ , TO(0)   , _______ , _______ , _______ , ....... , ....... , .......
    ),
	[_FUNCTION] = LAYOUT36(
        XXXXXXX , KC_F7   , KC_F8   , KC_F9   , KC_F10  , ....... , ....... , KC_INS  , RGB_VAD , RGB_TOG , RGB_VAI , XXXXXXX , 
        XXXXXXX , KC_F4   , KC_F5   , KC_F6   , KC_F11  , ....... , ....... , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , 
        XXXXXXX , KC_F1   , KC_F2   , KC_F3   , KC_F12  , ....... , ....... , KC_PSCR , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , 
        ....... , ....... , ....... , _______ , _______ , TO(0)   , _______ , _______ , _______ , ....... , ....... , .......
    )
};
// clang-format on

#ifdef OLED_ENABLE
oled_rotation_t oled_init_kb(oled_rotation_t rotation) {
    return OLED_ROTATION_180;
}

char states[20] = "00";

bool oled_task_kb(void) {
    if (!oled_task_user()) {
        return false;
    }
    if (true || is_keyboard_master()) {
        oled_set_cursor((oled_max_chars() - 9) / 2, (oled_max_lines() - 1) / 2);
        switch (get_highest_layer(layer_state | default_layer_state)) {
            case 0:
                oled_write_P(PSTR(" COLEMAK "), false);
                break;
            case 1:
                oled_write_P(PSTR("   OSL   "), false);
                break;
            case 2:
                oled_write_P(PSTR("   NAV   "), false);
                break;
            case 3:
                oled_write_P(PSTR("  MOUSE  "), false);
                break;
            case 4:
                oled_write_P(PSTR("   SYS   "), false);
                break;
            case 5:
                oled_write_P(PSTR("    5    "), false);
                break;
            case 6:
                oled_write_P(PSTR("    6    "), false);
                break;
            default:
                oled_write_P(PSTR("Undefined"), false);
        }

        oled_set_cursor((oled_max_chars() - 21) / 2, (oled_max_lines()) / 2);
        // Host Keyboard LED Status
        led_t led_usb_state = host_keyboard_led_state();
        oled_write_P(led_usb_state.num_lock ? PSTR("NUMLCK ") : PSTR("       "), false);
        oled_write_P(led_usb_state.caps_lock ? PSTR("CAPLCK ") : PSTR("       "), false);
        oled_write_P(led_usb_state.scroll_lock ? PSTR("SCRLCK ") : PSTR("       "), false);
        oled_write_ln("", false);

        char    modstr[] = "     ";
        uint8_t mods     = get_mods() | get_oneshot_mods();
        if (mods & MOD_MASK_CTRL) modstr[0] = 'C';
        if (mods & MOD_MASK_SHIFT) modstr[1] = 'S';
        if (mods & MOD_MASK_ALT) modstr[2] = 'A';
        if (mods & MOD_MASK_GUI) modstr[3] = 'G';
        if (is_caps_word_on()) modstr[4] = 'W';
        oled_set_cursor((oled_max_chars() - strlen(modstr)) / 2, (oled_max_lines()) / 2 + 1);
        oled_write(modstr, false);
        // oled_write_ln(states, false);
    } else {
    }
    return false;
}
#endif

layer_state_t layer_state_set_user(layer_state_t state) {
    uint8_t layer = get_highest_layer(state);

    // if (layer == 0) {
    //     del_oneshot_mods(MOD_MASK_SHIFT);
    //     del_oneshot_locked_mods(MOD_MASK_SHIFT);
    // }
    
    uint8_t val = rgblight_get_val();
    HSV hsv = {0x4B, 0x1a, val};

    
    switch (layer) {
        case 1:
            hsv = (HSV){0xDC, 0xA0, val};
            break;
        case 2:
            hsv = (HSV){0x94, 0xDD, val};
            break;
        case 3:
            hsv = (HSV){0x64, 0xFD, val};
            break;
        case 4:
            hsv = (HSV){0x40, 0xED, val};
            break;
    }
    rgblight_sethsv_noeeprom(hsv.h, hsv.s, hsv.v);
    return state;
}

void keyboard_post_init_user(void) {
    rgblight_enable_noeeprom();
    HSV hsv = {0x4B, 0x1a, 0xff};
    rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
    rgblight_sethsv_noeeprom(hsv.h, hsv.s, hsv.v);
}

void oneshot_layer_changed_user(uint8_t layer) {
    if (layer == 1) {
        // entering the osl should always exit other layers
        layer_move(1);
    }
}
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    uint8_t mod_state = get_mods();
    switch (keycode) {

    case KC_BSPC:
        {
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

// Tap Dance

void td_osl_panic(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        layer_move(_OSL);
        set_oneshot_layer(_OSL, ONESHOT_START);
    }
    if (state->count >= 2) {
        layer_move(0);
        reset_tap_dance(state);
    }
}

tap_dance_action_t tap_dance_actions[] = {
    [TD_OSL_PANIC] = ACTION_TAP_DANCE_FN(td_osl_panic),
};

// Combos 
const uint16_t PROGMEM combo_esc[] = {KC_A, KC_R, COMBO_END};
const uint16_t PROGMEM combo_dash[] = {KC_H, KC_COMMA, COMBO_END};
const uint16_t PROGMEM combo_underscore[] = {KC_H, KC_DOT, COMBO_END};

const uint16_t PROGMEM combo_pl[] = {KC_F, KC_P, COMBO_END};
const uint16_t PROGMEM combo_pr[] = {KC_L, KC_U, COMBO_END};

const uint16_t PROGMEM combo_sql[] = {KC_F, KC_W, COMBO_END};
const uint16_t PROGMEM combo_sqr[] = {KC_U, KC_Y, COMBO_END};

const uint16_t PROGMEM combo_curll[] = {KC_P, KC_W, COMBO_END};
const uint16_t PROGMEM combo_curlr[] = {KC_L, KC_Y, COMBO_END};

const uint16_t PROGMEM combo_caps[] = { MT(MOD_LALT, KC_R), KC_D, COMBO_END};
const uint16_t PROGMEM combo_caps_word[] = {KC_H, MT(MOD_LALT, KC_I), COMBO_END};

enum {
    COMBO_ESC,
    COMBO_DASH,
    COMBO_UNDERSCORE,
    COMBO_PL,
    COMBO_PR,
    COMBO_SQL,
    COMBO_SQR,
    COMBO_CURLL,
    COMBO_CURLR,
    COMBO_CAPS,
    COMBO_CAPS_WORD,
};

combo_t key_combos[] = {
    [COMBO_ESC] = COMBO(combo_esc, KC_ESC),
    [COMBO_DASH] = COMBO(combo_dash, KC_MINUS),
    [COMBO_UNDERSCORE] = COMBO(combo_underscore, KC_UNDERSCORE),
    
    [COMBO_PL] = COMBO(combo_pl, KC_LPRN),
    [COMBO_PR] = COMBO(combo_pr, KC_RPRN),
    [COMBO_SQL] = COMBO(combo_sql, KC_LEFT_BRACKET),
    [COMBO_SQR] = COMBO(combo_sqr, KC_RIGHT_BRACKET),
    [COMBO_CURLL] = COMBO(combo_curll, KC_LCBR),
    [COMBO_CURLR] = COMBO(combo_curlr, KC_RCBR),
    
    [COMBO_CAPS] = COMBO_ACTION(combo_caps),
    [COMBO_CAPS_WORD] = COMBO(combo_caps_word, CW_TOGG),
};

void process_combo_event(uint16_t combo_index, bool pressed) {
  switch(combo_index) {
    case COMBO_CAPS:
      if (pressed) {
          int m = get_mods();
          set_mods(m & MOD_MASK_SHIFT ? m & ~MOD_LSFT : m | MOD_LSFT);
      }
      break;
  }
}
