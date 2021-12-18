#include "kilolib.h"

message_t msg;
uint8_t sent_message;
uint8_t star_found = 0;

int distance;
int data_rcv;

// message transmission callback
message_t *tx_message() {
    return &msg;
}
// successful transmission callback
void tx_message_success() {
    sent_message = 1;
}

void message_rx(message_t *m, distance_measurement_t *d) {
    data_rcv = m.data[0];
    distance = estimate_distance(d);
}


void setup() {
}

void set_msg() {
    msg.type = NORMAL;
    msg.crc = message_crc(&msg);
}

void random_mov() {
    set_color(RGB(0, 0, 1));
    spinup_motors();
    set_motors(kilo_straight_left, kilo_straight_right);
    delay(2000);

    spinup_motors();
    set_motors(kilo_turn_left, 0);
    delay(500);
    
    spinup_motors();
    set_motors(0, kilo_turn_right);
    delay(500);
}


// blink red when a new message is sent
void loop() {
    // if star_found == 1, transmit
    if(star_found == 1 && sent_message == 1) {
        sent_message = 0;
        set_color(RGB(1,0,0));
        delay(100);
        set_color(RGB(0,0,0));
    }
    else {
        // if distance < 40, set star_found = 1
        if(distance < 40) {
            set_msg();
            star_found = 1;
        }
        else {      // else, random movement
            random_mov();
        }
    }
}
int main() {
    kilo_init();
    
    // register message transmission callback
    kilo_message_tx = tx_message;
    // register tranmsission success callback
    kilo_message_tx_success = tx_message_success;
    
    kilo_message_rx = message_rx;

    kilo_start(setup, loop);
    return 0;
}