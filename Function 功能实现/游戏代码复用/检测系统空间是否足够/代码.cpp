win32:
///@brief 检测sd卡是否可用容量足够
///@param[in] size---检测容量  注意：这里是已MB来就算
///@return true---可用容量足够， false---可用容量不够
	bool checkAvailableSDSize(const unsigned int& size);
	
bool HomeScene::checkAvailableSDSize( const unsigned int& size )
{
	bool availableOk = false;
	/// 目前，只做android平台检测，其他平台一律默认通过
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo methodInfo;
	jint ret = 0;
	if (JniHelper::getStaticMethodInfo(methodInfo, 
		"com/xtc/enchristmas/ENChristmas",
		"checkAvailableSDSize", 
		"(I)I"))
	{
		ret = methodInfo.env->CallStaticIntMethod(methodInfo.classID, methodInfo.methodID, 
			size);
		if (0 == ret)
		{
			CCLOG("sd's state is abnormal!");
			availableOk = false;
		}
		else if (1 == ret )
		{
			CCLOG("sd's availabel size is not enough!");
			availableOk = false;
		}
		else if (2 == ret )
		{
			CCLOG("sd's availabel size is enough!");
			availableOk = true;
		}
		else
		{
			CCAssert(false, "There is some wrong");
		}
	}
#else
	availableOk = true;			 
#endif
	return availableOk;
}

java:
 /*
     * 检测sd卡是否可用容量足够
     */
    public static int checkAvailableSDSize(int size){
    	/// 先检测SD卡是否可用
    	String state = Environment.getExternalStorageState(); 
        if(!Environment.MEDIA_MOUNTED.equals(state)){
        	/// 对sd卡上的存储可以进行读/写操作
        	Log.d("DEBUG", "SD's state is abnormal!");
        	return 0;
        }
        /// 获取sd卡用容量
        File path = Environment.getExternalStorageDirectory(); //取得sdcard文件路径
        StatFs stat = new StatFs(path.getPath());       
        long blockSize = stat.getBlockSize();      
        long availableBlocks = stat.getAvailableBlocks();
        long availableSize = (availableBlocks * blockSize)/1024/1024;
        Log.d("DEBUG", "可用空间:" + availableSize + "Mb");
        /// 判断容量是否足够
        if(size > availableSize){
        	Log.d("DEBUG", "The sd's availavle size is not enough!");
        	return 1;
        }
        /// 容量足够
        Log.d("DEBUG", "The sd's availavle size is enough!");
        return 2;
    }