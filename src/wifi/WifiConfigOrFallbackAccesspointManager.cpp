#include "WifiConfigOrFallbackAccesspointManager.h"

#include <HardwareSerial.h>
#include <IPAddress.h>
#include <WiFiManager.h>

WifiConfigOrFallbackAccesspointManager::WifiConfigOrFallbackAccesspointManager(Display &display) :
        display(display)
{
    IPAddress address(192, 168, 0, 1);
    IPAddress &gateway = address;
    IPAddress subnet(255, 255, 255, 0);
    String essid(String("ESP") + ESP.getChipId());
    WiFiManager wifiManager;

    display.reset();
    display.println("Configure");
    printConnection(essid.c_str(), address.toString().c_str());
    display.display();

    wifiManager.setAPStaticIPConfig(address, gateway, subnet);
    wifiManager.autoConnect(essid.c_str());
    Serial.println("setup: wifi connected");

    display.reset();
    display.println("Connected");
    printConnection(WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
    display.display();

}

void WifiConfigOrFallbackAccesspointManager::printConnection(const char *ssid, const char *address)
{
    display.printf("WiFi: %s\n", ssid);
    display.printf("http://%s\n", address);

    Serial.printf("WifiConfigOrFallbackAccesspointManager::printConnection WiFi: %s\n", ssid);
    Serial.printf("WifiConfigOrFallbackAccesspointManager::printConnection http://%s\n", address);
}

