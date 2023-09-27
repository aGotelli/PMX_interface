#include "pmx_interface.hpp"

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#include <unistd.h>
#include <string.h>

#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>

#include <real_time_tools/spinner.hpp>
#include <real_time_tools/timer.hpp>


PMXInterface::PMXInterface(const unsigned int t_number_of_sensors,
                           std::shared_ptr<bool> t_start_recording,
                           std::shared_ptr<bool> t_StopDemos)
    :   m_number_of_sensors(t_number_of_sensors),
    m_start_recording(t_start_recording),
    m_StopDemos(t_StopDemos)
{

    m_samples_stack.clear();

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        std::cerr << "Error creating socket." << std::endl;
    }

    // Set up the server address structure
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(TCP_PORT);
    inet_pton(AF_INET, TCP_IP.c_str(), &(serverAddress.sin_addr));

    // Bind socket to the specified IP and port
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Error binding socket." << std::endl;
    }

    // Start listening for incoming connections
    if (listen(serverSocket, 1) < 0) {
        std::cerr << "Error listening on the socket." << std::endl;
    }

    // Accept a connection from a client
    clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);
    if (clientSocket < 0) {
        std::cerr << "Error accepting the client connection." << std::endl;
    }

}


PMXInterface::~PMXInterface()
{

    // Close the client socket
    close(clientSocket);

    // Close the server socket
    close(serverSocket);

    thread.join();

}


void PMXInterface::startRecordingLoop()
{
    thread = std::thread([this](){recordingLoop();});
}




void PMXInterface::recordingLoop()
{

    std::cout << "Connection address: " << inet_ntoa(clientAddress.sin_addr) << std::endl;

    char buffer[BUFFER_SIZE];
    unsigned int bytesReceived;

    Sample sample;


    std::vector<double> forces(2);
    std::vector<double> timestamps(2);
    std::string values;


    const unsigned int size_double = sizeof(double);


    m_start = std::chrono::high_resolution_clock::now();
    while (not *m_StopDemos) {

        bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);

//        std::cout << buffer << std::endl;
//        std::cout << std::hex << buffer << std::endl;


        if(bytesReceived > size_double*m_number_of_sensors){
            //            buffer[bytesReceived] = '\0';

            if(*m_start_recording){


                values = std::string(buffer);

                sample.data_buffer = values;
                sample.time_stamp = std::chrono::high_resolution_clock::now();

                m_samples_stack.push_back( sample );
            }


        }


    }


}


void PMXInterface::getSamplesData(YAML::Node &t_PMX_node, Eigen::MatrixXd &t_PMX_data)const
{

    const unsigned int number_of_samples = m_samples_stack.size();

    t_PMX_data = Eigen::MatrixXd::Zero(1 + 2*m_number_of_sensors, number_of_samples);


    double F;
    double time;


    for(unsigned int index = 0;const auto& sample : m_samples_stack){

        const auto relative_time = std::chrono::high_resolution_clock::duration(sample.time_stamp - m_start).count()/1e9;

        unsigned int row = 0;
        t_PMX_data(row, index) = relative_time;

        std::istringstream iss(sample.data_buffer);
        std::string token;

        for(unsigned int sensor=0; sensor<m_number_of_sensors; sensor++){

            std::getline(iss, token, ':');
            F = std::stod(token);
            std::getline(iss, token, '/');
            time = std::stod(token);

            t_PMX_data(++row, index) = time;
            t_PMX_data(++row, index) = F;
        }

        index++;

    }

    t_PMX_node["number_of_samples"] = number_of_samples;
    t_PMX_node["number_of_sensors"] = m_number_of_sensors;

    t_PMX_node["data_order"].push_back("local_time");
    t_PMX_node["data_order"].push_back("sensor_timestamp");
    t_PMX_node["data_order"].push_back("sensor_data");


}
