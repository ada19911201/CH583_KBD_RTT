/*
Copyright (C) 2018,2019 Jim Jiang <jim@lotlab.org>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "keymap_common.h"
#include "zoro_keycode.h"

const action_t actionmaps[][MATRIX_ROWS][MATRIX_COLS] = {
	KEYMAP(
		AC_GRV, AC_1, AC_2, AC_3, AC_4, AC_5, AC_6, AC_7, AC_8, AC_9, AC_0, AC_MINS, AC_EQL, AC_BSPC, 
		AC_TAB, AC_Q, AC_W, AC_W, AC_R, AC_T, AC_Y, AC_U, AC_I, AC_O, AC_P, AC_LBRC, AC_RBRC, AC_BSLS, 
		AC_CAPS, AC_A, AC_S, AC_D, AC_F, AC_G, AC_H, AC_J, AC_K, AC_L, AC_SCLN, AC_QUOT, AC_ENT, 
		AC_LSFT, AC_Z, AC_X, AC_C, AC_V, AC_B, AC_N, AC_M, AC_COMM, AC_DOT, AC_SLSH, AC_RSFT, 
		AC_LCTL, AC_LGUI, AC_LALT, AC_SPC, AC_RALT, AC_RGUI, AC_MENU, AC_RCTL),

	KEYMAP(
		AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, 
		AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, 
		AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, 
		AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, 
		AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS),

	KEYMAP(
		AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, 
		AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, 
		AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, 
		AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, 
		AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS),

	KEYMAP(
		AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, 
		AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, 
		AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, 
		AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, 
		AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS, AC_TRNS)
};

const macro_t *get_macro_user(uint8_t id)
{
	switch (id)
	{
	case 0:
		return MACRO(T(Q), T(Q), T(R), T(D), END);
		break;
	}
	return MACRO_NONE;
}