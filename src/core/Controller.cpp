#include "Controller.hpp"
#define DUAL_SHOCK_RUMBLE_ENB       0x01
#define DUAL_SHOCK_LED_ENB          0x02
#define DUAL_SHOCK_LED_BLINK_ENB    0x04


DualShock4& DualShock4::enableRumble(void){
    this->controllerOutPacket->payload.flags |= DUAL_SHOCK_RUMBLE_ENB;
    return *this;
}

DualShock4& DualShock4::enableLED(void){
    this->controllerOutPacket->payload.flags |= DUAL_SHOCK_LED_ENB;
    return *this;
}

DualShock4& DualShock4::enableLEDBlink(void){
    this->controllerOutPacket->payload.flags |= DUAL_SHOCK_LED_BLINK_ENB;
    return *this;
}

DualShock4& DualShock4::setRumBle (unsigned char smallRumbleLevel, unsigned char bigRumbleLevel){
    this->controllerOutPacket->payload.smallMotorPower = smallRumbleLevel;
    this->controllerOutPacket->payload.largeMotorPower = bigRumbleLevel;
    return *this;
}

DualShock4& DualShock4::setLEDColor (unsigned char r, unsigned char g, unsigned char b){
    this->controllerOutPacket->payload.redLed =    r;
    this->controllerOutPacket->payload.greenLed =  g;
    this->controllerOutPacket->payload.blueLed =   b;
    return *this;
}

DualShock4& DualShock4::setLEDOnPeriod (unsigned char period){
    this->controllerOutPacket->payload.ledFlashOnTime = period;
    return *this;
}

DualShock4& DualShock4::setLEDOffPeriod (unsigned char period){
    this->controllerOutPacket->payload.ledFlashOffTime = period;
    return *this;
}

DualShock4& DualShock4::send(void){
    int returnValue;
    returnValue =  hid_send_output_report(this->dev,
        (const unsigned char *)this->controllerOutPacket,
        32);
    if(returnValue < 0){
        std::cout << "error" << std::endl;
    }else{
        std::cout << "success" << std::endl;
        std::cout << returnValue << std::endl;
    }
    return *this;
}

void DualShock4::send(dualshock4_hid_out_payload *payload){
    std::memcpy(&(this->controllerOutPacket->payload), payload, sizeof(this->controllerOutPacket->payload));
    int returnValue;
    returnValue =  hid_send_output_report(this->dev,
        (const unsigned char *)this->controllerOutPacket,
        sizeof(this->controllerOutPacket));
}