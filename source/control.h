#ifndef CONTROL
#define CONTROL

#include "utilites.h"

static int stop_flag;
static elevator_state_t state;

/**
 * @file
 * @brief A library to abstract the operations of the elevator
 */

/**
 * @brief Sets the floor light on @p floor
 */
void control_set_floor_light(int floor);

/** 
 * @brief Main function for control_moving between floors.
 * Moves in the given direction while updating order matrix.
 * Relinquishes program control on hitting a floor.
 * @return Int corresponding to floor hit or -1 on stop button termination.
 */
int control_moving(void);

/**
 * @brief Function for initalizing hardware, variables and structs. MUST be called at startup.
 * Also takes the elevator to floor 1.
 * 
 */
void control_init(void);

/**
 * @brief Opens the door upon order at given floor. This program keeps track of time and checks
 * both obstruction and stop signals.
 */
void control_open_door(void);

/**
 * @brief Sets last floor in state struct
 * 
 */
void control_set_last_floor(int floor);

/** 
 * @brief The stop function for when the door is open.
 */
void control_stop_at_floor(time_t *p_start_time);

/**
 * @brief Handles stop button pressed between floors.
 */
void control_stop_between(void);

#endif