void CCDirector::end()  
{  
	m_bPurgeDirecotorInNextLoop = true;  
}  

void CCDisplayLinkDirector::mainLoop(void)  
{//如果清除显示设备开关为true  
	if (m_bPurgeDirecotorInNextLoop)  
	{  
		//清楚设备  
		purgeDirector(); 	 
		m_bPurgeDirecotorInNextLoop = false;  
	}//否则判断显示设备是否有效  
	else if (! m_bInvalid)  
	 {  
		//如果有效，绘制场景  
		drawScene();  
		   
		//调用内存管理器释放其管理的内存  
		CCPoolManager::getInstance()->pop();  
	 }  
} 

void CCDirector::purgeDirector()  
{  
	…//其它暂不理。有兴趣的朋友请自行分析  
	// 调用Opengl窗口管理实例对象的release函数进行释放。  
	m_pobOpenGLView->release();  
	m_pobOpenGLView = NULL;  
}  