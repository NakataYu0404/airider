#pragma once
#include <DxLib.h>
#include "../../Common/Quaternion.h"
#include "Collider.h"

//	/	<summary>
//	/	���f������̊�{���
//	/	�傫���FVECTOR�
//	/	��]�@�FQuaternion�
//	/	�ʒu�@�FVECTOR�
//	/	</summary>
class Transform
{

public:

	//	�R���X�g���N�^
	Transform(void);
	Transform(int model);

	//	�f�X�g���N�^
	~Transform(void);

	void Update(void);
	
	void Release(void);

	//	���f���n���h�����Z�b�g
	void SetModel(int modelHId);

	//	�R���C�_�쐬
	void MakeCollider(Collider::Category category, Collider::TYPE type,int modelIdH = -1);

	//	�O���������擾
	VECTOR GetForward(void) const;

	//	����������擾
	VECTOR GetBack(void) const;

	//	�E�������擾
	VECTOR GetRight(void) const;

	//	���������擾
	VECTOR GetLeft(void) const;

	//	��������擾
	VECTOR GetUp(void) const;

	//	���������擾
	VECTOR GetDown(void) const;

	//	�Ώە������擾
	VECTOR GetDir(const VECTOR& vec) const;

	//	���f���̃n���h��ID
	int modelId;

	//	�傫��
	VECTOR scl;

	//	��]
	VECTOR rot;

	//	�ʒu
	VECTOR pos;

	//	���ʒu
	VECTOR headPos;

	//	���S�ʒu
	VECTOR midPos;

	//	���[�J�����W
	VECTOR localPos;

	//	�s��
	MATRIX matScl;
	MATRIX matRot;
	MATRIX matPos;

	//	��]
	Quaternion quaRot;

	//	���[�J����]
	Quaternion quaRotLocal;

	//	�R���C�_
	std::shared_ptr<Collider> collider_;

};
