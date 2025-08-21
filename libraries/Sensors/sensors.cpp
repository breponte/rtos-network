#include "sensors.hpp"

Adafruit_PWMServoDriver motors = Adafruit_PWMServoDriver(0x40);

/* HELPERS */

void debug() {
  Serial.print("Photoresistor:");
  Serial.print(analogRead(PHOTORESISTOR));
  Serial.print("\t");

  Serial.print("Joystick_X:");
  Serial.print(analogRead(JOYSTICK_X));
  Serial.print("\t");

  Serial.print("Joystick_Y:");
  Serial.print(analogRead(JOYSTICK_Y));
  Serial.print("\t");

  Serial.print("Joystick_Switch:");
  Serial.print(digitalRead(JOYSTICK_SWITCH));
  Serial.print("\t");

  Serial.print("Infrared_Sensor:");
  Serial.println(digitalRead(INFRARED_SENSOR));
}

/* SETUP */

void motorSetup() {
  motors.begin();
  motors.setPWMFreq(50);

  for(int i = 0; i < 2; i++) {
      motors.setPWM(i, 0, ANGLE_TO_PULSE(0));
  }

  Serial.println("Motors Set Up");
}

/* THREADS */

void infraredSensorTask(void *pvParameters) {
  while (true) {
    vTaskDelay(10);
  }
}

void joystickTask(void *pvParameters) {
  while (true) {
    digitalRead(JOYSTICK_SWITCH);
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void laserEmitterTask(void *pvParameters) {
  while (true) {
    digitalWrite(LASER_EMITTER, !digitalRead(JOYSTICK_SWITCH));
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void ledTask(void *pvParameters) {
  while (true) {
    if (digitalRead(INFRARED_SENSOR)) {
      digitalWrite(LED_RED, HIGH);
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_BLUE, LOW);
    } else {
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_GREEN, HIGH);
      digitalWrite(LED_BLUE, LOW);
    }

    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void motorDriverTask(void *pvParameters) {
  while (true) {
    int angleX = map(analogRead(JOYSTICK_X), 0, 4095, 0, 180);
    motors.setPWM(0, 0, ANGLE_TO_PULSE(angleX));

    int angleY = map(analogRead(JOYSTICK_Y), 0, 4095, 0, 135);
    motors.setPWM(1, 0, ANGLE_TO_PULSE(angleY));

    vTaskDelay(pdMS_TO_TICKS(20));
  }
}

void passiveBuzzerTask(void *pvParameters) {
  while (true) {
    if (analogRead(PHOTORESISTOR) < 500) {
      digitalWrite(LED_GREEN, 0);
      bool redLED = true;
      for (int i = 0; i < 100; i++) {
        digitalWrite(BUZZER_PASSIVE, HIGH);
        vTaskDelay(pdMS_TO_TICKS(2));
        digitalWrite(BUZZER_PASSIVE, LOW);
        vTaskDelay(pdMS_TO_TICKS(2));

        if (i % 20 == 0) redLED = !redLED; 

        if (redLED) {
          digitalWrite(LED_RED, HIGH);
          digitalWrite(LED_BLUE, LOW);
        } else {
          digitalWrite(LED_RED, LOW);
          digitalWrite(LED_BLUE, HIGH);
        }
      }
    }

    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void photoresistorTask(void *pvParameters) {
  while (true) {
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void udpTask(void *pvParameters) {
  while (true) {
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}