#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "hardware.h"
#include "control.h"

static void clear_all_order_lights()
{
    HardwareOrder order_types[3] = {
        HARDWARE_ORDER_UP,
        HARDWARE_ORDER_INSIDE,
        HARDWARE_ORDER_DOWN};

    for (int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++)
    {
        for (int i = 0; i < 3; i++)
        {
            HardwareOrder type = order_types[i];
            hardware_command_order_light(f, type, 0);
        }
    }
}

static void sigint_handler(int sig)
{
    (void)(sig);
    printf("Terminating elevator\n");
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    exit(0);
}

FSM_STATE_t FSM_STATE;
int current_floor;
direction_t current_direction;

int main()
{

    int error = hardware_init();
    if (error != 0)
    {
        fprintf(stderr, "Unable to initialize hardware\n");
        exit(1);
    }
    clear_all_order_lights();
    control_init();
    FSM_STATE = IDLE;
    utilities_to_string();

    signal(SIGINT, sigint_handler);
    while (1)
    {
        switch (FSM_STATE)
        {
        case IDLE:
            printf("IDLE \n");
            orders_set_direction(STOP);
            while (1)
            {
                if (orders_check_new() && orders_unserviced())
                {
                    printf("checked orders!\n");
                    int floor = orders_get_floor_idle();
                    printf("floor _ %i \n", floor);
                    if (!(floor == -1))
                    {
                        current_direction = orders_get_direction();
                        orders_set_direction(current_direction);
                        printf("IDLE\n");
                        if(current_direction == STOP){
                            control_open_door();
                        }
                        FSM_STATE = MOVING;
                        break;
                    }
                }
                /* if(hardware_read_stop_signal()){
                    control_open_door();
                } */
            }
            break;

        case MOVING:
            if (!orders_unserviced())
            {
                FSM_STATE = IDLE;
                break;
            }
            printf("MOVING\n");
            printf("direction : %i\n", current_direction);

            current_floor = control_moving();
            if (current_floor == -1)
            {
                FSM_STATE = MOVING_STOP;

                break;
            }
            if (orders_check_stop_floor(current_floor))
            {
                control_open_door();
            }

            if (!orders_in_direction())
            {
                if (current_direction == UP)
                {
                    current_direction = DOWN;
                }
                else
                {
                    current_direction = UP;
                }
                orders_set_direction(current_direction);
                if (!orders_in_direction())
                {
                    FSM_STATE = IDLE;
                    break;
                }
                break;
            }
            break;
        case MOVING_STOP:
            FSM_STATE = IDLE;
            break;
        default:
            FSM_STATE = IDLE;
            break;
        }
        utilities_to_string();
    }
    return 0;
}
