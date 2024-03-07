// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <zmq.hpp>
#include <string>
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <zmq_addon.hpp>

using namespace std;
std::mutex mtx;


void sendMessages()
{
    zmq::context_t ctx(1);
    zmq::socket_t sender (ctx, ZMQ_PUB);
    sender.bind("tcp://127.0.0.1:50261");

    std::cout << "Sender thread started...\n";
    
    while (true) {
       
        string message;
        std::cout << "Enter message to ('exit' to quit): ";
        getline(cin, message);

        if (message == "exit")
            break;
    

        /*short buffer[1];
        buffer[0] = 12;*/


        //zmq_send(sender, buffer, 2 * 1, 0);
        sender.send(zmq::buffer(message), zmq::send_flags::dontwait);

        cout << "done!\n";
        zmq_sleep(2);
       
    }
}

void recMessages() 
{
    zmq::context_t ctx(1);
    zmq::socket_t receiver(ctx, ZMQ_SUB);
    receiver.connect("tcp://127.0.0.1:50262");
    receiver.set(zmq::sockopt::subscribe, "");
    std::cout << "Receiver thread started...\n";

    //zmq::message_t rec_msg;
    while (true)
    {
        
        /*short buffer[1];
        zmq_recv(receiver, buffer, 2 * 1, 0);
        std::cout << buffer[0]  << "\n";
        std::cout << "received!\n";
        zmq_sleep(2);*/
        char buffer[256];

        int nbytes = zmq_recv(receiver, buffer, 256, 0);

        std::cout << "Received message: ";
        for (int i = 0; i < nbytes; i++) {
            std::cout << buffer[i];
        }
        std::cout << std::endl;
      
    }
}

int main()
{   
    std::thread senderThread(sendMessages);
    std::thread recThread(recMessages);

    senderThread.join();
    recThread.join();

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
