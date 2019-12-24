#include "WebService.h"

#include <FS.h>
#include <WiFiManager.h>

WebService::WebService(int16_t port) :
        ESP8266WebServer(port),
        updateServer(),
        mdns(MDNS)
{}

void WebService::setup()
{
    mdns.begin("Relays Board");
    updateServer.setup(this, "/firmware");
    begin();
    MDNS.addService("http", "tcp", 80);

    on("/", [&]()
    { indexHtml(); });
    on("/files", [&]()
    { listFiles(); });
    on("/reboot", [&]()
    { reboot(); });
    on("/status", [&]()
    { getDeviceStatus(); });
    on("/api/settings/wifi/reset", [&]()
    { resetSettings(); });
}

void WebService::indexHtml()
{
    Serial.println("WebService::on: /");

    String info;
    info += "<html>";
    info += "  <h1> Firmware ";
    info += ESP.getSketchMD5();
    info += " size  ";
    info += ESP.getSketchSize();
    info += "  </h1> ";
    if (extra_pre_html.length() > 0) info += extra_pre_html;
    info += "  <a href=/status>device status</a><br/>";
    info += "  <a href=/firmware>upload firmware</a><br/>";
    info += "  <a href=/files>ls</a><br/>";
    info += "  <a href=/reboot>reboot (switch from wifi to normal mode)</a><br/>";
    info += "  <a href=/api/settings/wifi/reset>reset WiFi settings</a><br/>";
    if (extra_post_html.length() > 0) info += extra_post_html;
    info += "</html>";
    send(200, "text/html", info);
}

void WebService::listFiles()
{
    Serial.println("WebService::on: /files");
    String file_listing;

    std::function<void(Dir &)> iterate_files = [&](Dir &dir)
    {
        while (dir.next())
        {
            if (dir.isDirectory())
            {
                iterate_files(dir);
            } else
            {
                File f = dir.openFile("r");
                file_listing.concat(dir.fileName() + " " + f.size() + "B\n");
                f.close();
            }
        }
    };

    SPIFFS.begin();
    Dir dir = SPIFFS.openDir("/");
    iterate_files(dir);
    SPIFFS.end();
    send(200, "text/plain", file_listing);
}

void WebService::reboot()
{
    Serial.println("WebService::on: /reboot");
    ESP.restart();
}

void WebService::getDeviceStatus()
{
    Serial.println("WebService::on: /status");

    String status;
    status += "<html>"
              "  <table>"
              "    <tr>"
              "      <th>status</th>"
              "      <th>value</th>"
              "    </tr>"
              "    <tr>"
              "      <td>shutdown reason</td>"
              "      <td>";
    status += ESP.getResetReason().c_str();
    status += "</td>"
              "    </tr>"
              "    <tr>"
              "      <td>VCC [mV]</td>"
              "      <td>";
    status += ESP.getVcc();
    status += "</td>"
              "    </tr>"
              "    <tr>"
              "      <td>free heap [B]</td>"
              "      <td>";
    status += ESP.getFreeHeap();
    status += "</td>"
              "    </tr>"
              "    <tr>"
              "      <td>max. free block size </td>"
              "      <td>";
    status += ESP.getMaxFreeBlockSize();
    status += "</td>"
              "    </tr>"
              "    <tr>"
              "      <td>heap fragmentation [%]</td>"
              "      <td>";
    status += ESP.getHeapFragmentation();
    status += "</td>"
              "    </tr>"
              "    <tr>"
              "      <td>free continuous stack [B]</td>"
              "      <td>";
    status += ESP.getFreeContStack();
    status += "</td>"
              "    </tr>"
              "    <tr>"
              "      <td>SDK version</td>"
              "      <td>";
    status += ESP.getSdkVersion();
    status += "</td>"
              "    </tr>"
              "    <tr>"
              "      <td>core version</td>"
              "      <td>";
    status += ESP.getCoreVersion();
    status += "</td>"
              "    </tr>"
              "    <tr>"
              "      <td>full version</td>"
              "      <td>";
    status += ESP.getFullVersion();

    status += "</td>"
              "    </tr>"
              "    <tr>"
              "      <td>boot loader version</td>"
              "      <td>";
    status += ESP.getBootVersion();

    status += "</td>"
              "    </tr>"
              "    <tr>"
              "      <td>boot mode</td>"
              "      <td>";
    status += ESP.getBootMode();

    status += "</td>"
              "    </tr>"
              "    <tr>"
              "      <td>CPU frequency [MHz]</td>"
              "      <td>";
    status += ESP.getCpuFreqMHz();
    status += "</td>"
              "    </tr>"
              "    <tr>"
              "      <td>flash chip ID</td>"
              "      <td>";
    status += ESP.getFlashChipId();
    status += "</td>"
              "    </tr>"
              "    <tr>"
              "      <td>flash chip vendor ID</td>"
              "      <td>";
    status += ESP.getFlashChipVendorId();
    status += "</td>"
              "    </tr>"
              "    <tr>"
              "      <td>flash chip real size[B]</td>"
              "      <td>";
    status += ESP.getFlashChipRealSize();
    status += "</td>"
              "    </tr>"
              "    <tr>"
              "      <td>flash chip size [B]</td>"
              "      <td>";
    status += ESP.getFlashChipSize();
    status += "</td>"
              "    </tr>"
              "    <tr>"
              "      <td>flash chip speed [Hz]</td>"
              "      <td>";
    status += ESP.getFlashChipSpeed();
    status += "</td>"
              "    </tr>"
              "    <tr>"
              "      <td>flash chip mode</td>"
              "      <td>";
    status += ESP.getFlashChipMode();
    status += "</td>"
              "    </tr>"
              "    <tr>"
              "      <td>flash chip size by chip ID</td>"
              "      <td>";
    status += ESP.getFlashChipSizeByChipId();
    status += "</td>"
              "    </tr>"
              "    <tr>"
              "      <td>sketch size [B]</td>"
              "      <td>";
    status += ESP.getSketchSize();
    status += "</td>"
              "    </tr>"
              "    <tr>"
              "      <td>sketch MD5</td>"
              "      <td>";
    status += ESP.getSketchMD5();
    status += "</td>"
              "    </tr>"
              "    <tr>"
              "      <td>free sketch space [B]</td>"
              "      <td>";
    status += ESP.getFreeSketchSpace();

    status += "</td>"
              "    </tr>"
              "    <tr>"
              "      <td>reset reason</td>"
              "      <td>";
    status += ESP.getResetReason();

    status += "</td>"
              "    </tr>"
              "    <tr>"
              "      <td>reset info</td>"
              "      <td>";
    status += ESP.getResetInfo();
    status += "</td>"
              "    </tr>"
              "  <table>"
              "</html>";

    send(200, "text/html", status);
}

void WebService::resetSettings()
{
    WiFiManager wifi_manager;
    wifi_manager.resetSettings();
    ESP.restart();
}

