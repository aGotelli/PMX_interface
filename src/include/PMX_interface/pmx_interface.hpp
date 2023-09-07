#ifndef PMX_INTERFACE_HPP
#define PMX_INTERFACE_HPP

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

#include <chrono>
#include <vector>

#include <mutex>
#include <thread>
#include <future>


#include <Eigen/Dense>
#include <deque>
#include <yaml-cpp/yaml.h>





struct Sample {
    double F1;
    double F2;

    std::string data_buffer;

    std::chrono::high_resolution_clock::time_point time_stamp;
};






struct PMXInterface{

    PMXInterface(const unsigned int t_number_of_sensors,
                std::shared_ptr<bool> t_start_recording,
                std::shared_ptr<bool> t_StopDemos);

    ~PMXInterface();


    void startRecordingLoop();

    void recordingLoop();


    void getSamplesData(YAML::Node &t_PMX_node, Eigen::MatrixXd &t_PMX_data)const;



    std::string m_tcp_ip_address { std::string("192.168.1.15") };
    unsigned int m_tcp_ip_port { 5005 };


    unsigned int m_number_of_sensors { 0 };
    unsigned int m_sensor_message_size { 2 };

    unsigned int m_buffer_size { [this]()->unsigned int {
        const unsigned int size_double = sizeof(double);

        const unsigned int sensor_output_dimension = 2*size_double;

        unsigned int buffer_size = sensor_output_dimension*m_number_of_sensors;


        buffer_size = 124;

        return buffer_size;
    }() };



    int serverSocket;
    int clientSocket;
    struct sockaddr_in serverAddress;
    struct sockaddr_in clientAddress;
    socklen_t clientAddressLength { sizeof(clientAddress) };


    std::thread thread;
    std::future<void> m_future;


    std::chrono::high_resolution_clock::time_point m_start;



    std::deque<Sample> m_samples_stack;


    std::shared_ptr<std::vector<double>> m_forces {
        std::make_shared<std::vector<double>>()
    };

    std::shared_ptr<bool> m_start_recording {
        std::make_shared<bool>(false) };

    std::shared_ptr<bool> m_StopDemos =
        std::make_shared<bool>(false);


};


#endif // PMX_INTERFACE_HPP
