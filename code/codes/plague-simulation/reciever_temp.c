#include "kilolib.h"
#include <stdlib.h>

message_t msg;
uint8_t recvd_message = 0;
int distance, sent_message = 0, flag = 0; 

void blank(message_t *msg, distance_measurement_t *d){
    recvd_message = 0;
}

// receive message callback
void rx_message(message_t *msg, distance_measurement_t *d) {
    recvd_message = 1;
    distance = estimate_distance(d);
}

void setup() {
    recvd_message = 0;
    kilo_message_rx = rx_message;   
}

message_t *tx_message() {   //callbck for msg transmission
    return &msg;
}

void tx_message_success() { // successful transmission callback
    sent_message = 1;
}

void set_msg(){ //function to setup the msg configuration
    msg.type = NORMAL;
    msg.crc = message_crc(&msg); //computing uint16_t CRC value - CRC is a check for raw data
}

void set_emitter(){ //function to set emitter callback configs
    kilo_message_tx = tx_message;
    kilo_message_tx_success = tx_message_success;
}

void random_motion() {
    int r = rand() % 4;
    switch(r) {
        case 0:
            set_motors(kilo_straight_left, kilo_straight_right);
            break;
        
        case 1:
            set_motors(kilo_turn_left, 0);
            break;

        case 2:
            set_motors(0, kilo_turn_right);
            break;
    }
    delay(500);
}

// blink green when a new message is received
void loop() {
    if ( recvd_message && flag == 0 ) {
        recvd_message = 0;  //reset the current signal to make space for other
        
        if(distance < 45){
            
            set_motors(0,0);
            
            if(flag == 0){  //to prevent reinitialization
                set_msg();
                set_emitter();
                flag = 1;
            }
            
            kilo_message_rx = blank;

        }else{  //if distance > 45mm
            
            //reset color
            set_color(RGB(1,0,0));
            
            //start random motion
            random_motion();       
        }
    }

    if ( sent_message && flag == 1) {   //blink red when msg is sent
        sent_message = 0;
        set_color(RGB(0,1,0));
        delay(100);
        set_color(RGB(0,0,0));
    }
}

int main() {
    kilo_init();
    
    // register message callback
    kilo_start(setup, loop);
    return 0;
}