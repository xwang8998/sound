#include<stdint.h>
#include<windows.h>
#include<Mmsystem.h>
#pragma comment(lib,"winmm.lib")
using namespace std;
void main()
{
	PlaySound(TEXT("D:\\msg.wav"),NULL,SND_FILENAME | SND_ASYNC);
	getchar();/*自己改了一下，不知道对不对，反正没报错*/
}
