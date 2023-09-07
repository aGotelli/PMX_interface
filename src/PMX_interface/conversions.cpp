//#include <pthread.h>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>

#include <thread>
#include <chrono>
using namespace std::chrono_literals;


std::vector<char> vBuffer(20*1024);



void grabData(boost::asio::ip::tcp::socket &t_socket)
{
    t_socket.async_read_some(boost::asio::buffer(vBuffer.data(), vBuffer.size()),
                             [&](std::error_code ec, std::size_t length){
                                 if(!ec){
                                     std::cout << "\n\nRead " << length << " bytes\n\n";

                                     for(int i=0; i<length; i++)
                                         std::cout << vBuffer[i];

                                     grabData(t_socket);

                                 }
                             });
}


std::string sendCommand(std::string t_command, boost::asio::ip::tcp::socket &t_socket)
{

//    std::cout << "sending : " << t_command << "     received : ";
    boost::asio::write(t_socket,
                       boost::asio::buffer(t_command.data(),
                                           sizeof(t_command)));

    std::string response;
    response.resize(256);
    boost::system::error_code ec;
    boost::asio::read(
        t_socket,
        boost::asio::buffer(response),
        boost::asio::transfer_at_least(1),
        ec
        );

//    std::cout << t_command << std::endl;

    return response;

}


int main(int argc, char* argv[])
{

    std::string ip_address = "192.172.1.2";
    std::string port = "55000";
    boost::asio::io_context io_context;
    boost::asio::ip::tcp::resolver resolver(io_context);
    boost::asio::ip::tcp::socket socket(io_context);
    //                std::cout << "[FBGS] Get endpoints... \n";
    boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(ip_address, port);

    boost::asio::ip::tcp::endpoint ep = *endpoints;

    std::cout << "Establishing connection to " << ep << "... \n";

    //Connect to socket and open connection
    boost::asio::connect(socket, endpoints);

    std::cout << "\n\n\n            Connected!\n\n\n" << std::endl << std::endl;


    std::vector<std::string> commands_list = {
        "idn?\n",
        "pcs?\n",
        "pcs1\n",
        "sps?\n",
        "sps1\n",
        "mrg0\n",
        "pcs1\n",
        "sms1\n",
        "tsv-1\n",
        "tsv?\n",
        "rmb?1,6409,0"
    };

//    for(const auto &command : commands_list){
//        auto response = sendCommand(command, socket);
//        std::cout << command << response << std::endl;
//        std::this_thread::sleep_for(1s);
//    }

    auto response = sendCommand("pcs?\n", socket);
    std::cout << "pcs?\n" << response << std::endl;

    response = sendCommand("pcs1\n", socket);
    std::cout << "pcs1\n" << response << std::endl;

    response = sendCommand("sps?\n", socket);
    std::cout << "sps?\n" << response << std::endl;

    response = sendCommand("sps1\n", socket);
    std::cout << "sps1\n" << response << std::endl;

//    response = sendCommand("mrg0\n", socket);
//    std::cout << "mrg0\n" << response << std::endl;

//    response = sendCommand("pcs1\n", socket);
//    std::cout << "pcs1\n" << response << std::endl;

//    response = sendCommand("sms1\n", socket);
//    std::cout << "sms1\n" << response << std::endl;

    response = sendCommand("tsv1\n", socket);
    std::cout << "tsv1\n" << response << std::endl;


    response = sendCommand("tsv?\n", socket);
    std::cout << "tsv?\n" << response << std::endl;


    while(true){


        response = sendCommand("rmb?1,6409,0\n", socket);
        std::cout << "rmb?1,6409,0\n" << response << std::endl;

        std::string data(response.begin()+2, response.end());

        float f[2];

        memcpy(&f, response.data() + 2, 8);

        std::cout << "f1 : " << f[0] << " f2 : " << f[1] << std::endl;


        std::this_thread::sleep_for(1ms);

    }
    return 0;
}

