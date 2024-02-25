/*
 * m8ec - Embedded Client for the Dirtywave M8 Headless device.
 * Copyright (C) 2023 Lukas Neverauskis https://github.com/lukasnee
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include "m8ec/periph/Uart4.hpp"

#include "stm32xxxx_hal.h"

#include <array>
#include <cstring>

extern UART_HandleTypeDef huart4;

namespace m8ec::periph {

Uart4 &Uart4::get_instance() {
    static Uart4 instance;
    return instance;
}

bool Uart4::ll_init() { return true; /* handled in usart.c */ }

bool Uart4::ll_async_read(std::uint8_t *data, std::size_t size) {
    if (HAL_OK != HAL_UART_Receive_DMA(&huart4, data, size)) {
        return false;
    }
    return true;
}

bool Uart4::ll_async_write(const std::uint8_t *data, std::size_t size) {
    if (HAL_OK != HAL_UART_Transmit_DMA(&huart4, data, size)) {
        return false;
    }
    return true;
}

bool Uart4::ll_deinit() { return true; /* handled in usart.c */ }

} // namespace m8ec::periph
