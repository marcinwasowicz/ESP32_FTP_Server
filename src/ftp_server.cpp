#include <ftp_server.h>
#include <control_connection.h>

void ftp_init(ftp_server* server, const char* ssid, const char* net_password, const char* srv_password, int delay_time, const char* root_path){
    server->password = srv_password;
    server->delay_time = delay_time;
    server->root_path = root_path;

    server->control_connection = WiFiServer(FTP_CONTROL_PORT);
    server->active_data_connection = WiFiClient();
    server->num_clients = 0;

    WiFi.begin(ssid, net_password);
    while(WiFiClass::status() != WL_CONNECTED){
        delay(server->delay_time);
    }
    server->control_connection.begin();
}

void ftp_accept(ftp_server* server){
    if(server->num_clients >= MAX_CLIENTS_COUNT){
        return;
    }
    WiFiClient wiFiClient = server->control_connection.available();
    if(wiFiClient.connected()){
        server->clients[server->num_clients].connection = wiFiClient;
        server->clients[server->num_clients].logged_in = false;
        server->clients[server->num_clients].data_port = -1;
        greet_client(&server->clients[server->num_clients]);
        server->num_clients++;
    }
}

void ftp_handle_client(ftp_server* server, int client_id){
   if(server->clients[client_id].connection.connected()){
       listen_command(&server->clients[client_id], server);
   }
}

void ftp_remove_disconected(ftp_server* server){
    for(int i = 0; i<server->num_clients; i++){
        if(!server->clients[i].connection.connected()){
            server->num_clients--;
            server->clients[i] = server->clients[server->num_clients];
        }
    }
}

void ftp_loop(ftp_server* server){
    ftp_accept(server);

    for(int i = 0; i<server->num_clients; i++){
        ftp_handle_client(server, i);
        delay(server->delay_time);
    }
    delay(server->delay_time);
    ftp_remove_disconected(server);
}

void ftp_shutdown(ftp_server* server){
    for(int i = 0; i<server->num_clients; i++){
        shut_down_ctrl_connection(&server->clients[i]);
        server->clients[i].connection.stop();
    }
    server->control_connection.stop();
}

