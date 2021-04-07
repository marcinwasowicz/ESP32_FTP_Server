#include <utils.h>

int get_socket_from_command(String& command, client_struct* client){
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

    client->data_port = port;
    client->data_transfer_ip = IPAddress(ip_bytes[0], ip_bytes[1], ip_bytes[2], ip_bytes[3]);

    return 0;
}

bool check_file_status(const char* path){
    return SD.exists(path);
}

void get_dir_listing(File dir, String& listing_buffer){
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
            listing_buffer += file_meta;
            get_dir_listing(entry, listing_buffer);
        }
        else{
            file_meta = "-" + file_meta;
            listing_buffer += file_meta;
        }
        entry.close();
    }
}

