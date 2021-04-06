#ifndef ESP32_FTP_SERVER_FTP_REPLY_CODES_H
#define ESP32_FTP_SERVER_FTP_REPLY_CODES_H

const char* OK_REPLY = "200 \r\n";
const char* USER_READY = "220 \r\n";
const char* USER_LOGGED_IN = "230 \r\n";
const char* USER_LOGGED_OUT = "221 \r\n";

const char* PASSWORD_NEEDED = "331 \r\n";

const char* PATHNAME = "257 ";

const char* DATA_CONNECTION_DUE_TO_OPEN = "150 \r\n";
const char* DATA_CONNECTION_OPENED = "125 \r\n";
const char* DATA_CONNECTION_CLOSED = "226 \r\n";
const char* DATA_ACTION_SUCCESFULL = "250 \r\n";
const char* DATA_CONNECTION_UNAVAILABLE = "425 \r\n";
const char* DATA_CONNECTION_ERROR = "426 \r\n";

const char* FILE_UNAVAILABLE = "450 \r\n";
const char* SERVICE_SHUT_DOWN = "421 \r\n";

const char* COMM_SYNTAX_ERROR = "500 \r\n";
const char* ARGS_SYNTAX_ERROR = "501 \r\n";
const char* COMM_NOT_SUPPORTED = "502 \r\n";
const char* USER_NOT_LOGGED_IN = "530 \r\n";

#endif
