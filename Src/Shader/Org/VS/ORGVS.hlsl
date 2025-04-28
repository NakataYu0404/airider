// VS/PS����
#include "../../Common/VertexToPixelHeader.hlsli"

//VS
#include "../../Common/Vertex/VertexShader3DHeader.hlsli"
#include "../../Common/Vertex/VertexInputType.hlsli"


// IN
#define VERTEX_INPUT DX_MV1_VERTEX_TYPE_NMAP_1FRAME

// OUT
#define VS_OUTPUT VertexToPixelLit

cbuffer cbParam : register(b7)
{
	float4 g_light_pos[16];
}

VS_OUTPUT main(VS_INPUT VSInput)
{
	VS_OUTPUT ret;
	
	// ���_���W�ϊ� +++++++++++++++++++++++++++++++++++++( �J�n )
	float4 lLocalPosition;
	float4 lWorldPosition;
	float4 lViewPosition;
	// float3 �� float4
	lLocalPosition.xyz = VSInput.pos;
	lLocalPosition.w = 1.0f;

	// ���[�J�����W�����[���h���W�ɕϊ�(����)
	lWorldPosition.w = 1.0f;
	lWorldPosition.xyz = mul(lLocalPosition, g_base.localWorldMatrix);
	// ���[���h���W���r���[���W�ɕϊ�
	lViewPosition.w = 1.0f;
	lViewPosition.xyz = mul(lWorldPosition, g_base.viewMatrix);
	ret.vwPos.xyz = lViewPosition.xyz;
	// �r���[���W���ˉe���W�ɕϊ�
	ret.svPos = mul(lViewPosition, g_base.projectionMatrix);
	// ���_���W�ϊ� +++++++++++++++++++++++++++++++++++++( �I�� )
	
	// ���̑��A�s�N�Z���V�F�[�_�ֈ��p&������ ++++++++++++( �J�n )
	
  // �|�C���g���C�g�̋��x�v�Z
	float3 lightEffect = float3(0.0f, 0.0f, 0.0f);
	
	for (int i = 0; i < 12; i++)
	{
		float3 lightPos = g_light_pos[i].xyz;
		float3 lightVec = lightPos - lWorldPosition.xyz;
		float distance = length(lightVec);
		lightVec = normalize(lightVec);

		float NdotL = max(dot(normalize(VSInput.norm), lightVec), 0.0f);
		float attenuation = 1.0f / (distance * distance);
		lightEffect += NdotL * attenuation;
	}
	
	lightEffect *= 50000.0f;

	lightEffect.x *= 0.05f;
	lightEffect.y *= 0.001f;
	
	float4 localCol = float4(0.0f, 0.0f, 0.0f, 0.0f);
	localCol = saturate(lLocalPosition.y * float4(0.0f, 0.7f, 1.0f, 1.0f));

// UV���W
	ret.uv = VSInput.uv0;

// �@��
// �@�������[�J����Ԃ��烏�[���h��Ԃ֕ϊ�
	ret.normal = normalize(
        mul(VSInput.norm, (float3x3) g_base.localWorldMatrix));
    	
	
	
	
  // �f�B�t���[�Y�J���[
	ret.diffuse = float4(VSInput.diffuse.xyz / 4.0f + localCol/10.0f + lightEffect, 1.0f);
	
// ���C�g����(���[�J��)
	ret.lightDir = float3(0.0f, -0.0f, 0.0f);

// ���C�g���猩�����W
	ret.lightAtPos = float3(0.0f, 0.0f, 0.0f);

// ���̑��A�s�N�Z���V�F�[�_�ֈ��p&������ ++++++++++++( �I�� )
    
	// �o�̓p�����[�^��Ԃ�
	return ret;
}