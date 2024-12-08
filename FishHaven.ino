#define BLYNK_TEMPLATE_ID "TMPL6gReiJnjU"
#define BLYNK_TEMPLATE_NAME "finpro iot"
#define BLYNK_AUTH_TOKEN "FMuCkehAhh4ZO-QBBDshlhnxw90xNTN5"

#define BLYNK_PRINT Serial

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// Handle untuk task
TaskHandle_t TaskHandleTemperature;
TaskHandle_t TaskHandleDepth;
TaskHandle_t TaskHandleClarity;
TaskHandle_t TaskHandleDataHandler;

// Queue untuk data sensor
QueueHandle_t tempSensorQueue;
QueueHandle_t depthSensorQueue;
QueueHandle_t claritySensorQueue;

// Semaphore untuk sinkronisasi
SemaphoreHandle_t tempSemaphore;
SemaphoreHandle_t depthSemaphore;
SemaphoreHandle_t claritySemaphore;
SemaphoreHandle_t resourceMutex;

// Informasi koneksi WiFi dan Blynk
char auth[] = BLYNK_AUTH_TOKEN;
const char *wifiSSID = "Apa Aja Boleh";
const char *wifiPass = "Jojo195$37";

// const int TempSensorPin = 18;
const int TempSensorPin = 32;
const int DepthSensorPin = 34;
const int ClaritySensorPin = 35;
const int IndicatorLEDPin = 5;

// Variabel untuk menyimpan nilai sensor
float waterTemperature = 0.0;
int waterDepthPercentage = 75;
float waterClarityLevel = 20;

float maxTemperature = 30.0; // Batas suhu
int minWaterDepth = 20;      // Batas kedalaman
float minClarity = 50.0;     // Batas kejernihan

// Struktur untuk data sensor
struct SensorInfo {
  float temperature;
  int depth;
  float clarity;
};

// Inisialisasi untuk sensor suhu (DS18B20)
OneWire oneWire(TempSensorPin);
DallasTemperature tempSensor(&oneWire);

// Fungsi untuk menghubungkan WiFi
void initializeWiFi() {
  Serial.println("Attempting to connect to WiFi...");
  WiFi.begin(wifiSSID, wifiPass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Still connecting...");
  }
  Serial.println("WiFi connected!");
}

// Fungsi untuk mengontrol LED menggunakan tombol di Blynk
BLYNK_WRITE(V3) {
  int ledState = param.asInt(); // Membaca status tombol
  if (ledState == 1) {
    digitalWrite(IndicatorLEDPin, HIGH); // Nyalakan LED
    Serial.println("Indicator LED ON");
  } else {
    digitalWrite(IndicatorLEDPin, LOW); // Matikan LED
    Serial.println("Indicator LED OFF");
  }
}

// Fungsi untuk mengatur batas suhu
BLYNK_WRITE(V4) {
  maxTemperature = param.asFloat();
  Serial.print("Updated Max Temperature: ");
  Serial.println(maxTemperature);
}

// Fungsi untuk mengatur batas kedalaman 
BLYNK_WRITE(V5) {
  minWaterDepth = param.asInt(); 
  Serial.print("Updated Min Water Depth: ");
  Serial.println(minWaterDepth);
}

// Fungsi untuk mengatur batas kejernihan 
BLYNK_WRITE(V6) {
  minClarity = param.asFloat(); 
  Serial.print("Updated Min Clarity: ");
  Serial.println(minClarity);
}

// Task untuk membaca suhu air. Sensor: DS18b20
void temperatureTask(void *pvParameters) {
  QueueHandle_t *queue = (QueueHandle_t *)pvParameters;

  while (true) {
    tempSensor.requestTemperatures();
    vTaskDelay(100);
    float currentTemperature = tempSensor.getTempCByIndex(0); // Temp in Celsius

    if (xQueueSend(*queue, &currentTemperature, portMAX_DELAY) != pdPASS) {
      Serial.println("Temperature data queue is full. Skipping...");
    }

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

// Task untuk mengecek kedalaman air. Sensor: Water Level Sensor
void depthTask(void *pvParameters) {
  QueueHandle_t *queue = (QueueHandle_t *)pvParameters;

  while (true) {
    int rawDepth = analogRead(DepthSensorPin);
    int depthPercentage = map(rawDepth, 0, 4095, 0, 100);
    if (depthPercentage < 0) depthPercentage = 0;

    if (xQueueSend(*queue, &depthPercentage, portMAX_DELAY) != pdPASS) {
      Serial.println("Depth data queue is full. Skipping...");
    }

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

// Task untuk mengecek kejernihan air. Sensor: Turbidity Sensor
void clarityTask(void *pvParameters) {
  QueueHandle_t *queue = (QueueHandle_t *)pvParameters;

  while (true) {
    int clarityRawValue = analogRead(ClaritySensorPin);
    float clarityLevel = map(clarityRawValue, 0, 4095, 0, 100);

    if (xQueueSend(*queue, &clarityLevel, portMAX_DELAY) != pdPASS) {
      Serial.println("Clarity data queue is full. Skipping...");
    }

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

// Task untuk menangani data dari semua sensor
void dataHandlerTask(void *pvParameters) {
  while (true) {
    SensorInfo data;

    // Terima data dari queue dan simpan di struct
    xQueueReceive(tempSensorQueue, &data.temperature, portMAX_DELAY);
    xQueueReceive(depthSensorQueue, &data.depth, portMAX_DELAY);
    xQueueReceive(claritySensorQueue, &data.clarity, portMAX_DELAY);

    // Cetak ke Serial Monitor
    Serial.println("=== Sensor Data ===");
    Serial.print("Temperature: ");
    Serial.print(data.temperature);
    Serial.println(" °C");

    Serial.print("Water Depth: ");
    Serial.print(data.depth);
    Serial.println(" %");

    Serial.print("Water Clarity: ");
    Serial.print(data.clarity);
    Serial.println(" %");

    // Kirim data ke Blynk
    Blynk.virtualWrite(V0, data.temperature);
    Blynk.virtualWrite(V1, data.depth);
    Blynk.virtualWrite(V2, data.clarity);

    // Kirim event jika melampaui batas
    if (data.temperature > maxTemperature) {
      Blynk.logEvent("high_temp", "Water temperature is too high!");
    }
    if (data.depth < minWaterDepth) {
      Blynk.logEvent("low_water_level", "Water level is too low!");
    }
    if (data.clarity < minClarity) {
      Blynk.logEvent("low_clarity", "Water clarity is too low!");
    }

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void setup() {
  tempSensor.begin();
  Serial.begin(115200);
  //initializeWiFi();
  Blynk.begin(auth, wifiSSID, wifiPass);

  pinMode(IndicatorLEDPin, OUTPUT);

  tempSensorQueue = xQueueCreate(10, sizeof(float));
  depthSensorQueue = xQueueCreate(10, sizeof(int));
  claritySensorQueue = xQueueCreate(10, sizeof(float));

  tempSemaphore = xSemaphoreCreateMutex();
  depthSemaphore = xSemaphoreCreateMutex();
  claritySemaphore = xSemaphoreCreateMutex();
  resourceMutex = xSemaphoreCreateMutex();

  xTaskCreatePinnedToCore(
      temperatureTask,
      "TemperatureTask",
      10000,
      (void *)&tempSensorQueue,
      2,
      &TaskHandleTemperature,
      1);

  xTaskCreatePinnedToCore(
      depthTask,
      "DepthTask",
      10000,
      (void *)&depthSensorQueue,
      1,
      &TaskHandleDepth,
      1);

  xTaskCreatePinnedToCore(
      clarityTask,
      "ClarityTask",
      10000,
      (void *)&claritySensorQueue,
      2,
      &TaskHandleClarity,
      0);

  xTaskCreatePinnedToCore(
      dataHandlerTask,
      "DataHandlerTask",
      10000,
      NULL,
      2,
      &TaskHandleDataHandler,
      0);
}

void loop() {
  Blynk.run();
}
