#include <ESPAsyncWebServer.h>
#include <DNSServer.h>

void captivePortal_setup();
void handleRoot_captive(AsyncWebServerRequest *request);