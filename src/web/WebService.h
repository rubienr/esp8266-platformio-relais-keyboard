#pragma once

#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266mDNS.h>

class Print;

class WebService : public ESP8266WebServer
{
protected:
    ESP8266HTTPUpdateServer updateServer;
    MDNSResponder &mdns;

    String extra_pre_html;
    String extra_post_html;

public:
    explicit WebService(int16_t port = 80);

    void setup();

    bool handleClient()
    {
        static HTTPClientStatus client_status = ESP8266WebServer::_currentStatus;
        ESP8266WebServer::handleClient();
        if (client_status != ESP8266WebServer::_currentStatus)
        {
            client_status = ESP8266WebServer::_currentStatus;
            return true;
        }
        client_status = ESP8266WebServer::_currentStatus;
        return false;
    }

protected:
    void indexHtml();

    void listFiles();

    void reboot();

    void getDeviceStatus();

    void resetSettings();

};

