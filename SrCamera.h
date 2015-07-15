/************************************************************************		
\link	www.twinklingstar.cn
\author twinklingstar
\file	SrCamera.h
\date	2015/05/20
****************************************************************************/
#ifndef SR_CAMERA_H_
#define SR_CAMERA_H_

class SrCamera
{
public:
	SrCamera();
	/**
	 \brief	Set the configuration of the camera.
	*/
	virtual void setView(IDirect3DDevice9* pd3dDevice);

	virtual void setProj(IDirect3DDevice9* pd3dDevice, float fovy, int width, int height, float near, float zfar);
	/*
	\brief	In the camera coordination system, there are three axises. The following methods provides
			with interfaces to rotate around an axis or move in the direction of an axis.
	*/
	virtual void rotateAxisX(float theta);

	virtual void rotateAxisY(float theta);

	virtual void rotateAxisZ(float theta);

	virtual void moveAxisX( float theta );

	virtual void moveAxisY( float theta );

	virtual void moveAxisZ( float theta );

	virtual void moveAxis(float nX,float nY,float nZ);
	/*
	\brief	Get the original position of camera.
	*/
	D3DXVECTOR3 getCameraPos() const;

	void	getViewMatrix(D3DXMATRIX* m) const;

	void	getProjMatrix(D3DXMATRIX* m) const;
protected:
	float	determ3x3(const float* ptrMatrix ) const;

private:
	D3DXMATRIX			mLastMatrix;
	D3DXMATRIX			mProjMatrix;
	D3DXVECTOR3			mMoveDist;
	D3DXVECTOR3			mRotAngle;
};


#endif