
#ifndef ORDERS
#define ORDERS

/**
 * @file
 * @brief A library to abstract the orders to the elevator.
 */

/**
 * @brief Checks for orders in the given @p direction above or below last_floor.
 * @return 1 on orders in given @p direction, -1 if there are orders in another direcion, and 0 otherwise.
 */
int orders_in_current_direction();

/**
 * @brief Polls the hardware via provided functions to check for new orders.
 * Then updates state struct.
 * 
 * @return 1 on new orders and 0 otherwise.
 */
int orders_check_new();

/**
 * @brief Checks if there is an order on @p floor in the given @p direction .
 * 
 * @return 1 if there is an order, 0 otherwise.
 */
int orders_on_floor(int floor, direction_t direction);

/**
 * @brief resets the order structs in the order array of the state struct
 */
void orders_reset(void);

/**
 * @brief Returns floor on which the order is when the elevator stands still.
 * 
 * @return returns -1 on no orders.
 */
int orders_get_floor_idle(void);

/**
 * @brief returns the direction of the floor with an order, and sets the sate struct accordingly.
 * Also checks if we stopped between floors and sets direction accordingly.
 * 
 * @return The direction to go.
 */
direction_t orders_get_direction(void);

/**
 * @brief Set the direction in the state struct to this.
 */
void orders_set_direction(direction_t dir);

/**
 * @brief Resets orders on given floors.
 */
void orders_reset_on_floor(int floor);

/**
 * @brief Checks if we should stop on given floor.
 * 
 * @return Bool if we should stop.
 */
int orders_check_stop_floor(int floor);

/**
 * @brief A function to check if there are unserviced orders
 * 
 * @return 1 on unserviced orders, 0 otherwise.
 */
int orders_unserviced(void);

#endif