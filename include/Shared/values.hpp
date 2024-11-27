#ifndef VALUES_H
#define VALUES_H

// SYS VALUES
#define LOC_POSITION 0
#define LOC_NORMAL 1
#define LOC_TEX_COORD 2
#define LOC_TANGENT 3
#define LOC_BITANGENT 4
#define LOC_IDS 5
#define LOC_WEIGHTS 6

namespace Essentia 
{
	static const char* DS_TOSTRING[] = { "FILE_PATH", "STR_DATA" };
	static const char* ST_TOSTRING[] = { "VERTEX", "FRAGMENT", "GEOMETRY" };
	static const char* IE_TOSTRING[] = { "SYS_START" };
	static const char* TT_TOSTRING[] = { "TEX_DIFF", "TEX_SPEC", "TEX_NORM", "TEX_HEIGHT", "TEX_CUBEMAP" };
	static const char* F_TOSTRING[]  = { "WRAP_S", "WRAP_R", "WRAP_T", "MIN_F", "MAG_F" };

	static const int MAX_BONE_INFLUENCE = 4;
}

#endif // !VALUES_H