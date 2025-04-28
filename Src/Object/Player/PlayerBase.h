#pragma once
#include <map>
#include <vector>
#include <array>
#include <memory>
#include "../ActorBase.h"
class AnimationController;
class Collider;
class Capsule;
class CollisionManager;


class PlayerBase : public ActorBase
{

public:

	//	���
	enum class STATE
	{
		NONE,
		PLAY,
		DEAD,
		VICTORY,
		END
	};

	static constexpr float DEFAULT_GRAVITY_POW = 10.0f;

	//	�R���X�g���N�^
	PlayerBase(void);

	//	�f�X�g���N�^
	~PlayerBase(void);

	void Init(void)override;
	void Update(void)override;
	void Draw(bool isDrawRaiderWindow = false, int screenH = -1)override;

	//	�Փ˔���ɗp������R���C�_����
	void AddCollider(std::shared_ptr<Collider> collider);
	void ClearCollider(void);

	void AddCapsule(std::shared_ptr<Capsule> capsule);

	//	�_���[�W�󂯂�
	void Damage(float damage);

	STATE GetState(void);

	void ChangeState(STATE state);

	bool IsMove(void);

	void SetPlayAnimation(int type, bool isLoop = true,
		float startStep = 0.0f, float endStep = -1.0f, bool isStop = false, bool isForce = false);

	void AnimUpdate(void);

protected:

	VECTOR DIR_F = { 0.0f,0.0f,1.0f };
	VECTOR DIR_B = { 0.0f,0.0f,-1.0f };
	VECTOR DIR_L = { -1.0f,0.0f,0.0f };
	VECTOR DIR_R = { 1.0f,0.0f,0.0f };

	static constexpr float ROT_F = 0.0f;
	static constexpr float ROT_B = 180.0f;
	static constexpr float ROT_L = 270.0f;
	static constexpr float ROT_R = 90.0f;

	static constexpr int MAX_HP = 100;

	static constexpr float DIV_MOVE_CONTROLL = 10.0f;

	static constexpr float CHECK_COL_POW = 20.0f;

	virtual void SetParam(void) override;


	void ChangeStateNone(void);
	void ChangeStatePlay(void);

	//	����
	virtual void ProcessMove(int plNum);

	//	��]
	virtual void SetGoalRotate(double rotRad);
	void CollisionGravity(void);

	void Rotate(void);

	//	�Փ˔���
	void Collision(void);

	//	����������Transform��������(�����Ő��K���A�l�𐳂̐��ɂ��܂�)
	float Myself2OtherDistance3D(std::weak_ptr<Transform> toTran);
	float Myself2OtherDistance2D(std::weak_ptr<Transform> toTran);
	VECTOR Myself2OtherDir(std::weak_ptr<Transform> toTran);

	//	��]�����܂ł̎���
	static constexpr float TIME_ROT = 0.6f;

	//	�A�j���[�V����
	std::shared_ptr<AnimationController> animationController_;

	//	�Փ˔���ɗp������R���C�_
	std::vector<std::shared_ptr<Collider>> colliders_;

	//	�ړ��X�s�[�h
	float speed_;

	//	��ԊǗ�
	STATE state_;

	//	�ړ�����
	VECTOR moveDir_;

	//	�ړ���
	VECTOR movePow_;

	//	�ړ���̍��W
	VECTOR movedPos_;

	float gravityPow_;

	//	��]
	Quaternion playerRotY_;
	Quaternion goalQuaRot_;
	float stepRotTime_;
	float rotRad_;


	//	�Փ˃`�F�b�N
	VECTOR gravHitPosDown_;
	VECTOR gravHitPosUp_;

	int ForwardKey_;
	int BackKey_;
	int RightKey_;
	int LeftKey_;

	//	�V���O���g���Q��
	CollisionManager& colMng_;

};
