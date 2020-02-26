#include "hardware.h"
#include "control.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "utilites.h"
#include "orders.c"

static volatile elevator_state_t state;
int stop_flag;

void control_set_floor_light(int floor)
{
   hardware_command_floor_indicator_on(floor);
}

void control_init(void)
{
    orders_reset();
    state.direction = STOP;
    state.last_floor = 0;
    stop_flag = 0;
    while (!(hardware_read_floor_sensor(0)))
    {
        hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
    }
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
}

void control_open_door(void)
{
    hardware_command_door_open(1);
    time_t start_time;
    time(&start_time);
    time_t current_time;
    while (1)
    {
        orders_check_new();
        if (hardware_read_stop_signal())
        {
            control_stop_at_floor(&start_time);
        }
        if (hardware_read_obstruction_signal())
        {
            time(&start_time);
        }
        int diff = (int)(time(&current_time) - start_time);
        if (diff > 3)
        {
            hardware_command_door_open(0);
            orders_reset_on_floor(state.last_floor);
            return;
        }
    }
}

void control_stop_at_floor(time_t *p_start_time)
{
    orders_reset();
    hardware_command_stop_light(1);
    while (hardware_read_stop_signal())
    {
    }
    hardware_command_stop_light(0);
    time(start_time);
}


void orders_set_direction(direction_t dir)
{
    state.direction = dir;
}


void control_set_last_floor(int floor)
{
    state.last_floor = floor;
}


void control_stop_between(void)
{
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    orders_reset();
    hardware_command_stop_light(1);
    if(state.last_direction != STOP){
            state.last_direction = state.direction;
    }
    state.direction = STOP;
    while (hardware_read_stop_signal())
    {
    }
    hardware_command_stop_light(0);
}

int control_moving()
{
    if (state.direction == STOP)
    {
        return state.last_floor;
    }
    hardware_command_movement(utilities_convert_enum(state.direction));
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
        if (hardware_read_stop_signal())
        {
            control_stop_between();
            stop_flag = 1;
            return -1;
        }
        hardware_command_movement(utilities_convert_enum(state.direction));
        orders_check_new();
    }
    state.last_floor = next_floor;
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    printf("Hit floor %i \n", next_floor);
    return next_floor;
}
