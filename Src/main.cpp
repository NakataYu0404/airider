#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <memory>
#include <DxLib.h>
#include "Application.h"

#ifdef _DEBUG
#define new new(_NORMAL_BLOCK,__FILE__,__LINE__)
#endif

//	WinMain�֐�
//	---------------------------------
int WINAPI WinMain(
	_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, 
	_In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{

	//	���������[�N���o
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//	�@���|�C���^
	//	int *test = new int();
	//	delete test;
	
	//	�A���j�[�N�|�C���^
	std::unique_ptr<int> testI = std::make_unique<int>();
	std::unique_ptr<std::string> testS = std::make_unique<std::string>();
	

	//	�C���X�^���X�̐���
	Application::CreateInstance();

	//	�C���X�^���X�̎擾
	Application& instance = Application::GetInstance();

	if (instance.IsInitFail())
	{
		//	���������s
		return -1;
	}

	//	���s
	instance.Run();

	//	���
	instance.Destroy();

	return 0;

}
