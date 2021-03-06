#include <iostream>

#ifdef _WINDOWS
#include <windows.h>
#else
#include <unistd.h>
#define Sleep(x) usleep((x)*1000)
#endif

#include "mace_digimesh_wrapper.h"


extern char VEHICLE[] = "Vehicle";


int main(int argc, char *argv[])
{
    const char* RADIO1 = "/dev/ttyUSB0";
    const char* RADIO2 = "/dev/ttyUSB1";


    MACEDigiMeshWrapper<VEHICLE> *wrapper1 = new MACEDigiMeshWrapper<VEHICLE>(RADIO1, DigiMeshBaudRates::Baud9600);
    MACEDigiMeshWrapper<VEHICLE> *wrapper2 = new MACEDigiMeshWrapper<VEHICLE>(RADIO2, DigiMeshBaudRates::Baud9600);




    wrapper1->AddHandler_NewRemoteComponentItem_Generic([RADIO1](const char* component, int num, uint64_t addr){
        printf("%s\n New Remote Vehicle\n", RADIO1);
        printf("  Vehicle ID:    %d\n", num);
        printf("  DigiMesh Addr: %llx\n\n", addr);
    });



    wrapper2->AddHandler_NewRemoteComponentItem_Generic([RADIO2](const char* component, int num, uint64_t addr){
        printf("%s\n New Remote Vehicle\n", RADIO2);
        printf("  Vehicle ID:    %d\n", num);
        printf("  DigiMesh Addr: %llx\n\n", addr);
    });




    wrapper1->AddHandler_Data([RADIO1](const std::vector<uint8_t> &data) {
        printf("%s\n New Data Received\n  Size: %d\n", RADIO1, data.size());
    });


    wrapper2->AddHandler_Data([RADIO2](const std::vector<uint8_t> &data) {
        printf("%s\n New Data Received\n  Size: %d\n", RADIO2, data.size());
    });


    Sleep(2000);

    wrapper1->AddComponentItem<VEHICLE>(1);
    wrapper2->AddComponentItem<VEHICLE>(2);

    Sleep(7000);

    std::thread thread1([wrapper1](){
        while(true)
        {
            wrapper1->SendData<VEHICLE>(2, {0x1, 0x02});
            int time = (((float)std::rand())/RAND_MAX)*100 + 200;
            Sleep(time);
        }

    });


    std::thread thread2([wrapper2](){
        while(true)
        {
            wrapper2->SendData<VEHICLE>(1, {0x1, 0x2, 0x3, 0x1, 0x2, 0x3, 0x1, 0x2, 0x3, 0x1, 0x2, 0x3, 0x1, 0x2, 0x3, 0x1, 0x2, 0x3, 0x1, 0x2, 0x3, 0x1, 0x2, 0x3, 0x1, 0x2, 0x3, 0x1, 0x2, 0x3, 0x1, 0x2, 0x3, 0x1, 0x2, 0x3, 0x1, 0x2, 0x3, 0x1, 0x2, 0x3, 0x1, 0x2, 0x3, 0x1, 0x2, 0x3, 0x1, 0x2, 0x3, 0x1, 0x2, 0x3, 0x1, 0x2, 0x3, 0x1, 0x2, 0x3, 0x1, 0x2, 0x3, 0x1, 0x2, 0x3, 0x1, 0x2, 0x3, 0x1, 0x2, 0x3, 0x1, 0x2, 0x3});
            int time = (((float)std::rand())/RAND_MAX)*100 + 200;
            Sleep(time);
        }

    });



    /*
    wrapper2->BroadcastData({0x1, 0x2, 0x3});

    Sleep(1000);

    wrapper1->RemoveComponentItem<VEHICLE>(1);
    wrapper2->RemoveComponentItem<VEHICLE>(2);
    */



    while(true) {

    }
}
