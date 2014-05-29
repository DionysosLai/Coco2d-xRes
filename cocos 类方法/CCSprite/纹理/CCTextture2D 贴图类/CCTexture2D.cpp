/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2008      Apple Inc. All Rights Reserved.

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



/*
* Support for RGBA_4_4_4_4 and RGBA_5_5_5_1 was copied from:
* https://devforums.apple.com/message/37855#37855 by a1studmuffin
*/

#include "CCTexture2D.h"
#include "ccConfig.h"
#include "ccMacros.h"
#include "CCConfiguration.h"
#include "platform/platform.h"
#include "platform/CCImage.h"
#include "CCGL.h"
#include "support/ccUtils.h"
#include "platform/CCPlatformMacros.h"
#include "textures/CCTexturePVR.h"
#include "textures/CCTextureETC.h"
#include "CCDirector.h"
#include "shaders/CCGLProgram.h"
#include "shaders/ccGLStateCache.h"
#include "shaders/CCShaderCache.h"

#if CC_ENABLE_CACHE_TEXTURE_DATA
    #include "CCTextureCache.h"
#endif

NS_CC_BEGIN

//CLASS IMPLEMENTATIONS:

// If the image has alpha, you can create RGBA8 (32-bit) or RGBA4 (16-bit) or RGB5A1 (16-bit)
// Default is: RGBA8888 (32-bit textures)
//静态全局的默认贴图像素格式。缺省为kCCTexture2DPixelFormat_Default，即RGBA8888。
static CCTexture2DPixelFormat g_defaultAlphaPixelFormat = kCCTexture2DPixelFormat_Default;

// By default PVR images are treated as if they don't have the alpha channel premultiplied
//静态全局的PVR是否有ALPHA渐变的开关变量，默认为否。
static bool PVRHaveAlphaPremultiplied_ = false;

CCTexture2D::CCTexture2D()
: m_bPVRHaveAlphaPremultiplied(true)
, m_uPixelsWide(0)
, m_uPixelsHigh(0)
, m_uName(0)
, m_fMaxS(0.0)
, m_fMaxT(0.0)
, m_bHasPremultipliedAlpha(false)
, m_bHasMipmaps(false)
, m_pShaderProgram(NULL)
{
}

CCTexture2D::~CCTexture2D()
{
	//如果使用可变纹理，删除此可变纹理中的数据。
#if CC_ENABLE_CACHE_TEXTURE_DATA
    VolatileTexture::removeTexture(this);
#endif

    CCLOGINFO("cocos2d: deallocing CCTexture2D %u.", m_uName);
	//释放所用到的Shader代码片段
    CC_SAFE_RELEASE(m_pShaderProgram);
	//释放OpenGL所用到的贴图。
    if(m_uName)
    {
        ccGLDeleteTexture(m_uName);
    }
}

CCTexture2DPixelFormat CCTexture2D::getPixelFormat()
{
    return m_ePixelFormat;
}

unsigned int CCTexture2D::getPixelsWide()
{
    return m_uPixelsWide;
}

unsigned int CCTexture2D::getPixelsHigh()
{
    return m_uPixelsHigh;
}
//取得贴图索引。
GLuint CCTexture2D::getName()
{
    return m_uName;
}
//取得图片大小（以点为单位）
CCSize CCTexture2D::getContentSize()
{
	// CC_CONTENT_SCALE_FACTOR宏返回的是在不同屏幕下的点与像素的比率。
/*	Mac电脑上返回1.而使用Retina显示屏的iphone上返回2。*/
    CCSize ret;
    ret.width = m_tContentSize.width / CC_CONTENT_SCALE_FACTOR();
    ret.height = m_tContentSize.height / CC_CONTENT_SCALE_FACTOR();
    
    return ret;
}
//取得图片大小（以像素为单位）
const CCSize& CCTexture2D::getContentSizeInPixels()
{
    return m_tContentSize;
}
//取得横向的贴图坐标终点 
GLfloat CCTexture2D::getMaxS()
{
    return m_fMaxS;
}
//设置横向的贴图坐标终点 
void CCTexture2D::setMaxS(GLfloat maxS)
{
    m_fMaxS = maxS;
}
//取得纵向的贴图坐标终点
GLfloat CCTexture2D::getMaxT()
{
    return m_fMaxT;
}
//设置纵向的贴图坐标终点
void CCTexture2D::setMaxT(GLfloat maxT)
{
    m_fMaxT = maxT;
}
//所用到的Shader代码片段。
CCGLProgram* CCTexture2D::getShaderProgram(void)
{
    return m_pShaderProgram;
}
//设置用到的Shader代码片段。
void CCTexture2D::setShaderProgram(CCGLProgram* pShaderProgram)
{
    CC_SAFE_RETAIN(pShaderProgram);
    CC_SAFE_RELEASE(m_pShaderProgram);
    m_pShaderProgram = pShaderProgram;
}
//释放数据 
void CCTexture2D::releaseData(void *data)
{
    free(data);
}
//保存数据 
void* CCTexture2D::keepData(void *data, unsigned int length)
{
	//这里只是使用CC_UNUSED_PARAM宏用一下length，没什么实际功能。作者给出这个函数是预备未来供子类重载。 
    CC_UNUSED_PARAM(length);
    //The texture data mustn't be saved because it isn't a mutable texture.
    return data;
}
//是否有ALPHA渐变的通道数据。
bool CCTexture2D::hasPremultipliedAlpha()
{
    return m_bHasPremultipliedAlpha;
}
//由数据指针创建指定大小和格式的贴图。取得创建成功后图片在贴图中的实际区域 。  
bool CCTexture2D::initWithData(const void *data, CCTexture2DPixelFormat pixelFormat, unsigned int pixelsWide, unsigned int pixelsHigh, const CCSize& contentSize)
{
    unsigned int bitsPerPixel;
    //Hack: bitsPerPixelForFormat returns wrong number for RGB_888 textures. See function.
	//如果是RGBA8888格式或者大小正好就是2的幂次方。像素数据按四字节(DWORD)对齐。否则按1字节(BYTE)进行对齐。
    if(pixelFormat == kCCTexture2DPixelFormat_RGB888)
    {
        bitsPerPixel = 24;
    }
    else
    {
        bitsPerPixel = bitsPerPixelForFormat(pixelFormat);
    }

    unsigned int bytesPerRow = pixelsWide * bitsPerPixel / 8;

    if(bytesPerRow % 8 == 0)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 8);
    }
    else if(bytesPerRow % 4 == 0)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    }
    else if(bytesPerRow % 2 == 0)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
    }
    else
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    }

	//产生一个OpenGL的贴图索引。
    glGenTextures(1, &m_uName);
	//将此贴图绑定为GL_TEXTURE_2D纹理。
    ccGLBindTexture2D(m_uName);
	//设置OpenGL中的贴图的过滤参数。
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	//设置贴图的横向纹理寻址模式为边缘截取模式。总是忽略边界。
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	//设置贴图的纵向纹理寻址模式为边缘截取模式。总是忽略边界。 
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

    // Specify OpenGL texture image
	//这里根据不同的像素格式来生成不同的OpenGL所用的贴图。注意：传入的宽和
// 	高在成功生成贴图后会返回实际贴图的宽和高。如果图片不是2的幂次方，这个数
// 		值会改成2的幂次方。比如你传入的图片宽高是148x245，则调用完成后宽高会转成256x256。
    switch(pixelFormat)
    {
    case kCCTexture2DPixelFormat_RGBA8888:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)pixelsWide, (GLsizei)pixelsHigh, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        break;
    case kCCTexture2DPixelFormat_RGB888:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)pixelsWide, (GLsizei)pixelsHigh, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        break;
    case kCCTexture2DPixelFormat_RGBA4444:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)pixelsWide, (GLsizei)pixelsHigh, 0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, data);
        break;
    case kCCTexture2DPixelFormat_RGB5A1:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)pixelsWide, (GLsizei)pixelsHigh, 0, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, data);
        break;
    case kCCTexture2DPixelFormat_RGB565:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)pixelsWide, (GLsizei)pixelsHigh, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, data);
        break;
    case kCCTexture2DPixelFormat_AI88:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, (GLsizei)pixelsWide, (GLsizei)pixelsHigh, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, data);
        break;
    case kCCTexture2DPixelFormat_A8:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, (GLsizei)pixelsWide, (GLsizei)pixelsHigh, 0, GL_ALPHA, GL_UNSIGNED_BYTE, data);
        break;
    case kCCTexture2DPixelFormat_I8:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, (GLsizei)pixelsWide, (GLsizei)pixelsHigh, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, data);
        break;
    default:
        CCAssert(0, "NSInternalInconsistencyException");

    }
	//图片大小
    m_tContentSize = contentSize;
	//保存实际的贴图宽高 
    m_uPixelsWide = pixelsWide;
    m_uPixelsHigh = pixelsHigh;
	//保存贴图的像素格式 
    m_ePixelFormat = pixelFormat;
	//计算图片处于贴图中的横向和纵向的纹理坐标终点。
    m_fMaxS = contentSize.width / (float)(pixelsWide);
    m_fMaxT = contentSize.height / (float)(pixelsHigh);
	//默认不使用ALPHA渐变通道。
    m_bHasPremultipliedAlpha = false;
	//默认不使用多级纹理。
    m_bHasMipmaps = false;
	//设置使用kCCShader_PositionTexture对应类型的Shader。此Shader的顶点格式由位置和纹理坐标组成。
    setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTexture));

    return true;
}


const char* CCTexture2D::description(void)
{
    return CCString::createWithFormat("<CCTexture2D | Name = %u | Dimensions = %u x %u | Coordinates = (%.2f, %.2f)>", m_uName, m_uPixelsWide, m_uPixelsHigh, m_fMaxS, m_fMaxT)->getCString();
}

// implementation CCTexture2D (Image)
// 由CCImage对象来初始化纹理生成OpenGL贴图。
bool CCTexture2D::initWithImage(CCImage *uiImage)
{
    if (uiImage == NULL)
    {
        CCLOG("cocos2d: CCTexture2D. Can't create Texture. UIImage is nil");
        return false;
    }
    //取得图片的宽高
    unsigned int imageWidth = uiImage->getWidth();
    unsigned int imageHeight = uiImage->getHeight();
    //取得引擎的配置信息
    CCConfiguration *conf = CCConfiguration::sharedConfiguration();
    //取得配置信息中指定的最大纹理大小
    unsigned maxTextureSize = conf->getMaxTextureSize();
	//如果当前图片大于指定的最大纹理大小，提示错误警告交释放当前纹理返回NULL。
    if (imageWidth > maxTextureSize || imageHeight > maxTextureSize) 
    {
        CCLOG("cocos2d: WARNING: Image (%u x %u) is bigger than the supported %u x %u", imageWidth, imageHeight, maxTextureSize, maxTextureSize);
        return false;
    }
    
    // always load premultiplied images
	//总是按加载ALPHA渐变的图片方式来生成OpenGL贴图
    return initPremultipliedATextureWithImage(uiImage, imageWidth, imageHeight);
}
//加载一个带ALPHA渐变的图片生成OpenGL贴图 
bool CCTexture2D::initPremultipliedATextureWithImage(CCImage *image, unsigned int width, unsigned int height)
{
	//取得图片的相关信息  
	//定义指针变量指向图片像素数据。
    unsigned char*            tempData = image->getData();
	//定义无符号int指针变量，也是为了指向32位色深的图片像素数据，以便使指针直接对应指定的一个像素数据位置。
    unsigned int*             inPixel32  = NULL;
	//定义无符号char指针变量，也是为了指向8位色深的图片像素数据，以便使指针直接对应指定的一个像素数据位置。
    unsigned char*            inPixel8 = NULL;
	//定义无符号short指针变量，指向16位色深的贴图像素数据，以便使指针直接对应指定的一个像素数据位置。 
    unsigned short*           outPixel16 = NULL;
	//定义bool变量hasAlpha取得图片是否有Alpha通道。
    bool                      hasAlpha = image->hasAlpha();
	//定义变量imageSize保存图片大小。
    CCSize                    imageSize = CCSizeMake((float)(image->getWidth()), (float)(image->getHeight()));
	//定义变量pixelFormat用来保存贴图的像素格式。
    CCTexture2DPixelFormat    pixelFormat;
	//定义变量bpp保存图片的色深。
    size_t                    bpp = image->getBitsPerComponent();

    // compute pixel format// 如果有ALPHA通道，使用默认的RGBA8888格式。
    if (hasAlpha)
    {
    	pixelFormat = g_defaultAlphaPixelFormat;
    }
    else
	{//如果没有ALPHA通道  
		//如果色深大于等于8，则转为RGB888格式，否则转为RGB565格式。
/*		这里有点问题，感觉应该按色深大于16来进行判断。即24和32位都转为RGB888，而16位及以下转为RGB565。*/
        if (bpp >= 8)
        {
            pixelFormat = kCCTexture2DPixelFormat_RGB888;
        }
        else 
        {
            pixelFormat = kCCTexture2DPixelFormat_RGB565;
        }
        
    }
    
    // Repack the pixel data into the right format// 取得数据的长度 
    unsigned int length = width * height;
	//根据图片的不同格式和要创建的纹理格式，将数据填充到纹理中。 
    if (pixelFormat == kCCTexture2DPixelFormat_RGB565)
    {//根据是否有ALPHA通道来分别进行填充处理 
        if (hasAlpha)
        {// 转换RGBA8888到RGB565
            // Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "RRRRRGGGGGGBBBBB"
            //一个像素占2个字节，所以图像中所有像素占用的字节数为width*height*2。由此大小申请内存作为贴图的像素数据。
            tempData = new unsigned char[width * height * 2];
			//将贴图像素数据的地址返回给unsigned short指针。这样outPixel16就指向了贴图中的第一个像素的数据位置。
            outPixel16 = (unsigned short*)tempData;
			//因为有alpha，则图片是32位RGBA8888格式。取得图像的像素数据地址返回给unsigned int指针，则inPixel32指向了贴图中第一个像素的数据位置。 
            inPixel32 = (unsigned int*)image->getData();
            //遍历图片中所有的像素，逐像素处理。
            for(unsigned int i = 0; i < length; ++i, ++inPixel32)
            {
				//将inPixel32指向的unsigned int数据通过取出R,G,B各8位数据值，然后组成RGB565值。放入outPixel16指向的unsigned short数据中。 
                *outPixel16++ = 
                ((((*inPixel32 >>  0) & 0xFF) >> 3) << 11) |  // R
                ((((*inPixel32 >>  8) & 0xFF) >> 2) << 5)  |  // G
                ((((*inPixel32 >> 16) & 0xFF) >> 3) << 0);    // B
            }
        }
        else 
        {
            // Convert "RRRRRRRRRGGGGGGGGBBBBBBBB" to "RRRRRGGGGGGBBBBB"
			// 转换RGB888到RGB565  
			// Convert "RRRRRRRRRGGGGGGGGBBBBBBBB" to "RRRRRGGGGGGBBBBB"  
			//一个像素占2个字节，所以图像中所有像素占用的字节数为width*height*2。由此大小申请内存作为贴图的像素数据。
            tempData = new unsigned char[width * height * 2];
			//将贴图像素数据的地址返回给unsigned short指针。这样outPixel16就指向了贴图中的第一个像素的数据位置。 
            outPixel16 = (unsigned short*)tempData;
			//如果图像的格式为RGB888。取得图像的像素数据地址返回给unsigned char指针，则inPixel8指向了贴图中第一个像素的R值位置。
            inPixel8 = (unsigned char*)image->getData();
            //遍历图片中所有的像素，逐像素处理。
            for(unsigned int i = 0; i < length; ++i)
            {
				 //inPixel8指向的是unsigned char值，通过++操作来取出R,G,B数据值，然后组成RGB565值。放入outPixel16指向的unsigned short数据中。
                *outPixel16++ = 
                (((*inPixel8++ & 0xFF) >> 3) << 11) |  // R
                (((*inPixel8++ & 0xFF) >> 2) << 5)  |  // G
                (((*inPixel8++ & 0xFF) >> 3) << 0);    // B
            }
        }    
    }
    else if (pixelFormat == kCCTexture2DPixelFormat_RGBA4444)
    {
        // Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "RRRRGGGGBBBBAAAA"
		// 转换RGBA8888到RGBA4444  
		// Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "RRRRGGGGBBBBAAAA"  
		//取得图像的像素数据地址返回给unsigned int指针，则inPixel32指向了贴图中第一个像素的数据位置。  
        inPixel32 = (unsigned int*)image->getData();  
		//一个像素占2个字节，所以图像中所有像素占用的字节数为width*height*2。由此大小申请内存作为贴图的像素数据。
        tempData = new unsigned char[width * height * 2];
		 //将贴图像素数据的地址返回给unsigned short指针。这样outPixel16就指向了贴图中的第一个像素的数据位置。
        outPixel16 = (unsigned short*)tempData;
         //遍历图片中所有的像素，逐像素处理。 
        for(unsigned int i = 0; i < length; ++i, ++inPixel32)
        {//将inPixel32指向的unsigned int数据通过取出R,G,B,A各8位数据值，然后组成RGBA4444值。放入outPixel16指向的unsigned short数据中。
            *outPixel16++ = 
            ((((*inPixel32 >> 0) & 0xFF) >> 4) << 12) | // R
            ((((*inPixel32 >> 8) & 0xFF) >> 4) <<  8) | // G
            ((((*inPixel32 >> 16) & 0xFF) >> 4) << 4) | // B
            ((((*inPixel32 >> 24) & 0xFF) >> 4) << 0);  // A
        }
    }
    else if (pixelFormat == kCCTexture2DPixelFormat_RGB5A1)
    {
		// 转换RGBA8888到RGBA5551
        // Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "RRRRRGGGGGBBBBBA"
		//一个像素占2个字节，所以图像中所有像素占用的字节数为width*height*2。由此大小申请内存作为贴图的像素数据。 
        inPixel32 = (unsigned int*)image->getData();   
        tempData = new unsigned char[width * height * 2];
		//将贴图像素数据的地址返回给unsigned short指针。这样outPixel16就指向了贴图中的第一个像素的数据位置。 
        outPixel16 = (unsigned short*)tempData;
        //遍历图片中所有的像素，逐像素处理。
        for(unsigned int i = 0; i < length; ++i, ++inPixel32)
        {
			//将inPixel32指向的unsigned int数据通过取出R,G,B,A各8位数据值，然后组成RGB5A1值。放入outPixel16指向的unsigned short数据中。 
            *outPixel16++ = 
            ((((*inPixel32 >> 0) & 0xFF) >> 3) << 11) | // R
            ((((*inPixel32 >> 8) & 0xFF) >> 3) <<  6) | // G
            ((((*inPixel32 >> 16) & 0xFF) >> 3) << 1) | // B
            ((((*inPixel32 >> 24) & 0xFF) >> 7) << 0);  // A
        }
    }
    else if (pixelFormat == kCCTexture2DPixelFormat_A8)
    {
		// 转换RGBA8888到A8，同理，不再赘述  
        // Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "AAAAAAAA"
        inPixel32 = (unsigned int*)image->getData();
        tempData = new unsigned char[width * height];
        unsigned char *outPixel8 = tempData;
        
        for(unsigned int i = 0; i < length; ++i, ++inPixel32)
        {
            *outPixel8++ = (*inPixel32 >> 24) & 0xFF;  // A
        }
    }
    
    if (hasAlpha && pixelFormat == kCCTexture2DPixelFormat_RGB888)
    {
		// 转换RGBA8888到RGB888，同理，不再赘述  
        // Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "RRRRRRRRGGGGGGGGBBBBBBBB"
        inPixel32 = (unsigned int*)image->getData();
        tempData = new unsigned char[width * height * 3];
        unsigned char *outPixel8 = tempData;
        
        for(unsigned int i = 0; i < length; ++i, ++inPixel32)
        {
            *outPixel8++ = (*inPixel32 >> 0) & 0xFF; // R
            *outPixel8++ = (*inPixel32 >> 8) & 0xFF; // G
            *outPixel8++ = (*inPixel32 >> 16) & 0xFF; // B
        }
    }
    //因为最终相应像素格式的数据都存放在tempData中，所以这里通过像素数据来生成OpenGL贴图。 
    initWithData(tempData, pixelFormat, width, height, imageSize);
    //如果是以上相应格式，则tempData都是新申请的内存块，则在这里释放申请的内存。
    if (tempData != image->getData())
    {
        delete [] tempData;
    }
	//取得是否有ALPHA渐变通道数据
    m_bHasPremultipliedAlpha = image->isPremultipliedAlpha();
    return true;
}

// implementation CCTexture2D (Text)
// 从字符串中创建OpenGL 贴图。  
//参1:字符串  
//参2:字体名称  
//参3:字体大小  
bool CCTexture2D::initWithString(const char *text, const char *fontName, float fontSize)
{
    return initWithString(text,  fontName, fontSize, CCSizeMake(0,0), kCCTextAlignmentCenter, kCCVerticalTextAlignmentTop);
}
// 从字符串中创建OpenGL 贴图,可指定更多参数。  
//参1:字符串  
//参2:返回参数，代表在屏幕上占用的区域大小  
//参3:文字的横向对齐方式  
//参4:文字的纵向对齐方式  
//参5:字体名称  
//参6:字体大小  
bool CCTexture2D::initWithString(const char *text, const char *fontName, float fontSize, const CCSize& dimensions, CCTextAlignment hAlignment, CCVerticalTextAlignment vAlignment)
{
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
        ccFontDefinition tempDef;
        
        tempDef.m_shadow.m_shadowEnabled = false;
        tempDef.m_stroke.m_strokeEnabled = false;
       
        
        tempDef.m_fontName      = std::string(fontName);
        tempDef.m_fontSize      = fontSize;
        tempDef.m_dimensions    = dimensions;
        tempDef.m_alignment     = hAlignment;
        tempDef.m_vertAlignment = vAlignment;
        tempDef.m_fontFillColor = ccWHITE;
    
        return initWithString(text, &tempDef);
    
    
    #else
    
    //如果定义使用可变纹理
    #if CC_ENABLE_CACHE_TEXTURE_DATA
        // cache the texture data
        VolatileTexture::addStringTexture(this, text, dimensions, hAlignment, vAlignment, fontName, fontSize);
    #endif
        
        bool bRet = false;
		//定义一个CCImage进行由字符串创建图片时指定的文字对齐方式的变量eAlign 
        CCImage::ETextAlign eAlign;
        //如果文字纵向对齐方式为顶部对齐。
        if (kCCVerticalTextAlignmentTop == vAlignment)
        {//根据文字横向对齐方式的不同分别对eAlign进行设置。
            eAlign = (kCCTextAlignmentCenter == hAlignment) ? CCImage::kAlignTop
            : (kCCTextAlignmentLeft == hAlignment) ? CCImage::kAlignTopLeft : CCImage::kAlignTopRight;
        }
		//如果文字纵向对齐方式为居中对齐。
        else if (kCCVerticalTextAlignmentCenter == vAlignment)
        {
            eAlign = (kCCTextAlignmentCenter == hAlignment) ? CCImage::kAlignCenter
            : (kCCTextAlignmentLeft == hAlignment) ? CCImage::kAlignLeft : CCImage::kAlignRight;
        }
		//如果文字纵向对齐方式为底部对齐
        else if (kCCVerticalTextAlignmentBottom == vAlignment)
        {
            eAlign = (kCCTextAlignmentCenter == hAlignment) ? CCImage::kAlignBottom
            : (kCCTextAlignmentLeft == hAlignment) ? CCImage::kAlignBottomLeft : CCImage::kAlignBottomRight;
        }
        else
        {//其它对齐方式不应存在，故打印错误。
            CCAssert(false, "Not supported alignment format!");
            return false;
        }
        
        do
        {
            CCImage* pImage = new CCImage();
            CC_BREAK_IF(NULL == pImage);
            bRet = pImage->initWithString(text, (int)dimensions.width, (int)dimensions.height, eAlign, fontName, (int)fontSize);
            CC_BREAK_IF(!bRet);
            bRet = initWithImage(pImage);
            CC_SAFE_RELEASE(pImage);
        } while (0);
    
    
        return bRet;
    
    
    #endif
    
}

bool CCTexture2D::initWithString(const char *text, ccFontDefinition *textDefinition)
{
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    #if CC_ENABLE_CACHE_TEXTURE_DATA
        // cache the texture data
        VolatileTexture::addStringTexture(this, text, textDefinition->m_dimensions, textDefinition->m_alignment, textDefinition->m_vertAlignment, textDefinition->m_fontName.c_str(), textDefinition->m_fontSize);
    #endif
        
        bool bRet = false;
        CCImage::ETextAlign eAlign;
        
        if (kCCVerticalTextAlignmentTop == textDefinition->m_vertAlignment)
        {
            eAlign = (kCCTextAlignmentCenter == textDefinition->m_alignment) ? CCImage::kAlignTop
            : (kCCTextAlignmentLeft == textDefinition->m_alignment) ? CCImage::kAlignTopLeft : CCImage::kAlignTopRight;
        }
        else if (kCCVerticalTextAlignmentCenter == textDefinition->m_vertAlignment)
        {
            eAlign = (kCCTextAlignmentCenter == textDefinition->m_alignment) ? CCImage::kAlignCenter
            : (kCCTextAlignmentLeft == textDefinition->m_alignment) ? CCImage::kAlignLeft : CCImage::kAlignRight;
        }
        else if (kCCVerticalTextAlignmentBottom == textDefinition->m_vertAlignment)
        {
            eAlign = (kCCTextAlignmentCenter == textDefinition->m_alignment) ? CCImage::kAlignBottom
            : (kCCTextAlignmentLeft == textDefinition->m_alignment) ? CCImage::kAlignBottomLeft : CCImage::kAlignBottomRight;
        }
        else
        {
            CCAssert(false, "Not supported alignment format!");
            return false;
        }
        
        // handle shadow parameters
        bool  shadowEnabled =  false;
        float shadowDX      = 0.0f;
        float shadowDY      = 0.0f;
        float shadowBlur    = 0.0f;
        float shadowOpacity = 0.0f;
        
        if ( textDefinition->m_shadow.m_shadowEnabled )
        {
            shadowEnabled =  true;
            shadowDX      = textDefinition->m_shadow.m_shadowOffset.width;
            shadowDY      = textDefinition->m_shadow.m_shadowOffset.height;
            shadowBlur    = textDefinition->m_shadow.m_shadowBlur;
            shadowOpacity = textDefinition->m_shadow.m_shadowOpacity;
        }
        
        // handle stroke parameters
        bool strokeEnabled = false;
        float strokeColorR = 0.0f;
        float strokeColorG = 0.0f;
        float strokeColorB = 0.0f;
        float strokeSize   = 0.0f;
        
        if ( textDefinition->m_stroke.m_strokeEnabled )
        {
            strokeEnabled = true;
            strokeColorR = textDefinition->m_stroke.m_strokeColor.r / 255.0f;
            strokeColorG = textDefinition->m_stroke.m_strokeColor.g / 255.0f;
            strokeColorB = textDefinition->m_stroke.m_strokeColor.b / 255.0f;
            strokeSize   = textDefinition->m_stroke.m_strokeSize;
        }
        
        CCImage* pImage = new CCImage();
        do
        {
            CC_BREAK_IF(NULL == pImage);
            
            bRet = pImage->initWithStringShadowStroke(text,
                                                      (int)textDefinition->m_dimensions.width,
                                                      (int)textDefinition->m_dimensions.height,
                                                      eAlign,
                                                      textDefinition->m_fontName.c_str(),
                                                      textDefinition->m_fontSize,
                                                      textDefinition->m_fontFillColor.r / 255.0f,
                                                      textDefinition->m_fontFillColor.g / 255.0f,
                                                      textDefinition->m_fontFillColor.b / 255.0f,
                                                      shadowEnabled,
                                                      shadowDX,
                                                      shadowDY,
                                                      shadowOpacity,
                                                      shadowBlur,
                                                      strokeEnabled,
                                                      strokeColorR,
                                                      strokeColorG,
                                                      strokeColorB,
                                                      strokeSize);
            
            
            CC_BREAK_IF(!bRet);
            bRet = initWithImage(pImage);
            
        } while (0);
        
        CC_SAFE_RELEASE(pImage);
        
        return bRet;
    
    
    #else
    
        CCAssert(false, "Currently only supported on iOS and Android!");
        return false;
    
    #endif
}


// implementation CCTexture2D (Drawing)
// 在指定的位置绘制贴图。
void CCTexture2D::drawAtPoint(const CCPoint& point)
{
	//定义贴图中图像区域的UV坐标。从左上至右下。
    GLfloat    coordinates[] = {    
        0.0f,    m_fMaxT,
        m_fMaxS,m_fMaxT,
        0.0f,    0.0f,
        m_fMaxS,0.0f };
	//取得贴图中图像区域的宽高
    GLfloat    width = (GLfloat)m_uPixelsWide * m_fMaxS,
        height = (GLfloat)m_uPixelsHigh * m_fMaxT;
	//定义对应的顶点坐标
    GLfloat        vertices[] = {    
        point.x,            point.y,
        width + point.x,    point.y,
        point.x,            height  + point.y,
        width + point.x,    height  + point.y };
	//Shader中使用位置和纹理坐标通道
    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position | kCCVertexAttribFlag_TexCoords );
	//后面的代码便用Shader进行渲染
    m_pShaderProgram->use();
	//设置Shader使用的最终结果矩阵  
    m_pShaderProgram->setUniformsForBuiltins();
	//将贴图绑定
    ccGLBindTexture2D( m_uName );


#ifdef EMSCRIPTEN
    setGLBufferData(vertices, 8 * sizeof(GLfloat), 0);
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, 0);

    setGLBufferData(coordinates, 8 * sizeof(GLfloat), 1);
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, 0);
#else
	//将vertices设置为顶点位置参数
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	//将coordinates设置为顶点的纹理坐标参数
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, coordinates);
#endif // EMSCRIPTEN
	//绘制三角形，参1为绘图方式，参2为顶点起始索引，参3为三角形面数。
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
//绘制纹理上的一个区域。
void CCTexture2D::drawInRect(const CCRect& rect)
{//定义贴图中图像区域的UV坐标。从左上至右下。
    GLfloat    coordinates[] = {    
        0.0f,    m_fMaxT,
        m_fMaxS,m_fMaxT,
        0.0f,    0.0f,
        m_fMaxS,0.0f };
		//绘制到的区域  
    GLfloat    vertices[] = {    rect.origin.x,        rect.origin.y,                            /*0.0f,*/
        rect.origin.x + rect.size.width,        rect.origin.y,                            /*0.0f,*/
        rect.origin.x,                            rect.origin.y + rect.size.height,        /*0.0f,*/
        rect.origin.x + rect.size.width,        rect.origin.y + rect.size.height,        /*0.0f*/ };
	//Shader中使用位置和纹理坐标通道。
    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position | kCCVertexAttribFlag_TexCoords );
	//后面的代码便用Shader进行渲染
    m_pShaderProgram->use();
	//设置Shader使用的最终结果矩阵
    m_pShaderProgram->setUniformsForBuiltins();
	//将贴图绑定
    ccGLBindTexture2D( m_uName );

#ifdef EMSCRIPTEN
    setGLBufferData(vertices, 8 * sizeof(GLfloat), 0);
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, 0);

    setGLBufferData(coordinates, 8 * sizeof(GLfloat), 1);
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, 0);
#else
	//将vertices设置为顶点位置参数
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	//将coordinates设置为顶点的纹理坐标参数
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, coordinates);
#endif // EMSCRIPTEN
	//绘制三角形，参1为绘图方式，参2为顶点起始索引，参3为三角形面数。  
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
//加载PVR普通文件的函数。
bool CCTexture2D::initWithPVRFile(const char* file)
{
    bool bRet = false;
    // nothing to do with CCObject::init
    
    CCTexturePVR *pvr = new CCTexturePVR;
    bRet = pvr->initWithContentsOfFile(file);
        
    if (bRet)
    {
        pvr->setRetainName(true); // don't dealloc texture on release
        
        m_uName = pvr->getName();
        m_fMaxS = 1.0f;
        m_fMaxT = 1.0f;
        m_uPixelsWide = pvr->getWidth();
        m_uPixelsHigh = pvr->getHeight();
        m_tContentSize = CCSizeMake((float)m_uPixelsWide, (float)m_uPixelsHigh);
        m_bHasPremultipliedAlpha = PVRHaveAlphaPremultiplied_;
        m_ePixelFormat = pvr->getFormat();
        m_bHasMipmaps = pvr->getNumberOfMipmaps() > 1;       

        pvr->release();
    }
    else
    {
        CCLOG("cocos2d: Couldn't load PVR image %s", file);
    }

    return bRet;
}

bool CCTexture2D::initWithETCFile(const char* file)
{
    bool bRet = false;
    // nothing to do with CCObject::init
    
    CCTextureETC *etc = new CCTextureETC;
    bRet = etc->initWithFile(file);
    
    if (bRet)
    {
        m_uName = etc->getName();
        m_fMaxS = 1.0f;
        m_fMaxT = 1.0f;
        m_uPixelsWide = etc->getWidth();
        m_uPixelsHigh = etc->getHeight();
        m_tContentSize = CCSizeMake((float)m_uPixelsWide, (float)m_uPixelsHigh);
        m_bHasPremultipliedAlpha = true;
        
        etc->release();
    }
    else
    {
        CCLOG("cocos2d: Couldn't load ETC image %s", file);
    }
    
    return bRet;
}
//设置PVR文件加载时是否使用ALPHA渐变。
void CCTexture2D::PVRImagesHavePremultipliedAlpha(bool haveAlphaPremultiplied)
{
    PVRHaveAlphaPremultiplied_ = haveAlphaPremultiplied;
}

    
//
// Use to apply MIN/MAG filter
//
// implementation CCTexture2D (GLFilter)
//生成多级纹理。 
void CCTexture2D::generateMipmap()
{
    CCAssert( m_uPixelsWide == ccNextPOT(m_uPixelsWide) && m_uPixelsHigh == ccNextPOT(m_uPixelsHigh), "Mipmap texture only works in POT textures");
    ccGLBindTexture2D( m_uName );
    glGenerateMipmap(GL_TEXTURE_2D);
    m_bHasMipmaps = true;
}
//是否有多级纹理 
bool CCTexture2D::hasMipmaps()
{
    return m_bHasMipmaps;
}
//设置纹理参数
void CCTexture2D::setTexParameters(ccTexParams *texParams)
{
    CCAssert( (m_uPixelsWide == ccNextPOT(m_uPixelsWide) || texParams->wrapS == GL_CLAMP_TO_EDGE) &&
        (m_uPixelsHigh == ccNextPOT(m_uPixelsHigh) || texParams->wrapT == GL_CLAMP_TO_EDGE),
        "GL_CLAMP_TO_EDGE should be used in NPOT dimensions");

    ccGLBindTexture2D( m_uName );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texParams->minFilter );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texParams->magFilter );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texParams->wrapS );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texParams->wrapT );

#if CC_ENABLE_CACHE_TEXTURE_DATA
    VolatileTexture::setTexParameters(this, texParams);
#endif
}
//设置为非抗锯齿纹理模式
void CCTexture2D::setAliasTexParameters()
{//绑定纹理
    ccGLBindTexture2D( m_uName );
	//设置多级纹理 
    if( ! m_bHasMipmaps )
    {//设置最小滤波方式为最近点采样,这种方式最快，但有锯齿。
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    }
    else
    {//设置最小滤波方式为多级纹理方式
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST );
    }
	//设置放大滤波方式为最近点采样
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
#if CC_ENABLE_CACHE_TEXTURE_DATA
    ccTexParams texParams = {m_bHasMipmaps?GL_NEAREST_MIPMAP_NEAREST:GL_NEAREST,GL_NEAREST,GL_NONE,GL_NONE};
    VolatileTexture::setTexParameters(this, &texParams);
#endif
}
//设置为抗锯齿纹理模式
void CCTexture2D::setAntiAliasTexParameters()
{//绑定纹理 
    ccGLBindTexture2D( m_uName );
	//设置多级纹理
    if( ! m_bHasMipmaps )
    {//设置最小滤波方式为线性过滤,这种方式纹理会有一定程度模糊。
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    }
    else
    {//设置最小滤波方式为多级纹理方式。
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
    }
	//设置放大滤波方式为最近点采样
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
#if CC_ENABLE_CACHE_TEXTURE_DATA
    ccTexParams texParams = {m_bHasMipmaps?GL_LINEAR_MIPMAP_NEAREST:GL_LINEAR,GL_LINEAR,GL_NONE,GL_NONE};
    VolatileTexture::setTexParameters(this, &texParams);
#endif
}
//取得纹理格式的名字字符串
const char* CCTexture2D::stringForFormat()
{
	switch (m_ePixelFormat) 
	{
		case kCCTexture2DPixelFormat_RGBA8888:
			return  "RGBA8888";

		case kCCTexture2DPixelFormat_RGB888:
			return  "RGB888";

		case kCCTexture2DPixelFormat_RGB565:
			return  "RGB565";

		case kCCTexture2DPixelFormat_RGBA4444:
			return  "RGBA4444";

		case kCCTexture2DPixelFormat_RGB5A1:
			return  "RGB5A1";

		case kCCTexture2DPixelFormat_AI88:
			return  "AI88";

		case kCCTexture2DPixelFormat_A8:
			return  "A8";

		case kCCTexture2DPixelFormat_I8:
			return  "I8";

		case kCCTexture2DPixelFormat_PVRTC4:
			return  "PVRTC4";

		case kCCTexture2DPixelFormat_PVRTC2:
			return  "PVRTC2";

		default:
			CCAssert(false , "unrecognized pixel format");
			CCLOG("stringForFormat: %ld, cannot give useful result", (long)m_ePixelFormat);
			break;
	}

	return  NULL;
}

//
// Texture options for images that contains alpha
//
// implementation CCTexture2D (PixelFormat)
//设置默认带ALPHA通道的纹理格式
void CCTexture2D::setDefaultAlphaPixelFormat(CCTexture2DPixelFormat format)
{
    g_defaultAlphaPixelFormat = format;
}
//取得默认带ALPHA通道的纹理格式 
CCTexture2DPixelFormat CCTexture2D::defaultAlphaPixelFormat()
{
    return g_defaultAlphaPixelFormat;
}
//取得相应纹理格式的色深。
unsigned int CCTexture2D::bitsPerPixelForFormat(CCTexture2DPixelFormat format)
{
	unsigned int ret=0;

	switch (format) {
		case kCCTexture2DPixelFormat_RGBA8888:
			ret = 32;
			break;
		case kCCTexture2DPixelFormat_RGB888:
			// It is 32 and not 24, since its internal representation uses 32 bits.
			ret = 32;
			break;
		case kCCTexture2DPixelFormat_RGB565:
			ret = 16;
			break;
		case kCCTexture2DPixelFormat_RGBA4444:
			ret = 16;
			break;
		case kCCTexture2DPixelFormat_RGB5A1:
			ret = 16;
			break;
		case kCCTexture2DPixelFormat_AI88:
			ret = 16;
			break;
		case kCCTexture2DPixelFormat_A8:
			ret = 8;
			break;
		case kCCTexture2DPixelFormat_I8:
			ret = 8;
			break;
		case kCCTexture2DPixelFormat_PVRTC4:
			ret = 4;
			break;
		case kCCTexture2DPixelFormat_PVRTC2:
			ret = 2;
			break;
		default:
			ret = -1;
			CCAssert(false , "unrecognized pixel format");
			CCLOG("bitsPerPixelForFormat: %ld, cannot give useful result", (long)format);
			break;
	}
	return ret;
}
//取得当前纹理格式的色深。
unsigned int CCTexture2D::bitsPerPixelForFormat()
{
	return this->bitsPerPixelForFormat(m_ePixelFormat);
}


NS_CC_END
