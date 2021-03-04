# Pogostuck TAS Client

WIP cpp client to create pogostuck TAS runs.

# IMPORTANT

Don't run this with a wrong memory address or without Pogostuck running.
This program works overwritting memory therefore you have to be careful with what you do.

# Requirements

* Cheat engine (or something to search for memory addresses)
* Pogostuck TAS client.
* Visual Studio (with console apps plugin)

# Usage

1. Find the memory address following the guide at the end.
2. In utils.cpp replace the variable initial_address (line 55) with the value found at 1.
3. At the end of the main function, below the "Create TAS Pogostuck run" start creating the run, there's already an example for reference.
4. Once you have your run, execute the program and test it by going to playback mode on the pogostuck TAS client. 
Important, if you accidentaly go to record mode, your run will be overwritten and you will have to execute the program again.

### Finding initial tas block memory

Using cheat engine on the pogo TAS client, search for the value 111333555, it should return 2 addresses, pick both, now using HEX operations:
* Add 71CDD to the lower one.
* Substract 57E63 to the higher one.

If both results are the same, you found the address (make sure to replace it in hex format --> 0x[VALUE]).


