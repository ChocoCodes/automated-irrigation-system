// Defining pins
#define moistureSensor1 A0 
#define moistureSensor2 A1
#define moistureSensor3 A2
#define solenoid 13

// Max values from the calibration of three sensors
const int dryValue1 = 473;
const int dryValue2 = 479;
const int dryValue3 = 465;

const int wetValue1 = 190;
const int wetValue2 = 203;
const int wetValue3 = 195;

// Global variables for raw analog reading values and converted percentages
int moistureValue1 = 0;
int moistureValue2 = 0;
int moistureValue3 = 0;

int moisturePercent1 = 0;
int moisturePercent2 = 0;
int moisturePercent3 = 0;

// Global variable to hold data from Serial Monitor / Bluetooth
String data;

void setup() {
  Serial.begin(9600);
  delay(100);
  pinMode(moistureSensor1, INPUT);
  pinMode(moistureSensor2, INPUT);
  pinMode(moistureSensor3, INPUT);
  pinMode(solenoid, OUTPUT); 

}

void loop() {
  readMoisture();
  displayMoisture();
  readBTData();
  executeMode();
}

// Function to read the moisture and convert to percent
void readMoisture() {
  moistureValue1 = analogRead(moistureSensor1);
  moistureValue2 = analogRead(moistureSensor2);
  moistureValue3 = analogRead(moistureSensor3);

  moisturePercent1 = map(moistureValue1, dryValue1, wetValue1, 0, 100); 
  moisturePercent2 = map(moistureValue2, dryValue2, wetValue2, 0, 100); 
  moisturePercent3 = map(moistureValue3, dryValue3, wetValue3, 0, 100);
}

// Function to display moisture percentage readings
void displayMoisture() {
  // Output for sensor 1
  if (moisturePercent1 <= 0) {
    Serial.println("Sensor 1: 0%");
  }
  else if (moisturePercent1 >= 100) {
    Serial.println("Sensor 1: 100%");
  }
  else {
    Serial.println((String)"Sensor 1: " + moisturePercent1 + "%");
  }

  // Output for sensor 2
  if (moisturePercent2 <= 0) {
    Serial.println("Sensor 2: 0%");
  }
  else if (moisturePercent2 >= 100) {
    Serial.println("Sensor 2: 100%");
  }
  else {
    Serial.println((String)"Sensor 2: " + moisturePercent2 + "%");
  }

  // Output for sensor 3
  if (moisturePercent3 <= 0) {
    Serial.println("Sensor 3: 0%");
  }
  else if (moisturePercent3 >= 100) {
    Serial.println("Sensor 3: 100%");
  }
  else {
    Serial.println((String)"Sensor 3: " + moisturePercent3 + "%");
  }

  delay(1000);
}

// Function to read data from Serial Monitor
void readBTData() {
  if (Serial.available()) {
    data = Serial.readStringUntil('\n');
    Serial.println("Data Received: " + data);
  } 
}

// Function to execute the mode based on data value
void executeMode() {
  if (data == "0") { // If data is "0", close the solenoid valve
    digitalWrite (solenoid, HIGH);      
  }
  else if (data == "1") { // If data is "1", open the solenoid valve
    digitalWrite (solenoid, LOW);
  }
  else if (data == "2") { // If data is "2", set mode to automatic
    setAutomatic();
  }
}

// Function to set mode to automatic sensor reading ang reactive valve control
void setAutomatic() {
  // 3 = dry :: 98 = wet max values (in percent)
  if ((moisturePercent1 < 3) || (moisturePercent2 < 3) || (moisturePercent3 < 3)) { 
    while (true) {
      readMoisture();
      displayMoisture();

      // If any percentages are less than 98, keep the valve open
      if ((moisturePercent1 < 98) || (moisturePercent2 < 98) || (moisturePercent3 < 98)) {
        digitalWrite(solenoid, LOW);
        delay(5000);
      }
      else {
        digitalWrite(solenoid, HIGH);
        // Once valve is closed, this will break it out of the loop
        break;          
      }
    }
  }
  else {
    digitalWrite(solenoid, HIGH);
  }
}
  
