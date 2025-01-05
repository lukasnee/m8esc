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

#pragma once

#include "m8ec/m8_protocol.hpp"

#include <cstddef>

namespace m8ec::display {
bool initialize();
void draw_waveform(const m8_protocol::Waveform &waveform, uint16_t waveform_width);
void draw_rectangle(const m8_protocol::Rectangle &rectangle);
int draw_character(const m8_protocol::Character &character);
void set_large_mode(int enabled);
void view_changed(int view);
void draw_string(const char *str, uint16_t x = 0, uint16_t y = 0);
} // namespace m8ec::display
