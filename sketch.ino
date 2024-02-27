#include <Arduino.h>
#include "DHT.h"
#define BLYNK_TEMPLATE_ID "TMPL3PCqX_8CW"
#define BLYNK_TEMPLATE_NAME "Automatic Plant Monitoring System"
#define BLYNK_AUTH_TOKEN "ntvpFIbunUx_tYmBpX_Gi1eDfx9_PW8R"
#include "time.h"
#include <ESP_Google_Sheet_Client.h>

#include "NeuralNetwork.h"
NeuralNetwork *nn;

#include <DHTesp.h>
#include <BlynkSimpleEsp32.h>
#define DHTPIN 19
#define LED_PIN 23
#define moisture_sensor_PIN 35
#define ph_sensor_PIN 34
#define N_sensor_PIN 32
#define P_sensor_PIN 33
#define K_sensor_PIN 25

#include "pitches.h"


#include <LiquidCrystal_I2C.h>

#define I2C_ADDR    0x27
#define LCD_COLUMNS 20
#define LCD_LINES   4

LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLUMNS, LCD_LINES);

#include "SevSeg.h"

SevSeg sevseg;


DHTesp dht;
char auth[] = BLYNK_AUTH_TOKEN;
char WIFI_SSID[] = "Wokwi-GUEST";
char WIFI_PASSWORD[] = "";

// Google Project ID
#define PROJECT_ID "chs-project-412017"

// Service Account's client email
#define CLIENT_EMAIL "plant-monitoring@chs-project-412017.iam.gserviceaccount.com"

// Service Account's private key
const char PRIVATE_KEY[] PROGMEM = "-----BEGIN PRIVATE KEY-----\nMIIEvQIBADANBgkqhkiG9w0BAQEFAASCBKcwggSjAgEAAoIBAQC+8560WbYRzg0d\nonOcu+4KtJ3rhifoiYkI4TrcFpUMMJhs8vkVsH/0ZMkq0MlvA+urTBgehAy3/hPL\nCS/tZGkm1Ni3JAwV77EroKXurx8Tv1Cd4d0H83RzVkbh3gUVeG0Y4LvFza2FqRmp\n/GpjWFtyIRfjXozHeezD82T6pXSMjLQVA5rrXqYQ4r6bkGmJOoLdu2ivmUcHnuiu\nh0mlpvnpPP0E9wVWYG5HHPLBmq4QGx+VP1OVa52Rh/9kSk4YMLeQXDxAHypOKKZD\n+TxE+MLLzlwHlRACCru4ZQNexi/NDZGJxBY2O5j+2C4e/T1PVWZ8Ua4XPkAynioH\nfNR2/2XDAgMBAAECggEALUwIYVsIrEeXZ+jCCTOxf7uh3J2p+OkrZz5aopv+xnH+\nrpWizejdKaIA8JWvuxSQgsWd0RtdEwaENn1a77MKmKoUMLmt3kRKYIGO60SCmEIh\ngxpxvvGbNcMJNIpg5Vt4cdbZ5E3qtFLNYjwdFqz+oF/GVWr39VfwePlTgnveXnqX\nPXYrMp0ZKcr7kNBeXyVB3sJ0ViR3MStoWpSTVQhjcvEoCfNYI/KzB5oQJGKEPwwh\nfF+ebd2lpLJZ13Qzn462OtyfTIElbMuv7TkXJMFQR+EN07jiyl0HYUBqwadhr26f\nlqAQGCCHSTRjEMq70ZqnEjyqs4ueHHKAj1N8G4l0eQKBgQDmtw1CobSs4QiFUMWW\nKnSI5+Sj1HjeJcuvTRps92V6+OACqDmOPoH5fxAA+biupsLP2OacQbLtQnUIH8rz\n48mBlN88Ez32Y+iZC283GQwCjNQtGY0AzYDJH8cGs9gk3VfMocqT4bC0qG/IlZgF\nquDF+MYWVZPjN8XNFkH21vI4mwKBgQDT4PcAPEnA9HAlv98vCDKIdo3T+If89xcZ\ngE0wkZNNiIWzVM4YWNuIoIucWi3Ing6W4VIkEtVCIfBaKWaX5rld69SzKIiMDd2q\n4g471/pmus0zeFcbk2xR6t0SJbpsE6CLEAs/lmQW62M/DyqJTbexb2tFBi+/1jim\n8lUdYk71+QKBgAZlgtebFiBI6OyRTbkZGjwFeeBU7rjyAVCt6ZKl1Ulwhv1r3pSk\nBQR+0lnEkqs88l/C2rnwVF6hng8Imp7ibN7L2tiWi/w9zuAXYhD7/8o+T0+kyrT/\n/ZcV63WzHMYA4QNKZQxukHChzt+XziwyDaNPf2Z1HvGIxVInUNLgKL0LAoGBAIo2\nYwnNShNLoOiPHEiy4Tvxkh0nL5ZNnMzuy+uV2vAl0YmlFXf+Zt7eU5Qfo2QN7thv\njUSo4GJufzphbWJDiMICt9JS65fo56O0KLLS9a4Txw3/P1tmtK9PNPzEjbI4r25n\nfd7x/MKStEALZaUBhIWDeAcjvPw4Auash+dOHNIJAoGAQgr/+6MN3hn68Nnjv5ir\nrsSbWaDpRzOKOTdFm4P4scwjFWJN84QfqqUVj2+tGh98dZZHs8PBC0PYAtB6XMtG\nvisMMR/ePGVm7ROvNiHGh7DJdBuKQD+0Fi6jZcR5KrJ510Vs+1fIJMogCrwsMnUd\nUn0WLnBe17VPPZC92TL/Mlw=\n-----END PRIVATE KEY-----\n";

// The ID of the spreadsheet where you'll publish the data
const char spreadsheetId[] = "1E1810hD-4Gu2QGYCQhvfexgWC0va0bIQ9wh7MxCPM7E";

// Timer variables
unsigned long lastTime = 0;  
unsigned long timerDelay = 30000;



// Token Callback function
void tokenStatusCallback(TokenInfo info);

// NTP server to request epoch time
const char* ntpServer = "pool.ntp.org";

// Variable to save current epoch time
unsigned long epochTime; 

// Function that gets current epoch time
unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    //Serial.println("Failed to obtain time");
    return(0);
  }
  time(&now);
  return now;
}

BlynkTimer timer;

void playLevelUpSound() {
  tone(LED_PIN, NOTE_E4);
  delay(150);
  tone(LED_PIN, NOTE_G4);
  delay(150);
  tone(LED_PIN, NOTE_E5);
  delay(150);
  tone(LED_PIN, NOTE_C5);
  delay(150);
  tone(LED_PIN, NOTE_D5);
  delay(150);
  tone(LED_PIN, NOTE_G5);
  delay(150);
  noTone(LED_PIN);
}

void setup() {
  lcd.init();
  lcd.backlight();
  nn = new NeuralNetwork();

  pinMode(LED_PIN, OUTPUT);

  Serial.begin(115200);
  Blynk.begin(auth, WIFI_SSID, WIFI_PASSWORD);
  dht.setup(DHTPIN, DHTesp::DHT22); // Initialize DHT sensor
  byte numDigits = 4;
  byte digitPins[] = {14, 15, 2, 5};
  byte segmentPins[] = {12, 4, 19, 26, 27, 13, 18, 0};
  bool resistorsOnSegments = false; // 'false' means resistors are on digit pins
  byte hardwareConfig = COMMON_ANODE; // See README.md for options
  bool updateWithDelays = true; // Default 'false' is Recommended
  bool leadingZeros = false; // Use 'true' if you'd like to keep the leading zeros
  bool disableDecPoint = false; // Use 'true' if your decimal point doesn't exist or isn't connected. Then, you only need to specify 7 segmentPins[]

  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments,
               updateWithDelays, leadingZeros, disableDecPoint);

  sendData();
  //Configure time
  configTime(0, 0, ntpServer);

  GSheet.printf("ESP Google Sheet Client v%s\n\n", ESP_GOOGLE_SHEET_CLIENT_VERSION);

  // Connect to Wi-Fi
  WiFi.setAutoReconnect(true);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  // Set the callback for Google API access token generation status (for debug only)
  GSheet.setTokenCallback(tokenStatusCallback);

  // Set the seconds to refresh the auth token before expire (60 to 3540, default is 300 seconds)
  GSheet.setPrerefreshSeconds(10 * 60);

  // Begin the access token generation for Google API authentication
  GSheet.begin(CLIENT_EMAIL, PROJECT_ID, PRIVATE_KEY);
}

BLYNK_WRITE(V4) {
  int buttonState = param.asInt();
  if (buttonState == 1) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
}

void sendData() {
  float t = dht.getTemperature();
  float h = dht.getHumidity();

  float moisture = 20 + (analogRead(moisture_sensor_PIN) % 80)+ ((analogRead(moisture_sensor_PIN) % 100)*0.01);
  float pH = ((analogRead(ph_sensor_PIN) % 100)*0.01) + (analogRead(ph_sensor_PIN) % 14);
  
  if (isnan(h) || isnan(t) || isnan(moisture) || isnan(pH)) {
    Serial.println("Failed to read data!");
    return;
  }

  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, h);
  Blynk.virtualWrite(V2, moisture);
  Blynk.virtualWrite(V3, pH);

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print("%   ");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" °C  ");
  Serial.print("Soil Moisture: ");
  Serial.print(moisture);
  Serial.print("%   ");
  Serial.print("pH level: ");
  Serial.println(pH);
}
void showPH(){
  float ph_curr = ((analogRead(ph_sensor_PIN) % 100)*0.01) + (analogRead(ph_sensor_PIN) % 14);
  sevseg.setNumber(ph_curr*100,2);
  int ct=100;
  while(ct){
    sevseg.refreshDisplay();
    ct--;
  }
}

void loop() {
  Blynk.run();
  timer.run();
  //sendData();
  timer.setInterval(2000L, sendData); // Send DHT data every 2 seconds
  timer.setInterval(2000L, showPH); // Send DHT data every 2 seconds
  // Call ready() repeatedly in loop for authentication checking and processing
  bool ready = GSheet.ready();

  float temp_input = dht.getTemperature();
  float hum_input = dht.getHumidity();
  float moisture_curr = 20 + (analogRead(moisture_sensor_PIN) % 80)+ ((analogRead(moisture_sensor_PIN) % 100)*0.01);
  float ph_curr = ((analogRead(ph_sensor_PIN) % 100)*0.01) + (analogRead(ph_sensor_PIN) % 14);
  float n = 20 + analogRead(N_sensor_PIN) % 80;
  float p = 20 + analogRead(P_sensor_PIN) % 80;
  float k = 20 + analogRead(K_sensor_PIN) % 80;


  lcd.setCursor(0, 0);
  lcd.print("Soil Moisture: ");
  lcd.setCursor(15, 0);
  lcd.print(moisture_curr,1);
  lcd.setCursor(0, 1);
  lcd.print("N level: ");
  lcd.setCursor(15, 1);
  lcd.print(n);
  lcd.setCursor(0, 2);
  lcd.print("P level: ");
  lcd.setCursor(15, 2);
  lcd.print(p,1);
  lcd.setCursor(0, 3);
  lcd.print("K level: ");
  lcd.setCursor(15, 3);
  lcd.print(k,1);


  nn->getInputBuffer()[0] = temp_input;
  nn->getInputBuffer()[1] = hum_input;

  float moisture_pred = nn->predict();

  //const char *expected = number2 > number1 ? "True" : "False";

  //float *predicted_moisture = result;

  Serial.printf("Predicted moisture: %.2f\n", moisture_pred);


  // float moisture_pred = predict_moisture(temp_input,hum_input);
  // Serial.print("Moisture predicted: ");
  // Serial.println(moisture_pred);

  if (moisture_curr < moisture_pred) {
    digitalWrite(LED_PIN, HIGH);
    playLevelUpSound();
  } else {
    digitalWrite(LED_PIN, LOW);
  }

 

  if (ready && millis() - lastTime > timerDelay){
      lastTime = millis();

      FirebaseJson response;

      Serial.println("\nAppend spreadsheet values...");
      Serial.println("----------------------------");

      FirebaseJson valueRange;


      float hum = dht.getHumidity();
//   // Read temperature as Celsius (the default)
      float temp = dht.getTemperature();
      float moisture = 20 + (analogRead(moisture_sensor_PIN) % 80) + ((analogRead(moisture_sensor_PIN) % 100)*0.01);
      float pH = ((analogRead(ph_sensor_PIN) % 100)*0.01) + (analogRead(ph_sensor_PIN) % 14);
      // Get timestamp
      epochTime = getTime();

      valueRange.add("majorDimension", "COLUMNS");
      valueRange.set("values/[0]/[0]", epochTime);
      valueRange.set("values/[1]/[0]", temp);
      valueRange.set("values/[2]/[0]", hum);
      valueRange.set("values/[3]/[0]", moisture);
      valueRange.set("values/[4]/[0]", pH);

      // For Google Sheet API ref doc, go to https://developers.google.com/sheets/api/reference/rest/v4/spreadsheets.values/append
      // Append values to the spreadsheet
      bool success = GSheet.values.append(&response /* returned response */, spreadsheetId /* spreadsheet Id to append */, "Sheet1!A1" /* range to append */, &valueRange /* data range to append */);
      if (success){
          response.toString(Serial, true);
          valueRange.clear();
      }
      else{
          Serial.println(GSheet.errorReason());
      }
      Serial.println();
      Serial.println(ESP.getFreeHeap());
  }
  // sevseg.setNumber(ph_curr*100,2);
  // sevseg.refreshDisplay();
  
  // delay(10); // this speeds up the simulation
}


void tokenStatusCallback(TokenInfo info){
    if (info.status == token_status_error){
        GSheet.printf("Token info: type = %s, status = %s\n", GSheet.getTokenType(info).c_str(), GSheet.getTokenStatus(info).c_str());
        GSheet.printf("Token error: %s\n", GSheet.getTokenError(info).c_str());
    }
    else{
        GSheet.printf("Token info: type = %s, status = %s\n", GSheet.getTokenType(info).c_str(), GSheet.getTokenStatus(info).c_str());
    }
}
