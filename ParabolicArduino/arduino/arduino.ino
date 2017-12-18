 #include <CapacitiveSensor.h>

CapacitiveSensor cs_5_4 = CapacitiveSensor(5,4); // 1M resistor between pins 4 & 8, pin 8 is sensor pin, add a wire and or foil
// 5 is the send pin. (OUTPUT)
// 4 is the receive pin. (INPUT) - Connect from the dish

// Smoothing sensor1 values.
float smoothSensor1 = 0;
float correction = 0.95;

// Time
unsigned long startTime = 0;

void setup()                    
{ 
   // Turn off AutoCalibration. 
   cs_5_4.set_CS_AutocaL_Millis(0xFFFFFFFF);
   Serial.begin(9600);
}

void loop()                    
{ 
  unsigned long currentTime = millis();

  // Recalibrate every 3 minutes
  if (currentTime - startTime > 3 * 60000) { // Every 3 minutes
    startTime = currentTime;  
    cs_5_4.reset_CS_AutoCal();
  }
  
  // Sets the number of samples based on which the sensor value is
  // calculated. More the samples, higher the delay but more accuracy.
  long sensor1 = cs_5_4.capacitiveSensor(50);
  smoothSensor1 = correction * smoothSensor1 + (1 - correction) * sensor1; 

  // Data to be sent over serial communication to openFrameworks. 
  Serial.print(smoothSensor1);
  Serial.print("\n");
  
  // Delay to make sure serial buffers aren't overwhelmed.
  delay(20);
}


