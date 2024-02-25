/*
 * m8ec - Embedded Client for the Dirtywave M8 Headless device.
 * Copyright (C) 2023 Lukas Neverauskis https://github.com/lukasnee
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#if defined(STM32H750xx)
#include "m8ec/periph/Uart4.hpp"
#elif defined(STM32F411xE)
#include "m8ec/periph/Uart1.hpp"
#endif

#include "stm32xxxx_hal.h"

#if defined(STM32H750xx)
extern UART_HandleTypeDef huart4;
#elif defined(STM32F411xE)
extern UART_HandleTypeDef huart1;
#endif

extern "C" void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
#if defined(STM32H750xx)
    if (huart == &huart4) {
        m8ec::periph::Uart4::get_instance().ll_async_write_completed_cb_from_isr();
    }
#elif defined(STM32F411xE)
    if (huart == &huart1) {
        m8ec::periph::Uart1::get_instance().ll_async_write_completed_cb_from_isr();
    }
#endif
}

extern "C" void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
#if defined(STM32H750xx)
    if (huart == &huart4) {
        m8ec::periph::Uart4::get_instance().ll_async_read_completed_cb_from_isr();
    }
#elif defined(STM32F411xE)
    if (huart == &huart1) {
        m8ec::periph::Uart1::get_instance().ll_async_read_completed_cb_from_isr();
    }
#endif
}
