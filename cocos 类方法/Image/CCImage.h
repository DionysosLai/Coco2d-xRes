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

#ifndef __CC_IMAGE_H__
#define __CC_IMAGE_H__

#include "cocoa/CCObject.h"

NS_CC_BEGIN

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
class CCFreeTypeFont;
#endif

/**
 * @addtogroup platform
 * @{
 */

class CC_DLL CCImage : public CCObject
{
public:
    /**
     @js ctor
     */
    CCImage();
    /**
     * @js NA
     * @lua NA
     */
    ~CCImage();

	typedef enum  
    {     
        kFmtJpg = 0,    //JPG  
        kFmtPng,        //PNG  
        kFmtTiff,       //TIFF  
        kFmtRawData,    //数据流,要求为RGBA8888  
        kFmtUnKnown //无效  
    }EImageFormat;  
	// 对齐方式
    typedef enum
    {
        kAlignCenter        = 0x33, ///< Horizontal center and vertical center. 横纵向居中
        kAlignTop           = 0x13, ///< Horizontal center and vertical top.    横向居中，纵向居上.
        kAlignTopRight      = 0x12, ///< Horizontal right and vertical top.
        kAlignRight         = 0x32, ///< Horizontal right and vertical center.
        kAlignBottomRight   = 0x22, ///< Horizontal right and vertical bottom.
        kAlignBottom        = 0x23, ///< Horizontal center and vertical bottom.
        kAlignBottomLeft    = 0x21, ///< Horizontal left and vertical bottom.
        kAlignLeft          = 0x31, ///< Horizontal left and vertical center.
        kAlignTopLeft       = 0x11, ///< Horizontal left and vertical top.
    }ETextAlign;
    
    /**
    @brief  Load the image from the specified path. 
    @param strPath   the absolute file path.
    @param imageType the type of image, currently only supporting two types.
    @return  true if loaded correctly.
    */
	//从指定的路径载入一个所支持的格式的图片文件。
    bool initWithImageFile(const char * strPath, EImageFormat imageType = kFmtPng);

    /*
     @brief The same result as with initWithImageFile, but thread safe. It is caused by
            loadImage() in CCTextureCache.cpp.
     @param fullpath  full path of the file.
     @param imageType the type of image, currently only supporting two types.
     @return  true if loaded correctly.
     *///从指定的路径载入一个所支持的格式的图片文件,但它是线程安全的,因此可以用在多线程加载图片。
    bool initWithImageFileThreadSafe(const char *fullpath, EImageFormat imageType = kFmtPng);

    /**
    @brief  Load image from stream buffer.

    @warning kFmtRawData only supports RGBA8888.
    @param pBuffer  stream buffer which holds the image data.
    @param nLength  data length expressed in (number of) bytes.
    @param nWidth, nHeight, nBitsPerComponent are used for kFmtRawData.
    @return true if loaded correctly.
    @js NA
    */  //从内存中加载图片数据。  
    //参1:指向图片数据所处内存地址的指针。  
    //参2:图片数据长度  
    //参3:数据对应图片的格式，  
    //参4:数据对应图片的宽度  
    //参5:数据对应图片的高度  
    //参6:每像素的字节位数,即色深。
    bool initWithImageData(void * pData, 
                           int nDataLen, 
                           EImageFormat eFmt = kFmtUnKnown,
                           int nWidth = 0,
                           int nHeight = 0,
                           int nBitsPerComponent = 8);

    /**
    @brief    Create image with specified string.
    @param  pText       the text the image will show (cannot be nil).
    @param  nWidth      the image width, if 0, the width will match the text's width.
    @param  nHeight     the image height, if 0, the height will match the text's height.
    @param  eAlignMask  the test Alignment
    @param  pFontName   the name of the font used to draw the text. If nil, use the default system font.
    @param  nSize       the font size, if 0, use the system default size.
    @js NA
    */ //从字符串创建图片数据。  
    //参1:字符串  
    //参2:要创建的图片宽度，如果填0，则按照字符串的宽度进行设置。  
    //参3:要创建的图片高度，如果填0，则按照字符串的高度进行设置。  
    //参4:文字的对齐方式。  
    //参5:字体名称  
    //参6:字体大小  
    bool initWithString(
        const char *    pText, 
        int             nWidth = 0, 
        int             nHeight = 0,
        ETextAlign      eAlignMask = kAlignCenter,
        const char *    pFontName = 0,
        int             nSize = 0);
    
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        /**
         * @js NA
         * @lua NA
         */
        bool initWithStringShadowStroke(
                                            const char *    pText,
                                            int             nWidth      = 0,
                                            int             nHeight     = 0,
                                            ETextAlign      eAlignMask  = kAlignCenter,
                                            const char *    pFontName   = 0,
                                            int             nSize       = 0,
                                            float           textTintR   = 1,
                                            float           textTintG   = 1,
                                            float           textTintB   = 1,
                                            bool shadow                 = false,
                                            float shadowOffsetX         = 0.0,
                                            float shadowOffsetY         = 0.0,
                                            float shadowOpacity         = 0.0,
                                            float shadowBlur            = 0.0,
                                            bool  stroke                =  false,
                                            float strokeR               = 1,
                                            float strokeG               = 1,
                                            float strokeB               = 1,
                                            float strokeSize            = 1
                                        
                                        );
    
    #endif
    
		//取得图像数据地址
    unsigned char *   getData()               { return m_pData; }
	//取得图像数据的长度
    int               getDataLen()            { return m_nWidth * m_nHeight; }

	//是否有Alpha通道。
    bool hasAlpha()                     { return m_bHasAlpha;   }
	//是否有Alpha渐变 
    bool isPremultipliedAlpha()         { return m_bPreMulti;   }


    /**
    @brief    Save CCImage data to the specified file, with specified format.
    @param    pszFilePath        the file's absolute path, including file suffix.
    @param    bIsToRGB        whether the image is saved as RGB format.
    */
	//将当前图片数据保存成指定的文件格式。  
    //参1:绝对路径名  
    //参2:是否保存成RGB格式 
    bool saveToFile(const char *pszFilePath, bool bIsToRGB = true);
	//定义变量m_nWidth和get接口
    CC_SYNTHESIZE_READONLY(unsigned short,   m_nWidth,       Width);
	//定义变量m_nHeight和get接口
    CC_SYNTHESIZE_READONLY(unsigned short,   m_nHeight,      Height);
	//定义变量m_nBitsPerComponent和get接口  
    CC_SYNTHESIZE_READONLY(int,     m_nBitsPerComponent,   BitsPerComponent);

protected:
	//读取JPG图片数据  
    //参1:数据地址  
    //参2:数据长度 
    bool _initWithJpgData(void *pData, int nDatalen);
	//读取PNG图片数据到内存成成Cocos2d-x所用的图像数据保存到m_pData
    bool _initWithPngData(void *pData, int nDatalen);
	//读取TIFF图片数据到内存成成Cocos2d-x所用的图像数据保存到m_pData中 
    bool _initWithTiffData(void *pData, int nDataLen);
    bool _initWithWebpData(void *pData, int nDataLen);
    // @warning kFmtRawData only support RGBA8888
	//读取RGBA8888格式的图片数据。  
    //参1:数据地址  
    //参2:数据长度  
    //参3:图片宽度  
    //参4:图片高度  
    //参5:图片色深  
    bool _initWithRawData(void *pData, int nDatalen, int nWidth, int nHeight, int nBitsPerComponent, bool bPreMulti);
	//将图像数据保存为PNG图片
    bool _saveImageToPNG(const char *pszFilePath, bool bIsToRGB = true);
	//将图像数据保存为JPG图片
    bool _saveImageToJPG(const char *pszFilePath);
	//图像数据地址 
    unsigned char *m_pData;
	//是否有Alpha
    bool m_bHasAlpha;
    bool m_bPreMulti;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
    CCFreeTypeFont* m_ft;
#endif

private:
    // noncopyable
	 // 拷贝构造与重载等号拷贝S
    CCImage(const CCImage&    rImg);
    CCImage & operator=(const CCImage&);


};

// end of platform group
/// @}

NS_CC_END

#endif    // __CC_IMAGE_H__
