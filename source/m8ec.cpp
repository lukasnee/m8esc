/*
 * m8ec - Embedded Client for the Dirtywave M8 Headless device.
 * Copyright (C) 2023 Lukas Neverauskis https://github.com/lukasnee
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include "m8ec/m8ec.h"
#include "m8ec/KeysThread.hpp"
#include "m8ec/m8ec.hpp"

#if defined(STM32H750xx)
#include "m8ec/periph/Uart4.hpp"
#elif defined(STM32F411xE)
#include "m8ec/periph/Uart1.hpp"
#endif
#include "m8ec/periph/UsbCdc.hpp"

#include "m8ec/display.hpp"
#include "m8ec/m8_protocol.hpp"

#include "fonas/fonas.hpp"

#include <cstdio>

namespace m8ec {

static bool init_hw_periphs() {
#if defined(STM32H750xx)
    if (!periph::Uart4::get_instance().init()) {
        LOG("error: periph::Uart4::get_instance().init failed\n");
        FONAS_PANIC();
        return false;
    }
    LOGD("UART4 OK\n");
#elif defined(STM32F411xE)
    if (!periph::Uart1::get_instance().init()) {
        LOG("error: periph::Uart1::get_instance().init failed\n");
        FONAS_PANIC();
        return false;
    }
    LOGD("UART1 OK\n");
#endif

    if (!periph::UsbCdc::get_instance().init()) {
        LOG("error: periph::UsbCdc::get_instance().init failed\n");
        FONAS_PANIC();
        return false;
    }
    return true;
}

static bool init_apps() {
    if (!display::initialize()) {
        LOG("error: display::initialize failed\n");
        FONAS_PANIC();
        return false;
    }
    LOGD("Display OK\n");

    // TODO: figure out why KeysThread makes system hang
    // if (!KeysThread::get_instance().init()) {
    //     LOG("error: KeysThread::get_instance().init failed\n");
    //     FONAS_PANIC();
    //     return false;
    // }
    // LOGD("KeysThread OK\n");

    if (!m8_protocol::init()) {
        LOG("error: m8_protocol::init\n");
        FONAS_PANIC();
        return false;
    }
    return true;
}

void launch() {
    if (!init_hw_periphs()) {
        LOG("error: init_hw_periphs failed\n");
        FONAS_PANIC();
        return;
    }
    if (!init_apps()) {
        LOG("error: init_apps failed\n");
        FONAS_PANIC();
        return;
    }
}

} // namespace m8ec

extern "C" void m8ec_launch() { m8ec::launch(); }
