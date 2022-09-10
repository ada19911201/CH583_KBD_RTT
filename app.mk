C_SOURCES +=\
$(RTT)/src/clock.c \
$(RTT)/src/components.c \
$(RTT)/src/cpu.c \
$(RTT)/src/idle.c \
$(RTT)/src/ipc.c \
$(RTT)/src/irq.c \
$(RTT)/src/kservice.c \
$(RTT)/src/mem.c \
$(RTT)/src/memheap.c \
$(RTT)/src/mempool.c \
$(RTT)/src/object.c \
$(RTT)/src/scheduler.c \
$(RTT)/src/slab.c \
$(RTT)/src/thread.c \
$(RTT)/src/timer.c \
$(RTT)/libcpu/WCH/Qingke_V4A/cpuport.c \
$(RTT)/components/device/device.c \
$(RTT)/components/drivers/ipc/ringbuffer.c \
$(RTT)/components/finsh/cmd.c \
$(RTT)/components/finsh/finsh_port.c \
$(RTT)/components/finsh/msh.c \
$(RTT)/components/finsh/shell.c\
$(RTT)/bsp/board.c \
$(APP)/main.c \
$(APP)/task_thread.c \
$(CORE)/RVMSIS/core_riscv.c\
$(SDK_ROOT)/CH58x_adc.c \
$(SDK_ROOT)/CH58x_clk.c \
$(SDK_ROOT)/CH58x_flash.c \
$(SDK_ROOT)/CH58x_gpio.c \
$(SDK_ROOT)/CH58x_i2c.c \
$(SDK_ROOT)/CH58x_pwm.c \
$(SDK_ROOT)/CH58x_pwr.c \
$(SDK_ROOT)/CH58x_spi0.c \
$(SDK_ROOT)/CH58x_spi1.c \
$(SDK_ROOT)/CH58x_sys.c \
$(SDK_ROOT)/CH58x_timer0.c \
$(SDK_ROOT)/CH58x_timer1.c \
$(SDK_ROOT)/CH58x_timer2.c \
$(SDK_ROOT)/CH58x_timer3.c \
$(SDK_ROOT)/CH58x_uart0.c \
$(SDK_ROOT)/CH58x_uart1.c \
$(SDK_ROOT)/CH58x_uart2.c \
$(SDK_ROOT)/CH58x_uart3.c \
$(SDK_ROOT)/CH58x_usb2dev.c \
$(SDK_ROOT)/CH58x_usb2hostBase.c \
$(SDK_ROOT)/CH58x_usb2hostClass.c \
$(SDK_ROOT)/CH58x_usbdev.c \
$(SDK_ROOT)/CH58x_usbhostBase.c \
$(SDK_ROOT)/CH58x_usbhostClass.c 


C_INCLUDES +=\
-I$(CORE)/RVMSIS \
-I$(APP)\
-I$(RTT)/bsp\
-I$(RTT)/components/device \
-I$(RTT)/components/drivers/ipc \
-I$(RTT)/components/drivers/include\
-I$(RTT)/components/drivers/include/ipc\
-I$(RTT)/components/finsh\
-I$(RTT)/include/libc\
-I$(RTT)/libcpu/WCH/Qingke_V4A\
-I$(RTT)/include\
-I$(RTT)/src \
-I$(SDK_ROOT)/inc\
-I$(LIB)


ASM_SOURCES +=\
$(RTT)/libcpu/WCH/Qingke_V4A/context_gcc.s \
$(RTT)/libcpu/WCH/Qingke_V4A/interrupt_gcc.s \
$(CORE)/Startup/startup_CH583.s


LDSCRIPT = $(CORE)/Ld/Link.ld

LIBS := -lISP583 -lCH58xBLE
LIBDIR = $(LIB)

PREFIX = riscv-none-embed-
CC = $(PREFIX)gcc
CPP = $(PREFIX)g++
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

CFLAGS += -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -DDEBUG=1 $(C_INCLUDES) -std=gnu99
ASFLAGS = -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -x assembler-with-cpp -DENABLE_INTERRUPT_NEST=0
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
ASFLAGS += -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

ifdef CONFIG_H
CFLAGS += -DCONFIG_H_FILE=\"$(CONFIG_H)\"
endif

LDFLAGS = -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -T
LDFLAGS += $(LDSCRIPT) -nostartfiles -Xlinker --gc-sections -L $(LIBDIR) -Xlinker --print-memory-usage -Wl,-Map,$(BUILD_DIR)/$(TARGET).map --specs=nano.specs --specs=nosys.specs -o $(TARGET).elf

all: clean  $(TARGET).elf	$(TARGET).hex

OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.S $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR):
	mkdir $@

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) $(CFLAGS)

$(BUILD_DIR)/%.o: %.S Makefile | $(BUILD_DIR)
	$(CC) $(ASFLAGS)

$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS)
	$(SZ) $@

$(TARGET).hex: 
	$(HEX) $(TARGET).elf $(TARGET).hex

clean:
	rm -rf $(BUILD_DIR) $(TARGET).elf $(TARGET).hex
