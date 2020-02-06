VPATH						+= keyboards/gboards/engine
SRC							+= matrix.c engine.c
OPT_DEFS				+= -DONLYQWERTY 

COMBO_ENABLE			= no
CONSOLE_ENABLE  	= no
COMMAND_ENABLE		= yes
BOOTMAGIC_ENABLE = lite

DEBOUNCE_TYPE  = eager_pr
SRC += matrix.c
QUANTUM_LIB_SRC += i2c_master.c
