#include "processMessages.h"
#include <string>
#include "incident.h"
#include <iostream>
#include <dpp/dpp.h>
#include <regex>

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
		municipalityToken = municipalityToken.substr(4, municipalityToken.find("NAT:"));
		natureToken = inputMsg.substr(inputMsg.find(natDelim)-4);
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
	discordMsg = "## " + iType + " - " + natureToken + " - " + municipalityToken + "\n "; // create header of incident type
	discordMsg = discordMsg + "```txt\n" + inputMsg + "\n```";

	return discordMsg;
}

string processDelCo(const string& rawInputMsg){
	// Delims
	string locationDelim = "LOC:";
	string crossSt1Delim = "X1:";
	string crossSt2Delim = "X2:";
	string natureDelim = "Nature:";
	string timeDelim = "TIME:";
	string IncIdDelim = "Inc:";
	string beatDelim = "Beat:"; // map/Box
	string trucksDelim = "Disp:";
	string eotDelim = "<EOM>";
	string discordMsg;	
	string inputMsg = rawInputMsg.substr(0, rawInputMsg.find(eotDelim));

	try{
		discordMsg = "```txt\n" + inputMsg + "\n ```";
	}
	catch(...) {
		return inputMsg;
	}
	return discordMsg;
}

dpp::embed processDelCoEmbed(const string& rawInputMsg){
    /* Parse for needed info */
    // Delims
	string locationDelim = "LOC:";
	string crossSt1Delim = "X1:";
	string crossSt2Delim = "X2:";
	string natureDelim = "Nature:";
	string timeDelim = "TIME:";
	string IncIdDelim = "Inc:";
	string beatDelim = "Beat:"; // map/Box
	string trucksDelim = "Disp:";
	string eotDelim = "<EOM>";
    string PocsagAddressDelim = "Address:";

    /* Create embed values */

    /*---------- POCSAG Address ---------*/
    /* Extract pocsag address */
    string address;
    // Regular expression to match the address number after "Address:"
    std::regex address_regex("Address:\\s*(\\d+)");
    std::smatch match;

    // Search for the address number
    if (std::regex_search(rawInputMsg, match, address_regex)) {
        if (match.size() > 1) { // Ensure there is a captured group
            std::string address_number = match[1];
            address = address_number;
            std::cout << "Extracted address number: " << address_number << std::endl;
        } else {
            std::cerr << "Address number not found in the string." << std::endl;
        }
    } else {
        std::cerr << "Address number not found in the string." << std::endl;
    }
    /*--------- Incident Nature ---------*/
    /* Regex for getting incident nature */
    // Regular expression to match the nature after "Nature:"
    std::regex nature_regex("Nature:([^:]+?)[ \\t\\r\\n\\f\\v]+TIME:");
    std::string nature;
    // Search for the nature
    if (std::regex_search(rawInputMsg, match, nature_regex)) {
        if (match.size() > 1) { // Ensure there is a captured group
            nature = match[1];
            std::cout << "Extracted nature: " << nature << std::endl;
        } else {
            std::cerr << "Nature not found in the string." << std::endl;
        }
    } else {
        std::cerr << "Nature not found in the string." << std::endl;
    }

    /*---------- Color ---------*/
    uint32_t color;
    if (nature.find("EMS") != std::string::npos) {
        color = dpp::colors::blue;
    }
    else
    {
        color = dpp::colors::red;
    }

    /*--------- Description ----------*/
    // Regular expression to extract everything up until "Disp:"
    std::regex pre_disp_regex("^(.*?)(?=[ \\t\\r\\n\\f\\v]+Disp:)");
    string description;

    // Search for the content before "Disp:"
    if (std::regex_search(rawInputMsg, match, pre_disp_regex)) {
        if (match.size() > 1) { // Ensure there is a captured group
            description = match[1];
        } else {
            std::cerr << "Content up to 'Disp:' not found in the string." << std::endl;
        }
    } else {
        std::cerr << "Content up to 'Disp:' not found in the string." << std::endl;
    }

    /*--------- Extract Trucks ----------*/
    string trucks;
    // Regular expression to extract the content after "Disp:" and before "<EOT>"
    std::regex post_disp_regex("Disp:\\s*(.*?)\\s*<EOT>");

    // Search for the content after "Disp:" and before "<EOT>"
    if (std::regex_search(rawInputMsg, match, post_disp_regex)) {
        if (match.size() > 1) { // Ensure there is a captured group
            trucks = match[1];
        } else {
            std::cerr << "Content after 'Disp:' and before '<EOT>' not found in the string." << std::endl;
        }
    } else {
        std::cerr << "Content after 'Disp:' and before '<EOT>' not found in the string." << std::endl;
    }

    /*---------- Get Incident Number ----------*/
    string incidentNum;
     // Regular expression to extract the incident number after "Inc:"
    std::regex inc_regex("Inc:\\s*(\\S+)");

    // Search for the incident number
    if (std::regex_search(rawInputMsg, match, inc_regex)) {
        if (match.size() > 1) { // Ensure there is a captured group
            incidentNum = match[1];
        } else {
            std::cerr << "Incident number not found in the string." << std::endl;
        }
    } else {
        std::cerr << "Incident number not found in the string." << std::endl;
    }

    /*---------- Get author token (station name) ----------*/
    string stationName = "Undetermined";
    if (delCoCapCodes.find(address) != delCoCapCodes.end()) {
        stationName = delCoCapCodes.at(address)[1];
    }

    /* Create an embed */
    dpp::embed embed = dpp::embed()
        .set_color(color)
        .set_title(nature + " (" + incidentNum + ")")
        .set_url("")
        .set_author(stationName, "", "")
        .set_description(description)
        .add_field(
            "Trucks",
            trucks,
            true
        )
        .set_footer(
            dpp::embed_footer()
            .set_text("DelCo Dispatch")
            //.set_icon("data:image/png;base64,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")
        )
        .set_timestamp(time(0));

    return embed;
}
