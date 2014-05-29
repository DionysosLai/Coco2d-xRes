/****************************************************************************
Copyright (c) 2010 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#define __CC_PLATFORM_IMAGE_CPP__
#include "platform/CCImageCommon_cpp.h"

NS_CC_BEGIN

/**
@brief    A memory DC which uses to draw text on bitmap.
*/
//此处定义一个BitmapDC类在位图上进行文字绘制。
class BitmapDC
{
public:
	//构造函数
    BitmapDC(HWND hWnd = NULL)
    : m_hDC(NULL)
    , m_hBmp(NULL)
    , m_hFont((HFONT)GetStockObject(DEFAULT_GUI_FONT))
    , m_hWnd(NULL)
    {
		//保存窗口句柄
        m_hWnd = hWnd;
		//取得窗口的hdc
        HDC hdc = GetDC(hWnd);
		//创建兼容的hdc
        m_hDC   = CreateCompatibleDC(hdc);
		//释放hdc 
        ReleaseDC(hWnd, hdc);
    }
	//析构函数
    ~BitmapDC()
    {
        prepareBitmap(0, 0);
        if (m_hDC)
        {
            DeleteDC(m_hDC);
        }
		//创建字体
        HFONT hDefFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
        if (hDefFont != m_hFont)
        {
            DeleteObject(m_hFont);
            m_hFont = hDefFont;
        }
		// release temp font resource	
		if (m_curFontPath.size() > 0)
		{
			wchar_t * pwszBuffer = utf8ToUtf16(m_curFontPath);
			if (pwszBuffer)
			{
				RemoveFontResource(pwszBuffer);
				SendMessage( m_hWnd, WM_FONTCHANGE, 0, 0);
				delete [] pwszBuffer;
				pwszBuffer = NULL;
			}
		}
    }
	//多字节转宽字符
	wchar_t * utf8ToUtf16(std::string nString)
	{
		wchar_t * pwszBuffer = NULL;
		do 
		{
			if (nString.size() < 0)
			{
				break;
			}
			// utf-8 to utf-16
			int nLen = nString.size();
			int nBufLen  = nLen + 1;			
			pwszBuffer = new wchar_t[nBufLen];
			CC_BREAK_IF(! pwszBuffer);
			memset(pwszBuffer,0,nBufLen);
			nLen = MultiByteToWideChar(CP_UTF8, 0, nString.c_str(), nLen, pwszBuffer, nBufLen);		
			pwszBuffer[nLen] = '\0';
		} while (0);	
		return pwszBuffer;

	}
	//设置使用的字体和大小
    bool setFont(const char * pFontName = NULL, int nSize = 0)
    {
        bool bRet = false;
        do 
        {
			//创建字体
            std::string fontName = pFontName;
            std::string fontPath;
            HFONT       hDefFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
            LOGFONTA    tNewFont = {0};
            LOGFONTA    tOldFont = {0};
            GetObjectA(hDefFont, sizeof(tNewFont), &tNewFont);
            if (fontName.c_str())
            {    
				// 如果字体名称是ttf文件，取得其全路径名
                // create font from ttf file
                int nFindttf = fontName.find(".ttf");
                int nFindTTF = fontName.find(".TTF");
                if (nFindttf >= 0 || nFindTTF >= 0)
                {
                    fontPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(fontName.c_str());
                    int nFindPos = fontName.rfind("/");
                    fontName = &fontName[nFindPos+1];
                    nFindPos = fontName.rfind(".");
                    fontName = fontName.substr(0,nFindPos);                
                }
                tNewFont.lfCharSet = DEFAULT_CHARSET;
                strcpy_s(tNewFont.lfFaceName, LF_FACESIZE, fontName.c_str());
            }
            if (nSize)
            {
                tNewFont.lfHeight = -nSize;
            }
            GetObjectA(m_hFont,  sizeof(tOldFont), &tOldFont);

            if (tOldFont.lfHeight == tNewFont.lfHeight
                && 0 == strcmp(tOldFont.lfFaceName, tNewFont.lfFaceName))
            {
                // already has the font 
                bRet = true;
                break;
            }

            // delete old font// 删除旧的字体
            if (m_hFont != hDefFont)
            {
                DeleteObject(m_hFont);
				// release old font register
				if (m_curFontPath.size() > 0)
				{
					wchar_t * pwszBuffer = utf8ToUtf16(m_curFontPath);
					if (pwszBuffer)
					{
						if(RemoveFontResource(pwszBuffer))
						{
							SendMessage( m_hWnd, WM_FONTCHANGE, 0, 0);
						}						
						delete [] pwszBuffer;
						pwszBuffer = NULL;
					}
				}
				if (fontPath.size() > 0)
					m_curFontPath = fontPath;
				else
					m_curFontPath.clear();
				// register temp font
				if (m_curFontPath.size() > 0)
				{
					wchar_t * pwszBuffer = utf8ToUtf16(m_curFontPath);
					if (pwszBuffer)
					{
						if(AddFontResource(pwszBuffer))
						{
							SendMessage( m_hWnd, WM_FONTCHANGE, 0, 0);
						}						
						delete [] pwszBuffer;
						pwszBuffer = NULL;
					}
				}
            }
            m_hFont = NULL;

            // disable Cleartype// 字体不使用锐利字体
            tNewFont.lfQuality = ANTIALIASED_QUALITY;

            // create new font// 创建新字体
            m_hFont = CreateFontIndirectA(&tNewFont);
            if (! m_hFont)
            {
                // create failed, use default font
                m_hFont = hDefFont;
                break;
            }
            
            bRet = true;
        } while (0);
        return bRet;
    }
	//取得使用当前字体写出的字符串所占据的图像大小。 
    SIZE sizeWithText(const wchar_t * pszText, int nLen, DWORD dwFmt, LONG nWidthLimit)
    {
        SIZE tRet = {0};
        do 
        {
            CC_BREAK_IF(! pszText || nLen <= 0);

            RECT rc = {0, 0, 0, 0};
            DWORD dwCalcFmt = DT_CALCRECT;

            if (nWidthLimit > 0)
            {
                rc.right = nWidthLimit;
                dwCalcFmt |= DT_WORDBREAK
                    | (dwFmt & DT_CENTER)
                    | (dwFmt & DT_RIGHT);
            }
            // use current font to measure text extent// 使用当前字体。
            HGDIOBJ hOld = SelectObject(m_hDC, m_hFont);

            // measure text size// 写字。
            DrawTextW(m_hDC, pszText, nLen, &rc, dwCalcFmt);
            SelectObject(m_hDC, hOld);

            tRet.cx = rc.right;
            tRet.cy = rc.bottom;
        } while (0);

        return tRet;
    }
	//创建一个指定大小的位图。
    bool prepareBitmap(int nWidth, int nHeight)
    {
        // release bitmap// 释放原来的位图
        if (m_hBmp)
        {
            DeleteObject(m_hBmp);
            m_hBmp = NULL;
        }//如果大小有效则创建位图。
        if (nWidth > 0 && nHeight > 0)
        {
            m_hBmp = CreateBitmap(nWidth, nHeight, 1, 32, NULL);
            if (! m_hBmp)
            {
                return false;
            }
        }
        return true;
    }
	//在指定位置按照指定对齐方式写字。  
    //参1:字符串  
    //参2:指定拉置及大小  
    //参3:文字对齐方式 
    int drawText(const char * pszText, SIZE& tSize, CCImage::ETextAlign eAlign)
    {
        int nRet = 0;
        wchar_t * pwszBuffer = 0;
        do 
        {
            CC_BREAK_IF(! pszText);

            DWORD dwFmt = DT_WORDBREAK;
            DWORD dwHoriFlag = eAlign & 0x0f;
            DWORD dwVertFlag = (eAlign & 0xf0) >> 4;
			//设置横向对齐方式。
            switch (dwHoriFlag)
            {
            case 1: // left 左对齐
                dwFmt |= DT_LEFT;
                break;
            case 2: // right
                dwFmt |= DT_RIGHT;
                break;
            case 3: // center
                dwFmt |= DT_CENTER;
                break;
            }
			//取得字符串的字节长度
            int nLen = strlen(pszText);
            // utf-8 to utf-16
            int nBufLen  = nLen + 1;
			// 为转换后的宽字符串申请内存地址。
            pwszBuffer = new wchar_t[nBufLen];
            CC_BREAK_IF(! pwszBuffer);
			//内存数据置零 
            memset(pwszBuffer, 0, sizeof(wchar_t)*nBufLen);
			// 将多字节转宽字符串。
            nLen = MultiByteToWideChar(CP_UTF8, 0, pszText, nLen, pwszBuffer, nBufLen);
			//取得写字符串占据的图像区域大小
            SIZE newSize = sizeWithText(pwszBuffer, nLen, dwFmt, tSize.cx);
			//建立RECT变量做为实际绘制占据的图像区域大小 
            RECT rcText = {0};
            // if content width is 0, use text size as content size
			// 如果宽度为0，则使用显示字符串所需的图像大小。
            if (tSize.cx <= 0)
            {
                tSize = newSize;
                rcText.right  = newSize.cx;
                rcText.bottom = newSize.cy;
            }
            else
            {

                LONG offsetX = 0;
                LONG offsetY = 0;
                rcText.right = newSize.cx; // store the text width to rectangle
				//根据对齐方式计算横向偏移。
                // calculate text horizontal offset
                if (1 != dwHoriFlag          // and text isn't align to left
                    && newSize.cx < tSize.cx)   // and text's width less then content width,
                {                               // then need adjust offset of X.
                    offsetX = (2 == dwHoriFlag) ? tSize.cx - newSize.cx     // align to right
                        : (tSize.cx - newSize.cx) / 2;                      // align to center
                }

                // if content height is 0, use text height as content height
                // else if content height less than text height, use content height to draw text
				//如果指定矩形高度为0，则使用显示字符串所需的图像高度。
                if (tSize.cy <= 0)
                {
                    tSize.cy = newSize.cy;
                    dwFmt   |= DT_NOCLIP;
                    rcText.bottom = newSize.cy; // store the text height to rectangle
                }
                else if (tSize.cy < newSize.cy)
                {
                    // content height larger than text height need, clip text to rect
                    rcText.bottom = tSize.cy;
                }
                else
                {
                    rcText.bottom = newSize.cy; // store the text height to rectangle

                    // content larger than text, need adjust vertical position
                    dwFmt |= DT_NOCLIP;

                    // calculate text vertical offset
					//根据对齐方式计算纵向偏移。
                    offsetY = (2 == dwVertFlag) ? tSize.cy - newSize.cy     // align to bottom
                        : (3 == dwVertFlag) ? (tSize.cy - newSize.cy) / 2   // align to middle
                        : 0;                                                // align to top
                }
				//如果需要,调整偏移。
                if (offsetX || offsetY)
                {
                    OffsetRect(&rcText, offsetX, offsetY);
                }
            }
			//创建相应大小的位图。
            CC_BREAK_IF(! prepareBitmap(tSize.cx, tSize.cy));

            // draw text// 使用当前字体和位图
            HGDIOBJ hOldFont = SelectObject(m_hDC, m_hFont);
            HGDIOBJ hOldBmp  = SelectObject(m_hDC, m_hBmp);
            //设置背景透明模式和写字的颜色
            SetBkMode(m_hDC, TRANSPARENT);
            SetTextColor(m_hDC, RGB(255, 255, 255)); // white color

            // draw text//写字
            nRet = DrawTextW(m_hDC, pwszBuffer, nLen, &rcText, dwFmt);
            //DrawTextA(m_hDC, pszText, nLen, &rcText, dwFmt);

            SelectObject(m_hDC, hOldBmp);
            SelectObject(m_hDC, hOldFont);
        } while (0);
		//释放内存 
        CC_SAFE_DELETE_ARRAY(pwszBuffer);
        return nRet;
    }
	//成员变量m_hDC及get接口 
    CC_SYNTHESIZE_READONLY(HDC, m_hDC, DC);
	//成员变量m_hBmp及get接口
    CC_SYNTHESIZE_READONLY(HBITMAP, m_hBmp, Bitmap);
private:
	//友元类CCImage 
    friend class CCImage;
	//成员变量m_hFont代表字体
    HFONT   m_hFont;
	//成员变量m_hWnd代表当前窗口句柄
    HWND    m_hWnd;
	//成员m_curFontPath代表当前字体ttf文件全路径。
    std::string m_curFontPath;
};
//取得单例BitmapDC 
static BitmapDC& sharedBitmapDC()
{
    static BitmapDC s_BmpDC;
    return s_BmpDC;
}
//CCImage的成员函数，使用相应的字体写字符串生成图像数据。  
//参1:字符串  
//参2:要创建的图片宽度，如果填0，则按照字符串的宽度进行设置。  
//参3:要创建的图片高度，如果填0，则按照字符串的高度进行设置。  
//参4:文字的对齐方式。  
//参5:字体名称  
//参6:字体大小 
bool CCImage::initWithString(
                               const char *    pText, 
                               int             nWidth/* = 0*/, 
                               int             nHeight/* = 0*/,
                               ETextAlign      eAlignMask/* = kAlignCenter*/,
                               const char *    pFontName/* = nil*/,
                               int             nSize/* = 0*/)
{
    bool bRet = false;
    do 
    {
        CC_BREAK_IF(! pText);       
		//取得单例BitmapDC
        BitmapDC& dc = sharedBitmapDC();
		//设置使用的字体和大小S
        if (! dc.setFont(pFontName, nSize))
        {
            CCLog("Can't found font(%s), use system default", pFontName);
        }

        // draw text// 写字
        SIZE size = {nWidth, nHeight};
        CC_BREAK_IF(! dc.drawText(pText, size, eAlignMask));
		//申请图像大小的内存，成功申请后将其地址返回给指针pImageData。
        m_pData = new unsigned char[size.cx * size.cy * 4];
        CC_BREAK_IF(! m_pData);
		//创建位图头信息结构
        struct
        {
            BITMAPINFOHEADER bmiHeader;
            int mask[4];
        } bi = {0};
        bi.bmiHeader.biSize = sizeof(bi.bmiHeader);
		//取得写字符串的位图像素
        CC_BREAK_IF(! GetDIBits(dc.getDC(), dc.getBitmap(), 0, 0, 
            NULL, (LPBITMAPINFO)&bi, DIB_RGB_COLORS));

        m_nWidth    = (short)size.cx;
        m_nHeight   = (short)size.cy;
        m_bHasAlpha = true;
        m_bPreMulti = false;
        m_nBitsPerComponent = 8;
        // copy pixed data// 位图像素拷到pImageData中。
        bi.bmiHeader.biHeight = (bi.bmiHeader.biHeight > 0)
           ? - bi.bmiHeader.biHeight : bi.bmiHeader.biHeight;
        GetDIBits(dc.getDC(), dc.getBitmap(), 0, m_nHeight, m_pData, 
            (LPBITMAPINFO)&bi, DIB_RGB_COLORS);

        // change pixel's alpha value to 255, when it's RGB != 0
		// 双循环遍历每个像素。取得R，G，B值组成4字节的RGBA值保存。
        COLORREF * pPixel = NULL;
        for (int y = 0; y < m_nHeight; ++y)
        {
            pPixel = (COLORREF *)m_pData + y * m_nWidth;
            for (int x = 0; x < m_nWidth; ++x)
            {
                COLORREF& clr = *pPixel;

                clr |= (0xffffff | (GetRValue(clr) << 24));
                ++pPixel;
            }
        }

        bRet = true;
    } while (0);

    return bRet;
}

NS_CC_END

/********************************
*
CCImage类的代码都解析完了，其内部并没有对png,jpg,tiff做真正的解析，只是利用
第三方的库进行相应的处理。经过处理后，CCImage会被图片文件的数据读取转换为Cocos2d-x
所用的图像数据，存储在m_pData中。如果我们不是使用这些图片文件，那么我们就需要自已增
加接口函数进行相应的处理。比如有时候做游戏不希望使用标准的图像格式以防止别人能够很容
易打开图片进行查看，或者有时候将多个图片组成一个数据包。

例如：

          我们想读取一个自定义二进制图片数据(假设为Pic格式)

          (1)我们可以先定义一个新的图片类型kFmtPic,

          (2)然后定义函数

                    bool _initWithPicData(void *pData, int nDatalen);

          在其中将自定义图片文件数据pData解析到m_pData中,并设置一些属性变量。

         (3) 然后在initWithImageData函数中加入:

[cpp] view plaincopy
else if (kFmtPic == eFmt)  
  
   //读取自定义的图片数据。  
   bRet = _initWithPicData(pData, nDataLen);  
   break;  


          这样我们就可以让CCImage支持将我们自已定义的图像数据包加载为Cocos2d-x支持的图像类型了。
********************************/