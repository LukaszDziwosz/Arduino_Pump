// Lukasz Dziwosz 2019
// PC watercooling pump arduino controller
// Temperature controlled speed of 12v 2 pin pump
#include <math.h>
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define pwmPin 9           // Digital Pin 9
#define ThermistorPIN 0    // Analog Pin 0

float vcc = 4.21;                       // only used for display purposes, if used
                                        // set to the measured Vcc.
float pad = 9850;                       // balance/pad resistor value, set this to
                                        // the measured resistance of your pad resistor
float thermr = 10000;                   // thermistor nominal resistance

float Thermistor(int RawADC) {
 long Resistance;  
 float Temp;  // Dual-Purpose variable to save space.

 Resistance=pad*((1024.0 / RawADC) - 1); 
 Temp = log(Resistance); // Saving the Log(resistance) so not to calculate  it 4 times later
 Temp = 1 / (0.001129148 + (0.000234125 * Temp) + (0.0000000876741 * Temp * Temp * Temp));
 Temp = Temp - 273.15;  // Convert Kelvin to Celsius                      
  // BEGIN- Remove these lines for the function not to display anything
  //lcd.print("ADC: "); 
  //lcd.print(RawADC); 
  //lcd.print("/1024");                           // Print out RAW ADC Number
  //lcd.print(", vcc: ");
  //lcd.print(vcc,2);
  //lcd.print(", pad: ");
  //lcd.print(pad/1000,3);
  //lcd.print(" Kohms, Volts: "); 
  //lcd.print(((RawADC*vcc)/1024.0),3);   
  //lcd.print(", Resistance: "); 
  //lcd.print(Resistance);
  //lcd.print(" ohms, ");
  // END- Remove these lines for the function not to display anything

  // Uncomment this line for the function to return Fahrenheit instead.
  //temp = (Temp * 9.0)/ 5.0 + 32.0;                  // Convert to Fahrenheit
  
 return Temp;                                      // Return the Temperature
}

void setup() {
 pinMode(pwmPin, OUTPUT);
 lcd.begin(16, 2);
 lcd.print("Pump 100%");
 analogWrite(pwmPin, 255);
 delay(8000); // pump will run 100% on startup for 8 seconds
 analogWrite(pwmPin, 0);
 lcd.clear();
}

void loop() {
 lcd.setCursor(0,0);
 lcd.print("Pump = ");
 
 float T;
 T = Thermistor(analogRead(ThermistorPIN));       // read ADC and  convert it to Celsius
  // Control pump curve here with if statements. Various pumps might need different PWM values (0-255)
  // Also adjust min temperature to your room temperature
  if (T < 18.00) {analogWrite(pwmPin, 0); lcd.print("0%");} //pump is off when temp lower than 18 celcius.
  if (T >=18.00 && T < 19.00) {analogWrite(pwmPin, 125) ; lcd.print("10%");}
  if (T >=19.00 && T < 20.00) {analogWrite(pwmPin, 135) ; lcd.print("20%");}
  if (T >=20.00 && T < 21.00) {analogWrite(pwmPin, 155) ; lcd.print("30%");}
  if (T >=21.00 && T < 22.00) {analogWrite(pwmPin, 185) ; lcd.print("40%");}
  if (T >=22.00 && T < 25.00) {analogWrite(pwmPin, 205) ; lcd.print("50%");}
  if (T >=25.00 && T < 27.00) {analogWrite(pwmPin, 215) ; lcd.print("60%");}
  if (T >=27.00 && T < 30.00) {analogWrite(pwmPin, 225) ; lcd.print("70%");}
  if (T >=30.00 && T < 33.00) {analogWrite(pwmPin, 235) ; lcd.print("80%");}
  if (T >=33.00 && T < 36.00) {analogWrite(pwmPin, 245) ; lcd.print("90%");}
  if (T >36.00) {analogWrite(pwmPin, 255) ; lcd.print("100%");} // pump is running 100% if temp is over 36 cecius
  
 lcd.print(" PWM");
 lcd.setCursor(0,1);
 lcd.print("Temp = ");
 lcd.print(T);   
 lcd.print("'C");
 delay(1000);

}
