/*************************************************
 * Author: Alex Grutter
 * E-mail: agrutter87@gmail.com
 *************************************************/
/*************************************************
 * Includes
 *************************************************/
#include "arduino.h"

/*************************************************
 * Defines
 *************************************************/
#define TESTING_ENCODER   (1)

#define ENCODER_LEFT_PIN  (0)
#define ENCODER_RIGHT_PIN (1)

/*************************************************
 * Types
 *************************************************/
/*************************************************
 *  The states of how the encoder will use the ADC are as shown:
 *  0 - Before starting conversion on ENCODER_LEFT_PIN, and after reading analog value of ENCODER_RIGHT_PIN
 *  1 - After starting conversion on ENCODER_LEFT_PIN, but before the conversion is complete
 *  2 - After ADC conversion on ENCODER_LEFT_PIN, but before reading analog value of ENCODER_LEFT_PIN
 *  3 - After reading analog value of ENCODER_LEFT_PIN, but before starting conversion on ENCODER_RIGHT_PIN
 *  4 - After starting conversion on ENCODER_RIGHT_PIN, but before the conversion is complete
 *  5 - After ADC conversion on ENCODER_RIGHT_PIN, but before reading analog value of ENCODER_RIGHT_PIN
 *  
 *  It goes 0-1-2-3-4-5-0-1-2-3-4...
 *************************************************/
typedef enum
{
  ENCODER_STATE_0,
  ENCODER_STATE_1,
  ENCODER_STATE_2,
  ENCODER_STATE_3,
  ENCODER_STATE_4,
  ENCODER_STATE_5
} encoder_state_t;

typedef struct
{
  uint16_t cur_data;
  uint16_t prev_data;
  uint16_t data_max;
  uint16_t data_min;
  uint16_t threshold;
  uint32_t count;
} encoder_data_t;

/*************************************************
 * Function prototypes
 *************************************************/
bool process_encoder();


