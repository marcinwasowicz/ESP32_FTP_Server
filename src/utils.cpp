#include <utils.h>

int get_port_from_command(String& command){
    int i;
    char* tok_ptr;
    for(i = 0, tok_ptr = strtok(const_cast<char*>(command.c_str()), ","); tok_ptr != nullptr && i < 4; i++){
        tok_ptr = strtok(nullptr, ",");
    }
    if(!tok_ptr){
        return -1;
    }
    int p1 = String(tok_ptr).toInt();
    tok_ptr = strtok(nullptr, ",");
    if(!tok_ptr){
        return -1;
    }
    int p2 = String(tok_ptr).toInt();
    int port = 256 * p1 + p2;
    return port;
}

