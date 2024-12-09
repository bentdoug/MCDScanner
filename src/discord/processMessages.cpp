#include "processMessages.h"
#include <string>
#include "incident.h"
#include <iostream>

using namespace std;

// Houses functions to process POCSAG msgs in various formats

string processMontCo(const string& rawInputMsg){
	// First, start msg after 'Alpha:' and end msg at end of transmission (<EOT>)
	string alphaDelim = "Alpha:";
	string eotDelim = "<EOT>";
	string munDelim = "MUN:";
	string natDelim = "NAT:";
	string mapboxDelim = "MAP/BOX:";
	string notesDelim = "NOTES:";
	string trucksDelim = "TRUCKS:";

	string iType;
	string locationToken;
	string municipalityToken;
	string natureToken;
	string notesToken;

	string inputMsg = rawInputMsg.substr(0, rawInputMsg.find(eotDelim));

	try
	{
		if (inputMsg.find("I#F") != std::string::npos) // if this exists, it should be a fire call
		{
			iType = "FIRE";
		}
		else if (inputMsg.find("I#E") != std::string::npos) // if this exists, it should be an ems call
		{
			iType = "EMS";
		}
		else
		{
			iType = "OTHER";
		}

		locationToken = inputMsg.substr(0, inputMsg.find(munDelim));
		municipalityToken = inputMsg.substr(inputMsg.find(munDelim), inputMsg.find(natDelim));
		natureToken = inputMsg.substr(inputMsg.find(natDelim));
		natureToken = natureToken.substr(4, natureToken.find(',')-4);
		notesToken = inputMsg.substr(inputMsg.find(notesDelim), inputMsg.find(eotDelim));
		try // try to determine assigned trucks. This may not work if the notes are super long
		{
			std::string trucksToken = inputMsg.substr(inputMsg.find(trucksDelim), inputMsg.find(eotDelim));
		}
		catch(...)
		{
			std::cerr << "Unable to determind assigned trucks!\n";
		}
		

	}
	catch(...)
	{
		std::cerr << "Something went wrong with msg processing!\n";
		return inputMsg;
	}
	
	// build discord msg
	std::string discordMsg;
	std::cout << "\n\nNature: " << natureToken << "\n\n\n";
	discordMsg = "## " + iType + " - " + natureToken + "\n "; // create header of incident type
	discordMsg = discordMsg + municipalityToken + " : " + locationToken + "\n";
	discordMsg = discordMsg + notesToken; // add notes to the msg

	return discordMsg;
}

string processDelCo(const string& inputMsg){
	return "Not yet implemented";
}