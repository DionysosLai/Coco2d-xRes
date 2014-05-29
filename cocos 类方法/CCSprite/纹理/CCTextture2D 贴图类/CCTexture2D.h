/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (C) 2008      Apple Inc. All Rights Reserved.

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

#ifndef __CCTEXTURE2D_H__
#define __CCTEXTURE2D_H__

#include <string>
#include "cocoa/CCObject.h"
#include "cocoa/CCGeometry.h"
#include "ccTypes.h"
#ifdef EMSCRIPTEN
#include "base_nodes/CCGLBufferedNode.h"
#endif // EMSCRIPTEN

NS_CC_BEGIN

class CCImage;

/**
 * @addtogroup textures
 * @{
 */

//CONSTANTS:

/** @typedef CCTexture2DPixelFormat
Possible texture pixel formats
*/
//纹理格式:即每个纹理中的像素单位分别是怎么为颜色值进行实际内存分配的。
// 这个非常重要，我们在进行游戏开发的过程中，会常常与各种图片类型打交通。
// 	每种图片往往也有各自的像素格式。但当它们一旦加载到游戏中后，就会根
// 	据我们的要求变成以下某种类型的纹理。不同的纹理格式所占据的内存大小
// 	可能不同，我们要根据实际情况和需求来选择相应的纹理格式。比如我们用
// 	RGBA8888纹理格式来创建纹理，它占据的内存容量很大，如果我们要显示的
// 	纹理中没有ALPHA值，那就不应该使用带ALPHA通道的纹理格式。我们就可以改成RGB565像素格式。
typedef enum {

    //! 32-bit texture: RGBA8888//32位真彩色，最真但最耗内存
    kCCTexture2DPixelFormat_RGBA8888,
    //! 24-bit texture: RGBA888//24位真彩色，去掉了ALPHA通道
    kCCTexture2DPixelFormat_RGB888,
    //! 16-bit texture without Alpha channel//16位色，将RGB压缩在一个字中。绿色多了1位，因为人眼对绿色更敏感。
    kCCTexture2DPixelFormat_RGB565,
    //! 8-bit textures used as masks//8位色，只存ALPHA值，做遮罩图用
    kCCTexture2DPixelFormat_A8,
    //! 8-bit intensity texture//8位色，只存灰度或者强度值，做灰度图用 
    kCCTexture2DPixelFormat_I8,
    //! 16-bit textures used as masks  //16位色，只存ALPHA值与强度值，双功能 
    kCCTexture2DPixelFormat_AI88,
    //! 16-bit textures: RGBA4444//16位色，RGBA四通道各占4位
    kCCTexture2DPixelFormat_RGBA4444,
    //! 16-bit textures: RGB5A116位色，RGB三通道各占5位，多1位留做ALPHA镂空使用
    kCCTexture2DPixelFormat_RGB5A1,    
    //! 4-bit PVRTC-compressed texture: PVRTC4// PVR的PVRTC4压缩格式
    kCCTexture2DPixelFormat_PVRTC4,
    //! 2-bit PVRTC-compressed texture: PVRTC2// PVRTC的PVRTC2压缩格式
    kCCTexture2DPixelFormat_PVRTC2,


    //! Default texture format: RGBA8888//默认格式RGBA8888
    kCCTexture2DPixelFormat_Default = kCCTexture2DPixelFormat_RGBA8888,

    // backward compatibility stuff // 为了兼容性而保留的枚举值
    kTexture2DPixelFormat_RGBA8888 = kCCTexture2DPixelFormat_RGBA8888,
    kTexture2DPixelFormat_RGB888 = kCCTexture2DPixelFormat_RGB888,
    kTexture2DPixelFormat_RGB565 = kCCTexture2DPixelFormat_RGB565,
    kTexture2DPixelFormat_A8 = kCCTexture2DPixelFormat_A8,
    kTexture2DPixelFormat_RGBA4444 = kCCTexture2DPixelFormat_RGBA4444,
    kTexture2DPixelFormat_RGB5A1 = kCCTexture2DPixelFormat_RGB5A1,
    kTexture2DPixelFormat_Default = kCCTexture2DPixelFormat_Default

} CCTexture2DPixelFormat;
//需要使用Shader代码片段，这里声明一下 
class CCGLProgram;

/**
Extension to set the Min / Mag filter
*/
// typedef struct _ccTexParams {
//     GLuint    minFilter;
//     GLuint    magFilter;
//     GLuint    wrapS;
//     GLuint    wrapT;
// } ccTexParams;
//定义了纹理的一些参数  
typedef struct _ccTexParams {  
	GLuint    minFilter;//纹理过滤器：缩小过滤器  
	GLuint    magFilter;//纹理过滤器：放大过滤器  
	GLuint    wrapS;//横向纹理寻址模式  
	GLuint    wrapT;//纵向纹理寻址模式  
} ccTexParams;  

//CLASS INTERFACES:

/** @brief CCTexture2D class.
* This class allows to easily create OpenGL 2D textures from images, text or raw data.
* The created CCTexture2D object will always have power-of-two dimensions. 
* Depending on how you create the CCTexture2D object, the actual image area of the texture might be smaller than the texture dimensions i.e. "contentSize" != (pixelsWide, pixelsHigh) and (maxS, maxT) != (1.0, 1.0).
* Be aware that the content of the generated textures will be upside-down!
*/
// CCTexture2D类可以方便的从图片，文本或raw数据文件中创建OpenGL所用贴图，
/*创建的贴图会自动转为2的幂次方大小，所以要注意对于贴图坐标的影响。*/
class CC_DLL CCTexture2D : public CCObject
#ifdef EMSCRIPTEN
, public CCGLBufferedNode
#endif // EMSCRIPTEN
{
public:
    /**
     * @js ctor
     */
    CCTexture2D();
    /**
     * @js NA
     * @lua NA
     */
    virtual ~CCTexture2D();
    /**
     *  @js NA
     *  @lua NA
     */
	//取得纹理的描述
    const char* description(void);

    /** These functions are needed to create mutable textures 
     * @js NA
     */
	//释放数据
    void releaseData(void *data);
    /**
     * @js NA
     */
	//保存数据
    void* keepData(void *data, unsigned int length);

    /** Initializes with a texture2d with data 
     * @js NA
     * @lua NA
     */
	//由数据指针和指定的像素格式，图片宽高，来生成OpenGL贴图。
    bool initWithData(const void* data, CCTexture2DPixelFormat pixelFormat, unsigned int pixelsWide, unsigned int pixelsHigh, const CCSize& contentSize);

    /**
    Drawing extensions to make it easy to draw basic quads using a CCTexture2D object.
    These functions require GL_TEXTURE_2D and both GL_VERTEX_ARRAY and GL_TEXTURE_COORD_ARRAY client states to be enabled.
    */
    /** draws a texture at a given point */
	//在指定的位置绘制贴图
    void drawAtPoint(const CCPoint& point);
    /** draws a texture inside a rect */
	//纹制贴图上的一个图像块 
    void drawInRect(const CCRect& rect);

    /**
    Extensions to make it easy to create a CCTexture2D object from an image file.
    Note that RGBA type textures will have their alpha premultiplied - use the blending mode (GL_ONE, GL_ONE_MINUS_SRC_ALPHA).
    */
    /** Initializes a texture from a UIImage object */
	//由CCImage指针生成OpenGL贴图
    bool initWithImage(CCImage * uiImage);

    /** Initializes a texture from a string with dimensions, alignment, font name and font size */
	//由一个字符串生成OpenGL贴图。
    bool initWithString(const char *text,  const char *fontName, float fontSize, const CCSize& dimensions, CCTextAlignment hAlignment, CCVerticalTextAlignment vAlignment);
    /** Initializes a texture from a string with font name and font size */
	//由一个字符串和指定的字体与大小生成OpenGL贴图
    bool initWithString(const char *text, const char *fontName, float fontSize);
    /** Initializes a texture from a string using a text definition*/
    bool initWithString(const char *text, ccFontDefinition *textDefinition);
    
    /** Initializes a texture from a PVR file */
	//从普通PVR文件生成OpenGL贴图
    bool initWithPVRFile(const char* file);
    
    /** Initializes a texture from a ETC file */
    bool initWithETCFile(const char* file);

    /** sets the min filter, mag filter, wrap s and wrap t texture parameters.
    If the texture size is NPOT (non power of 2), then in can only use GL_CLAMP_TO_EDGE in GL_TEXTURE_WRAP_{S,T}.

    @warning Calling this method could allocate additional texture memory.

    @since v0.8
    @code
    when this functon bound to js,the input param are changed
    js: var setTexParameters(var minFilter, var magFilter, var wrapS, var wrapT)
    @endcode
    */
	//设置贴图参数
    void setTexParameters(ccTexParams* texParams);

    /** sets antialias texture parameters:
    - GL_TEXTURE_MIN_FILTER = GL_LINEAR
    - GL_TEXTURE_MAG_FILTER = GL_LINEAR

    @warning Calling this method could allocate additional texture memory.

    @since v0.8
    */
	//设置为抗锯齿的贴图过滤方式（线性过滤）
    void setAntiAliasTexParameters();

    /** sets alias texture parameters:
    - GL_TEXTURE_MIN_FILTER = GL_NEAREST
    - GL_TEXTURE_MAG_FILTER = GL_NEAREST

    @warning Calling this method could allocate additional texture memory.

    @since v0.8
    */
	//设置为非抗锯齿的贴图过滤方式（最近点采样）
    void setAliasTexParameters();


    /** Generates mipmap images for the texture.
    It only works if the texture size is POT (power of 2).
    @since v0.99.0
    */
	//生成多级贴图: 由图片数据生成一系列尺寸为2的幂次方直至当前贴图大小的贴图。
/*	系统会根据距离自动选择纹理图片。可以解决大图片显示在小空间时的闪烁问题。*/
    void generateMipmap();

    /** returns the pixel format.
     @since v2.0
     */
	//取得像素格式名称 
    const char* stringForFormat();

    /** returns the bits-per-pixel of the in-memory OpenGL texture
    @since v1.0
    */
	//返回当前贴图色深，即每个像素占多少位
    unsigned int bitsPerPixelForFormat();  

    /** Helper functions that returns bits per pixels for a given format.
     @since v2.0
     */
	//通过参数贴图格式返回纹理色深
    unsigned int bitsPerPixelForFormat(CCTexture2DPixelFormat format);

    /** sets the default pixel format for UIImagescontains alpha channel.
    If the UIImage contains alpha channel, then the options are:
    - generate 32-bit textures: kCCTexture2DPixelFormat_RGBA8888 (default one)
    - generate 24-bit textures: kCCTexture2DPixelFormat_RGB888
    - generate 16-bit textures: kCCTexture2DPixelFormat_RGBA4444
    - generate 16-bit textures: kCCTexture2DPixelFormat_RGB5A1
    - generate 16-bit textures: kCCTexture2DPixelFormat_RGB565
    - generate 8-bit textures: kCCTexture2DPixelFormat_A8 (only use it if you use just 1 color)

    How does it work ?
    - If the image is an RGBA (with Alpha) then the default pixel format will be used (it can be a 8-bit, 16-bit or 32-bit texture)
    - If the image is an RGB (without Alpha) then: If the default pixel format is RGBA8888 then a RGBA8888 (32-bit) will be used. Otherwise a RGB565 (16-bit texture) will be used.

    This parameter is not valid for PVR / PVR.CCZ images.

    @since v0.8
    */
	//静态函数，用于设置默认带ALPHA通道的贴图像素格式。则图片创建为贴图时，如果有ALPHA通道，则生成此默认贴图像素格式。
    static void setDefaultAlphaPixelFormat(CCTexture2DPixelFormat format);

    /** returns the alpha pixel format
    @since v0.8
    @js getDefaultAlphaPixelFormat
    */
	//静态函数，取得默认带ALPHA通道的贴图像素格式。  
    static CCTexture2DPixelFormat defaultAlphaPixelFormat();

    /** treats (or not) PVR files as if they have alpha premultiplied.
     Since it is impossible to know at runtime if the PVR images have the alpha channel premultiplied, it is
     possible load them as if they have (or not) the alpha channel premultiplied.
     
     By default it is disabled.
     
     @since v0.99.5
     */
	//静态函数，设置载入PVR时是否开启ALPHA渐变，默认不开启，则ALPHA值只有是与否，无渐变。
    static void PVRImagesHavePremultipliedAlpha(bool haveAlphaPremultiplied);

    /** content size */
	//取得图片大小（以像素为单位）
    const CCSize& getContentSizeInPixels();
    //是否有ALPHA渐变值 
    bool hasPremultipliedAlpha();
	//是否有多级贴图 
    bool hasMipmaps();
private:
	//加载一个带ALPHA渐变的图片生成OpenGL贴图 
    bool initPremultipliedATextureWithImage(CCImage * image, unsigned int pixelsWide, unsigned int pixelsHigh);
    
    // By default PVR images are treated as if they don't have the alpha channel premultiplied
	//ALPHA渐变开关
    bool m_bPVRHaveAlphaPremultiplied;

    /** pixel format of the texture */
	//贴图格式变量及get接口
    CC_PROPERTY_READONLY(CCTexture2DPixelFormat, m_ePixelFormat, PixelFormat)
    /** width in pixels */
	//贴图宽度及get接口
    CC_PROPERTY_READONLY(unsigned int, m_uPixelsWide, PixelsWide)
    /** height in pixels */
	//贴图宽度及get接口
    CC_PROPERTY_READONLY(unsigned int, m_uPixelsHigh, PixelsHigh)

    /** texture name */
	//OpenGL贴图索引及get接口 
    CC_PROPERTY_READONLY(GLuint, m_uName, Name)

    /** texture max S */
	//横向贴图坐标终点。因为图片如果不是2的幂次方，图片大小会小于贴图的大小，
/*	贴图一定是2的幂次方嘛，这时候横向的贴图坐标终点不是1.0。*/
    CC_PROPERTY(GLfloat, m_fMaxS, MaxS)
    /** texture max T */
	//纵向贴图坐标终点。
    CC_PROPERTY(GLfloat, m_fMaxT, MaxT)
    /** content size */
	//图片大小及get接口
    CC_PROPERTY_READONLY(CCSize, m_tContentSize, ContentSize)

    /** whether or not the texture has their Alpha premultiplied */
	// ALPHA渐变开关
    bool m_bHasPremultipliedAlpha;
	// 多级纹理开关
    bool m_bHasMipmaps;

    /** shader program used by drawAtPoint and drawInRect */
	//Shader代码片段指针
    CC_PROPERTY(CCGLProgram*, m_pShaderProgram, ShaderProgram);
};

// end of textures group
/// @}

NS_CC_END

#endif //__CCTEXTURE2D_H__

