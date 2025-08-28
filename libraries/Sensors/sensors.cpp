#include "sensors.hpp"

Adafruit_PWMServoDriver motors = Adafruit_PWMServoDriver(0x40);
SemaphoreHandle_t mutexMotor = xSemaphoreCreateMutex();
SemaphoreHandle_t mutexInfrared = xSemaphoreCreateMutex();
SemaphoreHandle_t mutexPhotoresistor = xSemaphoreCreateMutex();
SensorStates sensors = {0, 0, 500, false, false};

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
    if(xSemaphoreTake(mutexInfrared, pdMS_TO_TICKS(10)) == pdTRUE) {
      sensors.isInfraredDetecting = digitalRead(INFRARED_SENSOR) == 0;
      xSemaphoreGive(mutexInfrared);
    }
    vTaskDelay(pdMS_TO_TICKS(50));
  }
}

void joystickTask(void *pvParameters) {
  while (true) {
    if(xSemaphoreTake(mutexMotor, pdMS_TO_TICKS(10)) == pdTRUE) {
      sensors.isJoystickPressed = digitalRead(JOYSTICK_SWITCH) == 0;
      sensors.joystickX = map(analogRead(JOYSTICK_X), 0, 4095, 0, 180);
      sensors.joystickY = map(analogRead(JOYSTICK_Y), 0, 4095, 0, 135);
      xSemaphoreGive(mutexMotor);
    }
    vTaskDelay(pdMS_TO_TICKS(20));
  }
}

void laserEmitterTask(void *pvParameters) {
  while (true) {
    if(xSemaphoreTake(mutexMotor, pdMS_TO_TICKS(10)) == pdTRUE) {
      digitalWrite(LASER_EMITTER, sensors.isJoystickPressed);
      xSemaphoreGive(mutexMotor);
    }
    vTaskDelay(pdMS_TO_TICKS(50));
  }
}

void ledTask(void *pvParameters) {
  while (true) {
    if(xSemaphoreTake(mutexInfrared, pdMS_TO_TICKS(10)) == pdTRUE) {
      if(xSemaphoreTake(mutexPhotoresistor, pdMS_TO_TICKS(10)) == pdTRUE) {
        if (sensors.isInfraredDetecting) {
          digitalWrite(LED_RED, LOW);
          digitalWrite(LED_GREEN, HIGH);
          digitalWrite(LED_BLUE, LOW);
        } else {
          digitalWrite(LED_RED, HIGH);
          digitalWrite(LED_GREEN, LOW);
          digitalWrite(LED_BLUE, LOW);
        }

        static bool redLED = true;
        static int counter = 0;
        if (sensors.photoresistorValue < 500) {
          digitalWrite(LED_GREEN, 0);
          
          if (counter++ % 3 == 0) redLED = !redLED;

          if (redLED) {
            digitalWrite(LED_RED, HIGH);
            digitalWrite(LED_BLUE, LOW);
          } else {
            digitalWrite(LED_RED, LOW);
            digitalWrite(LED_BLUE, HIGH);
          }
        } else {
          counter = 0;
        }
      
        xSemaphoreGive(mutexPhotoresistor);
      }
      xSemaphoreGive(mutexInfrared);
    }

    vTaskDelay(pdMS_TO_TICKS(50));
  }
}

void motorDriverTask(void *pvParameters) {
  while (true) {
    if(xSemaphoreTake(mutexMotor, pdMS_TO_TICKS(10)) == pdTRUE) {
      motors.setPWM(0, 0, ANGLE_TO_PULSE(sensors.joystickX));
      motors.setPWM(1, 0, ANGLE_TO_PULSE(sensors.joystickY));
      xSemaphoreGive(mutexMotor);
    }

    vTaskDelay(pdMS_TO_TICKS(20));
  }
}

void passiveBuzzerTask(void *pvParameters) {
  while (true) {
    if(xSemaphoreTake(mutexPhotoresistor, pdMS_TO_TICKS(10)) == pdTRUE) {
      if (sensors.photoresistorValue < 500) {
        xSemaphoreGive(mutexPhotoresistor);
        for (int i = 0; i < 25; i++) {
          digitalWrite(BUZZER_PASSIVE, HIGH);
          vTaskDelay(pdMS_TO_TICKS(2));
          digitalWrite(BUZZER_PASSIVE, LOW);
          vTaskDelay(pdMS_TO_TICKS(2));
        }
      } else {
        xSemaphoreGive(mutexPhotoresistor);
      }
    }

    vTaskDelay(pdMS_TO_TICKS(50));
  }
}

void photoresistorTask(void *pvParameters) {
  while (true) {
    if(xSemaphoreTake(mutexPhotoresistor, pdMS_TO_TICKS(10)) == pdTRUE) {
      sensors.photoresistorValue = analogRead(PHOTORESISTOR);
      xSemaphoreGive(mutexPhotoresistor);
    }

    vTaskDelay(pdMS_TO_TICKS(50));
  }
}