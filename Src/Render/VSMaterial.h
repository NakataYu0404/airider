#pragma once
#include <vector>
#include <string>
#include <DxLib.h>

class PSMaterial;

class VSMaterial
{
public:

	//	PixelShader�̃p�X,�萔�o�b�t�@�������̂�
	VSMaterial(std::string VSPath, int bufVolume,int modelId);
	~VSMaterial(void);

	//	�萔�o�b�t�@�ɒ��g�����邽�߂̊֐�
	void AddConstBuffer(float buf1 = -1.0f, float buf2 = -1.0f, float buf3 = -1.0f, float buf4 = -1.0f);
	void AddConstBuffer(FLOAT4 buf);

	void ChangeConstBuffer(int registerNo = 0, float buf1 = -1.0f, float buf2 = -1.0f, float buf3 = -1.0f, float buf4 = -1.0f);
	void ChangeConstBuffer(int registerNo = 0, FLOAT4 buf = {-1.0f,-1.0f,-1.0f,-1.0f});

	void AddModelId(int modelId);

	//	�Q�b�^�[
	int GetConstBuffer(void);


	std::vector<FLOAT4>& GetConstBuffers(void);

	std::vector<int>& GetTextureImgs(void);

	int GetShaderH(void);

	std::vector<int> GetModelId(void);

	//	�e�N�X�`���̏����擾���Ă���(���ۂ�Set����������̂�Renderer)
	void AddTexture(int texH);

	void Clear(void);
private:
	//	�V�F�[�_�[H
	int shaderH_;
	//	�o�b�t�@H
	int constBufH_;

	//	�萔�o�b�t�@�̃|�C���^�̒��g�ɐݒ肷��l,vector��[1]�̃f�[�^��[2]�̃f�[�^���A�����ă������m�ۂ��Ă���̂������Ƃ���
	std::vector<FLOAT4> constBufs_;

	std::vector<int> textureimgsHdl_;

	std::vector<int> modelId_;
};

