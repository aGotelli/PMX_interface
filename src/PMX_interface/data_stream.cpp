//#include <pthread.h>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>



#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <thread>
#include <chrono>
using namespace std::chrono_literals;


std::vector<char> vBuffer(20*1024);



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

    std::string ip_address = "127.0.0.1";
    std::string port = "9001";
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

    std::string data;

    boost::asio::streambuf buffer;


    while(true){
        data.clear();
        boost::asio::read(socket, buffer, boost::asio::transfer_exactly(4)); // Read exactly 4 bytes

        std::istream is(&buffer);
        is >> data;

        // Convert the string to an integer
        uint32_t size;
        std::memcpy(&size, data.data(), sizeof(size));
        data.clear();

        std::cout << size << std::endl;


        float floats[size];

        auto res = recv_all(socket, reinterpret_cast<char *>(&floats[0]), sizeof floats);

        boost::asio::read(socket, buffer, boost::asio::transfer_at_least(size)); // Read exactly 4 bytes

        std::istream iss(&buffer);
        iss >> data;

        std::cout << data << std::endl;
    }



    return 0;
}

