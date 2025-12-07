#include "ui/UIController.h"

void UIController::resize_separator_V0(UiData& ui, uint16_t dx)
{
    uint16_t middle_width = ui.separator_V1 - dx;
    if (middle_width >= ui.min_middle_width)
    {
        uint16_t left_width = dx;
        if (left_width >= 0)
        {
            ui.separator_V0 = dx;
            return;
        }
        if (left_width < 0)//litrally never happens
        {
            ui.separator_V0 = 0;
            return;
        }
    }
    if ((middle_width < ui.min_middle_width) && (middle_width > 0))
    {
        ui.separator_V0 = dx;
        resize_separator_V1(ui, dx + ui.min_middle_width);
        ui.separator_V0 = ui.separator_V1 - ui.min_middle_width;//if the obove failed, this will add nothing
        return;
    }
}
void UIController::resize_separator_V1(UiData& ui, uint16_t dx)
{
    uint16_t middle_width = dx - ui.separator_V0;
    if (middle_width >= ui.min_middle_width)
    {
        uint16_t right_width = ui.window_width - dx;
        if (right_width >= 0)
        {
            ui.separator_V1 = dx;
            return;
        }
        if (right_width < 0)
        {
            ui.separator_V1 = ui.window_width;
            return;
        }
    }
    if ((middle_width < ui.min_middle_width) && (middle_width > 0))
    {
        ui.separator_V1 = dx;
        resize_separator_V0(ui, dx - ui.min_middle_width);
        ui.separator_V1 = ui.separator_V0 + ui.min_middle_width;//if the obove failed, this will add nothing
        return;
    }

}
void UIController::resize_separator_H0(UiData& ui, uint16_t dy)
{

}
void UIController::resize_separator_H1(UiData& ui, uint16_t dy)
{

}
