#ifndef _STARWAR_UTIL_H_
#define _STARWAR_UTIL_H_

//Use strict type checking
#define STRICT
//Remove redundant window api
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <mmsystem.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <assert.h>
#include <math.h>
#include <vector>

const float CAMERA_PITCH_LIMITATION = 0.3f;

//Vertex containing position, normal and texture.
struct PositionNormalTexVertex
{
	//Position of vertex in 3D space
	float _x, _y, _z;
	//Normal for lighting calculations
	float _nx, _ny, _nz;
	//Position in texture
	float _tx, _ty;

	//Format of vertex
	static const DWORD FVF = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1);

	PositionNormalTexVertex(float x, float y, float z,
		float nx, float ny, float nz,
		float tx, float ty) : _x(x), _y(y), _z(z), 
		_nx(nx), _ny(ny), _nz(nz), _tx(tx), _ty(ty){}
};

//Vertex containing position, normal, diffuse color and texture.
struct PositionNormalDiffUseColorTexVertex
{
	//Position of vertex in 3D space
	float _x, _y, _z;
	//Normal for lighting calculations
	float _nx, _ny, _nz;
	//Color 
	D3DCOLOR _color;
	//Position in texture
	float _tx, _ty;

	//Format of vertex
	static const DWORD FVF = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1);

	PositionNormalDiffUseColorTexVertex(float x, float y, float z,
		float nx, float ny, float nz, D3DCOLOR color,
		float tx, float ty) : _x(x), _y(y), _z(z), 
		_nx(nx), _ny(ny), _nz(nz), _color(color), _tx(tx), _ty(ty){}
};

//Vertex containing position, normal, specular color and texture.
struct PositionNormalSpecularColorTexVertex
{
	//Position of vertex in 3D space
	float _x, _y, _z;
	//Normal for lighting calculations
	float _nx, _ny, _nz;
	//Color 
	D3DCOLOR _color;
	//Position in texture
	float _tx, _ty;

	//Format of vertex
	static const DWORD FVF = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_SPECULAR | D3DFVF_TEX1);

	PositionNormalSpecularColorTexVertex(float x, float y, float z,
		float nx, float ny, float nz, D3DCOLOR color,
		float tx, float ty) : _x(x), _y(y), _z(z), 
		_nx(nx), _ny(ny), _nz(nz), _color(color), _tx(tx), _ty(ty){}
};

class Util
{
public:
	static inline void Clip(float low, float high, float &number)
	{
		number = (number > low) ? number : low;
		number = (number < high) ? number : high;	
	}
};

#endif