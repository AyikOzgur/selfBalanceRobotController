#include <cmath>
#include <cstdint> // Include for int16_t
#include <iostream>

#include "plotOpenCv.h"
#include "UdpChannel.h"
#include "SerialPort.h"

using namespace cr::utils;


int main()
{
    UdpChannel udpChannel;
	if(!udpChannel.init("192.168.1.12", 7032, "192.168.1.18", 7031))
	{
		std::cout << "UdpChannel init error." << std::endl;
	}

    // Buffer for reading.
    const int sizeOfBuf = 64;
    uint8_t buf[sizeOfBuf];

    // Vector for angle plotting.
    //const int maxNumberOfAngles = 1280;
    //std::vector<int> angles;
    //Plot graph("Test graph", 1280, 720, 
    //            cv::Scalar(0, 128, 128), 
    //            cv::Scalar(50, 50, 50));

    cr::clib::SerialPort serialPort; // COM30
    if (!serialPort.open("\\\\.\\COM30", 9600, 0, "8N1"))
    {
        std::cout << "Serial port open error." << std::endl;
    }
    else
    {
        std::cout << "Serial port opened." << std::endl;
    }

    while(true)
    {

        /*
        // Read data.
        if(udpChannel.receiveData(buf, sizeOfBuf) > 0)
        {
            int8_t angle =  (int8_t)buf[0];
            std::cout << "Data : " << (int)angle << std::endl;
            
            if (angles.size() < maxNumberOfAngles)
            {
                angles.push_back(angle);
            }
            else 
            {
                // Array is full move everything by one to left
                for (int i = 1; i < angles.size(); ++i)
                {
                    angles.at(i -1) = angles.at(i);
                }

                // Put new one to end.
                angles.at(maxNumberOfAngles -1) = angle;
            }
        }

        // Draw data.
        graph.clean();
        graph.addPlot(angles, 0, 0, 0, cv::Scalar(0, 0, 255), 1);
        graph.show();
        cv::waitKey(1);
        */

        float angle = 0.0f;
        float angle2 = 0.0f;
        float angle3 = 0.0f;
        int bytesRead = udpChannel.receiveData(buf, sizeOfBuf);
        if (bytesRead > 0)
        {
            memcpy(&angle, buf, sizeof(float));
            //std::cout << "angle : " << angle << std::endl;
            memcpy(&angle2, buf + sizeof(float), sizeof(float));
            //std::cout << "angle2 : " << angle2 << std::endl;
            memcpy(&angle3, buf + 2 * sizeof(float), sizeof(float));
            //std::cout << "angle3 : " << angle3 << std::endl;
        }

        // Send data to serial port.
        char buffer[64];
        int size = 0;
        size = sprintf(buffer, "%.2f,%.2f,%.2f\n", angle, angle2, angle3);
        serialPort.write((uint8_t*)buffer, size);


    }
    return 0;
}