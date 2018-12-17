/*************************************************
 * Author: Alex Grutter
 * E-mail: agrutter87@gmail.com
 *************************************************/
/*************************************************
 * Includes
 *************************************************/
extern "C"
{
  #include "motor.h"
  #include "encoder.h"
}

/*************************************************
 * Defines
 *************************************************/
/* If you want to observe the encoder min/max values
 * experimentally, set TESTING to (1) and 
 * DEBUG_THRESHOLDING to (1)
 * 
 * If you want to see the count value printed, set
 * TESTING to (1) and DEBUG_COUNTING to (1) */
#define TESTING             (0)

#if TESTING
#define DEBUG_THRESHOLDING  (0)
#define DEBUG_COUNTING      (0)
#endif

/*************************************************
 * Global variables
 *************************************************/
motor_state_t g_motor_state;

extern encoder_data_t g_left_encoder;
extern encoder_data_t g_right_encoder;

/*************************************************
 * Function prototypes
 *************************************************/
void process_command(void);

/*************************************************
 * setup() function
 *************************************************/
void setup() 
{
  /* Initialize motor driver pins */
  motor_init();

  /* Initialize motors to be off */
  g_motor_state.left_en = MOTOR_STATE_OFF;
  g_motor_state.right_en = MOTOR_STATE_OFF;
  g_motor_state.left_dir = MOTOR_STATE_FORWARD;
  g_motor_state.right_dir = MOTOR_STATE_FORWARD;
  motor_update(g_motor_state);

  /* Start serial communications to 921600
   * NOTE: I'd recommend using TeraTerm instead of
   * the Arduino IDE's terminal. The only issue is
   * that you need to disconnect the TeraTerm from
   * the COM port before you try programming again.
   * https://ttssh2.osdn.jp/index.html.en */
  Serial.begin(921600);
}

/*************************************************
 * loop() function
 *************************************************/
void loop() 
{
  /* If there is new command data available, process it */
  process_command();

#if TESTING
  /* Process encoder, and if new data is available, output it */
  if(process_encoder())
  {
#if DEBUG_THRESHOLDING
    Serial.println();
    Serial.print("L: ");
    Serial.print(g_left_encoder.data_min);
    Serial.print(" - ");
    Serial.println(g_left_encoder.data_max);
    Serial.print("R: ");
    Serial.print(g_right_encoder.data_min);
    Serial.print(" - ");
    Serial.println(g_right_encoder.data_max);
#endif

#if DEBUG_COUNTING
    Serial.println();
    Serial.print("L: ");
    Serial.println(g_left_encoder.count);
    Serial.print("R: ");
    Serial.println(g_right_encoder.count);
#endif
  }
#else
  process_encoder();
#endif
}

/*************************************************
 * process_command() function
 * 
 * This function reads characters from the terminal
 * and issues commands to the motor controller.
 * This provides basic WASD control to the rover
 * while it is connected. This will be upgraded to
 * Bluetooth later, and will allow for characters being
 * sent on release of a key. For now, to stop the Rover
 * you must press any other key besides those with
 * functionality attached:
 * 
 * All commands:
 * W - Forward
 * A - Rotate Left
 * S - Reverse
 * D - Rotate Right
 * Q - Turn Left
 * E - Turn Right
 * R - Reset encoder min/max values
 * ALL OTHER KEYS - Stop the motors
 *************************************************/
void process_command(void)
{
  int command = 0;
  if(Serial.available() > 0)
  {
    command = Serial.read();
    switch(command)
    {
      case 'W':
      case 'w':
        Serial.println("Forward");
        g_motor_state.left_en = MOTOR_STATE_ON;
        g_motor_state.right_en = MOTOR_STATE_ON;
        g_motor_state.left_dir = MOTOR_STATE_FORWARD;
        g_motor_state.right_dir = MOTOR_STATE_FORWARD;
        motor_update(g_motor_state);
      break;
  
      case 'A':
      case 'a':
        Serial.println("Rotate Left");
        g_motor_state.left_en = MOTOR_STATE_ON;
        g_motor_state.right_en = MOTOR_STATE_ON;
        g_motor_state.left_dir = MOTOR_STATE_REVERSE;
        g_motor_state.right_dir = MOTOR_STATE_FORWARD;
        motor_update(g_motor_state);
      break;
  
      case 'S':
      case 's':
        Serial.println("Reverse");
        g_motor_state.left_en = MOTOR_STATE_ON;
        g_motor_state.right_en = MOTOR_STATE_ON;
        g_motor_state.left_dir = MOTOR_STATE_REVERSE;
        g_motor_state.right_dir = MOTOR_STATE_REVERSE;
        motor_update(g_motor_state);
      break;
  
      case 'D':
      case 'd':
        Serial.println("Rotate Right");
        g_motor_state.left_en = MOTOR_STATE_ON;
        g_motor_state.right_en = MOTOR_STATE_ON;
        g_motor_state.left_dir = MOTOR_STATE_FORWARD;
        g_motor_state.right_dir = MOTOR_STATE_REVERSE;
        motor_update(g_motor_state);
      break;
      
      case 'Q':
      case 'q':
        Serial.println("Turn Left");
        g_motor_state.left_en = MOTOR_STATE_ON;
        g_motor_state.right_en = MOTOR_STATE_OFF;
        g_motor_state.left_dir = MOTOR_STATE_FORWARD;
        g_motor_state.right_dir = MOTOR_STATE_FORWARD;
        motor_update(g_motor_state);
      break;
      
      case 'E':
      case 'e':
        Serial.println("Turn Right");
        g_motor_state.left_en = MOTOR_STATE_OFF;
        g_motor_state.right_en = MOTOR_STATE_ON;
        g_motor_state.left_dir = MOTOR_STATE_FORWARD;
        g_motor_state.right_dir = MOTOR_STATE_FORWARD;
        motor_update(g_motor_state);
      break;
  
      case 'R':
      case 'r':
        Serial.println("Resetting Encoder Min/Max");
        g_left_encoder.data_min = 0xFFFF;
        g_left_encoder.data_max = 0;
        g_right_encoder.data_min = 0xFFFF;
        g_right_encoder.data_max = 0;
      break;
  
      default:
        Serial.println("Stop");
        g_motor_state.left_en = MOTOR_STATE_OFF;
        g_motor_state.right_en = MOTOR_STATE_OFF;
        motor_update(g_motor_state);
      break;
    }
  }
}

