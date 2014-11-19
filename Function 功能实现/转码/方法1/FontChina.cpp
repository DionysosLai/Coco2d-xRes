
#include "FontChina.h"
#include "cocos2d.h"
USING_NS_CC;

FontChina::FontChina(void)
{
}


FontChina::~FontChina(void)
{
}

const char* FontChina::G2U(const char* gb2312)    
{    
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	int len = MultiByteToWideChar(0, 0, gb2312, -1, NULL, 0);    
	wchar_t* wstr = new wchar_t[len+1];    
	memset(wstr, 0, len+1);    
	MultiByteToWideChar(0, 0, gb2312, -1, wstr, len);    
	len = WideCharToMultiByte(65001, 0, wstr, -1, NULL, 0, NULL, NULL);    
	char* str = new char[len+1];    
	memset(str, 0, len+1);    
	WideCharToMultiByte(65001, 0, wstr, -1, str, len, NULL, NULL);    
	if(wstr) delete[] wstr;    
	return str;    
	#endif

	#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	return gb2312; 
	#endif
}  