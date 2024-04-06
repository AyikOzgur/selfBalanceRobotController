#include <cmath>
#include <cstdint> // Include for int16_t
#include <iostream>

#include <plotOpenCv.h>
#include "UdpChannel.h"

using namespace cr::utils;


int main()
{
    UdpChannel udpChannel;
	if(!udpChannel.init("192.168.1.12", 7032, "192.168.1.18", 7031))
	{
		std::cout << "UdpChannel init error." << std::endl;
	}

    // Buffer for reading.
    const int sizeOfBuf = 1;
    uint8_t buf[sizeOfBuf];

    // Vector for angle plotting.
    const int maxNumberOfAngles = 1280;
    std::vector<int> angles;
    Plot graph("Test graph", 1280, 720, 
                cv::Scalar(0, 128, 128), 
                cv::Scalar(50, 50, 50));

    while(true)
    {
        // Read data.
        if(udpChannel.receiveData(buf, sizeOfBuf) > 0)
        {
            int8_t angle = buf[0];
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
    }
    return 0;
}