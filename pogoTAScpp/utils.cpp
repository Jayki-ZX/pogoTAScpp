// pogoTAScpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>
#include <fstream>

// global variables (ill maybe change this logic in the future)
DWORD initial_address = { NULL }; // value found following the README (section "finding initial memory")
DWORD address = initial_address;

//in frames
int MAX_RECORDING_LENGTH = 5000;

void saveCommand(std::ofstream file, int frames, bool left, bool right, bool jump) {

}

// Returns given inputs as a string
std::string parseInputs(bool left, bool right, bool jump) {
    std::string output;
    /*if (up == 1) {
        output += "up,";
    }
    if (down == 1) {
        output += "down,";
    }*/
    if (left) {
        output += "left,";
    }
    if (right) {
        output += "right,";
    }
    if (jump) {
        output += "jump,";
    }
    output += "end of input.";
    return output;
}

// Takes the currently recorded run on memory and stores it in a file with the following format:
// writeInputsByFrames(processHandle, 60, 0, 1, 1); 
// writeInputsByFrames(processHandle, x, b, b, b);
// ... etc ...
void extractRun(HANDLE processHandler) {
    std::ofstream run_file;
    run_file.open("TASrun.txt");
    
    int frames = 1;

    bool prev_left;
    bool prev_right;
    bool prev_jump;

    bool current_left;
    bool current_right;
    bool current_jump;

    //ReadProcessMemory(processHandler, (LPCVOID)address, &up, 1, NULL);
    //ReadProcessMemory(processHandler, (LPVOID)(address + 1), &down, 1, NULL);
    ReadProcessMemory(processHandler, (LPCVOID)(address + 2), &prev_left, 1, NULL);
    ReadProcessMemory(processHandler, (LPCVOID)(address + 3), &prev_right, 1, NULL);
    ReadProcessMemory(processHandler, (LPCVOID)(address + 4), &prev_jump, 1, NULL);
    address += 5;

    for (int current_frame = 1; current_frame <= MAX_RECORDING_LENGTH; current_frame++) {
        //ReadProcessMemory(processHandler, (LPCVOID)address, &up, 1, NULL);
        //ReadProcessMemory(processHandler, (LPCVOID)(address + 1), &down, 1, NULL);
        ReadProcessMemory(processHandler, (LPCVOID)(address + 2), &current_left, 1, NULL);
        ReadProcessMemory(processHandler, (LPCVOID)(address + 3), &current_right, 1, NULL);
        ReadProcessMemory(processHandler, (LPCVOID)(address + 4), &current_jump, 1, NULL);
        address += 5;

        // repeated input for consecutive frames
        if (!(prev_left == current_left && prev_right == current_right && prev_jump == current_jump) || current_frame == MAX_RECORDING_LENGTH) {
            run_file << "writeInputsByFrames(processHandle, " << frames << ", " << prev_left << ", " << prev_right << ", " << prev_jump << ");\n";
            frames = 1;
        }
        // if not repeated, write the command to a file and continue
        else {
            frames++;
        }

        prev_left = current_left;
        prev_right = current_right;
        prev_jump = current_jump;
    }



    run_file.close();

    std::cout << "Run extracted.\n";

    // when recording is finished, reset the memory address
    address = initial_address;
}

// Overwrites "frames" frames with the given inputs (left, right, jump).
// For reference: 120 frames = 1 second
void writeInputsByFrames(HANDLE processHandler, int frames, bool left, bool right, bool jump) {
    if (!(address - initial_address >= 70000)) {//safety check to not overwrite not pogoTAS stuff
        // Write inputs for the current TAS frame
        for (int i = 0; i < frames; i++) {
            //WriteProcessMemory(processHandler, (LPVOID)address, &up, 1, NULL);
            //WriteProcessMemory(processHandler, (LPVOID)(address + 1), &down, 1, NULL);
            WriteProcessMemory(processHandler, (LPVOID)(address + 2), &left, 1, NULL);
            WriteProcessMemory(processHandler, (LPVOID)(address + 3), &right, 1, NULL);
            WriteProcessMemory(processHandler, (LPVOID)(address + 4), &jump, 1, NULL);
            address += 5; //go to next frame
        }
        std::cout << "wrote " << frames << " frames with the inputs: " << parseInputs(left, right, jump) << std::endl;
    }
}


int main()
{
    if (initial_address == NULL) {
        std::cout << "Remember to get the starting tas block address before running the project.\n";
        return 0;
    }

    std::cout << "Started pogotas.\n";

    // Get handler for Pogostuck process
    HWND hGameWindow = FindWindowA(NULL, "POGOSTUCK"); //NOT CASE SENSITIVE
    if (hGameWindow == NULL) {
        std::cout << "Window not found, make sure the name is correctly written and that the process is running\n";
        return 0;
    }
    DWORD pID = NULL;
    GetWindowThreadProcessId(hGameWindow, &pID);

    HANDLE processHandle = NULL;
    processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);
    //

    // Create TAS Pogostuck run

    // Example --> "For 60 frames, execute the following inputs: left = 0, right = 1, jump = 1 --> "rotate right jumping""
    //writeInputsByFrames(processHandle, 60, 0, 1, 1);
    
    // To record a run, uncomment the following line, it will output the run to pogoTAScpp/TASrun.txt
    //extractRun(processHandle);

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

