#include <data_connection.h>

bool open_data_connection(ftp_server* server, client_struct* client){
    if(!client->logged_in || client->data_port == -1){
        return false;
    }
    if(!server->active_data_connection.connect(client->data_transfer_ip, client->data_port)){
        return false;
    }
    return true;
}

void send_dir_listing(ftp_server* server, String& path){
    String listing;
    File dir = SD.open(path);
    get_dir_listing(dir, listing);
    dir.close();
    server->active_data_connection.write(listing.c_str());
}

bool send_file(ftp_server* server, String& path){
    File file = SD.open(path);
    if(!file){
        return false;
    }
    while(file.available()){
        uint8_t read_bytes = file.read();
        if(!server->active_data_connection.connected() || !server->active_data_connection.write(read_bytes)){
            file.close();
            return false;
        }
    }
    file.close();
    return true;
}

bool store_file(ftp_server* server, String& path){
    if(SD.exists(path)){
        SD.remove(path);
    }
    File file = SD.open(path, FILE_WRITE);
    if(!file){
        return false;
    }
    int delay_factor = 1;
    int delay_count = delay_factor * MIN_BYTE_RETRIES;
    bool delay_detected = false;
    while(server->active_data_connection.connected()){
        if(!delay_count){
            return false;
        }
        if(!server->active_data_connection.available()){
            delay(server->delay_time);
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
        uint8_t read_bytes = server->active_data_connection.read();
        file.write(read_bytes);
        delay_count = delay_factor * MIN_BYTE_RETRIES;
    }
    file.close();
    return true;
}

void close_data_connection(ftp_server* server){
    server->active_data_connection.stop();
}
