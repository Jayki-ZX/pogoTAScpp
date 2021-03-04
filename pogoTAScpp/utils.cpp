// pogoTAScpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>
#include <fstream>

// Takes the currently recorded run on memory and stores it in a file with the following format:
// * 20 frames; up, down 
// * 14 frames; down, left
// * x frames; [inputs]
// ... etc ...
void extractRun() {
    /* code for reference
    std::ofstream myfile;
    myfile.open("example.txt");
    myfile << "Writing this to a file.\n";
    myfile.close();
    
    logic
    current frame = 0
    frames = 1
    if frames + current_frame >= 28800 stop recording and do the last record
    else
    if inputs of current_frame == inputs of current_frame+1
        current_frame++
        frames++
    else
        write "[frames] frames; parseInput(inputofcurrentframe)

    */
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

DWORD initial_address = {  }; // value found following GUIDE.txt
DWORD address = initial_address; 

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
    writeInputsByFrames(processHandle, 60, 0, 1, 0);

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

