#include "hardware.h"
#include "control.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "utilites.h"
#include "orders.h"

volatile elevator_state_t g_state;
int g_stop_flag;

void control_set_floor_light(int floor)
{
   hardware_command_floor_indicator_on(floor);
}

void control_init(void)
{
    orders_reset();
    g_state.direction = STOP;
    g_state.last_floor = 0;
    g_stop_flag = 0;
    while (!(hardware_read_floor_sensor(0)))
    {
        hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
    }
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
}

void control_open_door(void)
{
    hardware_command_door_open(1);
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    time_t start_time;
    time(&start_time);
    time_t current_time;
    while (1)
    {
        orders_check_new();
        orders_reset_on_floor(g_state.last_floor);
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
            orders_reset_on_floor(g_state.last_floor);
            return;
        }
    }
}

void control_stop_at_floor(time_t *p_start_time)
{
    orders_reset();
    hardware_command_stop_light(1);
    printf("turned ON stop light \n");
    while (hardware_read_stop_signal())
    {
    }
    printf("turned OFF stop light \n");
    hardware_command_stop_light(0);
    time(p_start_time);
}


void orders_set_direction(direction_t dir)
{
    g_state.direction = dir;
}


void control_set_last_floor(int floor)
{
    g_state.last_floor = floor;
}


void control_stop_between(void)
{
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    orders_reset();
    hardware_command_stop_light(1);
    if(g_state.last_direction != STOP){
            g_state.last_direction = g_state.direction;
    }
    g_state.direction = STOP;
}

int control_moving()
{
    if (g_state.direction == STOP)
    {
        return g_state.last_floor;
    }
    hardware_command_movement(utilities_convert_enum(g_state.direction));
    int next_floor;
    if (g_state.direction == UP)
    {
        next_floor = g_state.last_floor + 1;
    }
    else
    {
        next_floor = g_state.last_floor - 1;
    }
    printf("Moving to next floor : %i \n", next_floor);
    while (!hardware_read_floor_sensor(next_floor))
    {
        if (hardware_read_stop_signal())
        {
            control_stop_between();
            g_stop_flag = 1;
            return -1;
        }
        hardware_command_movement(utilities_convert_enum(g_state.direction));
        orders_check_new();
    }
    g_state.last_floor = next_floor;
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    control_set_floor_light(next_floor);
    printf("Hit floor %i \n", next_floor);
    return next_floor;
}
