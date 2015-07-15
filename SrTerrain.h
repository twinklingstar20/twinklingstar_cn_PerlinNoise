/************************************************************************		
\link	www.twinklingstar.cn
\author twinklingstar
\file	SrTerrain.h
\date	2015/05/20
****************************************************************************/
#ifndef SR_TERRAIN_H_
#define SR_TERRAIN_H_

#include <math.h>
#include <stdlib.h>

#include "SrClassicalPerlinNoise.h"
#include "SrModel.h"


//Terrain parameter.
struct SrTerrainParam
{
	float		mXSize;
	float		mYSize;
	float		mZSize;
	D3DXVECTOR3	mCenter;
	int			mNumRows;
	int			mNumCols;
	float		mF;
	float		mD;
	float		mErode;

};

/**
\brief Generate terrain based on Perlin noise.
 */
class SrTerrain:public SrModel
{
private:
	struct TERRAINVERTEX
	{
		D3DXVECTOR3   mPos;
		D3DXVECTOR3   mNormal;
		D3DXVECTOR2   mTexture;
	};

	#define D3DFVF_TERRAIN_VERTEX (D3DFVF_XYZ | D3DFVF_NORMAL| D3DFVF_TEX1)

public:
	SrTerrain(const TCHAR* file, const SrTerrainParam& para);

	virtual bool init(IDirect3DDevice9* pd3dDevice);

	virtual void render(IDirect3DDevice9* pd3dDevice) const;

	~SrTerrain();

protected:
	void calHeightMap(float *h, D3DXVECTOR3* m);
	/**
	\brief Calculate the normal of each vertex.
	 */
	void calNormal(D3DXVECTOR3* m, D3DXVECTOR3* norm);
	/**
	\brief Calculate the texture coordinates of each vertex.
	 */
	void calTextureCoord(D3DXVECTOR2* coords);

	void calVertes(TERRAINVERTEX* v, D3DXVECTOR3 * m, D3DXVECTOR3* n, D3DXVECTOR2* coords);

	void buildMesh(IDirect3DDevice9* pd3dDevice);
	/**
	\brief Calculate the height.
	 */
	void calHeight(float* height);

	void addPerlinNoise(float* height, float f, float base = 10.0f);

	void perturb(float* height, float f, float d);

	void erode(float * height, float smoothness);

	void smoothen(float * height);
private:
	SrClassicalPerlinNoise3D*	mPerlin;
	LPDIRECT3DVERTEXBUFFER9		mVB;
	LPDIRECT3DTEXTURE9			mTexture;
	SrTerrainParam				mParam;
	TCHAR						mTextureFile[MAX_PATH];

};

#endif