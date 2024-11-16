#include <string>

// Houses functions to process POCSAG msgs in various formats


string processMontCo(const string& inputMsg){
	// First, start msg after 'Alpha:' and end msg at end of transmission (<EOT>)
	string alphaDelim = "Alpha:";
	string eotDelim = "<EOT>";


