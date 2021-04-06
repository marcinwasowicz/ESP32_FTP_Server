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

void sent_raw_bytes(ftp_server* server, const char* bytes){
    server->active_data_connection.write(bytes);
}

void close_data_connection(ftp_server* server){
    server->active_data_connection.stop();
}
