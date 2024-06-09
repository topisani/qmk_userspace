#pragma once


// Tapping settings
#define TAPPING_TERM 200                 // Base tapping term
#define QUICK_TAP_TERM TAPPING_TERM / 2  // Quick Tap Term to trigger auto-repeat

// Allows media codes to properly register in macros and rotary encoder code
#define TAP_CODE_DELAY 15               // Introduces delay between code taps so no keys get eaten.

// Combos!
#define COMBO_TERM 25                   // Gotta combo fast.
#define COMBO_ONLY_FROM_LAYER 1         // compare only with the COMBOREF layer.
#define COMBO_TERM_PER_COMBO            // Adjust combo term for ease of combo.
#define COMBO_STRICT_TIMER              // all keys must be hit within the term from the first keypress.

#define ONESHOT_TAP_TOGGLE 2

// SPACE SAVERS FOR ALL BOARDS.
#undef LOCKING_SUPPORT_ENABLE
#undef LOCKING_RESYNC_ENABLE
#define LAYER_STATE_16BIT

#define AUTO_SHIFT_TIMEOUT 150
#define AUTO_SHIFT_REPEAT
#define NO_AUTO_SHIFT_SPECIAL
#define RETRO_SHIFT 500


#ifdef RGBLIGHT_ENABLE
// #    define RGBLIGHT_EFFECT_BREATHING
// #    define RGBLIGHT_EFFECT_RAINBOW_MOOD
// #    define RGBLIGHT_EFFECT_RAINBOW_SWIRL
// #    define RGBLIGHT_EFFECT_SNAKE
// #    define RGBLIGHT_EFFECT_KNIGHT
// #    define RGBLIGHT_EFFECT_STATIC_GRADIENT

#    define RGBLIGHT_HUE_STEP 50
#    define RGBLIGHT_SAT_STEP 50
#    define RGBLIGHT_VAL_STEP 50
#    define RGBLIGHT_LIMIT_VAL 20
#    define RGBLIGHT_SPLIT 
#    define RGBLIGHT_SLEEP
#    define SPLIT_LED_STATE_ENABLE
#    define SPLIT_TRANSPORT_MIRROR
#    define SPLIT_MODS_ENABLE
#    define SPLIT_LAYER_STATE_ENABLE
#endif
