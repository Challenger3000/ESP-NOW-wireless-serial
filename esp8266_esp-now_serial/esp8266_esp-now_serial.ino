#include <ESP8266WiFi.h>
#include <espnow.h>

// Replace with your receiver's MAC address
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// Buffer to store bytes pooled for sending
uint8_t buffer[240];
// Buffer to store bytes received
uint8_t bufferRec[250];
// Index for the buffer
size_t bufferIndex = 0;

// Last time a byte was received
unsigned long lastByteTime = 0;

void setup() {
  // Start the Serial communication
  Serial.begin(115200);

  // Set device as a Wi-Fi station
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
  
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  while (Serial.available()) {
    // Read the incoming byte
    buffer[bufferIndex++] = Serial.read();
    lastByteTime = millis();
    
    // Check if buffer is full
    if (bufferIndex == sizeof(buffer)) {
      // Send the packet
      esp_now_send(broadcastAddress, buffer, bufferIndex);
      // clear buffer
      memset(buffer, 0, sizeof(buffer));
      // Reset the buffer index
      bufferIndex = 0;
    }
  }

  // Check for timeout (1ms since last byte received)
  if (millis() - lastByteTime >= 10 && bufferIndex > 0) {
    // Send the packet
    esp_now_send(broadcastAddress, buffer, bufferIndex);
    // clear buffer
    memset(buffer, 0, sizeof(buffer));
    // Reset the buffer index
    bufferIndex = 0;
  }
  yield();
}

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {

}

// Callback when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  // Ensure we don't read past the buffer size
  size_t copySize = (len <= sizeof(bufferRec)) ? len : sizeof(bufferRec);
  memcpy(bufferRec, incomingData, copySize);
  Serial.write(bufferRec, copySize);
}
