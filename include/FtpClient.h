#ifndef ESP32_FTP_SERVER_FTPCLIENT_H
#define ESP32_FTP_SERVER_FTPCLIENT_H

#include "WiFi.h"
#include "FtpCtrlCommands.h"
#include "FtpReplyCodes.h"
class FtpServer;

class FtpClient {
private:
    bool loggedIn;
    int dataPort;

    WiFiClient connection;
    String name;
    IPAddress dataTransferIp;

    void sendReplyCode(const char* code);
    void handleUser(String& command);
    void handlePassword(FtpServer& server, String& command);
    void handleQuit();
    void handleSetDataSocket(String& command);
    void handleDirListing(FtpServer& server, String& command);
    void handlePrintWorkingDirectory(FtpServer& server);
    void handleType();
    void handleRetrieveFile(FtpServer& server, String& command);
    void handleStoreFile(FtpServer& server, String& command);

public:
    FtpClient();
    explicit FtpClient(const WiFiClient& wifiClient);
    ~FtpClient();

    void greet();
    void listenCommand(FtpServer& server);

    bool connected();
    bool getLoggedIn() const;

    void setDataPort(int port);
    int getDataPort() const;

    void setDataTransferIp(const IPAddress& address);
    IPAddress getDataTransferIp() const;
};
#endif
