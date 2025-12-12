#include "ui/UIController.h"
UIData* UIController::ui = nullptr;
void UIController::resize_V0(uint16_t dx)
{
    int16_t middle_width = ui->V1 - dx;
    if (middle_width >= ui->min_middle_width)
    {
        int16_t left_width = dx;
        if (left_width >= 0)
        {
            ui->V0 = dx;
            return;
        }
        if (left_width < 0)//litrally never happens
        {
            ui->V0 = 0;
            return;
        }
    }
    if ((middle_width < ui->min_middle_width) && (middle_width > 0))
    {
        resize_V1(dx + ui->min_middle_width);
        ui->V0 = ui->V1 - ui->min_middle_width;//if the obove failed, this will add nothing
        return;
    }
}
void UIController::resize_V1(uint16_t dx)
{
    int16_t middle_width = dx - ui->V0;
    if (middle_width >= ui->min_middle_width)
    {
        int16_t right_width = ui->window_width - dx;
        if (right_width >= 0)
        {
            ui->V1 = dx;
            return;
        }
        if (right_width < 0)
        {
            ui->V1 = ui->window_width;
            return;
        }
    }
    if ((middle_width < ui->min_middle_width) && (middle_width > 0))
    {
        resize_V0(dx - ui->min_middle_width);
        ui->V1 = ui->V0 + ui->min_middle_width;//if the obove failed, this will add nothing
        return;
    }

}
void UIController::resize_H0(uint16_t dy)
{
    ui->H0 = CLAMP(dy, ui->min_middle_height, ui->window_height);
    return;
}
void UIController::resize_H1(uint16_t dy)
{
    ui->H1 = CLAMP(dy, 0, ui->window_height);
    return;
}
void UIController::resize_V2(uint16_t dx)
{
    dx = CLAMP(dx, 0, ui->V1);
    int16_t middle_width = ui->V3 - dx;
    if (middle_width >= 0)
    {
        ui->V2 = dx;
        return;
    }
    if (middle_width < 0)
    {
        resize_V3(dx);
        ui->V2 = ui->V3;
        return;
    }
}
void UIController::resize_V3(uint16_t dx)
{
    dx = CLAMP(dx, 0, ui->V1);
    int16_t middle_width = dx - ui->V2;
    if (middle_width >= 0)
    {
        ui->V3 = dx;
        return;
    }
    if (middle_width < 0)
    {
        resize_V2(dx);
        ui->V3 = ui->V2;
        return;
    }
}
void UIController::on_window_resize()
{
    
}