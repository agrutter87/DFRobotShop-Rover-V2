/*************************************************
 * Author: Alex Grutter
 * E-mail: agrutter87@gmail.com
 *************************************************/
/*************************************************
 *  INFO:
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
/*************************************************
 * Includes
 *************************************************/
#include "encoder.h"
#include "adc.h"

/*************************************************
 * Global variables
 *************************************************/
/*************************************************
 *  For now, this design uses global variables to
 *  simplify the process of tracking the encoder states. 
 *  Eventually, this code could be updated to handle an array 
 *  of encoders passed into it so it could be flexible
 *  on how many encoders it could handle. This design
 *  will always only use two encoders, left and right.
 *************************************************/
encoder_data_t g_left_encoder =
{
  .cur_data = 0,
  .prev_data = 0,
  .data_max = 0,
  .data_min = 0xFFFF,
  .threshold = 741,
  .count = 0
};

encoder_data_t g_right_encoder =
{
  .cur_data = 0,
  .prev_data = 0,
  .data_max = 0,
  .data_min = 0xFFFF,
  .threshold = 625,
  .count = 0
};

/*************************************************
 * process_encoder() function
 * 
 * Handles the ADC using a state machine to prevent 
 * unnecessary waiting. 
 * 
 * Returns true if the ADC is read and new, valid 
 * data is obtained. Otherwise, returns false.
 * 
 * Currently this state machine is simple and rotates
 * through 6 different states, 3 for each encoder. 
 * Eventually, this may be modified to only go have 
 * 3 states, with an additional static index to index 
 * through an array of encoders.
 *************************************************/
bool process_encoder()
{
  static encoder_state_t state = ENCODER_STATE_0;
  bool status = false;
  bool new_data = false;

  switch(state)
  {
    case ENCODER_STATE_0:
      /* Start the ADC conversion for the first (left) encoder */
      status = adc_start_conversion(ENCODER_LEFT_PIN);
      if(true == status)
      {
        /* If the ADC conversion was started successfully, move 
         * to the next state the next time process_encoder() is called */
        state = ENCODER_STATE_1;
      }
    break;

    case ENCODER_STATE_1:
      /* Check to see if the ADC conversion is complete (done in hardware,
       * separate from the code loop) */
      status = adc_check_complete();
      if(true == status)
      {
        /* If the ADC conversion is complete, move to the next state
         *  the next time process_encoder() is called */
        state = ENCODER_STATE_2;
      }
    break;

    case ENCODER_STATE_2:
      /* Save the current data for later */
      g_left_encoder.prev_data = g_left_encoder.cur_data;

      /* Read the ADC */
      g_left_encoder.cur_data = adc_read();

      /* Make sure its not 0 before using it */
      if(g_left_encoder.cur_data)
      {
        /* Set the return value to true to let the calling function know the data is valid */
        new_data = true;
        
        /* Update the max value if new data is higher */
        if(g_left_encoder.data_max < g_left_encoder.cur_data)
        {
          g_left_encoder.data_max = g_left_encoder.cur_data;
        }
        
        /* Update the min value if new data is lower */
        if(g_left_encoder.data_min > g_left_encoder.cur_data)
        {
          g_left_encoder.data_min = g_left_encoder.cur_data;
        }

        /* Increment the counter if the threshold was crossed */
        if(((g_left_encoder.cur_data > g_left_encoder.threshold) && (g_left_encoder.prev_data < g_left_encoder.threshold)) ||
           ((g_left_encoder.cur_data < g_left_encoder.threshold) && (g_left_encoder.prev_data > g_left_encoder.threshold)))
        {
          g_left_encoder.count++;
        }

        /* Move to the next state the next time process_encoder() is called */
        state = ENCODER_STATE_3;
      }
    break;

    case ENCODER_STATE_3:
      /* Start the ADC conversion for the second (right) encoder */
      status = adc_start_conversion(ENCODER_RIGHT_PIN);
      if(true == status)
      {
        /* If the ADC conversion is complete, move to the next state
         *  the next time process_encoder() is called */
        state = ENCODER_STATE_4;
      }
    break;

    case ENCODER_STATE_4:
      /* Check to see if the ADC conversion is complete (done in hardware,
       * separate from the code loop) */
      status = adc_check_complete();
      if(true == status)
      {
        /* If the ADC conversion is complete, move to the next state
         *  the next time process_encoder() is called */
        state = ENCODER_STATE_5;
      }
    break;

    case ENCODER_STATE_5:
      /* Save the current data for later */
      g_right_encoder.prev_data = g_right_encoder.cur_data;

      /* Read the ADC */
      g_right_encoder.cur_data = adc_read();

      /* Make sure its not 0 before using it */
      if(g_left_encoder.cur_data)
      {
        new_data = true;
        
        /* Update the max value if new data is higher */
        if(g_right_encoder.data_max < g_right_encoder.cur_data)
        {
          g_right_encoder.data_max = g_right_encoder.cur_data;
        }

        /* Update the min value if new data is lower */
        if(g_right_encoder.data_min > g_right_encoder.cur_data)
        {
          g_right_encoder.data_min = g_right_encoder.cur_data;
        }

        /* Increment the counter if the threshold was crossed */
        if(((g_right_encoder.cur_data > g_right_encoder.threshold) && (g_right_encoder.prev_data < g_right_encoder.threshold)) ||
           ((g_right_encoder.cur_data < g_right_encoder.threshold) && (g_right_encoder.prev_data > g_right_encoder.threshold)))
        {
          g_right_encoder.count++;
        }
        state = ENCODER_STATE_0;
      }
    break;
  }

  /* new_data will only be true if the ADC was read on this call */
  return new_data;
}



