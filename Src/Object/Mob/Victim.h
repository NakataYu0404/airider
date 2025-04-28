#pragma once
#include <memory>
#include "../ActorBase.h"

class AnimationController;
class EffectController;

class Victim : public ActorBase
{
public:

	//	���C�_�[���E�������ɓ���|�C���g 100�Ői��
	static constexpr int POINT_EVOLUTION = 25;

	//	���
	enum class STATE
	{
		NONE,
		PLAY,
		DEAD,
		END
	};

	//	�A�j���[�V�������
	enum class ANIM_TYPE
	{
		IDLE,
		DEAD,
	};

	enum class EFFECT_TYPE
	{
		POWERUP,
	};

	Victim();
	~Victim();

	void Init() override;
	void Update() override;
	void Draw(bool isDrawRaiderWindow = false, int screenH = -1) override;
	void SetParam() override;

	void OnCollision(std::weak_ptr<Collider> collider) override;
	void OnCollision(Collider::Category category) override;

	bool IsAlive(void);
	void SetAlive(bool);

	void SetPos(VECTOR pos);

private:

	static constexpr VECTOR SCALE = { 2.0f,1.5f,2.0f };
	static constexpr VECTOR EFFECT_SIZE = { 50.0f,10.0f,50.0f };

	static constexpr VECTOR CAPSULE_TOP = { 0.0f, 110.0f, 0.0f };
	static constexpr VECTOR CAPSULE_BOTTOM = { 0.0f, 30.0f, 0.0f };
	static constexpr float CAPSULE_RADIUS = 60.0f;

	static constexpr float ROT_TIME_DIV = 10.0f;


	void EffectInit(void);

	void StartEffect(EFFECT_TYPE type);

	//	��ԑJ��
	void ChangeState(STATE state);
	void ChangeStateNone(void);
	void ChangeStatePlay(void);

	//	�X�V�X�e�b�v
	void UpdateNone(void);
	void UpdatePlay(void);

	//	�`��n
	void DrawShadow(void);

	std::shared_ptr<EffectController> effectController_;

	int EffpowerupId_;

	//	�����Ă邩
	bool isAlive_;

	//	�A�j���[�V����
	std::shared_ptr<AnimationController> animationController_;

	//	��ԊǗ�
	STATE state_;

	//	�ۉe
	int imgShadowHdl_;

	int cnt_;
};

