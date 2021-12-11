#include <WiFi.h>
#include <HTTPClient.h>
#define WIFI_STA_NAME "SSID_NAME"
#define WIFI_STA_PASS "SSID_PASSWORD"
String url = "https://api.bitkub.com/api/market/ticker";
HTTPClient http;

#include <ArduinoJson.h>
StaticJsonDocument<20000> doc;

#include <Wire.h>
#include "SSD1306Wire.h"
#include "OLEDDisplayUi.h"
SSD1306Wire display(0x3c, 5, 4);
OLEDDisplayUi ui     (&display);
String content;

unsigned long previousMillis = 0;
const long interval = 5000;

class CRYPTO {
  public:
    CRYPTO(String type);
    void docodeData(String dataText);
    float last;
    float high24hr;
    float low24hr;
    float percentChange;
    String _type;
};
CRYPTO::CRYPTO(String type) {
  _type = type;
}
void CRYPTO::docodeData(String dataText) {
  DeserializationError error = deserializeJson(doc, content);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  last = doc[_type]["last"];
  high24hr = doc[_type]["high24hr"];
  low24hr = doc[_type]["low24hr"];
  percentChange = doc[_type]["percentChange"];
}

CRYPTO btc("THB_BTC");
CRYPTO iost("THB_IOST");
CRYPTO eth("THB_ETH");
CRYPTO ada("THB_ADA");
CRYPTO doge("THB_DOGE");

void drawFrame1(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  display->drawString(0 + x, 0 + y, "BTC:");
  display->drawString(50 + x, 0 + y, String(btc.last) + " B");
  display->drawString(0 + x, 15 + y, "Low:");
  display->drawString(50 + x, 15 + y, String(btc.low24hr) + " B");
  display->drawString(0 + x, 30 + y, "High:");
  display->drawString(50 + x, 30 + y, String(btc.high24hr) + " B");
  display->drawString(0 + x, 45 + y, "Change:");
  display->drawString(50 + x, 45 + y, String(btc.percentChange) + " %");
}
void drawFrame2(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  display->drawString(0 + x, 0 + y, "IOST:");
  display->drawString(50 + x, 0 + y, String(iost.last) + " B");
  display->drawString(0 + x, 15 + y, "Low:");
  display->drawString(50 + x, 15 + y, String(iost.low24hr) + " B");
  display->drawString(0 + x, 30 + y, "High:");
  display->drawString(50 + x, 30 + y, String(iost.high24hr) + " B");
  display->drawString(0 + x, 45 + y, "Change:");
  display->drawString(50 + x, 45 + y, String(iost.percentChange) + " %");
}
void drawFrame3(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  display->drawString(0 + x, 0 + y, "ETH:");
  display->drawString(50 + x, 0 + y, String(eth.last) + " B");
  display->drawString(0 + x, 15 + y, "Low:");
  display->drawString(50 + x, 15 + y, String(eth.low24hr) + " B");
  display->drawString(0 + x, 30 + y, "High:");
  display->drawString(50 + x, 30 + y, String(eth.high24hr) + " B");
  display->drawString(0 + x, 45 + y, "Change:");
  display->drawString(50 + x, 45 + y, String(eth.percentChange) + " %");
}
void drawFrame4(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  display->drawString(0 + x, 0 + y, "ADA:");
  display->drawString(50 + x, 0 + y, String(ada.last) + " B");
  display->drawString(0 + x, 15 + y, "Low:");
  display->drawString(50 + x, 15 + y, String(ada.low24hr) + " B");
  display->drawString(0 + x, 30 + y, "High:");
  display->drawString(50 + x, 30 + y, String(ada.high24hr) + " B");
  display->drawString(0 + x, 45 + y, "Change:");
  display->drawString(50 + x, 45 + y, String(ada.percentChange) + " %");
}
void drawFrame5(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  display->drawString(0 + x, 0 + y, "DOGE:");
  display->drawString(50 + x, 0 + y, String(doge.last) + " B");
  display->drawString(0 + x, 15 + y, "Low:");
  display->drawString(50 + x, 15 + y, String(doge.low24hr) + " B");
  display->drawString(0 + x, 30 + y, "High:");
  display->drawString(50 + x, 30 + y, String(doge.high24hr) + " B");
  display->drawString(0 + x, 45 + y, "Change:");
  display->drawString(50 + x, 45 + y, String(doge.percentChange) + " %");
}

FrameCallback frames[] = { drawFrame1, drawFrame2, drawFrame3, drawFrame4, drawFrame5 };
int frameCount = 5;
//OverlayCallback overlays[] = { msOverlay };
int overlaysCount = 1;

void setup() {
  Serial.begin(115200);
  display.init();
  Serial.println();

  display.clear();
  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 20, "BITKUP API");
  display.display();

  Serial.print("Connecting to ");
  Serial.println(WIFI_STA_NAME);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_STA_NAME, WIFI_STA_PASS);
  String waitConnect = ".";
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    display.drawString(64, 40, waitConnect);
    display.display();
    waitConnect += ".";
  }

  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);

  ui.setTargetFPS(60);
  ui.setIndicatorPosition(BOTTOM);
  ui.setIndicatorDirection(LEFT_RIGHT);
  ui.setFrameAnimation(SLIDE_LEFT);
  ui.setFrames(frames, frameCount);
  ui.init();

  http.begin(url);
  int httpCode = http.GET();
  if (httpCode == 200) {
    content = http.getString();
    btc.docodeData(content);
    iost.docodeData(content);
    eth.docodeData(content);
    ada.docodeData(content);
    doge.docodeData(content);
  } else {
    Serial.println("Fail. error code " + String(httpCode));
  }
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    http.begin(url);
    int httpCode = http.GET();
    if (httpCode == 200) {
      content = http.getString();
      btc.docodeData(content);
      iost.docodeData(content);
      eth.docodeData(content);
      ada.docodeData(content);
      doge.docodeData(content);
    } else {
      Serial.println("Fail. error code " + String(httpCode));
    }
  }

  int remainingTimeBudget = ui.update();
  if (remainingTimeBudget > 0) {
    delay(remainingTimeBudget);
  }
}
