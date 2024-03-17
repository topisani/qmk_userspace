#pragma once

#define EE_HANDS

#ifdef RGBLIGHT_ENABLE
#    define RGBLIGHT_EFFECT_BREATHING
// #    define RGBLIGHT_EFFECT_RAINBOW_MOOD
// #    define RGBLIGHT_EFFECT_RAINBOW_SWIRL
// #    define RGBLIGHT_EFFECT_SNAKE
// #    define RGBLIGHT_EFFECT_KNIGHT
#    define RGBLIGHT_EFFECT_STATIC_GRADIENT

#    define RGBLIGHT_HUE_STEP 50
#    define RGBLIGHT_SAT_STEP 50
#    define RGBLIGHT_VAL_STEP 50
#    define RGBLIGHT_LIMIT_VAL 150
#    define RGBLIGHT_SPLIT
#    define RGBLIGHT_SLEEP
#    define SPLIT_LED_STATE_ENABLE
#    define SPLIT_TRANSPORT_MIRROR
#    define SPLIT_MODS_ENABLE
#    define SPLIT_LAYER_STATE_ENABLE
#endif
