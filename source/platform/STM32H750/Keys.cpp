#include "m8ec/Keys.hpp"

#include "fonas/fonas.hpp"

#include "stm32h7xx_hal.h"

#define ARRAY_SZ(x) (sizeof(x) / sizeof((x)[0]))

namespace m8ec {

struct Config {
    static constexpr std::size_t gpio_transient_delay_ms = 1;
};

const uint16_t rows[] = {GPIO_PIN_13, GPIO_PIN_1, GPIO_PIN_0};
const uint16_t columns[] = {GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7};

const IKeys::Key key_map[ARRAY_SZ(columns)][ARRAY_SZ(rows)] = {
    {IKeys::Key::none, IKeys::Key::left, IKeys::Key::none},
    {IKeys::Key::up, IKeys::Key::down, IKeys::Key::shift},
    {IKeys::Key::option, IKeys::Key::right, IKeys::Key::play},
    {IKeys::Key::edit, IKeys::Key::none, IKeys::Key::none},
};

Keys &Keys::getInstance() {
    static Keys instance;
    return instance;
}

bool Keys::init() {

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOB_CLK_ENABLE();

    // key rows
    GPIO_InitStruct.Pin = GPIO_PIN_13 | GPIO_PIN_1 | GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // key columns
    GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    return true;
}

IKeys::State Keys::getState() {
    State state = {};
    // keyboard matrix with diodes
    for (size_t cIdx = 0; cIdx < ARRAY_SZ(columns); ++cIdx) {
        HAL_GPIO_WritePin(GPIOB, columns[cIdx], GPIO_PIN_RESET);
        fonas::delay_ms(Config::gpio_transient_delay_ms);
        for (size_t rIdx = 0; rIdx < ARRAY_SZ(rows); ++rIdx) {
            const bool is_pressed = HAL_GPIO_ReadPin(GPIOB, rows[rIdx]) == GPIO_PIN_RESET;
            state.set(key_map[cIdx][rIdx], is_pressed);
        }
        HAL_GPIO_WritePin(GPIOB, columns[cIdx], GPIO_PIN_SET);
    }
    return state;
}

} // namespace m8ec
