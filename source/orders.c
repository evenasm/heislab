#include "utilites.h"
#include "control.h"


void orders_reset(void)
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
        for (int i = floors_visited +1; i < NUMBER_OF_FLOORS; i++)
        {
            for(int j = 1; j < NUM_OF_ORDER_TYPES; j++){
                if(state.orders[i][j] == 1){
                    return 1;
                }
            }
        }
    }
    if (state.direction == DOWN)
    {
        int floors_left = state.last_floor;
        for (int i = 0; i < floors_left; i++)
        {
           for(int j = 1; j < NUM_OF_ORDER_TYPES; j++){
                if(state.orders[i][j] == 1){
                    return 1;
                }
            }
        }
    }
    printf("No orders remaining\n");
    return 0;
}

int orders_check_new(void)
{
    int new_order = 0;
    for (int i = 0; i < NUMBER_OF_FLOORS; i++)
    {
        for (int j = 0; j < NUM_OF_ORDER_TYPES; j++)
        {
            HardwareOrder order = utilities_int_to_order(j);
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

int orders_get_floor_idle(void)
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


direction_t orders_get_direction(void)
{
    direction_t dir = STOP;
    if (state.last_floor > orders_get_floor_idle())
    {
        dir = DOWN;
    }
    else if (state.last_floor < orders_get_floor_idle())
    {
        dir = UP;
    }
    else if((dir == STOP) && (stop_flag)){
        if(state.last_direction == UP){
            dir = DOWN;
            state.last_floor++;
        }else{
            dir = UP;
            state.last_floor--;

        }
    }
    stop_flag = 0;

    return dir;
}

int orders_on_floor(int floor, direction_t direction)
{
    if (state.orders[floor][utilities_direction_to_int(direction)])
    {
        return 1;
    }
    return 0;
}


void orders_reset_on_floor(int floor)
{
    for (int i = 0; i < NUM_OF_ORDER_TYPES; i++)
    {
        state.orders[floor][i] = 0;
    }
}

int orders_check_stop_floor(int floor)
{
    if (orders_on_floor(floor, STOP))
    {
        return 1;
    }
    else if (orders_on_floor(floor, state.direction))
    {
        return 1;
    }
    else
    {
        direction_t opposite_direction = state.direction == UP ? DOWN : UP;
        if (!orders_in_direction() && orders_on_floor(floor, opposite_direction))
        {
            return 1;
        }
    }
    return 0;
}

int orders_underviced(void){
    for(int i = 0; i<NUMBER_OF_FLOORS; i++){
        for(int j = 0; j < NUM_OF_ORDER_TYPES; j++){
            if(state.orders[i][j] == 1){
                return 1;
            }
        }
    }return 0;
}