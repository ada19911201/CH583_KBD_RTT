ifndef TMK
    TMK := ../TMK
endif

C_SOURCES +=$(TMK)/host.c \
	$(TMK)/keyboard.c \
	$(TMK)/matrix.c \
	$(TMK)/action.c \
	$(TMK)/action_tapping.c \
	$(TMK)/action_macro.c \
	$(TMK)/action_layer.c \
	$(TMK)/action_util.c \
	$(TMK)/print.c \
	$(TMK)/debug.c \
	$(TMK)/util.c \
	$(TMK)/hook.c \

C_INCLUDES += -I$(TMK)

ifeq (yes,$(strip $(UNIMAP_ENABLE)))
    C_SOURCES += $(TMK)/unimap.c
    CFLAGS += -DUNIMAP_ENABLE
    CFLAGS += -DACTIONMAP_ENABLE
else
    ifeq (yes,$(strip $(ACTIONMAP_ENABLE)))
    C_SOURCES += $(TMK)/actionmap.c
    CFLAGS += -DACTIONMAP_ENABLE
    else
    C_SOURCES += $(TMK)/keymap.c
    endif
endif

ifeq (yes,$(strip $(BOOTMAGIC_ENABLE)))
    C_SOURCES += $(TMK)/bootmagic.c
    CFLAGS += -DBOOTMAGIC_ENABLE
endif

ifeq (yes,$(strip $(MOUSEKEY_ENABLE)))
    C_SOURCES += $(TMK)/mousekey.c
    CFLAGS += -DMOUSEKEY_ENABLE
    CFLAGS += -DMOUSE_ENABLE
endif

ifeq (yes,$(strip $(EXTRAKEY_ENABLE)))
    CFLAGS += -DEXTRAKEY_ENABLE
endif

ifeq (yes,$(strip $(CONSOLE_ENABLE)))
    C_SOURCES += $(TMK)/mbed/xprintf.c

    C_INCLUDES += -I$(TMK)/mbed

    CFLAGS += -DCONSOLE_ENABLE
else
    CFLAGS += -DNO_PRINT
    CFLAGS += -DNO_DEBUG
endif

ifeq (yes,$(strip $(COMMAND_ENABLE)))
    C_SOURCES += $(DRIVERS)/keyboard_command.c \
    
    CFLAGS += -DCOMMAND_ENABLE
endif

ifeq (yes,$(strip $(NKRO_ENABLE)))
    CFLAGS += -DNKRO_ENABLE
    CFLAGS += -DPROTOCOL_LUFA

    C_INCLUDES += -I$(TMK)/protocol/lufa
endif

ifeq (yes,$(strip $(USB_6KRO_ENABLE)))
    CFLAGS += -DUSB_6KRO_ENABLE
endif

ifeq (yes, $(strip $(KEYBOARD_LOCK_ENABLE)))
    CFLAGS += -DKEYBOARD_LOCK_ENABLE
endif

ifeq (yes,$(strip $(BACKLIGHT_ENABLE)))
    C_SOURCES += $(TMK)/backlight.c
    CFLAGS += -DBACKLIGHT_ENABLE
endif

ifeq (yes,$(strip $(KEYMAP_SECTION_ENABLE)))
    CFLAGS += -DKEYMAP_SECTION_ENABLE
endif

ifeq (yes,$(strip $(STATUS_LED_ENABLE)))
    CFLAGS += -DSTATUS_LED_ENABLE
endif


include $(DRIVERS)/drivers.mk
include $(ROOT_DIR)/app.mk