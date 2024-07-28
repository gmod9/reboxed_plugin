#pragma once

#include <stdint.h>

namespace big
{
    struct sdk_color {

		sdk_color(int r_, int g_, int b_, int a_) {
			r = static_cast<uint8_t>(r_);
			g = static_cast<uint8_t>(g_);
			b = static_cast<uint8_t>(b_);
			a = static_cast<uint8_t>(a_);
		}

		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;
	};
    typedef void(*Msg_t)(const char* fmt, ...);
    typedef void(*ConColorMsg_t)(const sdk_color& color, const char* fmt, ...);

    extern Msg_t Msg;
	extern Msg_t Warning;
	extern ConColorMsg_t ConColorMsg;
    extern sdk_color pink;
} // namespace big
