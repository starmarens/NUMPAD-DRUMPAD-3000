#include "serial.hpp"
#include <iostream>
#include <string>
#include <filesystem>
#include <cstring>
#ifdef _WIN32
#include <windows.h>
#elif __APPLE__
#include "TargetConditionals.h"
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#endif

using namespace std;

string Serial::findCOMports(){
    #ifdef _WIN32
    for(int i = 1; i <= 20; i++){
        string port = "COM" + to_string(i);
         HANDLE h = CreateFileA(port.c_str(), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
        if (h != INVALID_HANDLE_VALUE) {
            DCB dcbSerialParams = {0};
            dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

            if (!GetCommState(h, &dcbSerialParams)) {
                std::cerr << "Error getting serial state\n";
                CloseHandle(hSerial);
                return 1;
                }

            dcbSerialParams.BaudRate = CBR_115200;
            dcbSerialParams.ByteSize = 8;
            dcbSerialParams.StopBits = ONESTOPBIT;
            dcbSerialParams.Parity   = NOPARITY;

            if (!SetCommState(h, &dcbSerialParams)) {
                std::cerr << "Error setting serial state\n";
                CloseHandle(hSerial);
                return 1;
            }

            // 3. Clear any existing data in buffers
            PurgeComm(h, PURGE_RXCLEAR | PURGE_TXCLEAR);

            // 4. The string to send
            string message = "Are you ESP32";

            // 5. Send the string
            DWORD bytesWritten;
            if (!WriteFile(h, message.c_str(), message.size(), &bytesWritten, nullptr)) {
                std::cerr << "Write failed\n";
            } else {
                cout << "Sent " << bytesWritten << " bytes: " << message << "\n";
            }
        if (ReadFile(h, buffer, sizeof(buffer) - 1, &bytesRead, nullptr) && bytesRead > 0) {
            buffer[bytesRead] = '\0'; // Null-terminate the buffer
            if(string received(buffer) == "yes"){
                return port;
            }
        } 
            else {
            std::cerr << "Read failed or no data received\n";
        }
        }
    }
#else
    // macOS/Linux: Look for tty.* and cu.* devices
    for (const auto& entry : filesystem::directory_iterator("/dev")) {
        string name = entry.path().filename();
        if (name.find("tty.usb") == 0 || name.find("tty.usbserial")) {
           int port = open(name.c_str(),  O_RDWR | O_NOCTTY | O_SYNC);

            termios tty{};

            cfsetospeed(&tty, B115200);
            cfsetispeed(&tty, B115200);

            tty.c_cflag &= ~PARENB;    // no parity
            tty.c_cflag &= ~CSTOPB;    // 1 stop bit
            tty.c_cflag &= ~CSIZE;
            tty.c_cflag |= CS8;        // 8 bits per byte
            tty.c_cflag &= ~CRTSCTS;   // no flow control
            tty.c_cflag |= CREAD | CLOCAL; // turn on READ and ignore ctrl lines

            // Make raw
            tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // raw input
            tty.c_iflag &= ~(IXON | IXOFF | IXANY);         // no software flow
            tty.c_oflag &= ~OPOST;    
            tcsetattr(port, TCSANOW, &tty);

            string msg = "Are you ESP32\n";
            write(port, msg.c_str(), strlen(msg.c_str()));

            char buffer[256];
            int bytes = read(port, buffer, sizeof(buffer) - 1);
            buffer[bytes] = "\0";
            string response = received_data(buffer);
            if (response == "yes" )
                return name;


            
                }

}
#endif
}

