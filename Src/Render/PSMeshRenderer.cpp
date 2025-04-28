#include "../Application.h"
#include "PSMaterial.h"
#include "PSMeshRenderer.h"

PSMeshRenderer::PSMeshRenderer(std::shared_ptr<PSMaterial> psMaterial)
{
	psMaterial_ = psMaterial;
	MakeVertexs();
}

PSMeshRenderer::~PSMeshRenderer(void)
{
}

void PSMeshRenderer::Update(void)
{
}

void PSMeshRenderer::Draw(int screenH)
{
	//	AddConstBuf,Texture���R���X�g���N�^�̒i�K�ŉ���Ă��Ȃ�
	constBuf_ = psMaterial_->GetConstBuffer();
	constBufs_ = psMaterial_->GetConstBuffers();
	shaderH_ = psMaterial_->GetShaderH();
	textures_ = psMaterial_->GetTextureImgs();

	auto& ins = SceneManager::GetInstance();

	//	�`���ݒ�
	SetDrawScreen(screenH);
	ClsDrawScreen();
	//	�V�F�[�_���g�p����
	MV1SetUseOrigShader(true);

	//	�g�p�V�F�[�_
	SetUsePixelShader(shaderH_);
	//	�g�p�e�N�X�`��
	for(int i = 0; i < textures_.size(); i++)
	{ 
		SetUseTextureToShader(i, textures_[i]);
	}
	//	�萔�o�b�t�@��`
	FLOAT4* constBufsPtr = (FLOAT4*)GetBufferShaderConstantBuffer(constBuf_);
	for(int i = 0; i< constBufs_.size();i++)
	{ 

		constBufsPtr->x = constBufs_[i].x;
		constBufsPtr->y = constBufs_[i].y;
		constBufsPtr->z = constBufs_[i].z;
		constBufsPtr->w = constBufs_[i].w;
		constBufsPtr++;

	}

	//	�萔�o�b�t�@���X�V���ď������񂾓��e�𔽉f
	UpdateShaderConstantBuffer(constBuf_);

	SetShaderConstantBuffer(constBuf_, DX_SHADERTYPE_PIXEL, CONSTANT_BUF_SLOT_BEGIN_PS);

	DrawPolygonIndexed2DToShader(vertexs_, NUM_VERTEX, indexes_, NUM_POLYGON);

	SetUseTextureToShader(0, -1);
	SetUsePixelShader(-1);
	MV1SetUseOrigShader(false);
}

void PSMeshRenderer::MakeVertexs(void)
{
	float sX = static_cast<float>(0.0f);
	float sY = static_cast<float>(0.0f);
	float eX = static_cast<float>(Application::SCREEN_SIZE_X);
	float eY = static_cast<float>(Application::SCREEN_SIZE_Y);

	// �S���_�̏�����
	for (int i = 0; i < 4; i++)
	{
		vertexs_[i].rhw = 1.0f;
		vertexs_[i].dif = GetColorU8(255, 255, 255, 255);
		vertexs_[i].spc = GetColorU8(255, 255, 255, 255);
		vertexs_[i].su = 0.0f;
		vertexs_[i].sv = 0.0f;
	}

	// ����
	vertexs_[0].pos = VGet(sX, sY, 0.0f);
	vertexs_[0].u = 0.0f;
	vertexs_[0].v = 0.0f;

	// �E��
	vertexs_[1].pos = VGet(eX, sY, 0.0f);
	vertexs_[1].u = 1.0f;
	vertexs_[1].v = 0.0f;

	// ����
	vertexs_[2].pos = VGet(sX, eY, 0.0f);
	vertexs_[2].u = 0.0f;
	vertexs_[2].v = 1.0f;

	// �E��
	vertexs_[3].pos = VGet(eX, eY, 0.0f);
	vertexs_[3].u = 1.0f;
	vertexs_[3].v = 1.0f;

	int cnt = 0;
	indexes_[cnt++] = 0;
	indexes_[cnt++] = 1;
	indexes_[cnt++] = 2;

	indexes_[cnt++] = 1;
	indexes_[cnt++] = 3;
	indexes_[cnt++] = 2;

}
