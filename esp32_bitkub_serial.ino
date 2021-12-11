#include <WiFi.h>
#include <HTTPClient.h>
#define WIFI_STA_NAME "SSID_NAME"
#define WIFI_STA_PASS "SSID_PASSWORD"
HTTPClient http;
String url = "https://api.bitkub.com/api/market/ticker";

#include <ArduinoJson.h>
StaticJsonDocument<20000> doc;
String content;

class CRYPTO {
  public:
    CRYPTO(String type);
    void docodeData(String dataText);
    float last;
    float low24hr;
    float high24hr;
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


void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_STA_NAME);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_STA_NAME, WIFI_STA_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
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
  Serial.println("");
  Serial.println("Crypto\t\tPrice\t\tLow24H\t\tLow24H\t\tChange(%)");
  Serial.printf("%s\t\t%f\t%f\t%f\t%f\n", btc._type, btc.last, btc.low24hr, btc.high24hr, btc.percentChange);
  Serial.printf("%s\t%f\t%f\t%f\t%f\n", iost._type, iost.last, iost.low24hr, iost.high24hr, iost.percentChange);
  Serial.printf("%s\t\t%f\t%f\t%f\t%f\n", eth._type, eth.last, eth.low24hr, eth.high24hr, eth.percentChange);
  Serial.printf("%s\t\t%f\t%f\t%f\t%f\n", ada._type, ada.last, ada.low24hr, ada.high24hr, ada.percentChange);
  Serial.printf("%s\t%f\t%f\t%f\t%f\n", doge._type, doge.last, doge.low24hr, doge.high24hr, doge.percentChange);
  delay(1000);
}
