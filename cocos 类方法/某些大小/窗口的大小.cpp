void CCEGLView::resize(int width, int height)
{
	//获取窗口的客户区大小矩形，这个客户区其实就是咱们OpenGL视窗的实际大小，不包含一般窗口的菜单，边框，状态栏等部分。

	//窗口句柄有效性判断
    if (! m_hWnd)
    {
        return;
    }

    RECT rcWindow;
	/// 这时候得到的数据都是系统默认的
    GetWindowRect(m_hWnd, &rcWindow);
	/// 这时候得到的数据都是系统默认的
    RECT rcClient;
    GetClientRect(m_hWnd, &rcClient);

    // calculate new window width and height
    POINT ptDiff;
    ptDiff.x = (rcWindow.right - rcWindow.left) - rcClient.right;
    ptDiff.y = (rcWindow.bottom - rcWindow.top) - rcClient.bottom;

	/// 根据设置客户窗口大小eglView->setFrameSize(2048, 1536); 这里设置的数字是客户窗口大小，调整客户窗口的位置
    rcClient.right = rcClient.left + width;
    rcClient.bottom = rcClient.top + height;

	/// 获得我们设置的客户窗口大小；
    const CCSize& frameSize = getFrameSize();
    if (frameSize.width > 0)
    {
        WCHAR wszBuf[MAX_PATH] = {0};
#ifdef _DEBUG
        char szBuf[MAX_PATH + 1];
        memset(szBuf, 0, sizeof(szBuf));
        snprintf(szBuf, MAX_PATH, "%s - %0.0fx%0.0f - %0.2f",
                   m_szViewName, frameSize.width, frameSize.height, m_fFrameZoomFactor);
        MultiByteToWideChar(CP_UTF8, 0, szBuf, -1, wszBuf, sizeof(wszBuf));
#else
        MultiByteToWideChar(CP_UTF8, 0, m_szViewName, -1, wszBuf, sizeof(wszBuf));
#endif
        SetWindowText(m_hWnd, wszBuf);
    }
	/// 根据客户区大小来计算整体窗口大小；函数返回时，会改变rcClient值使整体窗口可以容纳客户区大小；
    AdjustWindowRectEx(&rcClient, GetWindowLong(m_hWnd, GWL_STYLE), FALSE, GetWindowLong(m_hWnd, GWL_EXSTYLE));

    // change width and height
    SetWindowPos(m_hWnd, 0, 0, 0, width + ptDiff.x, height + ptDiff.y,
                 SWP_NOCOPYBITS | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
}