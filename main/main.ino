#include "sensors.hpp"

void setup() {
  Serial.begin(115200);

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(BUZZER_PASSIVE, OUTPUT);
  pinMode(LASER_EMITTER, OUTPUT);
  
  pinMode(JOYSTICK_SWITCH, INPUT_PULLUP);
  pinMode(INFRARED_SENSOR, INPUT);

  pinMode(PHOTORESISTOR, INPUT);
  pinMode(JOYSTICK_X, INPUT);
  pinMode(JOYSTICK_Y, INPUT);

  Wire.begin(MOTOR_SDA, MOTOR_SCL);

  motorSetup();

  xTaskCreate(infraredSensorTask, "Infrared_Task", 1024, NULL, 1, NULL);
  xTaskCreate(joystickTask, "Joystick_Task", 1024, NULL, 1, NULL);
  xTaskCreate(laserEmitterTask, "Laser_Emitter_Task", 1024, NULL, 1, NULL);
  xTaskCreate(ledTask, "LED_Task", 1024, NULL, 1, NULL);
  xTaskCreate(motorDriverTask, "Motor_Driver_Task", 4096, NULL, 1, NULL);
  xTaskCreate(passiveBuzzerTask, "Passive_Buzzer_Task", 1024, NULL, 1, NULL);
  xTaskCreate(photoresistorTask, "Photoresistor_Task", 1024, NULL, 1, NULL);
  xTaskCreate(udpTask, "UDP_Task", 4096, NULL, 1, NULL);
}

void loop() {
  // debug();
}