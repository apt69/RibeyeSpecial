// RibeyeSpecial.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <Windows.h>
#include <iostream>
#include "tclap/CmdLine.h"
#include "Helper.h"

// Struct that will determine the configuration to run this
struct Coconut
{
	int AES_Key = 0;
	bool sleep = false;
	int sleepTime = 5000;
	bool prime = false;
	bool mouse = false;
	bool acceleratedsleep = false;
	bool debugger = false;
	bool ram = false;
	bool cpu_core = true;
	int szData = 0;
	const char * opcodes = 0;
} coconut;


int main(int argc, char** argv)
{
	try {

		// Define the command line object, and insert a message
		// that describes the program. The "Command description message"
		// is printed last in the help text. The second argument is the
		// delimiter (usually space) and the last one is the version number.
		// The CmdLine object parses the argv array based on the Arg objects
		// that it contains.


		//                     MESSAGE                 DELIMITER  VERSION
		TCLAP::CmdLine cmd(R"(Shoutout to APT69 and the brothers that are in it. https://twitter.com/TomahawkApt69 )", ' ', "0.1");

		TCLAP::ValueArg<std::string> filepath("f", "file", "Path to file to execute", true, "C:\\Users\\Dev\\Desktop\\malware.exe", "string");
		TCLAP::ValueArg<int> sleeptime("t", "sleeptime", "How long to sleep for if -s is enabled", false , 5000, "integer");

		// Add the argument nameArg to the CmdLine object. The CmdLine object
		// uses this Arg to parse the command line.
		cmd.add(filepath);
		cmd.add(sleeptime);

		TCLAP::SwitchArg bSleep("s", "sleep", "Sleep to evade sandbox", cmd, false);
		TCLAP::SwitchArg bPrime("p", "prime", "Prime calculation to evade sandbox", cmd, false);
		TCLAP::SwitchArg bMouse("m", "mouse", "Check mouse movement, evade sandbox", cmd, false);
		TCLAP::SwitchArg bAccel("a", "accel", "Check if sleep is being accelerated, evade sandbox", cmd, false);
		TCLAP::SwitchArg bDebugger("d", "debugger", "Check if we are being debugged, anti debug", cmd, false);
		TCLAP::SwitchArg bRam("r", "ram", "Check RAM size, anti VM", cmd, false);
		TCLAP::SwitchArg bCore("c", "core", "Check CPU core count, anti VM", cmd, false);

		// Parse the argv array.
		cmd.parse(argc, argv);

		coconut.sleep = bSleep.getValue();
		coconut.sleepTime = sleeptime.getValue();
		coconut.prime = bPrime.getValue();
		coconut.mouse = bMouse.getValue();
		coconut.acceleratedsleep = bAccel.getValue();
		coconut.debugger = bDebugger.getValue();
		coconut.ram = bRam.getValue();
		coconut.cpu_core = bCore.getValue();

	}
	catch (TCLAP::ArgException &e)  // catch any exceptions
	{
		std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
	}

}