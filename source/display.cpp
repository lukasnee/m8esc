/*
 * m8ec - Embedded Client for the Dirtywave M8 Headless device.
 * Copyright (C) 2023 Lukas Neverauskis https://github.com/lukasnee
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

// Copyright 2021 Jonne Kokkonen
// Released under the MIT licence, https://opensource.org/licenses/MIT

#include "m8ec/m8ec.h"

#include "m8ec/display.hpp"
#include "m8ec/font.hpp"
#include "m8ec/m8_protocol.hpp"
#include "m8ec/periph/Spi1.hpp"

#include "ILI9341/ili9341_gfx.h"
#include "main.h"

#include <algorithm>
#include <array>
#include <cstdio>

extern SPI_HandleTypeDef hspi1; // main.c

static ili9341_t *ili9341_lcd = nullptr;

ili9341_color_t bg_color = ILI9341_BLACK;

namespace m8ec::display {

bool initialize() {
    ili9341_lcd = ili9341_new(
        &hspi1,
        [](uint8_t *data, uint16_t size) -> uint16_t { return m8ec::periph::Spi1::get_instance().write(data, size) ? 1 : 0; },
        [](uint8_t *rd_data, const uint8_t *wr_data, uint16_t size) -> uint16_t {
            return m8ec::periph::Spi1::get_instance().read_write(rd_data, wr_data, size) ? 1 : 0;
        },
        TFT_RESET_GPIO_Port, TFT_RESET_Pin, TFT_CS_GPIO_Port, TFT_CS_Pin, TFT_DC_GPIO_Port, TFT_DC_Pin, isoLandscape, NULL, 0,
        NULL, 0, itsNONE, itnNONE);
    if (!ili9341_lcd) {
        LOG("error: ili9341_new failed\n");
        return false;
    }
    ili9341_fill_screen(ili9341_lcd, bg_color);
    return true;
}

void set_large_mode(int enabled) { LOG("error: set_large_mode: %d: not implemented\n", enabled); }

int draw_character(const m8_protocol::Character &character) {
    if (!ili9341_lcd) {
        return -1;
    }
    ili9341_text_attr_t textAttr;
    textAttr.bg_color = __ILI9341_COLOR565(character.background.r, character.background.g, character.background.b);
    textAttr.fg_color = __ILI9341_COLOR565(character.foreground.r, character.foreground.g, character.foreground.b);
    textAttr.font = &font::trash80_stealth57;
    textAttr.origin_x = character.pos.x;
    constexpr decltype(textAttr.origin_y) char_to_rect_y_adjustment = 3U;
    textAttr.origin_y = character.pos.y + char_to_rect_y_adjustment;
    const auto c = static_cast<char>(character.c);
    ili9341_draw_char(ili9341_lcd, textAttr, c);
    return character.c;
}

void draw_rectangle(const m8_protocol::Rectangle &rectangle) {
    if (!ili9341_lcd) {
        return;
    }
    ili9341_color_t color = __ILI9341_COLOR565(rectangle.color.r, rectangle.color.g, rectangle.color.b);
    ili9341_fill_rect(ili9341_lcd, color, rectangle.pos.x, rectangle.pos.y, rectangle.size.w, rectangle.size.h);
    if (rectangle.size.h >= ili9341_lcd->screen_size.height || rectangle.size.w >= ili9341_lcd->screen_size.width) {
        bg_color = color; // remember the screen clear color
    }
}

struct Canvas {
    uint16_t x;
    uint16_t y;
    uint16_t w;
    uint16_t h;
};

constexpr auto canvas_max = Canvas{0, 0, 320, 21};
std::array<uint8_t, (canvas_max.w * canvas_max.h / 8)> bmp_buff = {0};

void draw_waveform(const m8_protocol::Waveform &waveform, uint16_t waveform_width) {
    if (!ili9341_lcd) {
        return;
    }
    if (waveform_width > canvas_max.w) {
        LOG("warning: draw_waveform: waveform_width: %zu: too large\n", waveform_width);
        waveform_width = canvas_max.w; // limit the width
    }

    static uint16_t last_waveform_width = 0;
    static bool was_blank = false;

    const bool is_blank = 0 == waveform_width;
    if (is_blank && was_blank) {
        return; // skip
    }
    const auto canvas_w = is_blank ? last_waveform_width : waveform_width;
    if(canvas_w > canvas_max.w) {
        LOG("warning: draw_waveform: canvas_w: %u: too large\n", canvas_w);
        return;
    }
    const auto canvas_x = static_cast<uint16_t>(canvas_max.w - canvas_w);
    const auto canvas = Canvas{canvas_x, canvas_max.y, canvas_w, canvas_max.h};
    last_waveform_width = waveform_width;
    bmp_buff.fill(0);
    for (std::size_t i = 0; i < waveform_width; i++) {
        // limit the waveform value (y) to canvas max height - allegedly it can glitch // TODO investigate
        const auto y = std::min(waveform.buffer[i], static_cast<uint8_t>(canvas.h - 1));
        const auto x = i;
        const auto bmp_index = (y * waveform_width) + x;
        const auto byte_index = bmp_index / 8;
        const auto byte_bit = bmp_index % 8;
        bmp_buff[byte_index] |= 1 << (7 - byte_bit);
    }
    const ili9341_color_t fg_color = __ILI9341_COLOR565(waveform.color.r, waveform.color.g, waveform.color.b);
    ili9341_draw_bitmap_1b(ili9341_lcd, fg_color, bg_color, canvas.x, canvas.y, canvas.w, canvas.h, bmp_buff.data());
    was_blank = is_blank;
}

void draw_string(const char *str, uint16_t x, uint16_t y) {
    if (!ili9341_lcd) {
        return;
    }
    ili9341_text_attr_t textAttr;
    textAttr.bg_color = bg_color;
    textAttr.fg_color = ILI9341_WHITE;
    textAttr.font = &font::trash80_stealth57;
    textAttr.origin_x = x;
    textAttr.origin_y = y;
    ili9341_draw_string(ili9341_lcd, textAttr, const_cast<char *>(str));
}
} // namespace m8ec::display
