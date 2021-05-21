#include "FtpServer.h"
#include "Utilities.h"


FtpServer::FtpServer(String srvPwd, int delayTime, String rootPath)
    : numClients(0), delayTime(delayTime), password(std::move(srvPwd)), rootPath(std::move(rootPath))
{
    this->controlConnection = WiFiServer(FtpServer::ftpControlPort);
    this->activeDataConnection = WiFiClient();
}

void FtpServer::init(const char *ssid, const char *netPassword)
{
    WiFi.begin(ssid, netPassword);
    while(WiFiClass::status() != WL_CONNECTED){
        delay(this->delayTime);
    }
    this->controlConnection.begin();
}

FtpServer::~FtpServer()
{
    this->controlConnection.stop();
    this->activeDataConnection.stop();
}

bool FtpServer::checkPassword(const String &clientPassword) const
{
    return this->password.equals(clientPassword);
}

String FtpServer::getRootPath() const
{
    return this->rootPath;
}

void FtpServer::accept()
{
    if(this->numClients >= FtpServer::maxClientCount){
        return;
    }
    WiFiClient wiFiClient = this->controlConnection.available();
    if(wiFiClient.connected()){
        this->clients[this->numClients] = FtpClient(wiFiClient);
        this->clients[this->numClients].greet();
        this->numClients++;
    }
}

void FtpServer::handleClient(FtpClient& client)
{
    if(client.connected()){
        client.listenCommand(*this);
    }
}

void FtpServer::removeDisconnected()
{
    for(int i = 0; i<this->numClients; i++){
        if(!this->clients[i].connected()){
            this->numClients--;
            this->clients[i] = this->clients[this->numClients];
        }
    }
}

void FtpServer::loop()
{
    this->accept();
    for(auto& client: this->clients){
        this->handleClient(client);
        delay(this->delayTime);
    }
    this->removeDisconnected();
}

bool FtpServer::openDataConnection(const FtpClient &client)
{
    if(!client.getLoggedIn() || client.getDataPort() == -1){
        return false;
    }
    if(!this->activeDataConnection.connect(client.getDataTransferIp(), client.getDataPort())){
        return false;
    }
    return true;
}

void FtpServer::sendDirListing(const String &path)
{
    String listing;
    File dir = SD.open(path);
    Utilities::getDirListing(dir, listing);
    dir.close();
    this->activeDataConnection.write(listing.c_str());
}

bool FtpServer::sendFile(const String &path)
{
    File file = SD.open(path);
    if(!file){
        return false;
    }
    size_t toSend = file.size();
    for(size_t i = 0; i<toSend && file.available(); i++){
        uint8_t read_bytes = file.read();
        if(!this->activeDataConnection.connected() || !this->activeDataConnection.write(read_bytes)){
            file.close();
            return false;
        }
    }
    file.close();
    return true;
}

bool FtpServer::storeFile(const String &path)
{
    if(SD.exists(path)){
        SD.remove(path);
    }
    File file = SD.open(path, FILE_WRITE);
    if(!file){
        return false;
    }
    int delay_factor = 1;
    int delay_count = delay_factor * FtpServer::minByteRetries;
    bool delay_detected = false;
    while(true){
        if(!delay_count){
            break;
        }
        if(!this->activeDataConnection.available()){
            delay(this->delayTime);
            delay_count--;
            delay_detected = true;
            continue;
        }
        if(delay_detected){
            delay_factor *= 2;
        }
        else if(delay_factor > 1){
            delay_factor /= 2;
        }
        uint8_t read_bytes = this->activeDataConnection.read();
        file.write(read_bytes);
        delay_count = delay_factor * FtpServer::minByteRetries;
    }
    file.close();
    return true;
}

void FtpServer::closeDataConnection()
{
    this->activeDataConnection.stop();
}







