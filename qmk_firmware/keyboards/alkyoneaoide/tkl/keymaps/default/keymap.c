#include QMK_KEYBOARD_H

#define TAP_TIME_FN_CAPS 200

enum layers {
	qwerty = 0,
	qwerty_media,
	fn,
	caps_fn
};

enum custom_keycodes {
    FN_CAPS = SAFE_RANGE
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

	[qwerty] = LAYOUT_tkl(
	     KC_ESC,            KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,  KC_F12, KC_PSCR, KC_SCRL, KC_PAUS,
	     KC_GRV,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_MINS,  KC_EQL, KC_BSPC,  KC_INS, KC_HOME, KC_PGUP,
	     KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P, KC_LBRC, KC_RBRC, KC_BSLS,  KC_DEL,  KC_END, KC_PGDN,
        FN_CAPS,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_QUOT,           KC_ENT,                           
	    KC_LSFT,             KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH, KC_RSFT,                     KC_UP,         
		KC_LCTL, KC_LGUI, KC_LALT,                             KC_SPC,                            KC_RALT,  MO(fn),  KC_APP, KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT
	),

	[qwerty_media] = LAYOUT_tkl(
		_______,          KC_BRID, KC_BRIU, KC_SLEP, KC_WAKE, KC_MPLY, KC_MSTP, KC_MPRV, KC_MNXT, KC_MUTE, KC_VOLD, KC_VOLU, KC_EJCT, _______, _______, _______,
		_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
		_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
		_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,                           
		_______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,                   _______,         
		_______, _______, _______,                            _______,                            _______, _______, _______, _______, _______, _______,  _______
	),

	[fn] = LAYOUT_tkl(
       TG(qwerty_media),          KC_BRID, KC_BRIU, KC_SLEP, KC_WAKE, KC_MPLY, KC_MSTP, KC_MPRV, KC_MNXT, KC_MUTE, KC_VOLD, KC_VOLU, KC_EJCT, _______, _______, _______,
                _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,  KC_DEL, _______, _______, _______,
                _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,                           
                _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,                   KC_PGUP,         
                _______, _______, _______,                            _______,                            _______, _______, _______, _______, KC_HOME, KC_PGDN,  KC_END 
	),

	[caps_fn] = LAYOUT_tkl(
	            _______,          _______, _______, _______, _______,    _______, _______, _______, _______, _______, _______,    _______, _______,    _______,    _______,     _______,
                _______, _______, _______, _______, _______, _______,    _______, _______, _______, _______, _______, _______,    _______,  KC_DEL,    _______,    _______,     _______,
                _______, _______,   KC_UP, _______, _______, _______,    _______, _______, _______, _______, _______, _______,    _______, _______,    _______,    _______,     _______,
                KC_CAPS, KC_LEFT, KC_DOWN, KC_RGHT, _______, _______,    _______, _______, _______, _______, _______, _______,             _______,                                     
                _______,          _______, _______, _______, _______,    _______, _______, _______, _______, _______, _______,    _______,                        KC_MS_UP,             
                _______, _______, _______,                            KC_MS_BTN1,                            _______, _______, KC_MS_BTN2, _______, KC_MS_LEFT, KC_MS_DOWN, KC_MS_RIGHT 
	)
};

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
    static uint16_t fn_caps_timer;
    static bool fn_caps_other_press = false;

    switch(keycode) {
        case FN_CAPS:
            if (record->event.pressed) {
                fn_caps_timer = timer_read();
                fn_caps_other_press = false;
                layer_on(caps_fn);
            } else {
                layer_off(caps_fn);

                if (!fn_caps_other_press) {
                    if (timer_elapsed(fn_caps_timer) < TAP_TIME_FN_CAPS) {
                        if (host_keyboard_led_state().caps_lock) {
                            tap_code(KC_CAPS);
                        }

                        caps_word_toggle();
                    } else {
                        if (is_caps_word_on()) {
                            caps_word_off();
                        }

                        tap_code(KC_CAPS);
                    }
                }
            }

            return false;
        default:
            if (record->event.pressed && IS_LAYER_ON(caps_fn)) {
                fn_caps_other_press = true;
            }
    }

    return true;
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    // PURPLE - momentary layers
    // GOLD - toggled transparent layers
    // RED - locked keys

    static uint8_t caps_fn_layer_leds[] = {19, 35, 62, 61, 60, 59, 75, 83, 78, 77, 76};
    static uint8_t fn_layer_leds[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 19, 75, 78, 77, 76};
    static uint8_t qwerty_media_layer_leds[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

    RGB red = hsv_to_rgb((HSV){0, 255, RGB_MATRIX_MAXIMUM_BRIGHTNESS});
    RGB gold = hsv_to_rgb((HSV){36, 255, RGB_MATRIX_MAXIMUM_BRIGHTNESS});
    RGB purple = hsv_to_rgb((HSV){191, 255, RGB_MATRIX_MAXIMUM_BRIGHTNESS});

    uint8_t current_layer = get_highest_layer(layer_state);

    switch(current_layer) {
        case caps_fn:
            for (uint8_t i = 0; i < sizeof(caps_fn_layer_leds); i++) {
                rgb_matrix_set_color(caps_fn_layer_leds[i], purple.r, purple.g, purple.b);
            }
            break;
        case fn:
            for (uint8_t i = 0; i < sizeof(fn_layer_leds); i++) {
                rgb_matrix_set_color(fn_layer_leds[i], purple.r, purple.g, purple.b);
            }
            break;
        case qwerty_media:
            for (uint8_t i = 0; i < sizeof(qwerty_media_layer_leds); i++) {
                rgb_matrix_set_color(qwerty_media_layer_leds[i], gold.r, gold.g, gold.b);
            }
            break;
        default:
            break;
    }

    if (host_keyboard_led_state().caps_lock) {
        rgb_matrix_set_color(62, red.r, red.g, red.b);
    } else if (is_caps_word_on()) {
        rgb_matrix_set_color(62, gold.r, gold.g, gold.b);
    }

    if (host_keyboard_led_state().scroll_lock) {
        rgb_matrix_set_color(14, red.r, red.g, red.b);
    }

    return true;
}

