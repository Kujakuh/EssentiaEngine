#ifndef ENUMS_H
#define ENUMS_H

enum DATA_SOURCE
{
	FILE_PATH,
	STR_DATA
};
enum SH_TYPE
{
	VERTEX,
	FRAGMENT,
	GEOMETRY
};
enum TEX_TYPE
{
	TEX_DIFF,
	TEX_SPEC,
	TEX_NORM,
	TEX_HEIGHT,
	TEX_OPA,
	TEX_ALBEDO,
	TEX_METALLIC,
	TEX_ROUGHNESS,
	TEX_AO,
	TEX_EMISSIVE,
	TEX_CUBEMAP
};
enum FILTERS
{
	WRAP_S,
	WRAP_R,
	WRAP_T,
	MIN_F,
	MAG_F
};
enum CAMERA_DIR
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN,
};
enum KEY_STATE
{
	IDLE = 0,
	PRESSED = 1,
	RELEASED = 2,
	HELD = 3
};
enum SORT_ORDER
{
	Z_ORDER,
	Y_ORDER
};
enum RENDER_MODE
{
	PONG_SHADING,
	PBR
};
enum LightType : int 
{
	Point = 0,
	Directional = 1,
	Spot = 2
};

#endif // !ENUMS_H