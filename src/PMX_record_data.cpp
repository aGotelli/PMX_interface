#include <iostream>
#include <signal.h>
#include <real_time_tools/timer.hpp>

#include "PMX_interface/pmx_interface.hpp"

#include "utilities/Eigen/eigen_io.hpp"

using namespace std::chrono;

std::shared_ptr<bool> StopDemos =
    std::make_shared<bool>(false);

std::shared_ptr<bool> start_recording =
    std::make_shared<bool>(false);


void countingFuntion()
{
    unsigned int i =0;
    while(not *StopDemos){
        std::this_thread::sleep_for(1s);
        std::cout << "Runnning for : " << ++i << "\r";
        std::cout.flush();
    }
}


void my_handler(int)
{
    *StopDemos = true;
}

int main() {

    // make sure we catch the ctrl+c signal to kill the application properly.
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = my_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);
    *StopDemos = false;

    unsigned int number_of_sensors = 4;
    PMXInterface PMX_interface(number_of_sensors,
                               start_recording,
                               StopDemos);

    PMX_interface.startRecordingLoop();


    std::cout << "input to continue \n\n\n\n";
    std::cout.flush();
    getchar();
    std::cout << "ok \n\n\n\n";
    std::cout.flush();

    *start_recording = true;






    while (!*StopDemos)
    {
        real_time_tools::Timer::sleep_sec(0.01);
    }



    std::cout << "\n\n\n""Finished to record data !!" << std::endl;


    YAML::Node PMX_node;
    Eigen::MatrixXd PMX_data;


    PMX_interface.getSamplesData(PMX_node, PMX_data);


    writeToFile("PMX_data", PMX_data, "data/prove/");









    return 0;
}















//#include "pmx_interface.hpp"

//#include "utilities/Eigen/eigen_io.hpp"

//using namespace std::chrono;



//void my_handler(int)
//{
//    *StopDemos = true;
//}



//#include <iostream>
//#include <cstring>
//#include <cstdlib>
//#include <sys/socket.h>
//#include <netinet/in.h>

//#define TCP_IP "192.168.1.15"
//#define TCP_PORT 5005
//#define BUFFER_SIZE 60

//int main() {
//    int serverSocket, clientSocket;
//    struct sockaddr_in serverAddress, clientAddress;
//    socklen_t clientAddressLength = sizeof(clientAddress);

//    // Create socket
//    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
//    if (serverSocket < 0) {
//        std::cerr << "Error creating socket." << std::endl;
//        return 1;
//    }

//    // Set up the server address structure
//    serverAddress.sin_family = AF_INET;
//    serverAddress.sin_port = htons(TCP_PORT);
//    inet_pton(AF_INET, TCP_IP, &(serverAddress.sin_addr));

//    // Bind socket to the specified IP and port
//    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
//        std::cerr << "Error binding socket." << std::endl;
//        return 1;
//    }

//    // Start listening for incoming connections
//    if (listen(serverSocket, 1) < 0) {
//        std::cerr << "Error listening on the socket." << std::endl;
//        return 1;
//    }

//    // Accept a connection from a client
//    clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);
//    if (clientSocket < 0) {
//        std::cerr << "Error accepting the client connection." << std::endl;
//        return 1;
//    }

//    std::cout << "Connection address: " << inet_ntoa(clientAddress.sin_addr) << std::endl;

//    char buffer[BUFFER_SIZE];
//    int bytesReceived;
//    double doubleValue1, doubleValue2;

//    while (true) {
//        bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
//        if (bytesReceived <= 0) {
//            if (bytesReceived < 0) {
//                std::cerr << "Error receiving data from the client." << std::endl;
//            }
//            break;
//        }

//        buffer[bytesReceived] = '\0';

//        // Extract the two double values from the received data
//        char *token;
//        token = strtok(buffer, "\t");
//        if (token) {
//            doubleValue1 = atof(token);
//            token = strtok(NULL, "\t");
//            if (token) {
//                doubleValue2 = atof(token);
//                std::cout << "Received data: " << doubleValue1 << "\t" << doubleValue2 << "\r";
//                std::cout.flush();
//            } else {
//                std::cerr << "Error parsing the second double value." << std::endl;
//            }
//        } else {
//            std::cerr << "Error parsing the first double value." << std::endl;
//        }
//    }

//    // Close the client socket
//    close(clientSocket);

//    // Close the server socket
//    close(serverSocket);

//    return 0;
//}





//int main()
//{

//    // make sure we catch the ctrl+c signal to kill the application properly.
//    struct sigaction sigIntHandler;
//    sigIntHandler.sa_handler = my_handler;
//    sigemptyset(&sigIntHandler.sa_mask);
//    sigIntHandler.sa_flags = 0;
//    sigaction(SIGINT, &sigIntHandler, NULL);
//    *StopDemos = false;


//    PMXInterface pmx_interface;

//    pmx_interface.startRecordingLoop();



//    for(int i=5; i>=0; i--){
//        std::cout << "Starting recording in : " << i << " s\r";
//        std::cout.flush();
//        std::this_thread::sleep_for(std::chrono::duration(std::chrono::seconds(1)));
//    }

////    pmx_interface.thread.join();




//    std::chrono::high_resolution_clock::time_point when_started;
//    std::chrono::high_resolution_clock::time_point target_time;



//    const double frequency = 100;
//    const double recording_time = 10.0;

//    const unsigned int number_of_steps = static_cast<unsigned int>(recording_time*frequency);


//    std::vector<Sample> samples_stack(number_of_steps);
//    unsigned int index = 0;

//    when_started = std::chrono::high_resolution_clock::now();
//    target_time = when_started + 10ms;
//    unsigned int count = 0;
//    unsigned int seconds = 0;
//    while(not *StopDemos and
//           index < number_of_steps) {

//        pmx_interface.getSample( samples_stack[index] );
//        std::this_thread::sleep_until(target_time);
//        target_time += 10ms;

//        index ++;
//        count ++;
//        if(count==100){
//            seconds ++;
//            std::cout << "simulation time " << seconds << "\r";
//            std::cout.flush();
//            count =0;
//        }

//    }



//    std::cout << "Finished to record data !!" << std::endl;


//    Eigen::MatrixXd samples_data(3, number_of_steps);

//    for(unsigned int col=0; const auto& sample : samples_stack){

//        samples_data.col(col++) << //sample.time_stamp.time_since_epoch().count(),
//            sample.time_stamp,
//            sample.F1,
//            sample.F2;

//    }


//    const std::string path = "data/PMX/" + std::to_string(static_cast<int>(frequency)) + "Hz/mutex";



//    writeToFile("samples_data", samples_data, path);





//    return 0;
//}
