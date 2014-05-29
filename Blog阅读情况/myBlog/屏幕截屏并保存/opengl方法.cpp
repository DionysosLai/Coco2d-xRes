///@brief 使用opengl来截屏--实现全屏截屏
///@param[in] upsidedown---是否图片要上下旋转（false--上下颠倒）,path--保存的路径
///@return 保存路径为"/sdcard/DCIM/Camera/path"; 
///@author DionysosLai,906391500@qq.com 
///@retval  
///@post 
///@version 1.0 
///@data 2014-5-27 10:33
unsigned char screenBuffer[1024 * 1024 * 8];	///< 这个大小要根据屏幕尺寸来定		
bool GameScene::screenPlot1(bool upsidedown, string path)
{	
	CCSize winSize = CCDirector::sharedDirector()->getWinSizeInPixels();
	int w = winSize.width;
	int h = winSize.height;
	int  myDataLength = w * h * 4;

	GLubyte* buffer = screenBuffer;
	/// 获取屏幕像素
	glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

	CCImage* image = new CCImage();
	if(upsidedown)
	{
		GLubyte* buffer2 = (GLubyte*) malloc(myDataLength);
		/// 像素数据倒序排列
		for(int y = 0; y <h; y++)
		{
			for(int x = 0; x <w * 4; x++)
			{
				buffer2[(h - 1 - y) * w * 4 + x] = buffer[y * 4 * w + x];
			}
		}
		image->initWithImageData(buffer2, myDataLength, CCImage::kFmtRawData, w, h);
		free(buffer2);
	}
	else 
	{
		image->initWithImageData(buffer, myDataLength, CCImage::kFmtRawData, w, h);
	}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	string filePath = "/sdcard/DCIM/Camera/" + path; 
	CCLOG("wanna save file path = %s",filePath.c_str());
	image->saveToFile(filePath.c_str());
	image->release();
	return true;
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	string filePath = CCFileUtils::sharedFileUtils()->getWritablePath() + path; 
	CCLOG("wanna save file path = %s",filePath.c_str());
	image->saveToFile(filePath.c_str());
	image->release();
	return true;
#endif
}