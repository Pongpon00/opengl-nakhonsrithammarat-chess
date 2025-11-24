#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <stdlib.h>
#include <cstring>
#include <hidapi.h>
#include <iostream>

#define SONY_INTERACTIVE_ENTERTAINMENT_VENDOR_ID 	(unsigned short)1356
#define	DUALSHOCK_4_CONTROLLER_PRO_ID				(unsigned short)2508

#pragma pack (1)
struct dualshock4_hid_out_payload {
	unsigned char flags;
	unsigned char reserved_unk;
	unsigned char Empty;
	unsigned char smallMotorPower;
	unsigned char largeMotorPower;
	unsigned char redLed;
	unsigned char greenLed;
	unsigned char blueLed; 
	unsigned char ledFlashOnTime; 
	unsigned char ledFlashOffTime;
    //Other stuff that we dont use in the project (eg. audio out, I2C communication)
	unsigned char padding[21];
};


#pragma pack(1)
struct dualshock4_hid_out_packet{
    unsigned char reportId;
    dualshock4_hid_out_payload payload;
};



class DualShock4 {
    
    private:
        hid_device *dev;
        uint8_t    isUsb;
        dualshock4_hid_out_packet       *controllerOutPacket;
    
    public:
        DualShock4(hid_device_info *openDeviceInfo){

            if(openDeviceInfo == NULL){
                std::cout<< "The given openning device infor pointer is NULL" << std::endl;
                std::abort();
            }

            if(openDeviceInfo->product_id != DUALSHOCK_4_CONTROLLER_PRO_ID){
                std::cout<< "Controller type mismathced" << std::endl;
                std::abort();
            }

            this->dev =  hid_open(  openDeviceInfo->vendor_id,
                                    openDeviceInfo->product_id,
                                    openDeviceInfo->serial_number);
            
            if(this->dev == NULL){
                hid_error(this->dev);
                std::abort();
            }

            // Check if controller connect via USB or not
            if(openDeviceInfo->usage == -1){
                isUsb = 0;
            }else{
                isUsb = 1;
            }

            //Output packet allocation
            this->controllerOutPacket = (struct dualshock4_hid_out_packet *)std::calloc(1,sizeof(struct dualshock4_hid_out_packet));
            if(isUsb == 1){
                this->controllerOutPacket->reportId = 0x05;
            }else{
                this->controllerOutPacket->reportId = 0x11;
            }
            std::memset((void *) (&this->controllerOutPacket->payload) ,0x00,sizeof(this->controllerOutPacket->payload));
        }
        
        ~DualShock4(){
            // Send reset signal
            this->controllerOutPacket->payload.reserved_unk = 0x01; 
            hid_send_output_report(this->dev,(unsigned char*)this->controllerOutPacket,sizeof(this->controllerOutPacket));

            std::free((void *)this->controllerOutPacket);
            std::cout << "test1" << std::endl;
            if(this->dev != NULL)
                hid_close(this->dev);
            std::cout << "test1" << std::endl;
        }

        public:
            DualShock4& enableRumble(void);
            DualShock4& enableLED(void);
            DualShock4& enableLEDBlink(void);
            DualShock4& setRumBle (unsigned char smallRumbleLevel, unsigned char bigRumbleLevel);
            DualShock4& setLEDColor (unsigned char r, unsigned char g, unsigned char b);
            DualShock4& setLEDOnPeriod (unsigned char period);
            DualShock4& setLEDOffPeriod (unsigned char period);
            DualShock4& send(void);
            
            void send(dualshock4_hid_out_payload *payload);


};


#endif 