使用时，只需在函数第一行，添加“LOG_FUNCTION_LIFE”即可
eg：
void AppDelegate::applicationWillEnterForeground()
{
	LOG_FUNCTION_LIFE

    CCDirector::sharedDirector()->resume();
    
    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}