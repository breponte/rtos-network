#include "Adafruit_PWMServoDriver.h"
#include "Wire.h"

#define SERVOMIN  150
#define SERVOMAX  600
#define ANGLE_TO_PULSE(ang) map((ang), 0, 180, SERVOMIN, SERVOMAX)

/* Digital Write */
#define LED_GREEN 12
#define LED_RED 13
#define LED_BLUE 14
#define BUZZER_PASSIVE 19
#define LASER_EMITTER 26

/* Digital Read */
#define JOYSTICK_SWITCH 25
#define INFRARED_SENSOR 27

/* I2C */
#define MOTOR_SCL 22
#define MOTOR_SDA 21

/* Analog Read */
#define PHOTORESISTOR 33
#define JOYSTICK_X 34
#define JOYSTICK_Y 35

extern Adafruit_PWMServoDriver motors;

/* FUNCTIONS */

void debug();

void motorSetup();

void infraredSensorTask(void *pvParameters);

/**
 * Reads the current state of the joystick switch and stores it in a shared variable.
 *
 * @param sv Pointer to a SharedVariable structure where the joystick switch state will be stored.
 */
void joystickTask(void *pvParameters);

/**
 * Uses the current state of the joystick switch to actuate the laser emitter, on when pressed.
 *
 * @param sv Pointer to a SharedVariable structure where the joystick switch state will be stored.
 */
void laserEmitterTask(void *pvParameters);
void ledTask(void *pvParameters);
void motorDriverTask(void *pvParameters);
void passiveBuzzerTask(void *pvParameters);
void photoresistorTask(void *pvParameters);
void udpTask(void *pvParameters);