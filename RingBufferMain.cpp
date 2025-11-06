#include <iostream>

#include "CommunicationSystem.h"
#include "Ringbuffer.h"
#include <thread>   
#include <random>
#include <string>    // for std::string
#include <future>
#include <chrono>



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
      
        std::srand(static_cast<unsigned int>(std::time(nullptr))); // Seed RNG

        // Generate signal strength between -100 and -50
        int signal_strength = -100 + std::rand() % 51; // rand() % (max - min + 1)

        // Choose a random frequency from the list
        std::vector<int> frequencies = { 2400, 2450, 2500 };
        int frequency = frequencies[std::rand() % frequencies.size()];

    
        strDB = to_string(signal_strength);
        strFreq = to_string(frequency);

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

    CommunicationSystem comm(ip, 8082); // check IP is correct 
   
    std::thread t(&CommunicationSystem::listen, &comm);  

    //std::future<void> t = std::async(std::launch::async, &CommunicationSystem::listen, comm); // THIS DOESN'T WORK


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
    //t.get(); // applies if using std::async

    cout << "Reached the end....DEBUG \n";
    exit(0);
    return 0;
}
