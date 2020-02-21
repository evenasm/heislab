#include "hardware.h"
#include "heis.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

static volatile elevator_state_t state;

int check_valid_floor(int floor)
{
    if ((floor > 0) || (floor > NUMBER_OF_FLOORS))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

void set_floor_light(int floor)
{
    if (check_valid_floor(floor))
    {
        hardware_command_floor_indicator_on(floor);
    }
}

void init(void)
{
    reset_orders();
    state.direction = STOP;
    state.last_floor = 0;
    while (!(hardware_read_floor_sensor(0)))
    {
        hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
    }
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
}

void open_door(void)
{
    hardware_command_door_open(1);
    time_t start_time;
    time(&start_time);
    time_t current_time;
    while (1)
    {
        check_new_orders();
        if (hardware_read_stop_signal())
        {
            stop(&start_time);
        }
        if (hardware_read_obstruction_signal())
        {
            time(&start_time);
        }
        int diff = (int)(time(&current_time) - start_time);
        if (diff > 3)
        {
            hardware_command_door_open(0);
            reset_order_floor(state.last_floor);
            return;
        }
    }
}

void stop(time_t *start_time)
{
    reset_orders();
    hardware_command_stop_light(1);
    while (hardware_read_stop_signal())
    {
    }
    hardware_command_stop_light(0);
    time(start_time);
}

void reset_orders(void)
{
    for (int i = 0; i < NUMBER_OF_FLOORS; i++)
    {
        for (int j = 0; j < NUM_OF_ORDER_TYPES; j++)
        {
            state.orders[i][j] = 0;
        }
    }
}

int orders_in_direction(void)
{
    if (state.direction == UP)
    {
        int floors_visited = state.last_floor;
        for (int i = floors_visited; i < NUMBER_OF_FLOORS; i++)
        {
            if ((state.orders[i][1]) || (state.orders[i][1] || state.orders[i][2]))
            {
                printf("Order remaining upwards \n");
                return 1;
            }
        }
    }
    if (state.direction == DOWN)
    {
        int floors_left = state.last_floor;
        for (int i = 0; i < floors_left; i++)
        {
            if ((state.orders[i][0]) || (state.orders[i][1] || state.orders[i][2]))
            {
                printf("Order remaining downwards \n");
                return 1;
            }
        }
    }
    printf("No orders remaining\n");
    return 0;
}

int check_new_orders(void)
{
    int new_order = 0;
    for (int i = 0; i < NUMBER_OF_FLOORS; i++)
    {
        for (int j = 0; j < NUM_OF_ORDER_TYPES; j++)
        {
            HardwareOrder order = int_to_order(j);
            int order_state = hardware_read_order(i, order);
            if ((order_state == 1) && (state.orders[i][j] != order_state))
            {
                new_order = 1;
                state.orders[i][j] = order_state; //1
            }
        }
    }
    return new_order;
}

int get_floor(void)
{
    for (int i = 0; i < NUMBER_OF_FLOORS; i++)
    {
        for (int j = 0; j < NUM_OF_ORDER_TYPES; j++)
        {
            if (state.orders[i][j] == 1)
            {
                return i;
            }
        }
    }
    return -1;
}

direction_t get_direction(void)
{
    direction_t dir = STOP;
    if (state.last_floor > get_floor())
    {
        dir = DOWN;
    }
    else if (state.last_floor < get_floor())
    {
        dir = UP;
    }
    return dir;
}

void set_direction(direction_t dir)
{
    state.direction = dir;
}

HardwareOrder convert_enum(direction_t local_enum)
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

HardwareOrder int_to_order(int j)
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

void set_last_floor(int floor)
{
    state.last_floor = floor;
}

int direction_to_int(direction_t dir)
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

int order_on_floor(int floor, direction_t direction)
{
    if (state.orders[floor][direction_to_int(direction)])
    {
        return 1;
    }
    return 0;
}

void stop_between(void)
{
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    reset_orders();
    hardware_command_stop_light(1);
    state.last_direction = state.direction;
    state.direction = STOP;
    while (hardware_read_stop_signal())
    {
    }
    hardware_command_stop_light(0);
    return;
}

int moving()
{
    if (state.direction == STOP)
    {
        return state.last_floor;
    }
    hardware_command_movement(convert_enum(state.direction));
    int next_floor;
    if (state.direction == UP)
    {
        next_floor = state.last_floor + 1;
    }
    else
    {
        next_floor = state.last_floor - 1;
    }
    printf("Moving to next floor : %i \n", next_floor);
    while (!hardware_read_floor_sensor(next_floor))
    {
        while (hardware_read_stop_signal())
        {
            stop_between();
            return -1;
        }
        hardware_command_movement(convert_enum(state.direction));
        check_new_orders();
    }
    state.last_floor = next_floor;
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    printf("Hit floor %i \n", next_floor);
    return next_floor;
}

void reset_order_floor(int floor)
{
    for (int i = 0; i < NUM_OF_ORDER_TYPES; i++)
    {
        state.orders[floor][i] = 0;
    }
}

int check_stop_floor(int floor)
{
    if (order_on_floor(floor, STOP))
    {
        return 1;
    }
    else if (order_on_floor(floor, state.direction))
    {
        return 1;
    }
    else
    {
        direction_t opposite_direction = state.direction == UP ? DOWN : UP;
        if (!orders_in_direction() && order_on_floor(floor, opposite_direction))
        {
            return 1;
        }
    }
    return 0;
}

void toString(void)
{
    printf("=======================================\n");
    printf("Current state: (to be added) \n");
    for (int i = 0; i < NUMBER_OF_FLOORS; i++)
    {
        printf("Orders at floor %i : UP: %i     STOP: %i    DOWN: %i \n", i, state.orders[i][0], state.orders[i][1], state.orders[i][2]);
    }
    printf("\n ==================================\n");
}

int orders_unserviced()
{
    for (int i = 0; i < NUMBER_OF_FLOORS; i++)
    {
        for (int j = 0; j < NUM_OF_ORDER_TYPES; j++)
        {
            if (state.orders[i][j] == 1)
            {
                return 1;
            }
        }
    }
    return 0;
}