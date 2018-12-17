/*************************************************
 * Author: Alex Grutter
 * E-mail: agrutter87@gmail.com
 *************************************************/
/*************************************************
 * Includes
 *************************************************/
#include "arduino.h"

/*************************************************
 * Function prototypes
 *************************************************/
bool adc_start_conversion(uint8_t pin);
bool adc_check_complete(void);
uint16_t adc_read(void);



