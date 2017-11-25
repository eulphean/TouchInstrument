 #include <CapacitiveSensor.h>

CapacitiveSensor cs_1_0 = CapacitiveSensor(5,4); // 1M resistor between pins 4 & 8, pin 8 is sensor pin, add a wire and or foil
// 0 is the send pin. (OUTPUT)
// 1 is the receive pin. (INPUT) - Connect from the dish
CapacitiveSensor cs_3_2 = CapacitiveSensor(7,6);
// 3 is the send pin. (OUTPUT)
// 2 is the receive pin. (INPUT) - Connect from the dish

int LED0 = 8;
int LED1 = 9;

void setup()                    
{
   cs_1_0.set_CS_AutocaL_Millis(0xFFFFFFFF);// turn off autocalibrate on channel 1 - just as an example
   cs_3_2.set_CS_AutocaL_Millis(0xFFFFFFFF);
   Serial.begin(9600);

   // LEDS
   pinMode(LED0,OUTPUT);
   pinMode(LED1, OUTPUT);
}

void loop()                    
{
  // Sets the number of samples based on which the sensor value is
  // calculated. More the samples, higher the delay but more accuracy.
  long sensor1 = cs_1_0.capacitiveSensor(50);
  long sensor2 = cs_3_2.capacitiveSensor(50);

  // Data to be sent over serial communication to openFrameworks. 
  Serial.print(sensor1);
  Serial.print(",");
  Serial.print(sensor2);
  Serial.print("\n");
  
  if(sensor1 >=400)
  {
    int brightness = map(sensor1, 50, 1000, 10, 255); 
    digitalWrite(LED0,HIGH);
  }
  else{
    digitalWrite(LED0,LOW);
  }  

  /*if(sensor2 >=200)
  {
    digitalWrite(LED1,HIGH);
  }
  else{
    digitalWrite(LED1,LOW);
  }  */

  // Delay to make sure serial buffers aren't overwhelmed.
  delay(20);
}

