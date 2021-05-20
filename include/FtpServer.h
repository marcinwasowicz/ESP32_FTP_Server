#ifndef ESP32_FTP_SERVER_FTPSERVER_H
#define ESP32_FTP_SERVER_FTPSERVER_H

#include <WiFi.h>
#include "FtpClient.h"

class FtpServer {
private:
    int numClients;
    int delayTime;

    const String password;
    const String rootPath;

    const static int maxClientCount = 5;
    const static int ftpControlPort = 21;
    const static int minByteRetries = 3;

    WiFiServer controlConnection;
    WiFiClient activeDataConnection;

    FtpClient clients[maxClientCount];

    void accept();
    void removeDisconnected();
    void handleClient(FtpClient& client);

public:
    FtpServer(String srvPwd, int delayTime, String rootPath);
    ~FtpServer();

    void init(const char* ssid, const char* netPassword);
    void loop();
    bool checkPassword(const String& clientPassword) const;
    bool openDataConnection(const FtpClient& client);

    void sendDirListing(const String& path);
    bool sendFile(const String& path);
    bool storeFile(const String& path);
    void closeDataConnection();
    String getRootPath() const;
};

#endif
