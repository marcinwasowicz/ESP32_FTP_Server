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

void send_dir_listing(ftp_server* server, const char* path){
    String listing;
    File dir = SD.open(path);
    get_dir_listing(dir, listing);
    server->active_data_connection.write(listing.c_str());
}

bool send_file(ftp_server* server, const char* path){
    File file = SD.open(path);
    if(!file){
        return false;
    }
    while(file.available()){
        uint8_t read_bytes = file.read();
        if(!server->active_data_connection.write(read_bytes)){
            file.close();
            return false;
        }
    }
    file.close();
    return true;
}

void close_data_connection(ftp_server* server){
    server->active_data_connection.stop();
}
