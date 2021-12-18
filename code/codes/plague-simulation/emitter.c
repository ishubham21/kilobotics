#include "kilolib.h"

message_t msg;
int sent_message;   //flag for msg sent

message_t *tx_message() {   //callbck for msg transmission
    return &msg;
}

void tx_message_success() { // successful transmission callback
    sent_message = 1;
}

void setup() {  
   msg.type = NORMAL;
   msg.crc = message_crc(&msg); //computing uint16_t CRC value - CRC is a check for raw data
}

void loop(){
    if ( sent_message ) {   //blink red when msg is sent
        sent_message = 0;
        set_color(RGB(0,1,0));
        delay(100);
        set_color(RGB(0,0,1));
    }
}

int main(){
    
    kilo_init();
    
    kilo_message_tx = tx_message;
    kilo_message_tx_success = tx_message_success;
    
    kilo_start(setup, loop);
    return 0;

}