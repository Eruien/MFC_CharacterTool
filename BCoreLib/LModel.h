#pragma once
#include "LFbxObj.h"

class LModel
{
	// Instancing
public:
	std::vector<LFbxObj*> m_ActionList;
	std::vector<float> m_CurrentAnimTimeList;
	std::vector<LBoneWorld> m_matBoneArrayList;
	std::vector<LBoneWorld> m_matMeshBoneArrayList;
	int m_InstanceSize = 0;
	int m_matBoneArraySize = 100;
	ComPtr<ID3D11Texture2D> pAnimationArrayTex = nullptr;
	ComPtr<ID3D11ShaderResourceView> pAnimationArraySRV = nullptr;
public:
	LFbxObj* m_pModel = nullptr;
	LFbxObj* m_pActionModel = nullptr;
public:
	float m_fCurrentAnimTime = 0.0f;
	LBoneWorld m_matBoneArray;
	LBoneWorld m_matMeshBoneArray;
	ComPtr<ID3D11Buffer> m_pBoneArrayCB = nullptr;
	TMatrix m_matControl;
	std::wstring m_ParentBoneName;
public:
	virtual void SetAnimationArrayTexture();
	virtual void SetAnimationArraySRV();
	virtual void SetActionList(LFbxObj* fbxObj);
	virtual void SetInstancing(bool IsIntancing, int instancingCount);
	virtual void SetInstancing(bool IsIntancing);
	virtual void SetLFbxObj(LFbxObj* fbxObj);
	virtual LFbxObj* GetLFbxObj();
public:
	virtual bool CreateBoneBuffer();
	virtual bool Frame();
	virtual bool FrameInstancing();
	virtual bool Render();
	bool ShadowRender(TMatrix* matShadowWorld, TMatrix* matShadowView, TMatrix* matShadowProj);
	virtual bool Release();
public:
	LModel();
	virtual ~LModel();
};

class LSkinningModel : public LModel
{
public:
	virtual bool AllNodeRender();
	bool CreateBoneBuffer() override;
	bool Frame() override;
	bool FrameInstancing() override;
	bool Render() override;
	bool RenderInstancing();
	bool Release() override;
};