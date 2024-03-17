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
#include "keycodes.h"
#include "g/keymap_combo.h"

#include QMK_KEYBOARD_H

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

// Q W F P B J L U Y < 
// A R S T G M N E I O 
// Z X C D V K H , . / 
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_COLEMAK_DH]=LAYOUT36(
        KC_Q    , KC_W    , KC_F    , KC_P    , KC_B    , .......  , ....... , KC_J     , KC_L    , KC_U     , KC_Y    , KC_SCLN    , 
        KC_A    , HA(R)   , HS(S)   , HC(T)   , KC_G    , .......  , ....... , KC_M     , HC(N)   , HS(E)    , HA(I)   , KC_O       , 
        KC_Z    , KC_X    , KC_C    , KC_D    , KC_V    , .......  , ....... , KC_K     , KC_H    , KC_COMM  , KC_DOT  , KC_SLSH    , 
        ....... , ....... , ....... , LGNAV   , KC_SPC  , MO(_NAV) , KC_BSPC , L_OSL    , OLSFT   , .......  , ....... , .......
        )       , 
    [_COMBOREF]=LAYOUT36(
        KC_Q    , KC_W    , KC_F    , KC_P    , KC_B    , .......  , ....... , KC_J     , KC_L    , KC_U     , KC_Y    , KC_SCLN    , 
        KC_A    , KC_R    , KC_S    , KC_T    , KC_G    , .......  , ....... , KC_M     , KC_N    , KC_E     , KC_I    , KC_O       , 
        KC_Z    , KC_X    , KC_C    , KC_D    , KC_V    , .......  , ....... , KC_K     , KC_H    , KC_COMM  , KC_DOT  , KC_SLSH    , 
        ....... , ....... , ....... , LGNAV   , KC_SPC  , MO(_NAV) , KC_BSPC , L_OSL    , OLSFT   , .......  , ....... , .......
        )       , 
    [_OSL]=LAYOUT36(
        KC_GRV  , KC_7    , KC_8    , KC_9    , KC_0    , .......  , ....... , KC_PLUS  , KC_PLUS , KC_MINS  , KC_RBRC , XXXXXXX    , 
        DK_OE   , HA(4)   , HS(5)   , HC(6)   , DK_AA   , .......  , ....... , KC_EQL   , KC_EQL  , TO(_NAV) , KC_QUOT , KC_ENT     , 
        XXXXXXX , KC_1    , KC_2    , KC_3    , DK_AE   , .......  , ....... , KC_GRV   , KC_SCLN , KC_UNDS  , KC_BSLS , XXXXXXX    , 
        ....... , ....... , ....... , _______ , OLSFT   , _______  , _______ , _______  , _______ , .......  , ....... , .......
        )       , 
    [_NAV]=LAYOUT36(
        KC_Q    , KC_7    , KC_8    , KC_9    , KC_0    , .......  , ....... , TO(_SYS) , KC_PGUP , KC_RCTL  , KC_PGDN , _______    , 
        TMUX    , HA(4)   , HS(5)   , HC(6)   , KC_P    , .......  , ....... , XXXXXXX  , KC_LEFT , KC_UP    , KC_RGHT , TO(_MOUSE) , 
        OLGUI   , KC_1    , KC_2    , KC_3    , KC_ENT  , .......  , ....... , XXXXXXX  , KC_HOME , KC_DOWN  , KC_END  , TMUX       , 
        ....... , ....... , ....... , _______ , TO(0)   , _______  , _______ , _______  , _______ , .......  , ....... , .......
        )       , 
    [_MOUSE]=LAYOUT36(
        XXXXXXX , KC_BTN3 , KC_BTN2 , KC_BTN1 , XXXXXXX , .......  , ....... , TABP     , KC_WH_U , KC_BTN1  , KC_WH_D , TABN       , 
        XXXXXXX , HA(NO)  , HS(NO)  , HC(NO)  , XXXXXXX , .......  , ....... , KC_BTN4  , KC_MS_L , KC_MS_U  , KC_MS_R , KC_BTN5    , 
        XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , .......  , ....... , XXXXXXX  , KC_WH_L , KC_MS_D  , KC_WH_R , XXXXXXX    , 
        ....... , ....... , ....... , _______ , TO(0)   , _______  , _______ , _______  , _______ , .......  , ....... , .......
        )       , 
    [_SYS]=LAYOUT36(
        XXXXXXX , KC_F7   , KC_F8   , KC_F9   , KC_F10  , .......  , ....... , KC_INS   , RGB_VAD , RGB_TOG  , RGB_VAI , XXXXXXX    , 
        XXXXXXX , KC_F4   , KC_F5   , KC_F6   , KC_F11  , .......  , ....... , XXXXXXX  , HA(NO)  , HS(NO)   , HC(NO)  , XXXXXXX    , 
        XXXXXXX , KC_F1   , KC_F2   , KC_F3   , KC_F12  , .......  , ....... , KC_PSCR  , XXXXXXX , XXXXXXX  , XXXXXXX , XXXXXXX    , 
        ....... , ....... , ....... , _______ , TO(0)   , _______  , _______ , _______  , _______ , .......  , ....... , .......
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
            case _COLEMAK_DH:
                oled_write_P(PSTR(" COLEMAK "), false);
                break;
            case _OSL:
                oled_write_P(PSTR("   OSL   "), false);
                break;
            case _NAV:
                oled_write_P(PSTR("   NAV   "), false);
                break;
            case _MOUSE:
                oled_write_P(PSTR("  MOUSE  "), false);
                break;
            case _SYS:
                oled_write_P(PSTR("   SYS   "), false);
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

layer_state_t layer_state_set_kb(layer_state_t state) {
    uint8_t layer = get_highest_layer(state);
    uint8_t val = rgblight_get_val();
    HSV     hsv = {0x4B, 0x1a, val};

    switch (layer) {
        case _OSL:
            hsv = (HSV){0xDC, 0xA0, val};
            break;
        case _NAV:
            hsv = (HSV){0x94, 0xDD, val};
            break;
        case _MOUSE:
            hsv = (HSV){0x64, 0xFD, val};
            break;
        case _SYS:
            hsv = (HSV){0x40, 0xED, val};
            break;
    }
    rgblight_sethsv_noeeprom(hsv.h, hsv.s, hsv.v);
    return state;
}

void keyboard_post_init_kb(void) {
    rgblight_enable_noeeprom();
    HSV hsv = {0x4B, 0x1a, 0xff};
    rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
    rgblight_sethsv_noeeprom(hsv.h, hsv.s, hsv.v);
}
