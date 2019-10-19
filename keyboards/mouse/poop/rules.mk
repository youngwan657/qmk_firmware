# MCU name
MCU = atmega32u4
F_CPU = 8000000
BOOTLOADER = atmel-dfu

# Compiler
EXTRAFLAGS += -flto  -DONLYQWERTY
SRC += sten.c

# QMK Options
BOOTMAGIC_ENABLE 				= lite      # Virtual DIP switch configuration
POINTING_DEVICE_ENABLE 	= yes				# Mouse API enable
CONSOLE_ENABLE 					= yes 			# Console for debug
NKRO_ENABLE 						= yes       # USB Nkey Rollover
STENO_ENABLE 						= yes				# Steno Features
KEYBOARD_SHARED_EP			= yes				# Needed to reduce descriptor usage
MOUSEKEY_ENABLE			 		= yes 		  # 1500 bytes
