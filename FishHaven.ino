#define BLYNK_TEMPLATE_ID "TMPL69z-Bi-_s"
#define BLYNK_TEMPLATE_NAME "FishHaven"
#define BLYNK_AUTH_TOKEN "ey98oPBczRbNhUEDfUveEWVp-3tMvrTq"

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
const char *wifiSSID = "Wokwi-GUEST";
const char *wifiPass = "";

const int TempSensorPin = 18;
//const int DepthSensorPin = 34;
//const int ClaritySensorPin = 35;
const int IndicatorLEDPin = 5;

// Variabel untuk menyimpan nilai sensor
float waterTemperature = 0.0;
int waterDepthPercentage = 75;
float waterClarityLevel = 20;

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

// Task untuk membaca suhu air
void temperatureTask(void *pvParameters) {
  QueueHandle_t *queue = (QueueHandle_t *)pvParameters;

  while (true) {
    tempSensor.requestTemperatures();
    delay(100);
    float currentTemperature = tempSensor.getTempCByIndex(0);

    SensorInfo data;
    data.temperature = currentTemperature;

    if (xQueueSend(*queue, &data, portMAX_DELAY) != pdPASS) {
      Serial.println("Temperature data queue is full. Skipping...");
    }

    Blynk.virtualWrite(V0, currentTemperature);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

// Task untuk membaca kedalaman air
void depthTask(void *pvParameters) {
  QueueHandle_t *queue = (QueueHandle_t *)pvParameters;

  while (true) {
    //int rawDepth = analogRead(DepthSensorPin);
    //waterDepthPercentage = map(rawDepth, 1500, 4095, 0, 100);
    //if (waterDepthPercentage < 0) waterDepthPercentage = 0;

    SensorInfo data;
    data.depth = waterDepthPercentage;

    if (xQueueSend(*queue, &data, portMAX_DELAY) != pdPASS) {
      Serial.println("Depth data queue is full. Skipping...");
    }

    Blynk.virtualWrite(V1, waterDepthPercentage);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

// Task untuk membaca tingkat kekeruhan air
void clarityTask(void *pvParameters) {
  QueueHandle_t *queue = (QueueHandle_t *)pvParameters;

  while (true) {
    //int clarityRawValue = analogRead(ClaritySensorPin);
    //waterClarityLevel = clarityRawValue;

    SensorInfo data;
    data.clarity = waterClarityLevel;

    if (xQueueSend(*queue, &data, portMAX_DELAY) != pdPASS) {
      Serial.println("Clarity data queue is full. Skipping...");
    }

    Blynk.virtualWrite(V2, waterClarityLevel);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);
  initializeWiFi();
  Blynk.begin(auth, wifiSSID, wifiPass);

  pinMode(IndicatorLEDPin, OUTPUT);
  tempSensor.begin();

  tempSensorQueue = xQueueCreate(10, sizeof(SensorInfo));
  depthSensorQueue = xQueueCreate(10, sizeof(SensorInfo));
  claritySensorQueue = xQueueCreate(10, sizeof(SensorInfo));

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
}

void loop() {
  Blynk.run();
}