// VS/PS����
#include "../../Common/VertexToPixelHeader.hlsli"

//PS
#include "../../Common/Pixel/PixelShader3DHeader.hlsli"


// IN
#define PS_INPUT VertexToPixelLit

// �萔�o�b�t�@�F�X���b�g0�Ԗ�(b0�Ə���)
cbuffer cbParam : register(b0)
{
	float4 g_color;
}

Texture2D myTexture : register(t0); // t0 �̓e�N�X�`�����W�X�^�̔ԍ�
SamplerState mySampler : register(s0); // s0 �̓T���v���[�X�e�[�g�̃��W�X�^�̔ԍ�

float4 main(PS_INPUT PSInput) : SV_TARGET
{
	float2 uv = PSInput.uv;
	float4 TextureDiffuseColor = myTexture.Sample(mySampler, uv);

	if (TextureDiffuseColor.a == 0)
	{
        // �e�N�X�`�����������T���v������Ă��Ȃ��ꍇ�̃G���[�n���h�����O
		return float4(1.0, 0.0, 0.0, 1.0); // �ԐF��Ԃ��Ċm�F
	}

    // �ȉ��̃R�[�h�͐��퓮�삵�Ă���Ɖ���
	//float3 lightDir = g_common.light[0].direction;
	float4 srcCol = PSInput.diffuse * TextureDiffuseColor;
	//float dotNormLight = dot(PSInput.normal, lightDir);
	//srcCol.xyz *= (-dotNormLight + 1.0f);
	return srcCol;
}