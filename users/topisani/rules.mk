# Enable common features
COMBO_ENABLE 		   = yes
MOUSEKEY_ENABLE        = yes
CAPS_WORD_ENABLE       = yes
LTO_ENABLE             = yes
TAP_DANCE_ENABLE       = no
AUTO_SHIFT_ENABLE      = no
REPEAT_KEY_ENABLE      = yes
KEY_OVERRIDE_ENABLE    = no
SWAP_HANDS_ENABLE      = yes

# Disable unused features
MAGIC_ENABLE	       = no
CONSOLE_ENABLE         = no
COMMAND_ENABlE         = no
BOOTMAGIC_ENABLE       = no
UNICODE_ENABLE         = no
SPACE_CADET_ENABLE     = no
GRAVE_ESC_ENABLE       = no
AUDIO_ENABLE 	       = no

NKRO_ENABLE = no

VPATH += keyboards/gboards/
SRC += topisani.c
