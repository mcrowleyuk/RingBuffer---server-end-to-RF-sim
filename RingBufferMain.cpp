#include <iostream>

#include "CommunicationSystem.h"
#include "Ringbuffer.h"
#include <thread>   
#include <random>
#include <string>    // for std::string
#include <future>
#include <chrono>

const int DEFAULT_BASE = -75;
const int JITTERMINUS = -4;
const int JITTERPLUS = +4;

using namespace std;

// Not yet in use
void RandomFreqGen(CommunicationSystem& rcomm)
{
    int Db = 0;
    float Freq = 0;
    int dBmA = 0;
    int iBase = DEFAULT_BASE; 

    string strDB;
    string strFreq;
    string strMessage;

    int signal_strength = 0;
    std::vector<int> frequencies = { 2400, 2450, 2500 };

    cout << "dBm base =  (eg -75) \n";
    cin >> dBmA;
    


    while (true)
    {
        std::random_device rd;  // Non-deterministic seed
        std::mt19937 gen(rd()); // Mersenne Twister engine
        std::uniform_int_distribution<> jitter(JITTERMINUS, JITTERPLUS);

        signal_strength = iBase + jitter(gen);

        // Choose a random frequency from the list
       
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

    std::cout << "Enter IP address to send to: (comms port is 8082)";

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
