/*
Base Copyright 2017 Luiz Ribeiro <luizribeiro@gmail.com>
Modified 2017 Andrew Novak <ndrw.nvk@gmail.com>
Modified 2018 Wayne Jones (WarmCatUK) <waynekjones@gmail.com>
Modified 2019 AbstractKB
Modified 2019 JMMJ

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public LicensezZZ
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H
#include "action_layer.h"
#include "rgblight.h"
#include "quantum.h"

enum layers {
  _DEFLT,
  _RAISE,
  _LOWER,
  _FN
};

enum {
  TD_LALT = 0
};

qk_tap_dance_action_t tap_dance_actions[] = {
    //Tap once for Alt, twice for Alt Gr
  [TD_LALT]  = ACTION_TAP_DANCE_DOUBLE(KC_LALT, KC_RALT)
};

enum custom_keycodes {
  MYRGB_TG = SAFE_RANGE
};

bool rgbinit = true;
bool rgbon = true;

const uint8_t RGBLED_RAINBOW_SWIRL_INTERVALS[] PROGMEM = {1,5,5}; //only using the first one

void keyboard_post_init_user(void) {
  rgblight_enable_noeeprom();
  led_set_user(host_keyboard_leds());
}

uint32_t layer_state_set_user(uint32_t state) {
  switch (biton32(state)) {
    case _RAISE:
      rgblight_sethsv_noeeprom(240,255,255);
      rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
      break;
    case _LOWER:
      rgblight_sethsv_noeeprom(0,255,255);
      rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
      break;
    case _FN:
      rgblight_sethsv_noeeprom(0,255,255);
      rgblight_mode_noeeprom(RGBLIGHT_MODE_RAINBOW_SWIRL);
      break;
    default: //_DEFLT
      rgblight_sethsv_noeeprom(0,0,255);
      rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
      break;
  }
  
  return state;
}

void led_set_user(uint8_t usb_led) {
  if (usb_led & (1<<USB_LED_CAPS_LOCK)) {
    rgblight_mode_noeeprom(RGBLIGHT_MODE_ALTERNATING);
  } else {
    layer_state_set_user(layer_state);
  }
}

void myrgb_toggle(void) {
  if (rgbon) {
    rgblight_disable_noeeprom();
    rgbon = false;
  } else {
    rgblight_enable_noeeprom();
    layer_state_set_user(layer_state);
    led_set_user(host_keyboard_leds());
    rgbon = true;
  }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch(keycode) {
    case MYRGB_TG:
      if (record->event.pressed) {
        myrgb_toggle();
      }
      return false;
    default:
      return true;
  }
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [_DEFLT] = LAYOUT( \
        KC_GESC,	 KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,  KC_7,  KC_8,  KC_9,  KC_0,  KC_BSPC,       \
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_DEL,      \
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,      \
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_UP, SFT_T(KC_ENT),		\
        KC_LCTL, KC_LGUI, TD(TD_LALT) , MO(_FN), MO(_LOWER),KC_SPC,KC_SPC,MO(_RAISE),KC_SLSH, KC_LEFT, KC_DOWN, KC_RGHT       \
    ),

	
    [_RAISE] = LAYOUT( \
       KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_MINUS, KC_EQUAL , _______ ,  \
       KC_F10, KC_F11, KC_F12, _______, _______, _______, _______, _______ , _______  ,KC_LBRACKET ,KC_RBRACKET , KC_BSLASH,  \
       _______, _______, _______, _______, _______, _______, _______, _______ , _______ , _______ , _______ ,KC_BRIU ,  \
       _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_MPLY, KC_BRID,  \
       _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_VOLD, KC_MEDIA_STOP, KC_VOLU\
    ),

	
    [_LOWER] = LAYOUT( \
       KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10, KC_F11, KC_F12,  \
       RGB_HUI, RGB_SAI, RGB_VAI, RGB_TOG , _______ , _______, _______, _______, _______, _______, KC_PSCREEN,  _______,    \
        _______, _______,  _______, _______, _______, _______, _______, _______,KC_HOME,KC_PGUP, _______,  _______,    \
        _______, _______, _______, _______, _______, _______, _______, _______, KC_END, KC_PGDOWN, BL_ON , RGB_TOG, \
        _______, _______, _______, _______, _______, _______, _______, _______, _______, BL_DEC, BL_OFF ,  BL_INC\
    ),
    
	
    [_FN] = LAYOUT( \
       RESET, _______, _______, _______, _______, _______, _______, _______, KC_NLCK, KC_PSLS, KC_PAST, KC_PMNS, \
       _______, _______, _______, _______, _______, _______, _______, _______, KC_P7, KC_P8 , KC_P9, KC_PPLS, \
       _______, _______, _______, _______, _______, _______, _______, _______, KC_P4, KC_P5, KC_P6, KC_PPLS, \
       _______, _______, _______, _______, _______, _______, _______, _______, KC_P1, KC_P2, KC_P3,KC_PENT, \
       _______, _______, _______, _______, _______, _______, _______, _______, KC_P0, KC_PDOT, _______, KC_PENT\
    )
};



