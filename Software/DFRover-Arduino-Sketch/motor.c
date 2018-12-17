/*************************************************
 * Author: Alex Grutter
 * E-mail: agrutter87@gmail.com
 *************************************************/
/*************************************************
 * INFO:
 * This design allows for passing a motor_state_t type 
 * variable into the motor_update() function to control 
 * the motors. motor_init() just sets the pin modes.
 * 
 * NOTE:
 * This design doesn't include PWM control. This will
 * be added in the future.
 *************************************************/
/*************************************************
 * Includes
 *************************************************/
#include "motor.h"

/*************************************************
 * motor_init() function
 * 
 * Initializes pins for the motor controller
 *************************************************/
void motor_init()
{
  /* Set relevant pins to OUTPUT mode */
  DDRD |= (MOTOR_LEFT_ENABLE_PIN | MOTOR_RIGHT_ENABLE_PIN | MOTOR_RIGHT_DIRECTION_PIN);
  DDRB |= (MOTOR_LEFT_DIRECTION_PIN);
}

/*************************************************
 * motor_update() function
 * 
 * Updates the motor controller with a new state
 *************************************************/
void motor_update(motor_state_t new_state)
{
  /* Process state for left enable */
  if(new_state.left_en == MOTOR_STATE_ON)
  {
    MOTOR_LEFT_ENABLE;
  }
  else
  {
    MOTOR_LEFT_DISABLE;
  }

  /* Process state for right enable */
  if(new_state.right_en == MOTOR_STATE_ON)
  {
    MOTOR_RIGHT_ENABLE;
  }
  else
  {
    MOTOR_RIGHT_DISABLE;
  }

  /* Process state for left direction */
  if(new_state.left_dir == MOTOR_STATE_FORWARD)
  {
    MOTOR_LEFT_FORWARD;
  }
  else
  {
    MOTOR_LEFT_REVERSE;
  }
  
  /* Process state for right direction */
  if(new_state.right_dir == MOTOR_STATE_FORWARD)
  {
    MOTOR_RIGHT_FORWARD;
  }
  else
  {
    MOTOR_RIGHT_REVERSE;
  }
}



