#pragma once
#include <chrono>
#include <windows.h>
#include <DxLib.h>

class SceneBase;
class Fader;
class Camera;

class SceneManager
{

public:

	static constexpr float INIT_DELTATIME = 0.016f;

	//	�V�[���Ǘ��p
	enum class SCENE_ID
	{
		NONE,
		TITLE,
		GAME,
	};
	
	enum class PLNUM
	{
		SOLO,
		EVERYONE,
	};

	//	�C���X�^���X�̐���
	static void CreateInstance(void);

	//	�C���X�^���X�̎擾
	static SceneManager& GetInstance(void);

	void Init(void);
	void Init3D(void);
	void Update(void);
	void Draw(void);

	//	���\�[�X�̔j��
	void Destroy(void);

	//	��ԑJ��
	void ChangeScene(SCENE_ID nextId);

	//	�V�[��ID�̎擾
	SCENE_ID GetSceneID(void);

	//	�f���^�^�C���̎擾
	float GetDeltaTime(void) const;

	//	�J�����̎擾
	Camera* GetCamera(void) const;

	//	�T�u�E�B���h�E�̃n���h�����Z�b�g(Application���痈��)
	void SetSubWindowH(HWND hdl);

	int GetMainScreen(void);

	void SetPlayerNum(PLNUM plNum);
	
private:

	static constexpr float NANOSECOND_TO_SECONDS = 1000000000.0;

	static constexpr VECTOR BACKGROUND_COLOR = { 0,139,139 };
	static constexpr VECTOR INIT_LIGHT_DIR = { 0,-1,0 };
	static constexpr VECTOR FOG_COLOR = { 5,5,5 };
	static constexpr float FOG_START = 10000.0f;
	static constexpr float FOG_END = 20000.0f;

	//	�f�t�H���g�R���X�g���N�^��private�ɂ��āA
	//	�O�����琶���ł��Ȃ��l�ɂ���
	SceneManager(void);
	//	�R�s�[�R���X�g���N�^�����l
	SceneManager(const SceneManager& manager) = default;
	//	�f�X�g���N�^�����l
	~SceneManager(void) = default;

	//	�f���^�^�C�������Z�b�g����
	void ResetDeltaTime(void);

	//	�V�[���J��
	void DoChangeScene(SCENE_ID sceneId);

	//	�t�F�[�h
	void Fade(void);

	//	�ÓI�C���X�^���X
	static SceneManager* instance_;

	SCENE_ID sceneId_;
	SCENE_ID waitSceneId_;

	PLNUM plNum_;

	int mainScreen_;

	//	�e��V�[��
	SceneBase* scene_;

	//	�t�F�[�h
	Fader* fader_;

	//	�J����
	Camera* camera_;

	//	�V�[���J�ڒ�����
	bool isSceneChanging_;

	//	�f���^�^�C��
	std::chrono::system_clock::time_point preTime_;
	float deltaTime_;
	
	HWND subWindowH_;

};
