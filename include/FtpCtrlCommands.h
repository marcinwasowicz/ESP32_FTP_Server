#ifndef ESP32_FTP_SERVER_FTPCTRLCOMMANDS_H
#define ESP32_FTP_SERVER_FTPCTRLCOMMANDS_H

class FtpCtrlCommands {
public:
    static constexpr char* USER = "USER";
    static constexpr char* PASSWORD = "PASS";
    static constexpr char* QUIT = "QUIT";
    static constexpr char* SET_DATA_PORT = "PORT";
    static constexpr char* LIST_FILES = "LIST";
    static constexpr char* PRINT_WORKING_DIR = "PWD";
    static constexpr char* FILE_TYPE = "TYPE";
    static constexpr char* RETRIEVE_FILE = "RETR";
    static constexpr char* STORE_FILE = "STOR";
    static constexpr char* CRLF = "\r\n";
};

#endif
