/*     ---------------------------------------------------------
 *     |  Arduino Experimentation Kit Example Code             |
 *     |  CIRC-10 .: Temperature :. (TMP36 Temperature Sensor) |
 *     ---------------------------------------------------------
 *   
 *  A simple program to output the current temperature to the IDE's debug window 
 * 
 *  For more details on this circuit: http://tinyurl.com/c89tvd 
 */

//TMP36 Pin Variables
int temperaturePin = 0; //the analog pin the TMP36's Vout (sense) pin is connected to
                        //the resolution is 10 mV / degree centigrade 
                        //(500 mV offset) to make negative temperatures an option

/*
 * setup() - this function runs once when you turn your Arduino on
 * We initialize the serial connection with the computer
 */
void setup()
{
   // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  Serial.begin(9600);  //Start the serial connection with the copmuter
                       //to view the result open the serial monitor 
                       //last button beneath the file bar (looks like a box with an antenae)
}
 
void loop()                     // run over and over again
{
 float temperature = getVoltage(temperaturePin);  //getting the voltage reading from the temperature sensor
 temperature = (temperature - .5) * 100;          //converting from 10 mv per degree wit 500 mV offset
                                                  //to degrees ((volatge - 500mV) times 100)
 Serial.println(temperature);                     //printing the result
 if(temperature > 15.5){
   digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
   delay(1000);              // wait for a second
   digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
   delay(1000);              // wait for a second
   digitalWrite(2, HIGH);   // turn the LED on (HIGH is the voltage level)
   digitalWrite(3, LOW);   // turn the LED on (HIGH is the voltage level)
 }
 else {
   digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
   delay(100);              // wait for a second
   digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
   delay(100);              // wait for a second
   digitalWrite(2, LOW);    // turn the LED off by making the voltage LOW
   digitalWrite(3, HIGH);// turn the LED off by making the voltage LOW
 }   
}

/*
 * getVoltage() - returns the voltage on the analog input defined by
 * pin
 */
float getVoltage(int pin){
 return (analogRead(pin) * .004882814); //converting from a 0 to 1023 digital range
                                        // to 0 to 5 volts (each 1 reading equals ~ 5 millivolts
}
