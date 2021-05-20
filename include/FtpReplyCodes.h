#ifndef ESP32_FTP_SERVER_FTPREPLYCODES_H
#define ESP32_FTP_SERVER_FTPREPLYCODES_H

class FtpReplyCodes{
public:
    static constexpr char* OK_REPLY = "200 \r\n";
    static constexpr char* USER_READY = "220 \r\n";
    static constexpr char* USER_LOGGED_IN = "230 \r\n";
    static constexpr char* USER_LOGGED_OUT = "221 \r\n";

    static constexpr char* PASSWORD_NEEDED = "331 \r\n";

    static constexpr char* PATHNAME = "257 ";

    static constexpr char* DATA_CONNECTION_DUE_TO_OPEN = "150 \r\n";
    static constexpr char* DATA_CONNECTION_OPENED = "125 \r\n";
    static constexpr char* DATA_CONNECTION_CLOSED = "226 \r\n";
    static constexpr char* DATA_ACTION_SUCCESSFUL = "250 \r\n";
    static constexpr char* DATA_CONNECTION_UNAVAILABLE = "425 \r\n";
    static constexpr char* DATA_CONNECTION_ERROR = "426 \r\n";

    static constexpr char* FILE_UNAVAILABLE = "450 \r\n";
    static constexpr char* FILE_ACTION_ABORTED = "451 \r\n";
    static constexpr char* SERVICE_SHUT_DOWN = "421 \r\n";

    static constexpr char* COMM_SYNTAX_ERROR = "500 \r\n";
    static constexpr char* ARGS_SYNTAX_ERROR = "501 \r\n";
    static constexpr char* COMM_NOT_SUPPORTED = "502 \r\n";
    static constexpr char* USER_NOT_LOGGED_IN = "530 \r\n";
};

#endif
