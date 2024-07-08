#include "m8ec/periph/UsbCdc.hpp"

#include "m8ec/m8ec.hpp"

#include "stm32xxxx_hal.h"
#include "usb_host.h"
#include "usbh_cdc.h"

#include <algorithm>

extern USBH_HandleTypeDef hUsbHostFS; // TODO decouple

namespace m8ec::periph {

UsbCdc &UsbCdc::get_instance() {
    static UsbCdc instance(m8ec::Config::usb_cdc_rx_stream_buffer_size);
    return instance;
}

UsbCdc::UsbCdc(UBaseType_t rx_stream_buffer_size) : rx_stream_buffer(rx_stream_buffer_size, sizeof(char)) {}

bool UsbCdc::ready() const { return this->initialized && m8ec_virtual_com_ready(); }

std::uint8_t UsbCdc::read() {
    std::uint8_t byte = 0;
    this->rx_stream_buffer.receive(&byte, sizeof(byte), portMAX_DELAY);
    return byte;
}

std::size_t UsbCdc::read(std::uint8_t *buffer, std::size_t bufferSize) {
    return this->rx_stream_buffer.receive(buffer, bufferSize, portMAX_DELAY);
}

bool UsbCdc::ll_init() {
    this->initialized = true;
    return true; /* hUsbHostFS is expected to be initialized in main.c */
}

bool UsbCdc::ll_async_write(const std::uint8_t *data, std::size_t size) {
    if (!data) {
        return false;
    }
    if (size == 0) {
        return false;
    }
    if (!this->ready()) {
        return false;
    }
    const auto status = USBH_CDC_Transmit(&hUsbHostFS, const_cast<std::uint8_t *>(data), size);
    if (USBH_OK != status) {
        LOG("error: USBH_CDC_Transmit: %d\n", status);
        return false;
    }
    return true;
}

bool UsbCdc::ll_deinit() {
    this->initialized = false;
    return true; /* handled in main.c */
}

bool UsbCdc::ll_rx_input(const uint8_t *data, size_t size) {
    if (!data) {
        return false;
    }
    if (size == 0) {
        return false;
    }
    if (!this->ready()) {
        return false;
    }
    if (size != this->rx_stream_buffer.send(data, size, portMAX_DELAY)) {
        LOG("rx_stream_buffer.send failed\n");
        return false;
    }
    return true;
}

} // namespace m8ec::periph

extern "C" void USBH_CDC_TransmitCallback(USBH_HandleTypeDef *phost) {
    if (phost == &hUsbHostFS) {
        m8ec::periph::UsbCdc::get_instance().ll_async_write_completed_cb();
    }
}
#include "m8ec/slip.h"
#include "m8ec/m8_protocol.hpp"
namespace m8ec::m8_protocol {
extern slip_handler_s slip;
} // namespace m8ec::m8_protocol


extern "C" void USBH_CDC_ReceiveCallback(USBH_HandleTypeDef *phost, const uint8_t *data, uint32_t size) {
    if (phost == &hUsbHostFS) {
        // for (std::size_t i = 0; i < size; i++) {
        //         const slip_error_t n = slip_read_byte(&m8ec::m8_protocol::slip, data[i]);
        //         if (n != SLIP_NO_ERROR) {
        //             if (m8ec::Config::debug_slip) {
        //                 LOG("Error: SLIP: %d\n", n);
        //             }
        //             if (n == SLIP_ERROR_INVALID_PACKET) {
        //                 m8ec::m8_protocol::reset_display();
        //             }
        //         }
        //     }
        //     return;
        m8ec::periph::UsbCdc::get_instance().ll_rx_input(data, size);
    }
}
