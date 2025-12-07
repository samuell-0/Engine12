#pragma once
#include "core/States.hpp"
class UIController {
public:
    static void resize_separator_V0(UiData& ui, uint16_t dx);
    static void resize_separator_V1(UiData& ui, uint16_t dx);
    void resize_separator_H0(UiData& ui, uint16_t dy);
    void resize_separator_H1(UiData& ui, uint16_t dy);
};
