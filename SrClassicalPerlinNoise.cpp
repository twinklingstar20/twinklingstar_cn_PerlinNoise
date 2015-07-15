/************************************************************************		
\link	www.twinklingstar.cn
\author twinklingstar
\file	SrClassicalPerlinNoise.cpp
\date	2015/07/15
****************************************************************************/
#include "DXUT.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "SrClassicalPerlinNoise.h"


/************************************************************************/
/*                     class SrPerlinNoise                              */
/************************************************************************/

real SrPerlinNoise::ease(real x)
{
	return (real)(x * x * (3 - 2 * x));
}

int SrPerlinNoise::permutate(int x)
{
	return mP[x & SR_PERLIN_MASK];
}

real SrPerlinNoise::lerp(real t, real value0, real value1)
{
	return (real)(value0 + t * (value1 - value0));
}

ubyte SrPerlinNoise::mP[SR_PERLIN_N] = {151,160,137,91,90,15,
	131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
	190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
	88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
	77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
	102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
	135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
	5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
	223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
	129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
	251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
	49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
	138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
};

/************************************************************************/
/*               class SrClassicalPerlinNoise1D                         */
/************************************************************************/
SrClassicalPerlinNoise1D* SrClassicalPerlinNoise1D::create()
{
	if(!mIsInit)
	{
		mIsInit = true;
		init();
	}
	return (new SrClassicalPerlinNoise1D());
}

real SrClassicalPerlinNoise1D::noise(real x)
{
	int x0 = (int)floor(x);
	real fx0 = x - x0;
	real fx1 = fx0 - 1;

	real sx = ease(fx0);

	real u = lattice(x0    , fx0);
	real v = lattice(x0 + 1, fx1);

	return lerp(sx, u, v);
}
int SrClassicalPerlinNoise1D::index(int ix)
{
	return permutate(ix);
}

real SrClassicalPerlinNoise1D::lattice(int ix, real fx)
{
	int indx = index(ix);
	return (real)(mG[indx] * fx );
}

void SrClassicalPerlinNoise1D::init()
{
	int i;
	for (i = 0 ; i < SR_PERLIN_N ; i++) 
	{
		mG[i] = (float)((rand() % (SR_PERLIN_N + SR_PERLIN_N)) - SR_PERLIN_N) / SR_PERLIN_N;
	}
}

bool SrClassicalPerlinNoise1D::mIsInit = false;

real SrClassicalPerlinNoise1D::mG[SR_PERLIN_N] = {};

/************************************************************************/
/*              class SrClassicalPerlinNoise2D                          */
/************************************************************************/

SrClassicalPerlinNoise2D* SrClassicalPerlinNoise2D::create()
{
	if(!mIsInit)
	{
		mIsInit = true;
		init();
	}
	return (new SrClassicalPerlinNoise2D());
}

real SrClassicalPerlinNoise2D::noise(real x, real y)
{
	int x0 = (int)floor(x);
	real fx0 = x - x0;
	real fx1 = fx0 - 1;
	real sx = ease(fx0);

	int y0 = (int)floor(y);
	real fy0 = y - y0;
	real fy1 = fy0 - 1;
	real sy = ease(fy0);

	real s = lattice(x0	   ,y0	  , fx0, fy0);
	real t = lattice(x0 + 1,y0	  , fx1, fy0);
	real u = lattice(x0	   ,y0 + 1, fx0, fy1);
	real v = lattice(x0 + 1,y0 + 1, fx1, fy1);

	real a = lerp(sx, s, t);
	real b = lerp(sx, u, v);
	return lerp(sy, a, b);
}

int SrClassicalPerlinNoise2D::index(int ix, int iy)
{
	return permutate(ix + permutate(iy));
}

real SrClassicalPerlinNoise2D::lattice(int ix, int iy, real fx, real fy)
{
	int indx = index(ix, iy);
	return (real)(mG[indx][0] * fx + mG[indx][1] * fy);
}



void SrClassicalPerlinNoise2D::init()
{
	int i, j;
	real s;
	for (i = 0 ; i < SR_PERLIN_N ; i++) 
	{
		for (j = 0 ; j < 2 ; j++)
			mG[i][j] = (float)((rand() % (SR_PERLIN_N + SR_PERLIN_N)) - SR_PERLIN_N) / SR_PERLIN_N;

		s = sqrt((real)(mG[i][0] * mG[i][0] + mG[i][1] * mG[i][1]));
		mG[i][0] = mG[i][0] / s;
		mG[i][1] = mG[i][1] / s;
	}
}

bool SrClassicalPerlinNoise2D::mIsInit = false;

real SrClassicalPerlinNoise2D::mG[SR_PERLIN_N][2] = {};

/************************************************************************/
/*                class SrClassicalPerlinNoise3D                        */
/************************************************************************/

SrClassicalPerlinNoise3D* SrClassicalPerlinNoise3D::create()
{
	if(!mIsInit)
	{
		mIsInit = true;
		init();
	}
	return (new SrClassicalPerlinNoise3D());
}

real SrClassicalPerlinNoise3D::noise(real x, real y, real z)
{
	int x0 = (int)floor(x);
	real fx0 = x - x0;
	real fx1 = fx0 - 1;
	real wx = ease(fx0);

	int y0 = (int)floor(y);
	real fy0 = y - y0;
	real fy1 = fy0 - 1;
	real wy = ease(fy0);

	int z0 = (int)floor(z);
	real fz0 = z - z0;
	real fz1 = fz0 - 1;
	real wz = ease(fz0);

	real vx0 = lattice(x0	 , y0, z0, fx0, fy0, fz0);
	real vx1 = lattice(x0 + 1, y0, z0, fx1, fy0, fz0);
	real vy0 = lerp(wx, vx0, vx1);

	vx0 = lattice(x0	, y0 + 1, z0, fx0, fy1, fz0);
	vx1 = lattice(x0 + 1, y0 + 1, z0, fx1, fy1, fz0);
	real vy1 = lerp(wx, vx0, vx1);

	real vz0 = lerp(wy, vy0, vy1);

	vx0 = lattice(x0	, y0, z0 + 1, fx0, fy0, fz1);
	vx1 = lattice(x0 + 1, y0, z0 + 1, fx1, fy0, fz1);
	vy0 = lerp(wx, vx0, vx1);

	vx0 = lattice(x0	, y0 + 1, z0 + 1, fx0, fy1, fz1);
	vx1 = lattice(x0 + 1, y0 + 1, z0 + 1, fx1, fy1, fz1);
	vy1 = lerp(wx, vx0, vx1);

	real vz1 = lerp(wy, vy0, vy1);

	return lerp(wz, vz0, vz1);
}

int SrClassicalPerlinNoise3D::index(int ix, int iy, int iz)
{
	return permutate(ix + permutate(iy + permutate(iz)));
}

real SrClassicalPerlinNoise3D::lattice(int ix, int iy, int iz, float fx, float fy, float fz)
{
	int indx = index(ix, iy, iz);
	return (real)(mG[indx][0] * fx + mG[indx][1] * fy + mG[indx][2] * fz);
}

void SrClassicalPerlinNoise3D::init()
{
	int i;
	for (i = 0 ; i < SR_PERLIN_N ; i++) 
	{
		real z = 1.0f - 2.0f * ((real)::rand()/((real)RAND_MAX+1));
		real r = (real)sqrt(1.0f - z * z);
		real theta = 2 * (real)SR_PERLIN_PI * ((real)::rand()/((real)RAND_MAX+1));

		mG[i][0]	= r * (real)cos(theta);
		mG[i][1]	= r * (real)sin(theta);
		mG[i][2]	= z;
	}
}


bool SrClassicalPerlinNoise3D::mIsInit = false;

real SrClassicalPerlinNoise3D::mG[SR_PERLIN_N][3] = {};

