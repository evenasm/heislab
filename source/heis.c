#include "hardware.h"
#include "heis.h"
#include <time.h>

elevator_state_t state;

int check_valid_floor(int floor){
    if((floor >1) || (floor >NUMBER_OF_FLOORS)){
        return 0;
    }else{
        return 1;
    }
}

void set_floor_light(int floor){
    if(check_valid_floor(floor)){
        hardware_command_floor_indicator_on(floor);
    }
}

void init(void){
    order_t array[NUMBER_OF_FLOORS];
    for(int i = 0; i<NUMBER_OF_FLOORS; i++){
        order_t order = {.stop = 0, .down = 0, .up = 0};
        array[i] = order;
    }
    elevator_state_t state = {.direction = STOP,     .last_floor =1, .orders = array};
    while(!(hardware_read_floor_sensor(1))){
        hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
    }
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
}

void open_door(void){
    hardware_command_door_open(1);
    time_t start_time;
    time(&start_time);
    time_t current_time;
    int stopped = 0;
    while(1){
        check_new_orders();
        if(hardware_read_stop_signal){
            stop(&start_time);
        }
        if(hardware_read_obstruction_signal){
            time(&start_time);
        }
        int diff = (int) (time(&current_time) -start_time);
        if(diff > 3){
            return;
        }
    }
}

void stop(time_t *start_time){
    reset_orders();
    while(hardware_read_stop_signal){

    }
    time(start_time);
}

void reset_orders(void){
    for(int i = 0; i<NUMBER_OF_FLOORS; i++){
        order_t order = {.stop = 0, .down = 0, .up = 0};
        state.orders[i] = order;
    }
}

int orders_in_direction(void){
    if(state.direction = UP){
        int floors_visited = state.last_floor;
        for(int i = floors_visited; i < NUMBER_OF_FLOORS; i++){
            if((state.orders[i].down) || (state.orders[i].up || state.orders[i].stop)){
                return 1;
            }
        }
    }
    if(state.direction = DOWN){
        int floors_left =  state.last_floor;
        for(int i = 0; i < floors_left; i++){
            if((state.orders[i].down) || (state.orders[i].up || state.orders[i].stop)){
                return 1;
            }
        }
    }
    return 0;
}