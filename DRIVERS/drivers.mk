C_SOURCES +=\
			$(DRIVERS)/keyboard_matrix.c \
			$(DRIVERS)/keyboard_event.c \
			$(DRIVERS)/process_record_kb.c \
			$(DRIVERS)/keyboard_timer.c \
			$(DRIVERS)/bootloader.c \
			$(DRIVERS)/keyboard_host_driver.c
			

C_INCLUDES += -I$(DRIVERS)
				

ifeq (yes,$(strip $(USB_ENABLE)))
CFLAGS += -DUSB_ENABLE

C_SOURCES +=\
			$(DRIVERS)/USB/usbuser1.c \
			$(DRIVERS)/USB/usbdesc.c \
			$(DRIVERS)/USB/hid_configuration.c

C_INCLUDES += -I$(DRIVERS)/USB
endif
ifeq (yes,$(strip $(EEPROM_ENABLE)))
C_SOURCES +=\
			$(DRIVERS)/eeprom/eeprom.c

C_INCLUDES += -I$(DRIVERS)/eeprom
endif

ifeq (yes,$(strip $(RGB_BOTTOM_ENABLE)))
    CFLAGS += -DRGB_BOTTOM_ENABLE

    C_INCLUDES += \
	    		-I$(DRIVERS)/RGB/RGB_bottom\

    C_SOURCES +=$(DRIVERS)/RGB/RGB_bottom/ws2812.c \
	            $(DRIVERS)/RGB/RGB_bottom/rgblight.c \
				
    CIE1931_CURVE = yes
endif
#RGB轴灯3733
ifeq (IS31FL3733,$(strip $(RGB_TOP_ENABLE)))
    CFLAGS += -DIS31FL3733
    CFLAGS += -DRGB_TOP_ENABLE

    C_INCLUDES += \
				-I$(DRIVERS)/iic\
				-I$(DRIVERS)/RGB/RGB_top\
				-I$(DRIVERS)/RGB/RGB_top/issi\

    C_SOURCES +=$(DRIVERS)/RGB/color.c \
				$(DRIVERS)/iic/i2c_master.c\
				$(DRIVERS)/RGB/RGB_top/issi/is31fl3733.c \
				$(DRIVERS)/RGB/RGB_top/rgb_matrix_drivers.c\
				$(DRIVERS)/RGB/RGB_top/rgb_matrix.c

    CIE1931_CURVE = yes
endif

#RGB轴灯3737
ifeq (IS31FL3737,$(strip $(RGB_TOP_ENABLE)))
    CFLAGS += -DIS31FL3737
    CFLAGS += -DRGB_TOP_ENABLE

    C_INCLUDES += \
				-I$(DRIVERS)/iic\
				-I$(DRIVERS)/RGB/RGB_top\
				-I$(DRIVERS)/RGB/RGB_top/issi\

    C_SOURCES +=$(DRIVERS)/RGB/color.c \
				$(DRIVERS)/iic/i2c_master.c\
				$(DRIVERS)/RGB/RGB_top/issi/is31fl3737.c \
				$(DRIVERS)/RGB/RGB_top/rgb_matrix_drivers.c\
				$(DRIVERS)/RGB/RGB_top/rgb_matrix.c

CIE1931_CURVE = yes
endif

#RGB轴灯3741
ifeq (IS31FL3741,$(strip $(RGB_TOP_ENABLE)))
    CFLAGS += -DIS31FL3741
    CFLAGS += -DRGB_TOP_ENABLE

    C_INCLUDES += \
				-I$(DRIVERS)/iic\
				-I$(DRIVERS)/RGB/RGB_top\
				-I$(DRIVERS)/RGB/RGB_top/issi\

    C_SOURCES +=$(DRIVERS)/RGB/color.c \
				$(DRIVERS)/iic/i2c_master.c\
				$(DRIVERS)/RGB/RGB_top/issi/is31fl3741.c \
				$(DRIVERS)/RGB/RGB_top/rgb_matrix_drivers.c\
				$(DRIVERS)/RGB/RGB_top/rgb_matrix.c

    CIE1931_CURVE = yes
endif

ifeq (yes,$(strip $(CIE1931_CURVE)))
    CFLAGS += -DUSE_CIE1931_CURVE

    C_INCLUDES += -I$(DRIVERS)/RGB\
	
    C_SOURCES += $(DRIVERS)/RGB/led_tables.c\

endif

#自定义灯效KB
ifeq ($(strip $(RGB_MATRIX_CUSTOM_KB)), yes)
    CFLAGS += -DRGB_MATRIX_CUSTOM_KB
endif

#用户自定义灯效
ifeq ($(strip $(RGB_MATRIX_CUSTOM_USER)), yes)
    CFLAGS += -DRGB_MATRIX_CUSTOM_USER
endif