#include "FtpClient.h"
#include "Utilities.h"

int Utilities::getSocketFromCommand(FtpClient &client, String &command)
{
    int i = 0;
    int port = 0;
    char* tok_ptr = strtok(const_cast<char*>(command.c_str()), ",");
    int ip_bytes[4];
    while(tok_ptr && i < 4){
        ip_bytes[i] = String(tok_ptr).toInt();
        tok_ptr = strtok(nullptr, ",");
        i++;
    }
    if(!tok_ptr){
        return -1;
    }
    port = 256 * port + String(tok_ptr).toInt();
    tok_ptr = strtok(nullptr, ",");
    if(!tok_ptr){
        return -1;
    }
    port = 256 * port + String(tok_ptr).toInt();

    client.setDataPort(port);
    client.setDataTransferIp(IPAddress(ip_bytes[0], ip_bytes[1], ip_bytes[2], ip_bytes[3]));

    return 0;
}

bool Utilities::checkFileStatus(const String &path)
{
    return SD.exists(path);
}

bool Utilities::checkMakeDirectory(const String &path)
{
    if(!path.length()){
        return true;
    }
    if(SD.exists(path)){
        return true;
    }
    if(!SD.mkdir(path)){
        return false;
    }
    return true;
}

void Utilities::getDirListing(File& dir, String& listingBuffer)
{
    while(true) {
        File entry = dir.openNextFile();
        if (!entry) {
            return;
        }
        String file_meta;
        file_meta += "rw-rw-rw- 0 0 0 ";
        file_meta += String((int) entry.size());
        file_meta += " ";
        file_meta += entry.name();
        file_meta += "\r\n";
        if (entry.isDirectory()) {
            file_meta = "d" + file_meta;
            listingBuffer += file_meta;
            getDirListing(entry, listingBuffer);
        }
        else{
            file_meta = "-" + file_meta;
            listingBuffer += file_meta;
        }
        entry.close();
    }
}