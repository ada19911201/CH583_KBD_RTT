#include "keymap_common.h"

#ifdef IS31FL3733
#    include "is31fl3733.h"
#elif defined(IS31FL3737)
#    include "is31fl3737.h"
#elif defined(IS31FL3741)
#    include "is31fl3741.h"
#endif

#ifdef RGB_TOP_ENABLE
#include "rgb_matrix_types.h"
const is31_led g_is31_leds[DRIVER_LED_TOTAL] = {
    /* Refer to IS31 manual for these locations
     *   driver
     *   |    R location
     *   |    |     G location
     *   |    |     |     B location
     *   |    |     |     | */
        //0代表第一个RGB驱动芯片 1代表第二个RGB驱动芯片
//ROW0
        { 0, K_15, J_15, L_15},
        { 0, K_16, J_16, L_16},
        { 0, H_14, G_14, I_14},
        { 0, H_15, G_15, I_15},
        { 0, H_16, G_16, I_16},
        { 0, E_15, D_15, F_15},
        { 0, E_16, D_16, F_16},
        { 0, B_15, A_15, C_15},
        { 0, B_16, A_16, C_16},
        { 0, B_10, A_10, C_10},
        { 0, B_11, A_11, C_11},
        { 0, B_12, A_12, C_12},
        { 0, B_13, A_13, C_13},
        { 0, B_14, A_14, C_14},
//ROW1
        { 0, K_1 , J_1 , L_1 },
        { 0, K_2 , J_2 , L_2 },
        { 0, K_3 , J_3 , L_3 },
        { 0, K_4 , J_4 , L_4 },
        { 0, K_5 , J_5 , L_5 },
        { 0, K_6 , J_6 , L_6 },
        { 0, K_7 , J_7 , L_7 },
        { 0, K_8 , J_8 , L_8 },
        { 0, K_9 , J_9 , L_9 },
        { 0, K_10, J_10, L_10},
        { 0, K_11, J_11, L_11},
        { 0, K_12, J_12, L_12},
        { 0, K_13, J_13, L_13},
        { 0, K_14, J_14, L_14},
//ROW2
        { 0, H_1 , G_1 , I_1 },
        { 0, H_2 , G_2 , I_2 },
        { 0, H_3 , G_3 , I_3 },
        { 0, H_4 , G_4 , I_4 },
        { 0, H_5 , G_5 , I_5 },
        { 0, H_6 , G_6 , I_6 },
        { 0, H_7 , G_7 , I_7 },
        { 0, H_8 , G_8 , I_8 },
        { 0, H_9 , G_9 , I_9 },
        { 0, H_10, G_10, I_10},
        { 0, H_11, G_11, I_11},
        { 0, H_12, G_12, I_12},
        { 0, H_13, G_13, I_13},
//ROW3
        { 0, E_1 , D_1 , F_1 },
        { 0, E_2 , D_2 , F_2 },
        { 0, E_3 , D_3 , F_3 },
        { 0, E_4 , D_4 , F_4 },
        { 0, E_5 , D_5 , F_5 },
        { 0, E_6 , D_6 , F_6 },
        { 0, E_7 , D_7 , F_7 },
        { 0, E_8 , D_8 , F_8 },
        { 0, E_9 , D_9 , F_9 },
        { 0, E_10, D_10, F_10},
        { 0, E_11, D_11, F_11},

        { 0, E_13, D_13, F_13},

//ROW4
        { 0, B_1 , A_1 , C_1 },
        { 0, B_2 , A_2 , C_2 },
        { 0, B_3 , A_3 , C_3 },
        { 0, B_4 , A_4 , C_4 },
        { 0, B_5 , A_5 , C_5 },
        { 0, B_6 , A_6 , C_6 },
        { 0, B_8 , A_8 , C_8 },

        { 0, B_9 , A_9 , C_9 },
};

led_config_t g_led_config = {
        {
            { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13},
            { 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28 },
            { 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, NO_LED, 41 },
            { 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, NO_LED, 53, NO_LED },
            { 54, 55, 56, NO_LED, NO_LED, 57, NO_LED, NO_LED, NO_LED, 58, 59, 60, 61 }
        },
        {
            { 0, 0 },{ 17, 0 },{ 34, 0 },{ 52, 0 },{ 69, 0 },{ 86, 0 },{ 103, 0 },{ 120, 0 },{ 138, 0 },{ 155, 0 },{ 172, 0 },{ 189, 0 },{ 206, 0 },{ 224, 0 },
            { 0, 16},{ 17, 16},{ 34, 16},{ 52, 16},{ 69, 16},{ 86, 16},{ 103, 16},{ 120, 16},{ 138, 16},{ 155, 16},{ 172, 16},{ 189, 16},{ 206, 16},{ 224, 16},
            { 0, 32},{ 17, 32},{ 34, 32},{ 52, 32},{ 69, 32},{ 86, 32},{ 103, 32},{ 120, 32},{ 138, 32},{ 155, 32},{ 172, 32},{ 189, 32},           { 224, 32},
            { 0, 48},{ 17, 48},{ 34, 48},{ 52, 48},{ 69, 48},{ 86, 48},{ 103, 48},{ 120, 48},{ 138, 48},{ 155, 48},{ 178, 48},                      { 224, 48}, 
            { 0, 64},{ 17, 64},{ 34, 64},                    { 86, 64},                                 { 155, 64},{ 178, 64},           { 201, 64},{ 224, 64}
        },
        {
            1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
            1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
            4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,    4,
            1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,       4,
            1, 4, 4,       4,          4,  4,  4,  4
        }
};

#endif
