//#include <pthread.h>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>


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




        std::string command;
        std::string response;
        response.resize(256);
        while(true){

            command.clear();
            response.clear();
            response.resize(256);
            std::cin >> command;

            command.append("\n");

            boost::asio::write(socket,
                               boost::asio::buffer(command.data(),
                                                   sizeof(command)));


            boost::system::error_code ec;
            boost::asio::read(
                socket,
                boost::asio::buffer(response),
                boost::asio::transfer_at_least(1),
                ec
                );

            std::cout << response << std::endl;
        }


    return 0;
}

