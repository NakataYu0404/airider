#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "VSMaterial.h"
#include "PSMaterial.h"
#include "VSModelRenderer.h"

VSModelRenderer::VSModelRenderer(std::shared_ptr<VSMaterial> vsMate, std::shared_ptr<PSMaterial>psMate)
{
	vsMaterial_ = vsMate;
	psMaterial_ = psMate;
}

VSModelRenderer::~VSModelRenderer(void)
{
}

void VSModelRenderer::Update(void)
{
}

void VSModelRenderer::Draw(int screenH)
{

	modelId_ = vsMaterial_->GetModelId();

	//	AddConstBuf,Texture���R���X�g���N�^�̒i�K�ŉ���Ă��Ȃ�
	VSconstBuf_ = vsMaterial_->GetConstBuffer();
	VSconstBufs_ = vsMaterial_->GetConstBuffers();
	VSshaderH_ = vsMaterial_->GetShaderH();

	PSconstBuf_ = psMaterial_->GetConstBuffer();
	PSconstBufs_ = psMaterial_->GetConstBuffers();
	PSshaderH_ = psMaterial_->GetShaderH();

	textures_ = vsMaterial_->GetTextureImgs();

	auto& ins = SceneManager::GetInstance();

	ins.GetCamera()->SetBeforeDraw();

	//	�g�p�V�F�[�_
	SetUseVertexShader(VSshaderH_);
	SetUsePixelShader(PSshaderH_);

	//	�g�p�e�N�X�`��
	for (int i = 0; i < textures_.size(); i++)
	{
		SetUseTextureToShader(i, textures_[i]);
	}

	SetTextureAddressMode(DX_TEXADDRESS_CLAMP);

	FLOAT4* constBufsPtr;

	if (VSconstBufs_.size() > 0)
	{
		//	�萔�o�b�t�@��`
		constBufsPtr = (FLOAT4*)GetBufferShaderConstantBuffer(VSconstBuf_);
		for (int i = 0; i < VSconstBufs_.size(); i++)
		{

			constBufsPtr->x = VSconstBufs_[i].x;
			constBufsPtr->y = VSconstBufs_[i].y;
			constBufsPtr->z = VSconstBufs_[i].z;
			constBufsPtr->w = VSconstBufs_[i].w;
			constBufsPtr++;

		}

		//	�萔�o�b�t�@���X�V���ď������񂾓��e�𔽉f
		UpdateShaderConstantBuffer(VSconstBuf_);
		//	TODO:����if���̓����������ƁA�萔�o�b�t�@�ň�̃X���b�g�����g���Ȃ��B�X���b�g�ԍ���ݸ؂ł���d�g�݂ƁA�v�f���Ƃ�Buf��ݒ�ł���d�g�݂��~�����B
		SetShaderConstantBuffer(VSconstBuf_, DX_SHADERTYPE_VERTEX, CONSTANT_BUF_SLOT_BEGIN_VS);
	}

	if (PSconstBufs_.size() > 0)
	{
		//	�萔�o�b�t�@��`
		constBufsPtr = (FLOAT4*)GetBufferShaderConstantBuffer(PSconstBuf_);
		for (int i = 0; i < PSconstBufs_.size(); i++)
		{

			constBufsPtr->x = PSconstBufs_[i].x;
			constBufsPtr->y = PSconstBufs_[i].y;
			constBufsPtr->z = PSconstBufs_[i].z;
			constBufsPtr->w = PSconstBufs_[i].w;
			constBufsPtr++;

		}
		//	�萔�o�b�t�@���X�V���ď������񂾓��e�𔽉f
		UpdateShaderConstantBuffer(PSconstBuf_);
		SetShaderConstantBuffer(PSconstBuf_, DX_SHADERTYPE_PIXEL, CONSTANT_BUF_SLOT_BEGIN_PS);
	}

	//	�V�F�[�_���g�p����
	MV1SetUseOrigShader(true);

	for (auto& m : modelId_)
	{
		MV1DrawModel(m);
	}

	SetUseVertexShader(-1); // �܂��͌��̃V�F�[�_�n���h��
	SetUsePixelShader(-1); // �܂��͌��̃V�F�[�_�n���h��
	MV1SetUseOrigShader(false);

}
