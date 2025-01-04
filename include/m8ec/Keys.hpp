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

#include "m8ec/platform/IKeys.hpp"

namespace m8ec {

class Keys : public IKeys {
public:
    static Keys& getInstance();
    bool init() override final;
    State getState() override final;

private:
    Keys() = default;
    ~Keys() = default;
    Keys(const Keys&) = delete;
    Keys& operator=(const Keys&) = delete;
};

} // namespace m8ec
