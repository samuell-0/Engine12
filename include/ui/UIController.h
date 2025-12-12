#pragma once
#include "core/States.hpp"
class UIController {
public:
    static UIData* ui;
    static void resize_V0(uint16_t dx);
    static void resize_V1(uint16_t dx);
    static void resize_H0(uint16_t dy);
    static void resize_H1(uint16_t dy);
    static void resize_V2(uint16_t dx);
    static void resize_V3(uint16_t dx);
    static void on_window_resize();
};
