/************************************************************************		
\link	www.twinklingstar.cn
\author twinklingstar
\file	SrTerrain.cpp
\date	2015/05/20
****************************************************************************/
#include "DXUT.h"
#include "SrTerrain.h"



SrTerrain::SrTerrain(const TCHAR* file,const SrTerrainParam& para):SrModel()
{
	mPerlin = SrClassicalPerlinNoise3D::create();
	mTexture         = NULL;
	mParam	= para;
	float x = mParam.mCenter.x - mParam.mXSize * 0.5f;
	float z = mParam.mCenter.z - mParam.mZSize * 0.5f;
	float y = mParam.mCenter.y - mParam.mYSize * 0.5f;
	D3DXMatrixTranslation(&mMatrix, x, y, z);

	lstrcpy(mTextureFile, file);

}

bool SrTerrain::init(IDirect3DDevice9* pd3dDevice)
{
	buildMesh(pd3dDevice);
	D3DXCreateTextureFromFile( pd3dDevice,mTextureFile, &mTexture);

	return true;
}

void SrTerrain::calHeightMap(float *h, D3DXVECTOR3* m)
{
	int r, c, indx = 0;

	float cstep = (float)mParam.mXSize / (float)mParam.mNumCols;
	float rstep = (float)mParam.mZSize / (float)mParam.mNumRows;

	for(r = 0; r < mParam.mNumRows ; r += 1)
	{
		for(c = 0 ; c < mParam.mNumCols ; c += 1)
		{
			m[indx].x = c * cstep;
			m[indx].y = h[indx];
			m[indx].z = r * rstep;

			indx += 1;
		}
	}
}

void SrTerrain::calNormal(D3DXVECTOR3* m, D3DXVECTOR3* norm)
{
	int r, c, count = 0;
	float len;
	D3DXVECTOR3 v1, v2, v3, s1, s2, n, sum;
	D3DXVECTOR3* normals = new D3DXVECTOR3[(mParam.mNumRows - 1) * (mParam.mNumCols - 1)];

	for(r = 0; r < mParam.mNumRows - 1 ; r += 1)
	{
		for(c = 0 ; c < mParam.mNumCols - 1 ; c += 1)
		{
			v1 = m[(r * mParam.mNumCols) + c];
			v2 = m[(r * mParam.mNumCols) + c + 1];
			v3 = m[((r + 1) * mParam.mNumCols) + c];

			s1 = v3 - v1;
			s2 = v2 - v1;

			n.x = (s1.y * s2.z) - (s1.z * s2.y);
			n.y = (s1.z * s2.x) - (s1.x * s2.z);
			n.z = (s1.x * s2.y) - (s1.y * s2.x);

			normals[r * (mParam.mNumCols - 1) + c] = n;
		}
	}

	for(r = 0; r < mParam.mNumRows ; r += 1)
	{
		for(c = 0 ; c < mParam.mNumCols ; c += 1)
		{
			sum = D3DXVECTOR3(0,0,0);
			count = 0;
			if(((r - 1) >= 0) && (c - 1) >= 0)
			{
				sum += normals[(r - 1) * (mParam.mNumCols - 1) + c - 1];
				count += 1;
			}
			if((r < (mParam.mNumRows - 1)) && ((c - 1) >= 0))
			{
				sum += normals[r * (mParam.mNumCols - 1) + c - 1];
				count += 1;
			}
			if(((r - 1) >= 0) && (c < (mParam.mNumCols - 1)))
			{
				sum += normals[(r - 1) * (mParam.mNumCols - 1) + c];
				count += 1;
			}
			if((r < (mParam.mNumRows - 1)) && (c < (mParam.mNumCols - 1)))
			{
				sum += normals[r * (mParam.mNumCols - 1) + c];
				count += 1;
			}
			sum /= (float) count;
			len = sqrt(sum.x * sum.x + sum.y * sum.y + sum.z * sum.z);
			norm[r * mParam.mNumCols + c]= (sum / len);
		}
	}
	delete []normals;
}


void SrTerrain::calTextureCoord(D3DXVECTOR2* coords)
{
	int r, c, indx = 0;

	for(r = 0; r < mParam.mNumRows ; r += 1)
	{
		for(c = 0 ; c < mParam.mNumCols ; c += 1)
		{
			coords[indx] = D3DXVECTOR2((float)c/(float)mParam.mNumCols * 10.0f, (1 - (float)r/(float)mParam.mNumRows) * 10.0f); 
			indx += 1;
		}
	}
}

void SrTerrain::calVertes(TERRAINVERTEX* v, D3DXVECTOR3 * m, D3DXVECTOR3* n, D3DXVECTOR2* coords)
{
	int r, c, indx1, indx2, indx3, indx4;
	int indx = 0;
	for( r = 0; r < mParam.mNumRows - 1; r += 1)
	{
		for(c = 0 ; c < mParam.mNumCols - 1 ; c += 1)
		{
			indx1 = r * mParam.mNumCols + c;
			indx2 = (r + 1) * mParam.mNumCols + c;
			indx3 = r * mParam.mNumCols + (c + 1);
			indx4 = (r + 1) * mParam.mNumCols + (c + 1);

			//Triangle 1.
			v[indx].mPos	 = m[indx1];
			v[indx].mNormal	 = n[indx1];
			v[indx].mTexture = coords[indx1];
			indx += 1;

			v[indx].mPos	 = m[indx2];
			v[indx].mNormal  = n[indx2];
			v[indx].mTexture = coords[indx2];
			indx += 1;

			v[indx].mPos	 = m[indx4];
			v[indx].mNormal  = n[indx4];
			v[indx].mTexture = coords[indx4];
			indx += 1;
			//Triangle 2.
			v[indx].mPos	 = m[indx1];
			v[indx].mNormal  = n[indx1];
			v[indx].mTexture = coords[indx1];
			indx += 1;

			v[indx].mPos	 = m[indx4];
			v[indx].mNormal	 = n[indx4];
			v[indx].mTexture = coords[indx4];
			indx += 1;

			v[indx].mPos	 = m[indx3];
			v[indx].mNormal  = n[indx3];
			v[indx].mTexture = coords[indx3];
			indx += 1;
		}
	}
}

void SrTerrain::buildMesh(IDirect3DDevice9* pd3dDevice)  
{
	int count = mParam.mNumCols * mParam.mNumRows;
	float *h			= new float[count];
	D3DXVECTOR3* n		= new D3DXVECTOR3[count];
	D3DXVECTOR3* m		= new D3DXVECTOR3[count];
	D3DXVECTOR2* coords = new D3DXVECTOR2[count];
	memset(h, 0, sizeof(float) * count);

	calHeight(h);

	calHeightMap(h, m);

	calNormal(m, n);

	calTextureCoord(coords);

	TERRAINVERTEX* v;
	int numVerts = (mParam.mNumRows - 1) * (mParam.mNumCols - 1);
	pd3dDevice->CreateVertexBuffer(6 * numVerts * sizeof(TERRAINVERTEX), 0, D3DFVF_TERRAIN_VERTEX, D3DPOOL_MANAGED, &mVB, NULL );
	mVB->Lock( 0, 6 * numVerts * sizeof(TERRAINVERTEX), (void**)&v, 0 );
	calVertes(v, m, n, coords);
	mVB->Unlock();

	delete []h;
	delete []n;
	delete []m;
	delete []coords;
}
void SrTerrain::render(IDirect3DDevice9* pd3dDevice)const
{
	pd3dDevice->SetTransform( D3DTS_WORLD, &mMatrix );
	pd3dDevice->SetTexture(0, mTexture);
	pd3dDevice->SetStreamSource( 0, mVB, 0, sizeof(TERRAINVERTEX) );
	pd3dDevice->SetFVF( D3DFVF_TERRAIN_VERTEX );
	pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 * (mParam.mNumCols - 1) * (mParam.mNumRows - 1));

}

SrTerrain::~SrTerrain()
{
	SAFE_RELEASE(mTexture);
	SAFE_RELEASE(mVB);
	delete mPerlin;
}

void SrTerrain::calHeight(float* height)
{
	memset(height, 0, sizeof(float) * mParam.mNumCols * mParam.mNumRows);

	addPerlinNoise(height, mParam.mYSize);
	perturb(height, mParam.mF, mParam.mD);
	for (int i = 0; i < 10; i++ )
		erode(height, mParam.mErode);
	smoothen(height);
}

void SrTerrain::addPerlinNoise(float* height, float f, float base)
{
	int r, c;
	for (r = 0; r < mParam.mNumRows; r += 1)
	{
		for (c = 0; c < mParam.mNumCols; c += 1)
		{
			height[r * mParam.mNumCols + c] += base * mPerlin->noise(f * r / (float)mParam.mNumRows, f * c / (float)mParam.mNumCols, 0);
		}
	}
}
void SrTerrain::perturb(float* height, float f, float d)
{
	int u, v, i, j;
	float* tmp = new float[mParam.mNumCols * mParam.mNumRows];
	for (i = 0; i < mParam.mNumRows; i += 1)
	{
		for (j = 0; j < mParam.mNumCols; j += 1)
		{
			u = i + (int)(mPerlin->noise(f * i / (float)mParam.mNumRows, f * j / (float)mParam.mNumCols, 0) * d);
			v = j + (int)(mPerlin->noise(f * i / (float)mParam.mNumRows, f * j / (float)mParam.mNumCols, 1) * d);
			if (u < 0) u = 0; if (u >= mParam.mNumRows) u = mParam.mNumRows - 1;
			if (v < 0) v = 0; if (v >= mParam.mNumCols) v = mParam.mNumCols - 1;
			tmp[i * mParam.mNumCols + j] = height[u * mParam.mNumCols + v];
		}
	}
	memcpy(height, tmp, sizeof(float) * mParam.mNumCols * mParam.mNumRows);

	delete []tmp;
}
void SrTerrain::erode(float * height, float smoothness)
{
	int i, j;
	for (i = 1; i < mParam.mNumRows - 1; i++)
	{
		for (j = 1; j < mParam.mNumCols - 1; j++)
		{
			float d_max = 0.0f;
			int match[] = {0, 0};

			for (int u = -1; u <= 1; u++)
			{
				for (int v = -1; v <= 1; v++)
				{
					if(abs(u) +abs(v) > 0)
					{
						float d_i = height[i * mParam.mNumCols + j] - height[(i + u) * mParam.mNumCols + (j + v)];
						if (d_i > d_max)
						{
							d_max = d_i;
							match[0] = u; match[1] = v;
						}
					}
				}
			}
			if(0 < d_max && d_max <= smoothness)
			{
				float d_h = 0.5f * d_max;
				height[i * mParam.mNumCols + j] -= d_h;
				height[(i + match[0]) * mParam.mNumCols + (j + match[1])] += d_h;
			}
		}
	}
}

void SrTerrain::smoothen(float * height)
{
	for (int i = 1; i < mParam.mNumRows - 1; ++i)
	{
		for (int j = 1; j < mParam.mNumCols - 1; ++j)
		{
			float total = 0.0f;
			for (int u = -1; u <= 1; u++)
			{
				for (int v = -1; v <= 1; v++)
				{
					total += height[(i + u) * mParam.mNumCols + (j + v)];
				}
			}
			height[i * mParam.mNumCols + j] = total / 9.0f;
		}
	}
}

