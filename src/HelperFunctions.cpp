#include "HelperFunctions.h"

bool fileExists(string filename)
{
	struct stat buf;

	// returns -1 if file doesn't exist
	return stat(filename.c_str(), &buf) != -1;
}
