#pragma once
#include <memory>
#include <vector>
#include <Dxlib.h>
#include "../Manager/SceneManager.h"

class VSMaterial;
class PSMaterial;

class VSModelRenderer
{
public:
	struct float3
	{
		float x;
		float y;
		float z;
	};

	struct float4
	{
		float x;
		float y;
		float z;
		float w;
	};

	VSModelRenderer(std::shared_ptr<VSMaterial> vsMate, std::shared_ptr<PSMaterial>psMate);
	~VSModelRenderer(void);
	
	void Update(void);
	void Draw(int ScreenHHdl_);
private:
	
	std::shared_ptr<VSMaterial> vsMaterial_;
	std::shared_ptr<PSMaterial> psMaterial_;

	int timeBuf;
	int VertexMaxPosBuf;

	float count = 0;

	int indexNum_;

	//	�s�N�Z���V�F�[�_�p�I���W�i���萔�o�b�t�@�̎g�p�J�n�X���b�g
	static constexpr int CONSTANT_BUF_SLOT_BEGIN_PS = 4;
	static constexpr int CONSTANT_BUF_SLOT_BEGIN_VS = 7;

	int VSconstBuf_;
	int VSshaderH_;
	std::vector<FLOAT4> VSconstBufs_;

	int PSconstBuf_;
	int PSshaderH_;
	std::vector<FLOAT4> PSconstBufs_;

	std::vector<int> textures_;

	std::vector<int> modelId_;

	int oldScreenHHdl_;
};

