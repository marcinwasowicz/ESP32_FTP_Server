#include <ftp_reply_codes.h>
#include <ftp_ctrl_commands.h>
#include <control_connection.h>
#include <data_connection.h>

void send_reply_code(client_struct* client, const char* code){
    client->connection.write(code);
}

void greet_client(client_struct* client){
    send_reply_code(client, USER_READY);
}

void handle_user(client_struct* client, String& command){
    command = command.substring(strlen(USER));
    command.trim();
    client->name = command;
    send_reply_code(client, PASSWORD_NEEDED);
}

void handle_password(client_struct* client, const char* server_password, String& command){
    command = command.substring(strlen(PASSWORD));
    command.trim();
    if(!command.equals(server_password)){
        send_reply_code(client, USER_NOT_LOGGED_IN);
        return;
    }
    client->logged_in = true;
    send_reply_code(client, USER_LOGGED_IN);
}

void handle_quit(client_struct* client){
    if(!client->logged_in){
        send_reply_code(client, USER_NOT_LOGGED_IN);
        return;
    }
    send_reply_code(client, USER_LOGGED_OUT);
    client->connection.stop();
}

void handle_set_data_socket(client_struct* client, String& command){
    if(!client->logged_in){
        send_reply_code(client, USER_NOT_LOGGED_IN);
        return;
    }
    command = command.substring(strlen(SET_DATA_PORT));
    command.trim();

    int result = get_socket_from_command(command, client);
    if(result){
        send_reply_code(client, ARGS_SYNTAX_ERROR);
        return;
    }
    send_reply_code(client, OK_REPLY);
}

void handle_dir_listing(client_struct* client, String& command, ftp_server* server){
    if(!client->logged_in){
        send_reply_code(client, USER_NOT_LOGGED_IN);
        return;
    }
    command = command.substring(strlen(LIST_FILES));
    command.trim();
    const char* path = command.c_str();
    if(strlen(path) == 0){
        path = server->root_path;
    }
    if(!check_file_status(path)){
        send_reply_code(client, FILE_UNAVAILABLE);
        return;
    }
    if(!open_data_connection(server, client)){
        send_reply_code(client, DATA_CONNECTION_ERROR);
        return;
    }
    send_reply_code(client, DATA_CONNECTION_OPENED);
    send_dir_listing(server, path);
    send_reply_code(client, DATA_ACTION_SUCCESFULL);
    close_data_connection(server);
    send_reply_code(client, DATA_CONNECTION_CLOSED);
}

void handle_print_working_directory(client_struct* client, ftp_server* server){
    if(!client->logged_in){
        send_reply_code(client, USER_NOT_LOGGED_IN);
    }
    send_reply_code(client, (PATHNAME + String(server->root_path) + CRLF).c_str());
}

void handle_type(client_struct* client){
    if(!client->logged_in){
        send_reply_code(client, USER_NOT_LOGGED_IN);
    }
    send_reply_code(client, OK_REPLY);
}

void handle_retrieve_file(client_struct* client, ftp_server* server, String& command){
    if(!client->logged_in){
        send_reply_code(client, USER_NOT_LOGGED_IN);
        return;
    }
    command = command.substring(strlen(RETRIEVE_FILE));
    command.trim();
    const char* path = command.c_str();
    if(!check_file_status(path)){
        send_reply_code(client, FILE_UNAVAILABLE);
        return;
    }
    if(!open_data_connection(server, client)){
        send_reply_code(client, DATA_CONNECTION_ERROR);
        return;
    }
    send_reply_code(client, DATA_CONNECTION_OPENED);
    if(!send_file(server, path)){
        send_reply_code(client, FILE_ACTION_ABORTED);
        close_data_connection(server);
        return;
    }
    send_reply_code(client, DATA_ACTION_SUCCESFULL);
    close_data_connection(server);
    send_reply_code(client, DATA_CONNECTION_CLOSED);
}

void handle_store_file(client_struct* client, ftp_server* server, String& command){
    if(!client->logged_in){
        send_reply_code(client, USER_NOT_LOGGED_IN);
        return;
    }

    command = command.substring(strlen(STORE_FILE));
    command.trim();

    const char* dir_path = command.substring(0, command.lastIndexOf('/')).c_str();
    const char* file_path = command.c_str();

    if(!make_directory(dir_path)){
        send_reply_code(client, FILE_UNAVAILABLE);
        return;
    }
    if(!open_data_connection(server, client)){
        send_reply_code(client, DATA_CONNECTION_ERROR);
        return;
    }
    send_reply_code(client, DATA_CONNECTION_OPENED);
    if(!store_file(server, file_path)){
        send_reply_code(client, FILE_ACTION_ABORTED);
        close_data_connection(server);
        return;
    }
    send_reply_code(client, DATA_ACTION_SUCCESFULL);
    close_data_connection(server);
    send_reply_code(client, DATA_CONNECTION_CLOSED);
}

void listen_command(client_struct* client, ftp_server* server){
    String command = client->connection.readString();

    if(command.startsWith(USER)){
        handle_user(client, command);
        return;
    }
    if(command.startsWith(PASSWORD)){
        handle_password(client, server->password, command);
        return;
    }
    if(command.startsWith(QUIT)){
        handle_quit(client);
        return;
    }
    if(command.startsWith(SET_DATA_PORT)){
        handle_set_data_socket(client, command);
        return;
    }
    if(command.startsWith(LIST_FILES)){
        handle_dir_listing(client, command, server);
        return;
    }
    if(command.startsWith(PRINT_WORKING_DIR)){
        handle_print_working_directory(client, server);
        return;
    }
    if(command.startsWith(FILE_TYPE)){
        handle_type(client);
        return;
    }
    if(command.startsWith(RETRIEVE_FILE)){
        handle_retrieve_file(client, server, command);
        return;
    }
    if(command.startsWith(STORE_FILE)){
        handle_store_file(client, server, command);
        return;
    }
    send_reply_code(client, COMM_NOT_SUPPORTED);
}

void shut_down_ctrl_connection(client_struct* client){
    send_reply_code(client, SERVICE_SHUT_DOWN);
}