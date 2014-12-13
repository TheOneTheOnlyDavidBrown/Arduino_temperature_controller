char commandIn;
String commandInArray;
int sensorLevel;
//int relayPin = 9;
int sensorPin = 0; // will be 10 when new temp sensor is used
int targetTemp = 55;
int upButtonPin = 12;
int downButtonPin = 11;
int myleds[] = {8, 7, 6, 5, 4, 3, 2};
int i;
 
void setup() {
    Serial.begin(9600);
    for (i = 0; i < 7; i += 1) {
        pinMode(myleds[i],OUTPUT);
    }
}

void loop() {

    //reads serial input from command
    readDataIn();
    
    //toggles relay on/off based on if the temperature is greater than the target temperature
    if (overTargetTemp()) {
        Serial.println("over target. turning relay on");
        //turn on relay
        //digitalWrite(relayPin,HIGH);
    }
    else {
        Serial.println("not over target");
        //turn off relay
        //digitalWrite(relayPin,LOW);
    }
    
    Serial.print("targetTemp: ");
    Serial.println(targetTemp);
    
    if (digitalRead(upButtonPin) == HIGH) {
        targetTemp++;
        ledOutput();
        delay(200);
    }
    else if (digitalRead(downButtonPin) == HIGH) {
        targetTemp--;
        ledOutput();
        delay(200);aoeu
    }
    delay(3000);  
}

void readDataIn() {
    commandInArray = "";
    byte dataIn = Serial.available();

    if(dataIn>0) {
        for (i=0;i<dataIn;i++) {
            commandIn = Serial.read();
            commandInArray += commandIn;
        }
            
        Serial.print("commandInArray: ");
        Serial.println(commandInArray);
        
        const char * c = commandInArray.c_str();
        Serial.println(c);
        targetTemp=atoi(c);
        targetTemp = commandInArray.toInt();
    }
}

bool overTargetTemp() {

    sensorLevel = thermistor(analogRead(sensorPin));
    Serial.print("current sensor reading: ");
    Serial.println(sensorLevel);
    
    if (sensorLevel>targetTemp) {
        return true;
    }
    else {
        return false;
    }
}

float rPad = 2000;                       // balance/pad resistor value, set this to
                                        // the measured resistance of your pad resistor
float thermistor(int RawADC) {
  long Resistance;  
  float Temp;  // Dual-Purpose variable to save space.

  Resistance=((1024 * rPad / RawADC) - rPad); 
  Temp = log(Resistance); // Saving the Log(resistance) so not to calculate  it 4 times later
  Temp = 1 / (0.001129148 + (0.000234125 * Temp) + (0.0000000876741 * Temp * Temp * Temp));
  Temp = (Temp - 273.15) *  1.8 + 32; //K to F
  ledOutput();
  return Temp;  // Return the Temperature
}

void ledOutput() {
    float temp=targetTemp;
    
    String tbin = String(int(temp),BIN);
    
    //pad 0s so the same pins work regardless of the binary length of the temp
    int numZerosPadding = 7-tbin.length();
    for (i = 0; i < numZerosPadding; i++) {
        tbin = "0"+tbin;
    }
    
    //Serial.print("tbin: ");
    //Serial.println(tbin);
    
    //Serial.println("turning on pins: ");
    
    for (i = tbin.length(); i >= 0; i--) {
    
        if(tbin[i]=='1') {
            //Serial.print(myleds[i]);
            //Serial.print(" ");
            digitalWrite(myleds[i] , HIGH);
        }
        else {
            digitalWrite(myleds[i] , LOW);
        }
    }
    Serial.println();
}
