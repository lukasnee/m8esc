/*
 * m8ec - Embedded Client for the Dirtywave M8 Headless device.
 * Copyright (C) 2023 Lukas Neverauskis https://github.com/lukasnee
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include "fonas/EventDrivenReaderWriter.hpp"

namespace m8ec::periph {

class Uart4 : public fonas::EventDrivenReaderWriter {

public:
    static Uart4 &get_instance();

protected:
    bool ll_init() final override;
    bool ll_async_read(std::uint8_t *data, std::size_t size) final override;
    bool ll_async_write(const std::uint8_t *data, std::size_t size) final override;
    bool ll_deinit() final override;
};

} // namespace m8ec::periph
