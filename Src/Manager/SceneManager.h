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

	//	シーン管理用
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

	//	インスタンスの生成
	static void CreateInstance(void);

	//	インスタンスの取得
	static SceneManager& GetInstance(void);

	void Init(void);
	void Init3D(void);
	void Update(void);
	void Draw(void);

	//	リソースの破棄
	void Destroy(void);

	//	状態遷移
	void ChangeScene(SCENE_ID nextId);

	//	シーンIDの取得
	SCENE_ID GetSceneID(void);

	//	デルタタイムの取得
	float GetDeltaTime(void) const;

	//	カメラの取得
	Camera* GetCamera(void) const;

	//	サブウィンドウのハンドルをセット(Applicationから来る)
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

	//	デフォルトコンストラクタをprivateにして、
	//	外部から生成できない様にする
	SceneManager(void);
	//	コピーコンストラクタも同様
	SceneManager(const SceneManager& manager) = default;
	//	デストラクタも同様
	~SceneManager(void) = default;

	//	デルタタイムをリセットする
	void ResetDeltaTime(void);

	//	シーン遷移
	void DoChangeScene(SCENE_ID sceneId);

	//	フェード
	void Fade(void);

	//	静的インスタンス
	static SceneManager* instance_;

	SCENE_ID sceneId_;
	SCENE_ID waitSceneId_;

	PLNUM plNum_;

	int mainScreen_;

	//	各種シーン
	SceneBase* scene_;

	//	フェード
	Fader* fader_;

	//	カメラ
	Camera* camera_;

	//	シーン遷移中判定
	bool isSceneChanging_;

	//	デルタタイム
	std::chrono::system_clock::time_point preTime_;
	float deltaTime_;
	
	HWND subWindowH_;

};
