set positional-arguments

[private]
default:
    just --list

piantor *args:
    qmk flash -kb beekeeb/piantor -km topisani "$@"

kyria *args:
    qmk flash -kb splitkb/kyria/rev1 -km topisani "$@"

preonic *args:
    qmk flash -kb preonic/rev3_drop -km topisani "$@"
