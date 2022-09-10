#include "action.h"
#include "action_layer.h"
#include "action_util.h"
#include "process_record_kb.h"
#include "keyboard.h"
#include "keycode.h"
#include "zoro_keycode.h"
#include "action_macro.h"
#include "keyboard_event.h"
#ifdef RGB_BOTTOM_ENABLE
#include "rgblight.h"
#endif
#ifdef RGB_TOP_ENABLE
#include "rgb_matrix.h"
#endif
#include "eeconfig.h"
#include "wait.h"
#ifdef RF24G_ENABLE
#include "nrf_24g.h"
#endif

bool process_record_kb(keyrecord_t *record, action_t keycode)
{

}
