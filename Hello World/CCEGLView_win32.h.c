bool CCEGLView::Create(LPCTSTR pTitle, int w, int h)  
{  
	//定义一个bool型返回值  
	bool bRet = false;  
	do   
	{  
		//检测窗口句柄是否已经存在。确保只创建一个窗口  
		CC_BREAK_IF(m_hWnd);  
		//进入到这里，我们应该很高兴了。没错，终于找到了创建窗口的代码。  
		HINSTANCE hInstance = GetModuleHandle( NULL );  
		WNDCLASS  wc;// 窗口类  
		// 设置相关参数  
		wc.style          = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;    
		wc.lpfnWndProc    = _WindowProc;// 本窗口使用的WINDOWS消息处理函数  
		wc.cbClsExtra     = 0;                                
		wc.cbWndExtra     = 0;  
		wc.hInstance      = hInstance;  
		wc.hIcon          = LoadIcon( NULL, IDI_WINLOGO );  
		wc.hCursor        = LoadCursor( NULL, IDC_ARROW );  
		wc.hbrBackground  = NULL;                             
		wc.lpszMenuName   = NULL;                             
		wc.lpszClassName  = kWindowClassName;                 
		//注册窗口类,如果失败则判断失败原因是否是此类别已存在(错误码1410)，如果是因为此类存在而导致失败，则仍然可以继续。  
		CC_BREAK_IF(! RegisterClass(&wc) && 1410 != GetLastError());  
		// 取得窗口的窗体矩形  
		RECT rcDesktop;  
		GetWindowRect(GetDesktopWindow(), &rcDesktop);  
		// 调用创建口函数  
		m_hWnd = CreateWindowEx(  
				WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,  
				kWindowClassName,// 之前注册的窗口类  
				pTitle,// 窗口标题  
				WS_CAPTION | WS_POPUPWINDOW | WS_MINIMIZEBOX,// 窗体样式  
				0, 0,              // 窗体位置  
				0,                                                  // 窗体宽度  
				0,                                                  // 窗体高度  
				NULL,// 无父窗口  
				NULL,// 无菜单  
				hInstance,// 程序句柄  
				NULL );  
		//判断窗口是否创建成功  
		CC_BREAK_IF(! m_hWnd);  
	   //取得显示设备的摆放方向  
			m_eInitOrientation = CCDirector::sharedDirector()->getDeviceOrientation();  
	   //通过对摆放方向的判断得出水平还是垂直  
		m_bOrientationInitVertical = (CCDeviceOrientationPortrait == m_eInitOrientation  
			|| kCCDeviceOrientationPortraitUpsideDown == m_eInitOrientation) ? true : false;  
		m_tSizeInPoints.cx = w;  
		m_tSizeInPoints.cy = h;  
	   //设置窗口大小  
		resize(w, h);  
		// 使用此窗口进行OpenGL的设置  
		m_pEGL = CCEGL::create(this);  
		// 如果OpenGL创建失败,销毁窗体并中断  
		if (! m_pEGL)  
		{  
			DestroyWindow(m_hWnd);  
			m_hWnd = NULL;  
			break;  
		}  
			//将静态指针设置为当前实例对象。  
			s_pMainWindow = this;  
			bRet = true;  
	} while (0);  
	return bRet;  
}  

static LRESULT CALLBACK _WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)  
{  
	//如果OpenGL窗体初始化完成，则由OpenGL窗体进行消息处理，否则由WINDOWS系统进行默认处理  
	if (s_pMainWindow && s_pMainWindow->getHWnd() == hWnd)  
	{  
		return s_pMainWindow->WindowProc(uMsg, wParam, lParam);  
	}  
	else  
	{  
		return DefWindowProc(hWnd, uMsg, wParam, lParam);  
	}  
}  

LRESULT CCEGLView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)  
{  
	PAINTSTRUCT ps;  
	switch (message)  
	{…//其它消息暂不理，可自行分析  
	case WM_PAINT:  
		//这里是刷新屏幕，因为使用了OpenGL来进行绘制，所以这里不作任何处理。  
		BeginPaint(m_hWnd, &ps);  
		EndPaint(m_hWnd, &ps);  
		break;  
	case WM_CLOSE:  
		//调用单件显示设备管理器的end函数关闭窗口  
		CCDirector::sharedDirector()->end();  
		break;  
	case WM_DESTROY:  
		//向消息循环发送WM_QUIT消息退出程序  
		PostQuitMessage(0);  
		break;  
	default:  
		return DefWindowProc(m_hWnd, message, wParam, lParam);  
	}  
	return 0;  
} 

void CCEGLView::release()  
{//销毁窗体  
if (m_hWnd)  
{  
DestroyWindow(m_hWnd);  
m_hWnd = NULL;  
}  
s_pMainWindow = NULL;  
//注销所使用的窗口类  
UnregisterClass(kWindowClassName, GetModuleHandle(NULL));  
    //释放使用到的指针  
    CC_SAFE_DELETE(m_pSet);  
    CC_SAFE_DELETE(m_pTouch);  
    CC_SAFE_DELETE(m_pDelegate);  
CC_SAFE_DELETE(m_pEGL);  
//关键点：在最后释放了自已在AppDelegate::initInstance()中通过new创建对象而占用的内存。  
    delete this;  
}  