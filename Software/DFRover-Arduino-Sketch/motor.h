/*************************************************
 * Author: Alex Grutter
 * E-mail: agrutter87@gmail.com
 *************************************************/
/*************************************************
 * Includes
 *************************************************/
#include "pins_arduino.h"

/*************************************************
 * Defines
 *************************************************/
/* States for users to set motor_state_t variable to */
#define MOTOR_STATE_ON              (1)
#define MOTOR_STATE_OFF             (0)
#define MOTOR_STATE_FORWARD         (1)
#define MOTOR_STATE_REVERSE         (0)

/* Port/Pin definitions for motor controller */
#define MOTOR_ENABLE_PORT           (PORTD)
#define MOTOR_LEFT_ENABLE_PIN       (1 << 5)
#define MOTOR_RIGHT_ENABLE_PIN      (1 << 6)

#define MOTOR_LEFT_DIRECTION_PORT   (PORTB)
#define MOTOR_LEFT_DIRECTION_PIN    (1 << 0)

#define MOTOR_RIGHT_DIRECTION_PORT  (PORTD)
#define MOTOR_RIGHT_DIRECTION_PIN   (1 << 7)

/* Port/Pin direct functions for motor controller */
#define MOTOR_LEFT_ENABLE           (MOTOR_ENABLE_PORT |= MOTOR_LEFT_ENABLE_PIN)
#define MOTOR_RIGHT_ENABLE          (MOTOR_ENABLE_PORT |= MOTOR_RIGHT_ENABLE_PIN)
#define MOTOR_BOTH_ENABLE           (MOTOR_ENABLE_PORT |= (MOTOR_LEFT_ENABLE_PIN | MOTOR_RIGHT_ENABLE_PIN))

#define MOTOR_LEFT_DISABLE          (MOTOR_ENABLE_PORT &= ~MOTOR_LEFT_ENABLE_PIN)
#define MOTOR_RIGHT_DISABLE         (MOTOR_ENABLE_PORT &= ~MOTOR_RIGHT_ENABLE_PIN)
#define MOTOR_BOTH_DISABLE          (MOTOR_ENABLE_PORT &= ~(MOTOR_LEFT_ENABLE_PIN | MOTOR_RIGHT_ENABLE_PIN))

#define MOTOR_LEFT_REVERSE          (MOTOR_LEFT_DIRECTION_PORT |= MOTOR_LEFT_DIRECTION_PIN)
#define MOTOR_RIGHT_REVERSE         (MOTOR_RIGHT_DIRECTION_PORT |= MOTOR_RIGHT_DIRECTION_PIN)

#define MOTOR_LEFT_FORWARD          (MOTOR_LEFT_DIRECTION_PORT &= ~MOTOR_LEFT_DIRECTION_PIN)
#define MOTOR_RIGHT_FORWARD         (MOTOR_RIGHT_DIRECTION_PORT &= ~MOTOR_RIGHT_DIRECTION_PIN)

/*************************************************
 * Types
 *************************************************/
typedef union
{
  uint8_t state;
  struct
  {
    uint8_t left_en;
    uint8_t right_en;
    uint8_t left_dir;
    uint8_t right_dir;
  };
} motor_state_t;

/*************************************************
 * Function prototypes
 *************************************************/
void motor_init(void);
void motor_update(motor_state_t motor_state);


