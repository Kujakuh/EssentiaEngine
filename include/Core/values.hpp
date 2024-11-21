#ifndef VALUES_H
#define VALUES_H

// SYS VALUES
#define LOC_POSITION 0
#define LOC_TEX_COORD 1
#define LOC_NORMAL 2

namespace Essentia 
{
	static const char* DS_TOSTRING[] = { "FILE_PATH", "STR_DATA" };
	static const char* ST_TOSTRING[] = { "VERTEX", "FRAGMENT", "GEOMETRY" };

	// USER VALUES
	static const int MAX_BONE_INFLUENCE = 4;
}

#endif // !VALUES_H