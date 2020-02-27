
#include "utilites.h"

volatile elevator_state_t g_state;

HardwareOrder utilities_convert_enum(direction_t local_enum)
{
    HardwareOrder order = HARDWARE_ORDER_INSIDE;
    switch (local_enum)
    {
    case UP:
        order = HARDWARE_ORDER_UP;
        break;
    case STOP:
        order = HARDWARE_ORDER_INSIDE;
        break;
    case DOWN:
        order = HARDWARE_ORDER_DOWN;
        break;
    }
    return order;
}

HardwareOrder utilities_int_to_order(int j)
{
    HardwareOrder order;
    switch (j)
    {
    case 0:
        order = HARDWARE_ORDER_UP;
        break;
    case 1:
        order = HARDWARE_ORDER_INSIDE;
        break;
    case DOWN:
        order = HARDWARE_ORDER_DOWN;
        break;
    }
    return order;
}

int utilities_direction_to_int(direction_t dir)
{
    switch (dir)
    {
    case UP:
        return 0;
        break;
    case STOP:
        return 1;
        break;
    case DOWN:
        return 2;
        break;
    }
    return -1;
}

void utilities_to_string(void)
{
    printf("============================================\n");
    for (int i = 0; i < NUMBER_OF_FLOORS; i++)
    {
        printf("Orders at floor %i : UP: %i     STOP: %i    DOWN: %i \n", i, g_state.orders[i][0], g_state.orders[i][1], g_state.orders[i][2]);
    }
    printf("\n==========================================\n");
}
