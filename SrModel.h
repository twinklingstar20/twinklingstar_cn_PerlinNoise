/************************************************************************		
\link	www.twinklingstar.cn
\author twinklingstar
\file	SrModel.h
\date	2015/05/20
****************************************************************************/
#ifndef SR_MODEL_H_
#define SR_MODEL_H_

//About the accumulation snow effect.
struct SrSnowModelParam
{
	bool	mIsSnowObstruct;
	bool	mIsSnowEffect;
};
/**
\brief	The common interface of all models.
 */
class SrModel
{
public:
	SrModel()
	{
		mSnowParam.mIsSnowObstruct	= false;
		mSnowParam.mIsSnowEffect	= false;
		D3DXMatrixIdentity(&mMatrix);
	}
	/**
	\brief	Initialize model after the 3d3 device is created.
	 */
	virtual bool init(IDirect3DDevice9* pd3dDevice) = 0;
	/**
	\brief	The destructor.
	 */
	virtual ~SrModel(){};
	/**
	\brief	Render the model.
	 */
	virtual void render(IDirect3DDevice9* pd3dDevice) const = 0;

	void scale(float x, float y, float z)
	{
		D3DXMATRIX mat;
		D3DXMatrixScaling(&mat, x, y, z);
		D3DXMatrixMultiply(&mMatrix, &mat, &mMatrix);
	}

	void translate(float x, float y, float z)
	{
		D3DXMATRIX mat;
		D3DXMatrixTranslation(&mat, x, y, z);
		D3DXMatrixMultiply(&mMatrix, &mat, &mMatrix);
	}

	void rotate(const D3DXVECTOR3& v, float angle)
	{
		D3DXMATRIX mat;
		D3DXMatrixRotationAxis(&mat, &v, angle);
		D3DXMatrixMultiply(&mMatrix, &mat, &mMatrix);
	}

	void identity()
	{
		D3DXMatrixIdentity(&mMatrix);
	}

	D3DXMATRIX* getWorld()
	{
		return &mMatrix;
	}

	void setWorld(const D3DXMATRIX* m)
	{
		mMatrix = *m;
	}

	void setSnowObstruct(bool flag)
	{
		mSnowParam.mIsSnowObstruct = flag; 
	}

	bool isSnowObstruct()
	{
		return mSnowParam.mIsSnowObstruct;
	}

	void setSnowEffect(bool flag)
	{
		mSnowParam.mIsSnowEffect = flag;
	}

	bool isSnowEffect()
	{
		return mSnowParam.mIsSnowEffect;
	}
protected:
	D3DXMATRIX	mMatrix;

	SrSnowModelParam	mSnowParam;
};


#endif