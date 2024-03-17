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
reg @ <percent>s<space>*=<space>*LAYOUT<ret>j<a-i>is<space><ret>dXs.<backspace>,<ret>&<gt><gt>i<space><right><esc>i<space><esc><left><left><esc>
*/
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_COLEMAK_DH]=LAYOUT_preonic_grid(
        KC_GRV  , KC_1    , KC_2    , KC_3     , KC_4     , KC_5    , KC_6     , KC_7    , KC_8     , KC_9    , KC_0       , KC_MINS , 
        KC_TAB  , KC_Q    , KC_W    , KC_F     , KC_P     , KC_B    , KC_J     , KC_L    , KC_U     , KC_Y    , XXXXXXX    , KC_BSPC , 
        KC_LCTL , KC_A    , HA(R)   , HS(S)    , HC(T)    , KC_G    , KC_M     , HC(N)   , HS(E)    , HA(I)   , KC_O       , KC_ENT  , 
        KC_LSFT , KC_Z    , KC_X    , KC_C     , KC_D     , KC_V    , KC_K     , KC_H    , KC_COMM  , KC_DOT  , KC_SLSH    , KC_RSFT , 
        XXXXXXX , KC_LCTL , KC_LALT , MO(_NAV) , KC_SPACE , LGNAV   , KC_BSPC  , L_OSL   , OLSFT    , KC_RALT , KC_RCTL    , XXXXXXX
        )       , 
    [_COMBOREF]=LAYOUT_preonic_grid(
        KC_GRV  , KC_1    , KC_2    , KC_3     , KC_4     , KC_5    , KC_6     , KC_7    , KC_8     , KC_9    , KC_0       , KC_MINS , 
        KC_TAB  , KC_Q    , KC_W    , KC_F     , KC_P     , KC_B    , KC_J     , KC_L    , KC_U     , KC_Y    , XXXXXXX    , KC_BSPC , 
        KC_LCTL , KC_A    , KC_R    , KC_S     , KC_T     , KC_G    , KC_M     , KC_N    , KC_E     , KC_I    , KC_O       , KC_ENT  , 
        KC_LSFT , KC_Z    , KC_X    , KC_C     , KC_D     , KC_V    , KC_K     , KC_H    , KC_COMM  , KC_DOT  , KC_SLSH    , KC_RSFT , 
        XXXXXXX , KC_LCTL , KC_LALT , MO(_NAV) , KC_SPACE , LGNAV   , KC_BSPC  , L_OSL   , OLSFT    , KC_RALT , KC_RCTL    , XXXXXXX
        )       , 
    [_QWERTY]=LAYOUT_preonic_grid(
        KC_GRV  , KC_1    , KC_2    , KC_3     , KC_4     , KC_5    , KC_6     , KC_7    , KC_8     , KC_9    , KC_0       , KC_MINS , 
        KC_TAB  , KC_Q    , KC_W    , KC_E     , KC_R     , KC_T    , KC_Y     , KC_U    , KC_I     , KC_O    , KC_P       , KC_BSPC , 
        KC_LCTL , KC_A    , HA(S)   , HS(D)    , HC(F)    , KC_G    , KC_H     , KC_J    , HC(K)    , HS(L)   , HA(SCLN)   , KC_ENT  , 
        KC_LSFT , KC_Z    , KC_X    , KC_C     , KC_V     , KC_B    , KC_N     , KC_M    , KC_COMM  , KC_DOT  , KC_SLSH    , KC_RSFT , 
        XXXXXXX , KC_LCTL , KC_LALT , MO(_NAV) , KC_SPACE , LGNAV   , KC_BSPC  , L_OSL   , OLSFT    , KC_RALT , KC_RCTL    , XXXXXXX
        )       , 
    [_OSL]=LAYOUT_preonic_grid(
        _______ , _______ , _______ , _______  , _______  , _______ , _______  , _______ , _______  , _______ , _______    , _______ , 
        _______ , KC_GRV  , KC_7    , KC_8     , KC_9     , KC_0    , KC_PLUS  , KC_SCLN , KC_MINS  , KC_RBRC , XXXXXXX    , _______ , 
        _______ , DK_OE   , HA(4)   , HS(5)    , HC(6)    , DK_AA   , KC_EQL   , KC_COLN , TO(_NAV) , KC_QUOT , KC_ENT     , _______ , 
        _______ , XXXXXXX , KC_1    , KC_2     , KC_3     , DK_AE   , KC_GRV   , KC_SCLN , KC_UNDS  , KC_BSLS , TMUX       , _______ , 
        _______ , _______ , _______ , _______  , OLSFT    , _______ , _______  , _______ , _______  , _______ , _______    , _______
        )       , 
    [_NAV]=LAYOUT_preonic_grid(
        _______ , _______ , _______ , _______  , _______  , _______ , _______  , _______ , _______  , _______ , _______    , _______ , 
        _______ , KC_Q    , KC_7    , KC_8     , KC_9     , KC_0    , TO(_SYS) , KC_PGUP , KC_RCTL  , KC_PGDN , _______    , _______ , 
        _______ , TMUX    , HA(4)   , HS(5)    , HC(6)    , KC_P    , XXXXXXX  , KC_LEFT , KC_UP    , KC_RGHT , TO(_MOUSE) , _______ , 
        _______ , TMUX    , KC_1    , KC_2     , KC_3     , KC_ENT  , XXXXXXX  , KC_HOME , KC_DOWN  , KC_END  , TMUX       , _______ , 
        _______ , _______ , _______ , _______  , TO(0)    , _______ , _______  , _______ , _______  , _______ , _______    , _______
        )       , 
    [_MOUSE]=LAYOUT_preonic_grid(
        _______ , _______ , _______ , _______  , _______  , _______ , _______  , _______ , _______  , _______ , _______    , _______ , 
        _______ , XXXXXXX , KC_BTN3 , KC_BTN2  , KC_BTN1  , XXXXXXX , TABP     , KC_WH_U , KC_BTN1  , KC_WH_D , TABN       , _______ , 
        _______ , XXXXXXX , HA(NO)  , HS(NO)   , HC(NO)   , XXXXXXX , KC_BTN4  , KC_MS_L , KC_MS_U  , KC_MS_R , KC_BTN5    , _______ , 
        _______ , XXXXXXX , XXXXXXX , XXXXXXX  , XXXXXXX  , XXXXXXX , XXXXXXX  , KC_WH_L , KC_MS_D  , KC_WH_R , XXXXXXX    , _______ , 
        _______ , _______ , _______ , _______  , TO(0)    , _______ , _______  , _______ , _______  , _______ , _______    , _______
        )       , 
    [_SYS]=LAYOUT_preonic_grid(
        _______ , COLEMAK , QWERTY  , _______  , _______  , _______ , _______  , _______ , _______  , _______ , _______    , _______ , 
        _______ , XXXXXXX , KC_F7   , KC_F8    , KC_F9    , KC_F10  , KC_INS   , RGB_VAD , RGB_TOG  , RGB_VAI , XXXXXXX    , _______ , 
        _______ , XXXXXXX , KC_F4   , KC_F5    , KC_F6    , KC_F11  , XXXXXXX  , HA(NO)  , HS(NO)   , HC(NO)  , XXXXXXX    , _______ , 
        _______ , XXXXXXX , KC_F1   , KC_F2    , KC_F3    , KC_F12  , KC_PSCR  , XXXXXXX , XXXXXXX  , XXXXXXX , XXXXXXX    , _______ , 
        _______ , _______ , _______ , _______  , TO(0)    , _______ , _______  , _______ , _______  , _______ , _______    , _______
)
};
// clang-format on

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (clockwise) {
        register_code(KC_PGDN);
        unregister_code(KC_PGDN);
    } else {
        register_code(KC_PGUP);
        unregister_code(KC_PGUP);
    }
    return true;
}

