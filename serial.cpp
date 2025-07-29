#include "serial.hpp"
#include <iostream>
#include <string>
#include <filesystem>
#ifdef _WIN32
#include <windows.h>
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
                return port
            }
        } 
            else {
            std::cerr << "Read failed or no data received\n";
        }
        }
    }
#else
    // macOS/Linux: Look for tty.* and cu.* devices
    for (const auto& entry : std::filesystem::directory_iterator("/dev")) {
        std::string name = entry.path().filename();
        if (name.find("tty.usb") == 0 || name.find("ttyACM") == 0 || name.find("ttyUSB") == 0 || name.find("cu.usb") == 0) {
            
        }

}
#endif
}

