#include "FtpClient.h"
#include "FtpServer.h"
#include "Utilities.h"

FtpClient::FtpClient()
    : loggedIn(false), dataPort(-1) {}

FtpClient::FtpClient(const WiFiClient &wifiClient)
    : loggedIn(false), dataPort(-1), connection(wifiClient) {}

FtpClient::~FtpClient()
{
    this->connection.stop();
}

bool FtpClient::connected()
{
    return this->connection.connected();
}

bool FtpClient::getLoggedIn() const
{
    return this->loggedIn;
}

void FtpClient::setDataPort(int port)
{
    this->dataPort = port;
}

int FtpClient::getDataPort() const
{
    return this->dataPort;
}

void FtpClient::setDataTransferIp(const IPAddress &address)
{
    this->dataTransferIp = address;
}

IPAddress FtpClient::getDataTransferIp() const
{
    return this->dataTransferIp;
}

void FtpClient::sendReplyCode(const char *code)
{
    this->connection.write(code);
}

void FtpClient::greet()
{
    this->sendReplyCode(FtpReplyCodes::USER_READY);
}

void FtpClient::handleUser(String &command)
{
    command = command.substring(strlen(FtpCtrlCommands::USER));
    command.trim();
    this->name = command;
    this->sendReplyCode(FtpReplyCodes::PASSWORD_NEEDED);
}

void FtpClient::handlePassword(FtpServer& server, String &command)
{
    command = command.substring(strlen(FtpCtrlCommands::PASSWORD));
    command.trim();
    if(!server.checkPassword(command)){
        this->sendReplyCode(FtpReplyCodes::USER_NOT_LOGGED_IN);
        return;
    }
    this->loggedIn = true;
    this->sendReplyCode(FtpReplyCodes::USER_LOGGED_IN);
}

void FtpClient::handleQuit()
{
    if(!this->loggedIn){
        this->sendReplyCode(FtpReplyCodes::USER_NOT_LOGGED_IN);
        return;
    }
    this->sendReplyCode(FtpReplyCodes::USER_LOGGED_OUT);
    this->connection.stop();
}

void FtpClient::handleSetDataSocket(String &command)
{
    if(!this->loggedIn){
        this->sendReplyCode(FtpReplyCodes::USER_NOT_LOGGED_IN);
        return;
    }
    command = command.substring(strlen(FtpCtrlCommands::SET_DATA_PORT));
    command.trim();

    if(Utilities::getSocketFromCommand(*this, command)){
        this->sendReplyCode(FtpReplyCodes::ARGS_SYNTAX_ERROR);
        return;
    }
    this->sendReplyCode(FtpReplyCodes::OK_REPLY);
}

void FtpClient::handleDirListing(FtpServer &server, String &command)
{
    if(!this->loggedIn){
        this->sendReplyCode(FtpReplyCodes::USER_NOT_LOGGED_IN);
        return;
    }
    command = command.substring(strlen(FtpCtrlCommands::LIST_FILES));
    command.trim();
    String path = command;

    if(!command.length()){
        path = server.getRootPath();
    }
    if(!Utilities::checkFileStatus(path)){
        this->sendReplyCode(FtpReplyCodes::FILE_UNAVAILABLE);
        return;
    }
    if(!server.openDataConnection(*this)){
        this->sendReplyCode(FtpReplyCodes::DATA_CONNECTION_ERROR);
        return;
    }
    this->sendReplyCode(FtpReplyCodes::DATA_CONNECTION_OPENED);
    server.sendDirListing(path);
    this->sendReplyCode(FtpReplyCodes::DATA_ACTION_SUCCESSFUL);
    server.closeDataConnection();
    this->sendReplyCode(FtpReplyCodes::DATA_CONNECTION_CLOSED);
}

void FtpClient::handlePrintWorkingDirectory(FtpServer &server)
{
    if(!this->loggedIn){
        this->sendReplyCode(FtpReplyCodes::USER_NOT_LOGGED_IN);
    }
    this->sendReplyCode((FtpReplyCodes::PATHNAME + server.getRootPath() + FtpCtrlCommands::CRLF).c_str());
}

void FtpClient::handleType()
{
    if(!this->loggedIn){
        this->sendReplyCode(FtpReplyCodes::USER_NOT_LOGGED_IN);
    }
    this->sendReplyCode(FtpReplyCodes::OK_REPLY);
}

void FtpClient::handleRetrieveFile(FtpServer &server, String &command)
{
    if(!this->loggedIn){
        this->sendReplyCode(FtpReplyCodes::USER_NOT_LOGGED_IN);
        return;
    }
    command = command.substring(strlen(FtpCtrlCommands::RETRIEVE_FILE));
    command.trim();
    String path = command;

    if(!Utilities::checkFileStatus(path)){
        this->sendReplyCode(FtpReplyCodes::FILE_UNAVAILABLE);
        return;
    }
    if(!server.openDataConnection(*this)){
        this->sendReplyCode(FtpReplyCodes::DATA_CONNECTION_ERROR);
        return;
    }
    this->sendReplyCode(FtpReplyCodes::DATA_CONNECTION_OPENED);
    if(!server.sendFile(path)){
        this->sendReplyCode(FtpReplyCodes::FILE_ACTION_ABORTED);
        server.closeDataConnection();
        return;
    }
    this->sendReplyCode(FtpReplyCodes::DATA_ACTION_SUCCESSFUL);
    server.closeDataConnection();
    this->sendReplyCode(FtpReplyCodes::DATA_CONNECTION_CLOSED);
}

void FtpClient::handleStoreFile(FtpServer &server, String &command)
{
    if(!this->loggedIn){
        this->sendReplyCode(FtpReplyCodes::USER_NOT_LOGGED_IN);
        return;
    }

    command = command.substring(strlen(FtpCtrlCommands::STORE_FILE));
    command.trim();
    String file_path = command;

    command = command.substring(0, command.lastIndexOf('/'));
    command.trim();
    String dir_path = command;

    if(!Utilities::checkMakeDirectory(dir_path)){
        this->sendReplyCode(FtpReplyCodes::FILE_UNAVAILABLE);
        return;
    }
    if(!server.openDataConnection(*this)){
        this->sendReplyCode(FtpReplyCodes::DATA_CONNECTION_ERROR);
        return;
    }
    this->sendReplyCode(FtpReplyCodes::DATA_CONNECTION_OPENED);
    if(!server.storeFile(file_path)){
        this->sendReplyCode(FtpReplyCodes::FILE_ACTION_ABORTED);
        server.closeDataConnection();
        return;
    }
    this->sendReplyCode(FtpReplyCodes::DATA_ACTION_SUCCESSFUL);
    server.closeDataConnection();
    this->sendReplyCode(FtpReplyCodes::DATA_CONNECTION_CLOSED);
}

void FtpClient::listenCommand(FtpServer &server)
{
    String command = this->connection.readString();
    if(!command.length()){
        return;
    }
    if(command.startsWith(FtpCtrlCommands::USER)){
        this->handleUser(command);
        return;
    }
    if(command.startsWith(FtpCtrlCommands::PASSWORD)){
        this->handlePassword(server, command);
        return;
    }
    if(command.startsWith(FtpCtrlCommands::QUIT)){
        this->handleQuit();
        return;
    }
    if(command.startsWith(FtpCtrlCommands::SET_DATA_PORT)){
        this->handleSetDataSocket(command);
        return;
    }
    if(command.startsWith(FtpCtrlCommands::LIST_FILES)){
        this->handleDirListing(server, command);
        return;
    }
    if(command.startsWith(FtpCtrlCommands::PRINT_WORKING_DIR)){
        this->handlePrintWorkingDirectory(server);
        return;
    }
    if(command.startsWith(FtpCtrlCommands::FILE_TYPE)){
        this->handleType();
        return;
    }
    if(command.startsWith(FtpCtrlCommands::RETRIEVE_FILE)){
        this->handleRetrieveFile(server, command);
        return;
    }
    if(command.startsWith(FtpCtrlCommands::STORE_FILE)){
        this->handleStoreFile(server, command);
        return;
    }
    this->sendReplyCode(FtpReplyCodes::COMM_NOT_SUPPORTED);
}



