#ifndef ZORO_KEYCODE_H
#define ZORO_KEYCODE_H
/*
    ACT_MODS            = 0b0000,
    ACT_LMODS           = 0b0000,
    ACT_RMODS           = 0b0001,
    ACT_MODS_TAP        = 0b0010,
    ACT_LMODS_TAP       = 0b0010,
    ACT_RMODS_TAP       = 0b0011,
    ACT_USAGE           = 0b0100,
    ACT_MOUSEKEY        = 0b0101,

    NOT_DEFINE          = 0b0110|xxxx|xxxx|xxxx   
    NOT_DEFINE          = 0b0111|xxxx|xxxx|xxxx

    ACT_LAYER           = 0b1000,
    NOT_DEFINE          = 0b1001|xxxx|xxxx|xxxx
    ACT_LAYER_TAP       = 0b1010, 
    ACT_LAYER_TAP_EXT   = 0b1011, 
    ACT_MACRO           = 0b1100,
    ACT_BACKLIGHT       = 0b1101,
    ACT_COMMAND         = 0b1110,
    ACT_FUNCTION        = 0b1111
*/
enum zoro_keycodes
{
    MACRO_0 = 0x6000,    //不使用tmk的宏定义
    MACRO_1,
    MACRO_2,
    MACRO_3,
    MACRO_4,
    MACRO_5,
    MACRO_6,
    MACRO_7,
    MACRO_8,
    MACRO_9,
    MACRO_10,
    MACRO_11,
    MACRO_12,
    MACRO_13,
    MACRO_14,
    MACRO_15,
    MACRO_16,
    MACRO_17,
    MACRO_18,
    MACRO_19,
    MACRO_20,
    MACRO_21,
    MACRO_22,
    MACRO_23,
    MACRO_24,
    MACRO_25,
    MACRO_26,
    MACRO_27,
    MACRO_28,
    MACRO_29,
    MACRO_30,
    MACRO_31,

    RGB_B_TOG = 0x6040, //开启或关闭
    RGB_B_STEP,        //模式+
    RGB_B_STEP_R,      //模式-
    RGB_B_HUI,         //色相+
    RGB_B_HUD,         //色相-
    RGB_B_SAI,         //饱和+
    RGB_B_SAD,         //饱和-
    RGB_B_VAI,         //亮度+
    RGB_B_VAD,         //亮度-

    //0X0400 --- 0X04FF       256            RGB轴灯控制键值
    RGB_M_TOG = 0x6050, //开启或关闭
    RGB_M_STEP,        //模式+
    RGB_M_STEP_R,      //模式-
    RGB_M_HUI,         //色相+
    RGB_M_HUD,         //色相-
    RGB_M_SAI,         //饱和+
    RGB_M_SAD,         //饱和-
    RGB_M_VAI,         //亮度+
    RGB_M_VAD,         //亮度-
    RGB_M_SPI,         //Speed+
    RGB_M_SPD,         //Speed-

    CUT_AUTO = 0x6060,
    SW_BLE_1,
    SW_BLE_2,
    SW_BLE_3,
    BLE_AD_RESET,
    CUT_USB,
    CUT_24G,
    CUT_BLE,
    CLEAN_BIND,
    RESET_KB,
    SW_NKRO,
    KB_OFF,
    KB_SLEEP,
    KB_DFU,
    PRT_BAT,
    REVICE_24G_DFU,
    SLEEP_TIME_I,
    SLEEP_TIME_D,
    SW_24G_0,
    SW_24G_1,
    SW_24G_2,
    SW_24G_3,
    SW_24G_4,
    SW_24G_5,
    SW_24G_6,
    SW_24G_7,
    CHAN_24G_0,
    CHAN_24G_1,
    CHAN_24G_2,
    CHAN_24G_3,
    CHAN_24G_4,

    F_ESC = 0x60F0,
    F_BSPACE
};

#endif /* KEYCODE_H */
