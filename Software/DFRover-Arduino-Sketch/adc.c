/*************************************************
 * Author: Alex Grutter
 * E-mail: agrutter87@gmail.com
 *************************************************/
/*************************************************
 * INFO: This code is the Arduino source for analogRead()
 * modified to allow for separating out the different
 * phases of the process for reading the ADC. This has
 * been done to allow other tasks to happen while waiting
 * for ADC conversion to complete. The process_encoder() 
 * function in encoder.c shows how to use this in a way
 * that doesn't cause problems.
 * 
 * Care must be taken if using these functions elsewhere, 
 * as there are no checks to see if you are using them 
 * in the correct order. This code was not meant to be portable.
 *************************************************/


/*************************************************
 * Includes
 *************************************************/
#include "wiring_private.h"
#include "pins_arduino.h"

/*************************************************
 * Global variables
 *************************************************/
uint8_t analog_reference = DEFAULT;

/*************************************************
 * adc_start_conversion function
 * 
 * Derived from the source for Arduino's analogRead()
 * Starts the ADC conversion
 *************************************************/
bool adc_start_conversion(uint8_t pin)
{
#if defined(analogPinToChannel)
#if defined(__AVR_ATmega32U4__)
  if (pin >= 18) pin -= 18; // allow for channel or pin numbers
#endif
  pin = analogPinToChannel(pin);
#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  if (pin >= 54) pin -= 54; // allow for channel or pin numbers
#elif defined(__AVR_ATmega32U4__)
  if (pin >= 18) pin -= 18; // allow for channel or pin numbers
#elif defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)
  if (pin >= 24) pin -= 24; // allow for channel or pin numbers
#else
  if (pin >= 14) pin -= 14; // allow for channel or pin numbers
#endif

#if defined(ADCSRB) && defined(MUX5)
  // the MUX5 bit of ADCSRB selects whether we're reading from channels
  // 0 to 7 (MUX5 low) or 8 to 15 (MUX5 high).
  ADCSRB = (ADCSRB & ~(1 << MUX5)) | (((pin >> 3) & 0x01) << MUX5);
#endif
  
  // set the analog reference (high two bits of ADMUX) and select the
  // channel (low 4 bits).  this also sets ADLAR (left-adjust result)
  // to 0 (the default).
#if defined(ADMUX)
#if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
  ADMUX = (analog_reference << 4) | (pin & 0x07);
#else
  ADMUX = (analog_reference << 6) | (pin & 0x07);
#endif
#endif

  // without a delay, we seem to read from the wrong channel
  //delay(1);

#if defined(ADCSRA) && defined(ADCL)
  // start the conversion
  sbi(ADCSRA, ADSC);
  return true;
#else
  return false;
#endif
}

/*************************************************
 * adc_check_complete function
 * 
 * Derived from the source for Arduino's analogRead()
 * Checks to see if the ADC conversion is complete, to allow
 * processing other tasks in the same while() loop
 *************************************************/
bool adc_check_complete(void)
{
#if defined(ADCSRA) && defined(ADCL)
  return bit_is_set(ADCSRA, ADSC);
#else
  return false;
#endif
}

/*************************************************
 * adc_read function
 * 
 * Derived from the source for Arduino's analogRead()
 * Reads the ADC after a conversion is complete
 *************************************************/
uint16_t adc_read(void)
{
  uint8_t low, high;
#if defined(ADCSRA) && defined(ADCL)
  // we have to read ADCL first; doing so locks both ADCL
  // and ADCH until ADCH is read.  reading ADCL second would
  // cause the results of each conversion to be discarded,
  // as ADCL and ADCH would be locked when it completed.
  low  = ADCL;
  high = ADCH;
#else
  // we dont have an ADC, return 0
  low  = 0;
  high = 0;
#endif  
  // combine the two bytes
  return (high << 8) | low;
}



