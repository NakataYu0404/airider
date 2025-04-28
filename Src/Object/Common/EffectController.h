#pragma once
#include <EffekseerForDXLib.h>
#include <string>
#include <map>
class SceneManager;

class EffectController
{
	
public:
	static constexpr VECTOR POS_DEFAULT = { 0.0f,0.0f,0.0f };
	static constexpr VECTOR SCL_DEFAULT = { 1.0f,1.0f,1.0f };
	static constexpr VECTOR ROT_DEFAULT = { 0.0f,0.0f,0.0f };

	//	�A�j���[�V�����f�[�^
	struct Effect
	{
		int Type = -1;
		int ResId = -1;
		int PlayId = -1;
		bool IsLoop = false;
		int startFrame = 0;
		int endFrame = -1;
		float nowTime = 0;
		VECTOR pos = POS_DEFAULT;
		VECTOR scl = SCL_DEFAULT;
		VECTOR rot = ROT_DEFAULT;
	};

	//	�R���X�g���N�^
	EffectController(void);
	//	�f�X�g���N�^
	~EffectController(void);

	//	�A�j���[�V�����ǉ�
	void Add(int type, const int ResId);

	//	�A�j���[�V�����Đ�
	void Play(int type, bool isLoop = false, int startFrame = 0, int endFrame = -1, float speed = 1.0f);

	//	�ʒu�X�V
	void TransUpdate(int type, VECTOR pos = POS_DEFAULT, VECTOR scl = SCL_DEFAULT, VECTOR rot = ROT_DEFAULT);

	void Update(void);
	void Draw(void);

	//	without���U�Ȃ�type�̂݁A�^�Ȃ�type�ȊO��effect��S�Ď~�߂܂�
	void Stop(int type,bool without = false);

	//	�Đ����̃A�j���[�V����
	int GetPlayType(void) const;

	//	�I�����Ă��邩����
	bool IsEnd(int type);
private:

	//	��ޕʂ̃A�j���[�V�����f�[�^
	std::map<int, Effect> effects_;

	//	�Đ����G�t�F�N�g�̃^�C�v
	int playType_;

	//	�Đ����G�t�F�N�g
	Effect playEffect_;

	//	�A�j���[�V���������[�v���邩���Ȃ���
	bool isLoop_;

	//	�A�j���[�V�������~�߂��܂܂ɂ���
	bool isStop_;

	//	�A�j���[�V�����I����ɌJ��Ԃ����[�v�X�e�b�v
	float stepEndLoopStart_;
	float stepEndLoopEnd_;
	float endLoopSpeed_;

	//	�t�Đ�
	float switchLoopReverse_;

	//	�Đ����x
	float speed_;
};

