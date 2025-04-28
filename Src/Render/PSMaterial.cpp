#include "../Utility/Utility.h"
#include "../Application.h"
#include "PSMaterial.h"

PSMaterial::PSMaterial(std::string PSPath, int bufVolume)
{
	//	�V�F�[�_�ǂݍ���
	shaderH_ = LoadPixelShader((Application::PATH_SHADER + PSPath).c_str());
	//	�o�b�t�@��`
	constBufH_ = CreateShaderConstantBuffer(sizeof(FLOAT4) * bufVolume);
}

PSMaterial::~PSMaterial(void)
{
}

void PSMaterial::AddConstBuffer(float buf1, float buf2, float buf3, float buf4)
{
	FLOAT4 buf;
	buf.x = buf1;
	buf.y = buf2;
	buf.z = buf3;
	buf.w = buf4;
	constBufs_.push_back(buf);
}

void PSMaterial::AddConstBuffer(FLOAT4 buf)
{
	constBufs_.push_back(buf);
}

void PSMaterial::ChangeConstBuffer(int registerNo, float buf1, float buf2, float buf3, float buf4)
{
	FLOAT4 buf;
	buf.x = buf1;
	buf.y = buf2;
	buf.z = buf3;
	buf.w = buf4;
	constBufs_[registerNo] = buf;

}

void PSMaterial::ChangeConstBuffer(int registerNo, FLOAT4 buf)
{
	constBufs_[registerNo] = buf;
}

int PSMaterial::GetConstBuffer(void)
{
	return constBufH_;
}

std::vector<FLOAT4>& PSMaterial::GetConstBuffers(void)
{
	return constBufs_;
}

std::vector<int>& PSMaterial::GetTextureImgs(void)
{
	return textureimgsHdl_;
}

int PSMaterial::GetShaderH(void)
{
	return shaderH_;
}

void PSMaterial::AddTexture(int texH)
{
	textureimgsHdl_.push_back(texH);
}

void PSMaterial::Clear(void)
{
	textureimgsHdl_.clear();
	constBufs_.clear();
}

