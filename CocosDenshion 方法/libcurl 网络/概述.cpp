2014-02-28
	Cocos2D-x中使用libcurl的c语音接口。如果要在程序中使用libcurl，首先要进行全局的
初始化，这些初始化一般在程序的开始调用初始化函数，并且只调用一次。使用curl_global_init
函数进行全局初始化，该函数通过参数来决定，参数和其对应描述如下：E:\Cocos2d-x\cocos2d-x-2.2.2\cocos2dx\platform\third_party\win32\curl
#define CURL_GLOBAL_SSL (1<<0)		///< 是libcurl有SSL(Secure Sockets Layer, 安全套接层)的特征，SSL可
									///< 确保数据在网络传输中，不会被窃听和和截取。
#define CURL_GLOBAL_WIN32 (1<<1)	///< 在windows操作系统上，libcurl初始化win32套接字的内容。
									///< 没有正确的初始化，你的程序将不能正确的使用套接字
#define CURL_GLOBAL_ALL (CURL_GLOBAL_SSL|CURL_GLOBAL_WIN32) ///< 使用默认参数初始化所有已知的内部模块
#define CURL_GLOBAL_NOTHING 0
#define CURL_GLOBAL_DEFAULT CURL_GLOBAL_ALL

如果在curl_easy_perform之前调用curl_global_init 函数进行全局初始化，那么出现会自动地使用默认的初始化模式来执行。