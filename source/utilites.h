
#ifndef UTILITIES
#define UTILITIES

#define NUMBER_OF_FLOORS 4
#define NUM_OF_ORDER_TYPES 3

/**
 * @file
 * @brief Utility functions for the elevator
 */


/**
 * @brief The states of the elevator
 */
typedef enum
{
    INIT,
    MOVING,
    MOVING_STOP,
    OBSTRUCTION,
    FLOOR_WITH_ORDER,
    IDLE,
} FSM_STATE_t;

typedef enum
{
    UP = 0,
    STOP = 1,
    DOWN = 2,
} direction_t;

/**
 * @brief A struct to represent the internal state of the elevator
 */
typedef struct
{
    direction_t direction;                            //!< Enum for the direction of the elevator
    direction_t last_direction;                       //!< Enum for the direction the elevator was going, used when stopping between floors
    int last_floor;                                   //!< Last floor the elevator hit. Must be greater than 0 and less than 5.
    int orders[NUMBER_OF_FLOORS][NUM_OF_ORDER_TYPES]; //!< Array with floor that have orders. orders[a][b] means there is an order of type b on floor a. 0 = up, 1 = inside.
} elevator_state_t;


/**
 * @brief Converts local movement enum to hardware enum
 * 
 * @param local_enum The local enum to be converted
 * 
 * @return The corresponding hardware enum
 */
HardwareOrder utilities_convert_enum(direction_t local_enum);

/**
 * @brief Converts int to corresponding hardware order
 * 
 * @param Int to convert
 * 
 * @return the corresponding Hardware order.
 */
HardwareOrder utilities_int_to_order(int j);

/**
 * @brief Prints the state of the elevator.
 */
void utilities_to_string(void);

/**
 * @brief Converts local direction enum to int for ease of navigating order matrix.
 */
int utilities_direction_to_int(direction_t dir);

#endif //  UTILITIES