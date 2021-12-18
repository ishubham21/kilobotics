#include "kilolib.h"

message_t msg;
int sent_message;   //flag for msg sent
int rcvd_message = 0;
int distance, flag_e = 0, flag_p = 0;

message_t *tx_message() {   //callbck for msg transmission
    return &msg;
}

void tx_message_success() { // successful transmission callback
    sent_message = 1;
}

void rx_message(message_t *msg, distance_measurement_t *d) {
    rcvd_message = 1;
    distance = estimate_distance(d);
}

void setup() {  
   msg.type = NORMAL;
   msg.crc = message_crc(&msg); //computing uint16_t CRC value - CRC is a check for raw data
}

void register_as_emitter(){            
    kilo_message_tx = tx_message;
    kilo_message_tx_success = tx_message_success;
}

void loop(){
    
    if( rcvd_message &&  (flag_p == 0)){
        rcvd_message = 0;
        if(distance > 35){
            set_color(RGB(0,0,1));
            set_motors(kilo_straight_left, kilo_straight_right);
        }
    }

    if(distance <= 35){
        
        flag_p = 1;

        //stop motors
        set_motors(0,0);
        
        //register msg
        if(flag_e == 0){
            register_as_emitter();
            flag_e = 1;
        }
        
        //transmit msg
        if ( sent_message ) {   //blink red when msg is sent
            sent_message = 0;
            set_color(RGB(0,1,0));
            delay(100);
            set_color(RGB(0,0,0));
        }

    }
}

int main(){
    
    kilo_init();
    kilo_message_rx = rx_message;

    kilo_start(setup, loop);
    return 0;

}