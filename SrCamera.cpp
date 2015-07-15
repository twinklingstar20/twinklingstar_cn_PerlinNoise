/************************************************************************		
\link	www.twinklingstar.cn
\author twinklingstar
\file	SrCamera.cpp
\date	2015/05/20
****************************************************************************/
#include "DXUT.h"
#include "SrCamera.h"

SrCamera::SrCamera()
{
	D3DXMatrixIdentity(&mLastMatrix);
}

void SrCamera::setView(IDirect3DDevice9* pd3dDevice)
{ 
	D3DXMATRIX m;
	getViewMatrix(&m);
	mLastMatrix = m;
	pd3dDevice->SetTransform( D3DTS_VIEW, &m);
}

void SrCamera::setProj(IDirect3DDevice9* pd3dDevice, float fovy, int width, int height, float znear, float zfar)
{
	D3DXMatrixPerspectiveFovLH(&mProjMatrix, fovy,(float)width/(float)height, znear, zfar);
	pd3dDevice->SetTransform( D3DTS_PROJECTION, &mProjMatrix );
}

void SrCamera::getProjMatrix(D3DXMATRIX* m) const
{
	*m = mProjMatrix;
}

void SrCamera::rotateAxisX(float theta)
{
	D3DXVECTOR3 u(mLastMatrix[0],mLastMatrix[4],mLastMatrix[8]);
	mRotAngle += theta*u;
}

void SrCamera::rotateAxisY(float theta)
{
	D3DXVECTOR3 v(mLastMatrix[1],mLastMatrix[5],mLastMatrix[9]);
	mRotAngle += theta*v;
}

void SrCamera::rotateAxisZ(float theta)
{
	D3DXVECTOR3 n(mLastMatrix[2],mLastMatrix[6],mLastMatrix[10]);
	mRotAngle += theta*n;
}


void SrCamera::moveAxisX( float theta )
{
	D3DXVECTOR3 u(mLastMatrix[0],mLastMatrix[4],mLastMatrix[8]);
	mMoveDist += theta*u;
}

void SrCamera::moveAxisY( float theta )
{
	D3DXVECTOR3 v(mLastMatrix[1],mLastMatrix[5],mLastMatrix[9]);
	mMoveDist += theta*v;
}

void SrCamera::moveAxisZ( float theta )
{
	D3DXVECTOR3 n(mLastMatrix[2],mLastMatrix[6],mLastMatrix[10]);
	mMoveDist += theta*n;
}

void SrCamera::moveAxis(float nX,float nY,float nZ)
{
	D3DXVECTOR3 u(mLastMatrix[0],mLastMatrix[4],mLastMatrix[8]);
	D3DXVECTOR3 v(mLastMatrix[1],mLastMatrix[5],mLastMatrix[9]);
	D3DXVECTOR3 n(mLastMatrix[2],mLastMatrix[6],mLastMatrix[10]);

	mMoveDist += nX*u;
	mMoveDist += nY*v;
	mMoveDist += nZ*n;
}

D3DXVECTOR3 SrCamera::getCameraPos() const
{
	D3DXVECTOR3 eyePos;

	float matrix[9];
	matrix[0] = mLastMatrix[0] ; matrix[1] = mLastMatrix[4] ; matrix[2] = mLastMatrix[8];
	matrix[3] = mLastMatrix[1] ; matrix[4] = mLastMatrix[5] ; matrix[5] = mLastMatrix[9];
	matrix[6] = mLastMatrix[2] ; matrix[7] = mLastMatrix[6] ; matrix[8] = mLastMatrix[10];
	float d = determ3x3(matrix);
	matrix[0] = mLastMatrix[12] ; matrix[1] = mLastMatrix[13] ; matrix[2] = mLastMatrix[14];
	eyePos.x = determ3x3(matrix)/d;
	matrix[0] = mLastMatrix[0] ; matrix[1] = mLastMatrix[4] ; matrix[2] = mLastMatrix[8];
	matrix[3] = mLastMatrix[12] ; matrix[4] = mLastMatrix[13] ; matrix[5] = mLastMatrix[14];
	eyePos.y = determ3x3(matrix)/d;
	matrix[3] = mLastMatrix[1] ; matrix[4] = mLastMatrix[5] ; matrix[5] = mLastMatrix[9];
	matrix[6] = mLastMatrix[12] ; matrix[7] = mLastMatrix[13] ; matrix[8] = mLastMatrix[14];
	eyePos.z = determ3x3(matrix)/d;

	return eyePos;
}

void	SrCamera::getViewMatrix(D3DXMATRIX* m) const
{
	D3DXMATRIX rotX, rotY, rotZ, shift;
	D3DXMatrixRotationX(&rotX, mRotAngle.x);
	D3DXMatrixRotationY(&rotY, mRotAngle.y);
	D3DXMatrixRotationZ(&rotZ, mRotAngle.z);
	D3DXMatrixMultiply(m, &rotY, &rotX);
	D3DXMatrixMultiply(m, &rotZ, m);

	D3DXMatrixTranslation(&shift, mMoveDist.x, mMoveDist.y, mMoveDist.z);
	D3DXMatrixMultiply(m, &shift, m);
}

float	SrCamera::determ3x3(const float* ptrMatrix ) const
{
	double determ = ptrMatrix[0]*(ptrMatrix[4]*ptrMatrix[8] - ptrMatrix[5]*ptrMatrix[7] );
	determ -= ptrMatrix[1]*(ptrMatrix[3]*ptrMatrix[8] - ptrMatrix[5]*ptrMatrix[6]);
	determ += ptrMatrix[2]*(ptrMatrix[3]*ptrMatrix[7] - ptrMatrix[4]*ptrMatrix[6]);
	return (float)determ;
}