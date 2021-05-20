#ifndef ESP32_FTP_SERVER_UTILITIES_H
#define ESP32_FTP_SERVER_UTILITIES_H

#include <FS.h>
#include <SD.h>
#include <SPI.h>

class FtpClient;

class Utilities {
public:
    static int getSocketFromCommand(FtpClient& client, String& command);
    static bool checkFileStatus(const String& path);
    static bool checkMakeDirectory(const String& path);
    static void getDirListing(File& dir, String& listingBuffer);
};

#endif
