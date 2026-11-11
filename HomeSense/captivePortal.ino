/*
  ============================================================================
  webserver.ino - Wi-Fi connection and the tiny web page
  ============================================================================

  The ESP32 has Wi-Fi built in, so the same board that reads the sensor can
  also serve a web page. Open the IP address printed in the Serial Monitor
  (something like http://192.168.1.42) in a browser on the same network and
  you get the current readings as plain text.

  ============================================================================
*/

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>


// "extern" means: these variables already exist somewhere else (they are
// defined in HomeSense.ino). We are only promising the compiler they exist,
// so this file may use them. No copy is made - both files share the same
// numbers.

 // The board's address on the home network, filled in after connecting.
 uint32_t lastDNSCheck = 0; // Time of the last DNS check, in milliseconds
 IPAddress wifi_ip;
 AsyncWebServer server(80); // Create a web server object that listens on port 80
 DNSServer dnsServer; // Create a DNS server object

 //callback function
// A callback is a function you hand to something else, to be called later.
// Here we hand this one to the web server: "when someone asks for /, run it".
void handleRoot_captive(AsyncWebServerRequest *request) {

    // 200 is the HTTP status code for OK. The reply is plain text with
    // newlines (\n) so it lines up neatly in a browser window.
    request->send(200, "text/plain", "Please Type in your home Wi-Fi credentials");
}
// Called once from setup().
void captivePortal_setup() {
    // Give the Wi-Fi radio time to settle after power-on.
    delay(5000);

    // Open the serial port for status messages. (setup() already does this,
    // but calling it twice in the same mode is harmless.)
    Serial.begin(9600);

    // Connecting Esp32 to home network
    // WIFI_STA means "station mode": the board behaves like a phone or laptop
    // joining an existing network, instead of creating its own hotspot (AP).
    WiFi.mode(WIFI_AP);
    WiFi.softAP("HomeSense"); // Start the ESP32 in Access Point mode with the name "HomeSense"
    dnsServer.start(53, "*", WiFi.softAPIP()); // Start the DNS server
    lastDNSCheck = millis(); // Record the time of the last DNS check
    wifi_ip = WiFi.softAPIP();
    Serial.println(wifi_ip);


    server.on("/", HTTP_GET, handleRoot_captive);
    // Start listening. From this point the board answers requests, whenever
    server.onNotFound([](AsyncWebServerRequest *request) {
        String redirectUrl = "http://" + wifi_ip.toString() + "/";
        request->redirect(redirectUrl);
    });
    // loop() happens to not be busy.
    server.begin();

    // Waits and listens for incoming connections on port 80
}

void captivePortal_loop() {
    // Handle DNS requests
    dnsServer.processNextRequest();
}