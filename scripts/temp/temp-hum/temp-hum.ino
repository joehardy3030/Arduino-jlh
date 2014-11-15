int tempHumPin0 = 0;

void setup()
{
   // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
  Serial.begin(9600);
}
 
void loop()                     // run over and over again
{
 float volt0 = getVoltage(tempHumPin0);
 float analog0 = analogRead(tempHumPin0); 
 float tempC0 = (volt0-0.5)*100;
 float tempF0 = (((volt0-0.5)*100)*1.8)+32;
 //Serial.println(volt1);                     //printing the result
 Serial.print("Analog output ");
 Serial.println(analog0);                     //printing the result
 Serial.print("Voltage ");
 Serial.println(volt0);                     //printing the result
 Serial.print("TempC0 ");
 Serial.println(tempC0);
 Serial.print("TempF0 ");
 Serial.println(tempF0);
 delay(1000);
}

// getVoltage() - returns the voltage on the analog input defined by pi n
float getVoltage(int pin){
 return ((analogRead(pin) / 1023.0)*5); //converting from a 0 to 1023 digital range
                                        // to 0 to 5 volts (each 1 reading equals ~ 5 millivolts
}
