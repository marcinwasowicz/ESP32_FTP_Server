#ifndef ESP32_FTP_SERVER_FTP_REPLY_CODES_H
#define ESP32_FTP_SERVER_FTP_REPLY_CODES_H

const char* OK_REPLY = "200 \r\n";
const char* USER_READY = "220 \r\n";
const char* SERVICE_SHUT_DOWN = "421 \r\n";
const char* COMM_SYNTAX_ERROR = "500 \r\n";
const char* ARGS_SYNTAX_ERROR = "501 \r\n";
const char* COMM_NOT_SUPPORTED = "502 \r\n";
const char* USER_NOT_LOGGED_IN = "530 \r\n";
const char* USER_LOGGED_IN = "230 \r\n";
const char* PASSWORD_NEEDED = "331 \r\n";
const char* USER_LOGGED_OUT = "221 \r\n";

#endif
