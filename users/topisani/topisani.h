#pragma once

#include <quantum/keycodes.h>

enum layers {
    _COLEMAK_DH,
    _COMBOREF,
    _QWERTY,
    _OSL,
    _MOUSE,
    _SYS,
    _NAV
};

enum custom_keycodes {
    PANIC = QK_USER,
    TMUX,
    TKC_NAV,
    OEM_START,
    OEMLG = OEM_START, // Oneshot mods that exit the osl layer
    OEMLC,
    OEMRS,
    OEMLA,
    OEM_END = OEMLA
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
#define HLS(K) MT(MOD_LSFT, KC_##K)
#define HRS(K) MT(MOD_RSFT, KC_##K)
#define HC(K) MT(MOD_LCTL, KC_##K)
#define HRC(K) MT(MOD_RCTL, KC_##K)
#define HN(K) LT(_NAV, KC_##K)
#define TABN C(KC_PGDN)
#define TABP C(KC_PGUP)
#define QWERTY DF(_QWERTY)
#define COLEMAK DF(_COLEMAK_DH)

#define DK_AE RALT(KC_E)
#define DK_OE RALT(KC_O)
#define DK_AA RALT(KC_A)

