# 为 Zoro64 生成
TARGET = Zora61

BUILD_DIR =	build
# 此工程的根目录
ROOT_DIR := ../..
CORE := $(ROOT_DIR)/Core
RTT := $(ROOT_DIR)/RTTHREAD
TMK := $(ROOT_DIR)/TMK
SDK_ROOT := $(ROOT_DIR)/StdPeriphDriver
LIB := $(ROOT_DIR)/LIB
DRIVERS := $(ROOT_DIR)/DRIVERS
APP := $(ROOT_DIR)/APP
BLE := $(ROOT_DIR)/BLE

CONFIG_H = config.h

C_SOURCES += keymap_plain.c 	
C_SOURCES += keymap_rgb.c

C_INCLUDES += -I.

include rules.mk
include $(TMK)/tmk.mk