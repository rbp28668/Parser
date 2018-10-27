#include <windows.h>
unsigned int GetStack()
{
	unsigned int uiStack;
	_asm
	{
		mov uiStack,ESP
	}
  return uiStack;
}

void ShowStack(char* pszMsg)
{
  char szBuff[256];
  wsprintf(szBuff,"%s %u\n",pszMsg,GetStack());
  OutputDebugString(szBuff);
}