#include <ftp_reply_codes.h>
#include <ftp_ctrl_commands.h>
#include <control_connection.h>

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

void listen_command(client_struct* client, const char* server_password){
    String command = client->connection.readString();

    if(command.startsWith(USER)){
        handle_user(client, command);
        return;
    }
    if(command.startsWith(PASSWORD)){
        handle_password(client, server_password, command);
        return;
    }
    if(command.startsWith(QUIT)){
        handle_quit(client);
    }
    send_reply_code(client, COMM_NOT_SUPPORTED);
}

void shut_down_ctrl_connection(client_struct* client){
    send_reply_code(client, SERVICE_SHUT_DOWN);
}