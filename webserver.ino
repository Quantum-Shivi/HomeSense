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


// Your home Wi-Fi name and password.
//
// NOTE: putting a real password in a file is fine while learning, but if this
// project ever goes on GitHub, move these into a separate file (for example a
// "secrets.h" that git ignores). Anyone who can read the repository can join
// the network otherwise.
#define WIFI_SSID
#define WIFI_PASSWORD 

// "extern" means: these variables already exist somewhere else (they are
// defined in HomeSense.ino). We are only promising the compiler they exist,
// so this file may use them. No copy is made - both files share the same
// numbers.
extern uint8_t current_temp, current_humidity, current_pressure;
extern bool values_ready;

 // The board's address on the home network, filled in after connecting.
 IPAddress ip;
 AsyncWebServer server(80); // Create a web server object that listens on port 80

 //callback function
// A callback is a function you hand to something else, to be called later.
// Here we hand this one to the web server: "when someone asks for /, run it".
void handleRoot(AsyncWebServerRequest *request) {
    // Wait for the very first sensor reading before answering.
    //
    // WATCH OUT: this is a busy-wait. Because nothing inside the loop sends
    // a reply or pauses, it spins the CPU at full speed until values_ready
    // becomes true. In practice you will not notice with one browser, but a
    // cleaner version would answer with the "initializing" message and stop.
    while (values_ready == false){
        request->send(200, "text/plain", "Sensor initializing, please wait...");
    }
    // 200 is the HTTP status code for OK. The reply is plain text with
    // newlines (\n) so it lines up neatly in a browser window.
    request->send(200, "text/plain", "Temperature: " + String(current_temp) + " F\nHumidity: " + String(current_humidity) + " %\nPressure: " + String(current_pressure) + " kPa");
}


// Called once from setup().
void wifi_setup() {
    // Give the Wi-Fi radio time to settle after power-on.
    delay(5000);

    // Open the serial port for status messages. (setup() already does this,
    // but calling it twice in the same mode is harmless.)
    Serial.begin(9600);

    // Connecting Esp32 to home network
    // WIFI_STA means "station mode": the board behaves like a phone or laptop
    // joining an existing network, instead of creating its own hotspot (AP).
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    // Wait until the ESP32 is connected to the Wi-Fi network
    // Wrapping it in a while loop is the standard pattern: keep checking the
    // status every half second until it says connected. If the password is
    // wrong this loop never ends - a very common beginner bug.
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
    Serial.println("\nConnected!");

    // store/print IP address
    // You need this number: type it into a browser to see the readings.
    ip = WiFi.localIP();
    Serial.println(ip);

    //Open port 8888 for incoming connections
    // (The comment above is a little out of date: the server was created with
    // port 80 above, which is the default port for normal web addresses.)
    // server.on("/", ...) = "when a GET request arrives for the path /,
    // call handleRoot".
    server.on("/", HTTP_GET, handleRoot);
    // Start listening. From this point the board answers requests, whenever
    // loop() happens to not be busy.
    server.begin();

    // Waits and listens for incoming connections on port 80
}