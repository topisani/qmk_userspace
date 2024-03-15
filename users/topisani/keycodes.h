#pragma once

#include <quantum/keycodes.h>

enum layers {
    _COLEMAK_DH,
    _COMBOREF,
    _OSL,
    _MOUSE,
    _SYS,
    _NAV
};

enum custom_keycodes {
    PANIC = QK_USER,
};

#define OLALT OSM(MOD_LALT)
#define OLCTL OSM(MOD_LCTL)
#define ORSFT OSM(MOD_RSFT)
#define OLSFT OSM(MOD_LSFT)
#define ORALT OSM(MOD_RALT)
#define OLGUI OSM(MOD_LGUI)
#define L_OSL OSL(_OSL)
#define LGNAV LM(_NAV, MOD_LGUI)

#define HG(K) MT(MOD_LGUI, KC_##K)
#define HA(K) MT(MOD_LALT, KC_##K)
#define HS(K) MT(MOD_LSFT, KC_##K)
#define HC(K) MT(MOD_LCTL, KC_##K)
#define HN(K) LT(_NAV, KC_##K)
#define TABN C(KC_PGDN)
#define TABP C(KC_PGUP)
#define TMUX C(KC_SPC)

#define DK_AE RALT(KC_E)
#define DK_OE RALT(KC_O)
#define DK_AA RALT(KC_A)
