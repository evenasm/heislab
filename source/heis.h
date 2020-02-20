#include <time.h>

#define NUMBER_OF_FLOORS 4
#define NUM_OF_ORDER_TYPES 3

/**
 * @file
 * @brief A library to abstract the operations of the elevator
 */

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
    int last_floor;                                   //!< Last floor the elevator hit. Must be greater than 0 and less than 5.
    int orders[NUMBER_OF_FLOORS][NUM_OF_ORDER_TYPES]; //!< Array with floor that have orders. orders[a][b] means there is an order of type b on floor a. 0 = up, 1 = inside.
} elevator_state_t;

/**
 * @brief A function to check that the @p floor argument is valid.
 * 
 * @return 1 if the floor is valid, 0 otherwise.
 */
int check_valid_floor(int floor);

/**
 * @brief Sets the floor light on @p floor
 */
void set_floor_light(int floor);

/**
 * @brief This function moves in the given @p direction until it hits the next floor or the stop button is pressed, 
 * at which point it relinquishes program control.
 */
void moving(void);

/**
 * @brief Function for initalizing hardware, variables and structs. MUST be called at startup.
 * Also takes the elevator to floor 1.
 * 
 */
void init(void);

/**
 * @brief Opens the door upon order at given floor. This program keeps track of time and checks
 * both obstruction and stop signals.
 */
void open_door();

/**
 * @brief Checks for orders in the given @p direction above or below last_floor.
 * @return 1 on orders in given @p direction, 0 otherwise.
 */
int orders_in_direction();

/**
 * @brief Polls the hardware via provided functions to check for new orders.
 * Then updates state struct.
 * 
 * @return 1 on new orders and 0 otherwise.
 */
int check_new_orders();

/**
 * @brief Checks if there is an order on @p floor in the given @p direction .
 * 
 * @return 1 if there is an order, 0 otherwise.
 */
int order_on_floor(int floor, direction_t direction);

/**
 * @brief resets the order structs in the order array of the state struct
 */
void reset_orders(void);

/**
 * @brief Returns floor on which the order is whne the elevator stands still.
 * 
 * @return returns -1 on no orders.
 */
int get_floor(void);

/**
 * @brief returns the direction of the floor with an order, and sets the sate struct accordingly.
 * 
 * @return The direction to go.
 */
direction_t get_direction(void);

/**
 * @brief Converts local movement enum to hardware enum
 * 
 * @param local_enum The local enum to be converted
 * 
 * @return The corresponding hardware enum
 */
HardwareOrder convert_enum(direction_t local_enum);

/**
 * @brief Converts int to corresponding hardware order
 * 
 * @param Int to convert
 * 
 * @return the corresponding Hardware order.
 */
HardwareOrder int_to_order(int j);

/**
 * @brief Sets last floor in state struct
 * 
 */
void set_last_floor(int floor);
