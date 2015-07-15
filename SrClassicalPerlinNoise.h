/************************************************************************		
\link	www.twinklingstar.cn
\author twinklingstar
\file	SrClassicalPerlinNoise.h
\date	2015/07/15
****************************************************************************/
#ifndef SR_CLASSIC_PERLIN_NOISE_H_
#define SR_CLASSIC_PERLIN_NOISE_H_

#define SR_PERLIN_N			0x100
#define SR_PERLIN_MASK		0xff
#define SR_PERLIN_PI		3.141592653589793

typedef float			real;
typedef unsigned char	ubyte;


class SrPerlinNoise
{
public:
	SrPerlinNoise(){}
protected:
	real	ease(real x);

	int		permutate(int x);

	real	lerp(real t, real value0, real value1);
protected:
	static ubyte mP[SR_PERLIN_N];
};


class SrClassicalPerlinNoise1D:public SrPerlinNoise
{
public:
	static SrClassicalPerlinNoise1D* create();

	real	noise(real x);
private:
	int		index(int ix);

	real	lattice(int ix, real fx);

	static void init();

	SrClassicalPerlinNoise1D(){}
	SrClassicalPerlinNoise1D(const SrClassicalPerlinNoise1D&);
	SrClassicalPerlinNoise1D& operator = (const SrClassicalPerlinNoise1D&);
private:
	static real mG[SR_PERLIN_N];
	static bool	mIsInit;
};

class SrClassicalPerlinNoise2D:public SrPerlinNoise
{
public:
	static SrClassicalPerlinNoise2D* create();

	real noise(real x, real y);
private:
	int		index(int ix, int iy);

	real	lattice(int ix, int iy, real fx, real fy);

	static void init();

	SrClassicalPerlinNoise2D(){}
	SrClassicalPerlinNoise2D(const SrClassicalPerlinNoise2D&);
	SrClassicalPerlinNoise2D& operator = (const SrClassicalPerlinNoise2D&);
private:
	static real mG[SR_PERLIN_N][2];
	static bool	mIsInit;
};

class SrClassicalPerlinNoise3D:public SrPerlinNoise
{
public:
	static SrClassicalPerlinNoise3D* create();

	real	noise(real x, real y, real z);
;
private:
	int		index(int ix, int iy, int iz);

	real	lattice(int ix, int iy, int iz, float fx, float fy, float fz);

	static void init();

	SrClassicalPerlinNoise3D(){}
	SrClassicalPerlinNoise3D(const SrClassicalPerlinNoise3D&);
	SrClassicalPerlinNoise3D& operator = (const SrClassicalPerlinNoise3D&);
private:
	static real mG[SR_PERLIN_N][3];
	static bool	mIsInit;
};


#endif