#include "cheats/aimbot.h"
#include <iostream>

#include <thread>
#include <chrono>

int main() {
    aimbot::processId = memory::GetProcID(L"cs2.exe");

    std::cout << "Csgo2's process Id: " << aimbot::processId << std::endl;

    aimbot::module_base = memory::GetModuleBaseAddress(aimbot::processId, L"client.dll");

    std::cout << "Found module base client id: 0x" << std::hex << aimbot::module_base << std::endl;

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        if (GetAsyncKeyState(VK_LSHIFT)) {
            aimbot::frame();
        }
    }

    return 0;
}