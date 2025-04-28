#pragma once
#include <memory>
#include <vector>
#include <Dxlib.h>
#include "../Manager/SceneManager.h"

class PSMaterial;

class PSMeshRenderer
{
public:
	PSMeshRenderer(std::shared_ptr<PSMaterial> psMaterial);
	~PSMeshRenderer(void);
	
	void Update(void);
	void Draw(int ScreenHHdl_);
private:
	
	void MakeVertexs(void);


	//	�s�N�Z���V�F�[�_�p�I���W�i���萔�o�b�t�@�̎g�p�J�n�X���b�g
	static constexpr int CONSTANT_BUF_SLOT_BEGIN_PS = 4;

	//	���_��
	static constexpr int NUM_VERTEX = 4;

	//	���_�C���f�b�N�X��
	static constexpr int NUM_VERTEX_IDX = 6;

	//	�|���S����
	static constexpr int NUM_POLYGON = 2;

	std::shared_ptr<PSMaterial> psMaterial_;

	//	���_���
	VERTEX2DSHADER vertexs_[NUM_VERTEX];

	//	���_�C���f�b�N�X
	WORD indexes_[NUM_VERTEX_IDX];

	int constBuf_;
	int shaderH_;
	std::vector<FLOAT4> constBufs_;
	std::vector<int> textures_;

	int oldScreenHHdl_;
};

