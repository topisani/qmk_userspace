#include "action.h"
#include "action_layer.h"
#include "action_util.h"
#include "keycodes.h"
#include "keymap_us.h"
#include "modifiers.h"
#include "process_combo.h"
#include "caps_word.h"
#include "topisani.h"
#include "g/keymap_combo.h"

#include QMK_KEYBOARD_H

// clang-format off
/*
kakoune code to highlight and format keymap:
addhl -override window/qmk regex _______|\bKC_|XXXXXXX|\.\.\.\.\.\.\. 0:comment
reg @ <percent>s<space>*=<space>*LAYOUT36<ret>j<a-i>is<space><ret>dXs.<backspace>,<ret>&<gt><gt>i<space><right><esc>i<space><esc><left><left><esc>
*/
#define LAYOUT36(K1, K2, K3, K4, K5, NO5,NO6, K6, K7, K8, K9, K10, K11, K12, K13, K14, K15, NO15,NO16, K16, K17, K18, K19, K20, K21, K22, K23, K24, K25, NO25,NO26, K26, K27, K28, K29, K30, NO1, NO2, NO3, K31, K32, K33, K34, K35, K36, NO4, NO7, NO8) \
    LAYOUT_split_3x6_3(                                                                                                                        \
     XXXXXXX , K1  ,  K2    ,  K3   ,  K4  , K5  ,              K6  , K7  , K8      , K9  , K10 , XXXXXXX , \
     XXXXXXX , K11 ,  K12   ,  K13  ,  K14 , K15 ,              K16 , K17 , K18     , K19 , K20 , XXXXXXX , \
     XXXXXXX , K21 ,  K22   ,  K23  ,  K24 , K25 ,              K26 , K27 , K28     , K29 , K30 , XXXXXXX , \
                                       K31 , K32 ,  K33 , K34 , K35 , K36 \
    )

#define SPC LT(_NAV, KC_SPC)

// COLEMAK-DH :
// Q W F P B J L U Y <
// A R S T G M N E I O
// Z X C D V K H , . /

// APTv3:
// w g d f b  q l u o y
// r s t h k  j n e a i ;
// x c m p v  z , . ' /
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_COMBOREF]=LAYOUT36(
        KC_Q      , KC_W    , KC_F    , KC_P    , KC_B    , .......   , ....... , KC_J     , KC_L    , KC_U    , KC_Y    , KC_QUOTE   ,
        KC_A      , KC_R    , KC_S    , KC_T    , KC_G    , .......   , ....... , KC_M     , KC_N    , KC_E    , KC_I    , KC_O       ,
        KC_Z      , KC_X    , KC_C    , KC_D    , KC_V    , .......   , ....... , KC_K     , KC_H    , KC_COMM , KC_DOT  , KC_SLSH    ,
        .......   , ....... , ....... , _______ , KC_SPC  , OSL(_NAV) , KC_BSPC , ORSFT    , _______ , ....... , ....... , .......
    ),
    [_COLEMAK_DH]=LAYOUT36(
        KC_Q      , KC_W    , KC_F    , KC_P    , KC_B    , .......   , ....... , KC_J     , KC_L    , KC_U    , KC_Y    , HRC(QUOT)  ,
        KC_A      , HA(R)   , HLS(S)  , HC(T)   , KC_G    , .......   , ....... , KC_M     , HC(N)   , HRS(E)  , HA(I)   , KC_O       ,
        KC_Z      , KC_X    , KC_C    , KC_D    , KC_V    , .......   , ....... , KC_K     , KC_H    , KC_COMM , KC_DOT  , KC_SLSH    ,
        .......   , ....... , ....... , _______ , KC_SPC  , OSL(_NAV) , KC_BSPC , ORSFT    , _______ , ....... , ....... , .......
    ),
    [_APT]=LAYOUT36(
        KC_W      , KC_G    , KC_D    , KC_F    , KC_B    , .......   , ....... , KC_Q     , KC_L    , KC_U    , KC_O    , HRC(Y)  ,
        KC_R      , HA(S)   , HLS(T)  , HC(H)   , KC_K    , .......   , ....... , KC_J     , HC(N)   , HRS(E)  , HA(A)   , KC_I       ,
        KC_X      , KC_C    , KC_M    , KC_P    , KC_V    , .......   , ....... , KC_Z     , KC_COMMA, KC_DOT  , KC_QUOT ,KC_SLSH    ,
        .......   , ....... , ....... , _______ , KC_SPC  , OSL(_NAV) , KC_BSPC , ORSFT    , _______ , ....... , ....... , .......
    ),
    //[_OSL]=LAYOUT36(
        //KC_GRV  , KC_7    , KC_8    , KC_9    , KC_0    , .......   , ....... , TO(_SYS) , KC_PGUP , KC_RCTL , KC_PGDN , TO(_MOUSE) ,
        //DK_AA   , HA(4)   , HLS(5)  , HC(6)   , DK_AE   , .......   , ....... , KC_EQL   , KC_LEFT , KC_UP   , KC_RGHT , KC_ENT     ,
        //OEMLG   , KC_1    , KC_2    , KC_3    , DK_OE   , .......   , ....... , DK_AA    , KC_HOME , KC_DOWN , KC_END  , KC_BSLS    ,
        //....... , ....... , ....... , _______ , OLSFT   , _______   , _______ , _______  , _______ , ....... , ....... , .......
    //),
    [_NAV]=LAYOUT36(
        KC_DEL    , KC_7    , KC_8    , KC_9    , KC_0    , .......   , ....... , DK_AE    , KC_PGUP , KC_RCTL , KC_PGDN , TO(_MOUSE) ,
        TMUX      , HA(4)   , HLS(5)  , HC(6)   , DK_AE   , .......   , ....... , DK_AA    , KC_LEFT , KC_UP   , KC_RGHT , KC_ENT     ,
        OLGUI     , KC_1    , KC_2    , KC_3    , DK_OE   , .......   , ....... , DK_OE    , KC_HOME , KC_DOWN , KC_END  , KC_BSLS    ,
        .......   , ....... , ....... , _______ , TO(0)   , _______   , _______ , _______  , _______ , ....... , ....... , .......
    ),
    [_MOUSE]=LAYOUT36(
        XXXXXXX   , KC_BTN3 , KC_BTN2 , KC_BTN1 , XXXXXXX , .......   , ....... , TABP     , KC_WH_U , KC_BTN1 , KC_WH_D , TABN       ,
        XXXXXXX   , HA(NO)  , HLS(NO) , HC(NO)  , XXXXXXX , .......   , ....... , KC_BTN4  , KC_MS_L , KC_MS_U , KC_MS_R , KC_BTN5    ,
        XXXXXXX   , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , .......   , ....... , XXXXXXX  , KC_WH_L , KC_MS_D , KC_WH_R , XXXXXXX    ,
        .......   , ....... , ....... , _______ , TO(0)   , _______   , _______ , _______  , _______ , ....... , ....... , .......
    ),
    [_SYS]=LAYOUT36(
        DF(_COLEMAK_DH), KC_F7   , KC_F8   , KC_F9   , KC_F10  , .......   , ....... , KC_INS   , XXXXXXX , RGB_TOG , XXXXXXX , XXXXXXX    ,
        DF(_APT), KC_F4   , KC_F5   , KC_F6   , KC_F11  , .......   , ....... , XXXXXXX  , HC(NO)  , HLS(NO) , HA(NO)  , XXXXXXX    ,
        XXXXXXX   , KC_F1   , KC_F2   , KC_F3   , KC_F12  , .......   , ....... , KC_PSCR  , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX    ,
        .......   , ....... , ....... , _______ , TO(0)   , _______   , _______ , _______  , _______ , ....... , ....... , .......
)
};
// clang-format on


// const keypos_t PROGMEM hand_swap_config[MATRIX_ROWS][MATRIX_COLS] = {
//   {{11, 0}, {10, 0}, {9, 0}, {8, 0}, {7, 0}, {6, 0}, {5, 0}, {4, 0}, {3, 0}, {2, 0}, {1, 0}, {0, 0}},
//   {{11, 1}, {10, 1}, {9, 1}, {8, 1}, {7, 1}, {6, 1}, {5, 1}, {4, 1}, {3, 1}, {2, 1}, {1, 1}, {0, 1}},
//   {{11, 2}, {10, 2}, {9, 2}, {8, 2}, {7, 2}, {6, 2}, {5, 2}, {4, 2}, {3, 2}, {2, 2}, {1, 2}, {0, 2}},
//   {{11, 3}, {10, 3}, {9, 3}, {8, 3}, {7, 3}, {6, 3}, {5, 3}, {4, 3}, {3, 3}, {2, 3}, {1, 3}, {0, 3}},
// };
