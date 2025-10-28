#include <iostream>

#include "CommunicationSystem.h"
#include "Ringbuffer.h"
#include <thread>   
#include <random>
#include <string>    // for std::string


using namespace std;

// Not yet in use
void RandomFreqGen(CommunicationSystem& rcomm)
{
    int Db = 0;
    float Freq = 0;

    string strDB;
    string strFreq;
    string strMessage;

    while (true)
    {
        Db = rand() % 100;           // Random value between 0 and 99
        Freq = static_cast<float>(rand() % 100); // Random float

        strDB = to_string(Db);
        strFreq = to_string(Freq);

        strMessage = strDB + " " + strFreq;

        rcomm.sendData(reinterpret_cast<const uint8_t*>(strMessage.c_str()), strMessage.length());
        // no exit!
        Sleep(10);
    }
}

int main() {
    // Create a communication system to send/receive data


    std::string ip;

    bool bExit = false;

    std::cout << "Enter IP address to send to: ";

    std::cin >> ip;

    int iChoice = 1;

    CommunicationSystem comm(ip, 8080); // check IP is correct 
   
    std::thread t(&CommunicationSystem::listen, &comm);

    cout << "Send 1) Text Messages or 2) Random Number Generator : ";

    std::cin >> iChoice;

    if (iChoice == 2)
    {
        RandomFreqGen(comm);
        exit(0); // will never reach this but okay for now
    }


    while (!bExit)
    {
        std::string str;
        std::cout << "Enter a message to send or Q to quit: ";

        std::cin >> str;

        if (str == "Q")
        {
            exit(0);
        }

        if (!bExit)
        {
            std::cout << "sending...." << str << "\n";


            const uint8_t* message = reinterpret_cast<const uint8_t*>(str.c_str());

     

            comm.sendData(message, str.size());
        }

    }
    

    t.join();
    exit(0);
    return 0;
}
