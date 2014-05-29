///@brief 使用纹理来截屏--实现全屏截屏
///@param[in] r0--要截取屏幕左上角 r1--左下角 path--文件保存名字
///@return 保存路径为"/sdcard/DCIM/Camera/path"; 
///@author DionysosLai,906391500@qq.com 
///@retval  
///@post 
///@version 1.0 
///@data 2014-5-27 10:41
bool GameScene::screenPlot2(const CCPoint r0, const CCPoint r1, string path )
{
	/// 判断r0 、r1能否组成一个矩形
	if (r0.x == r1.x || r0.y == r1.y)
	{
		return false;
	}

	//根据要截取屏幕大小，定义一个渲染纹理  
	CCRenderTexture* renderTexture = CCRenderTexture::create(abs(r0.x - r1.x), abs(r0.y - r1.y));

	CCScene* pCurScene = CCDirector::sharedDirector()->getRunningScene();
	/*	CCScene*/
	CCPoint ancPos = pCurScene->getAnchorPoint();
	////
	// 在这里你可以做一些屏幕操作，比例缩小、旋转等。
	//
	///
	
	//渲染纹理开始捕捉  
	renderTexture->begin();

	//绘制当前场景  
	pCurScene->visit();
	//结束  
	renderTexture->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	string filePath = "/sdcard/DCIM/Camera/" + path; 
	CCLOG("wanna save file path = %s",filePath.c_str());
	renderTexture->saveToFile(filePath.c_str());
	return true;
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	string filePath = CCFileUtils::sharedFileUtils()->getWritablePath() + path; 
	CCLOG("wanna save file path = %s",filePath.c_str());
	renderTexture->saveToFile(filePath.c_str());
	return true;
#endif
}