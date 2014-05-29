/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2011      Zynga Inc.

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

Use any of these editors to generate BMFonts:
http://glyphdesigner.71squared.com/ (Commercial, Mac OS X)
http://www.n4te.com/hiero/hiero.jnlp (Free, Java)
http://slick.cokeandcode.com/demos/hiero.jnlp (Free, Java)
http://www.angelcode.com/products/bmfont/ (Free, Windows only)

****************************************************************************/
#include "CCLabelBMFont.h"
#include "cocoa/CCString.h"
#include "platform/platform.h"
#include "cocoa/CCDictionary.h"
#include "CCConfiguration.h"
#include "draw_nodes/CCDrawingPrimitives.h"
#include "sprite_nodes/CCSprite.h"
#include "support/CCPointExtension.h"
#include "platform/CCFileUtils.h"
#include "CCDirector.h"
#include "textures/CCTextureCache.h"
#include "support/ccUTF8.h"

using namespace std;

NS_CC_BEGIN

// The return value needs to be deleted by CC_SAFE_DELETE_ARRAY.
static unsigned short* copyUTF16StringN(unsigned short* str)
{
    int length = str ? cc_wcslen(str) : 0;
    unsigned short* ret = new unsigned short[length+1];
    for (int i = 0; i < length; ++i) {
        ret[i] = str[i];
    }
    ret[length] = 0;
    return ret;
}

//
//FNTConfig Cache - free functions
//
static CCDictionary* s_pConfigurations = NULL;
//全局函数：从Fnt文件中取得一个CCBMFontConfiguration实例对象  
CCBMFontConfiguration* FNTConfigLoadFile( const char *fntFile)
{
	//创建CCBMFontConfiguration指针变量pRet并置空 
    CCBMFontConfiguration* pRet = NULL;
	//如果容器为空则新建容器  
    if( s_pConfigurations == NULL )
    {
        s_pConfigurations = new CCDictionary();
    }
	//在容器中取得该文件对应的CCBMFontConfiguration 
    pRet = (CCBMFontConfiguration*)s_pConfigurations->objectForKey(fntFile);
    if( pRet == NULL )
    {
		//如果为空则调用CCBMFontConfiguration静态函数新建一个CCBMFontConfiguration读取文件返回信息结构给pRet
        pRet = CCBMFontConfiguration::create(fntFile);
        if (pRet)
        {
			//并将其加入容器中 
            s_pConfigurations->setObject(pRet, fntFile);
        }        
    }

    return pRet;
}

//释放容器中的数据并释放容器占用内存
void FNTConfigRemoveCache( void )
{
    if (s_pConfigurations)
    {
        s_pConfigurations->removeAllObjects();
        CC_SAFE_RELEASE_NULL(s_pConfigurations);
    }
}

//
//BitmapFontConfiguration
//

CCBMFontConfiguration * CCBMFontConfiguration::create(const char *FNTfile)
{
    CCBMFontConfiguration * pRet = new CCBMFontConfiguration();
    if (pRet->initWithFNTfile(FNTfile))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

//由FNT文件对CCBMFontConfiguration实例对象进行初始化
bool CCBMFontConfiguration::initWithFNTfile(const char *FNTfile)
{
    m_pKerningDictionary = NULL;
    m_pFontDefDictionary = NULL;

    //解析FNT文件
    m_pCharacterSet = this->parseConfigFile(FNTfile);
    
    if (! m_pCharacterSet)
    {
        return false;
    }

    return true;
}

std::set<unsigned int>* CCBMFontConfiguration::getCharacterSet() const
{
    return m_pCharacterSet;
}

CCBMFontConfiguration::CCBMFontConfiguration()
: m_pFontDefDictionary(NULL)
, m_nCommonHeight(0)
, m_pKerningDictionary(NULL)
, m_pCharacterSet(NULL)
{

}

CCBMFontConfiguration::~CCBMFontConfiguration()
{
	//打印日志 
    CCLOGINFO( "cocos2d: deallocing CCBMFontConfiguration" );
    this->purgeFontDefDictionary();
    this->purgeKerningDictionary();
    m_sAtlasName.clear();
	//释放占用内存 
    CC_SAFE_DELETE(m_pCharacterSet);
}

//取得描述 
const char* CCBMFontConfiguration::description(void)
{
    return CCString::createWithFormat(
        "<CCBMFontConfiguration = " CC_FORMAT_PRINTF_SIZE_T " | Glphys:%d Kernings:%d | Image = %s>",
        (size_t)this,
        HASH_COUNT(m_pFontDefDictionary),
        HASH_COUNT(m_pKerningDictionary),
        m_sAtlasName.c_str()
    )->getCString();
}

//释放间距信息字典 
void CCBMFontConfiguration::purgeKerningDictionary()
{
    tCCKerningHashElement *current;
    while(m_pKerningDictionary) 
    {
        current = m_pKerningDictionary; 
        HASH_DEL(m_pKerningDictionary,current);
        free(current);
    }
}

void CCBMFontConfiguration::purgeFontDefDictionary()
{    
    tCCFontDefHashElement *current, *tmp;

    HASH_ITER(hh, m_pFontDefDictionary, current, tmp) {
        HASH_DEL(m_pFontDefDictionary, current);
        free(current);
    }
}

//解析指定索引的图片的说明信息
std::set<unsigned int>* CCBMFontConfiguration::parseConfigFile(const char *controlFile)
{    
	//通过文件名找出对应的文件路径
    std::string fullpath = CCFileUtils::sharedFileUtils()->fullPathForFilename(controlFile);
	//打开文件进行读操作，执行后会将文件读取到CCFileData创建的内存志中。
    CCString *contents = CCString::createWithContentsOfFile(fullpath.c_str());

	/// 有效性检查
    CCAssert(contents, "CCBMFontConfiguration::parseConfigFile | Open file error.");
    
    set<unsigned int> *validCharsString = new set<unsigned int>();

    if (!contents)
    {
        CCLOG("cocos2d: Error parsing FNTfile %s", controlFile);
        return NULL;
    }

    // parse spacing / padding
    std::string line;
	//将文件信息放入到字符串strLeft中 
    std::string strLeft = contents->getCString();
	//遍历字符串中每个字符进行解析 
    while (strLeft.length() > 0)
    {
		//根据换行符取出每一行
        int pos = strLeft.find('\n');
		//如果不是结尾 
        if (pos != (int)std::string::npos)
        {
            // the data is more than a line.get one line 取出此行
            line = strLeft.substr(0, pos);
			//更新当前字符串为新一行位置后面的字符串 
            strLeft = strLeft.substr(pos + 1);
        }
        else
        {
            // get the left data 最后一行
            line = strLeft; 
            strLeft.erase();
        }
		//如果是字体介绍信息
        if(line.substr(0,strlen("info face")) == "info face") 
        {
            // XXX: info parsing is incomplete
            // Not needed for the Hiero editors, but needed for the AngelCode editor
            //            [self parseInfoArguments:line];
			//解析信息行
            this->parseInfoArguments(line);
        }
        // Check to see if the start of the line is something we are interested in
		//如果是图块公共信息
        else if(line.substr(0,strlen("common lineHeight")) == "common lineHeight")
        {
			//解析图块公共信息 
            this->parseCommonArguments(line);
        }
		//如果是指定索引的图片说明信息
        else if(line.substr(0,strlen("page id")) == "page id")
        {
			//解析指定索引的图片的说明信息
            this->parseImageFileName(line, controlFile);
        }
		//如果是本图片中的字符数量
        else if(line.substr(0,strlen("chars c")) == "chars c")
        {
            // Ignore this line//本句忽略了，无意义，后面读取字符时可以统计
        }
		//如果是字符信息 
        else if(line.substr(0,strlen("char")) == "char")
        {
            // Parse the current line and create a new CharDef
			//一个字符的编码对应信息
            tCCFontDefHashElement* element = (tCCFontDefHashElement*)malloc( sizeof(*element) );
			//将本行字符串解析到字符的编码对应信息中
            this->parseCharacterDefinition(line, &element->fontDef);

            element->key = element->fontDef.charID;
            HASH_ADD_INT(m_pFontDefDictionary, key, element);
            
            validCharsString->insert(element->fontDef.charID);
        }//如果有间距调整信息
//        else if(line.substr(0,strlen("kernings count")) == "kernings count")
//        {
//            this->parseKerningCapacity(line);
//        }
		//解析后面的间距调整
        else if(line.substr(0,strlen("kerning first")) == "kerning first")
        {
			//解析间距调整信息
            this->parseKerningEntry(line);
        }
    }
    
    return validCharsString;
}

//解析指定索引的图片的说明信息
void CCBMFontConfiguration::parseImageFileName(std::string line, const char *fntFile)
{
    //////////////////////////////////////////////////////////////////////////
    // line to parse:
    // page id=0 file="bitmapFontTest.png"
    //////////////////////////////////////////////////////////////////////////

    // page ID. Sanity check //先取得id 
    int index = line.find('=')+1;
    int index2 = line.find(' ', index);
    std::string value = line.substr(index, index2-index);
	//这里只使用了第一张图。所以要求索引为0
    CCAssert(atoi(value.c_str()) == 0, "LabelBMFont file could not be found");

    // file // 取得文件名称 
    index = line.find('"')+1;
    index2 = line.find('"', index);
    value = line.substr(index, index2-index);
	//将文件名对应的全路径存入到字符串m_sAtlasName中
    m_sAtlasName = CCFileUtils::sharedFileUtils()->fullPathFromRelativeFile(value.c_str(), fntFile);
}

//解析字体介绍信息
void CCBMFontConfiguration::parseInfoArguments(std::string line)
{
    //////////////////////////////////////////////////////////////////////////
    // possible lines to parse:
    // info face="Script" size=32 bold=0 italic=0 charset="" unicode=1 stretchH=100 smooth=1 aa=1 padding=1,4,3,2 spacing=0,0 outline=0
    // info face="Cracked" size=36 bold=0 italic=0 charset="" unicode=0 stretchH=100 smooth=1 aa=1 padding=0,0,0,0 spacing=1,1
    //////////////////////////////////////////////////////////////////////////
	//实际使用中，其实只用到了内边距padding，因为最终显示结果还是需要从图块中获取，既然已经有了图块，那至于字体的字体，
// 	大小，是否平滑等信息其实不会影响什么，只是用来做一个说明罢了。
    // padding // 内边距padding 
    int index = line.find("padding=");
    int index2 = line.find(' ', index);
    std::string value = line.substr(index, index2-index);
    sscanf(value.c_str(), "padding=%d,%d,%d,%d", &m_tPadding.top, &m_tPadding.right, &m_tPadding.bottom, &m_tPadding.left);
    CCLOG("cocos2d: padding: %d,%d,%d,%d", m_tPadding.left, m_tPadding.top, m_tPadding.right, m_tPadding.bottom);
}

//解析字块公共信息
void CCBMFontConfiguration::parseCommonArguments(std::string line)
{
    //////////////////////////////////////////////////////////////////////////
    // line to parse:
    // common lineHeight=104 base=26 scaleW=1024 scaleH=512 pages=1 packed=0
    //////////////////////////////////////////////////////////////////////////

    // Height // LineHeight：行高
    int index = line.find("lineHeight=");
    int index2 = line.find(' ', index);
    std::string value = line.substr(index, index2-index);
    sscanf(value.c_str(), "lineHeight=%d", &m_nCommonHeight);
   
	// scaleW. sanity check 图片宽
    index = line.find("scaleW=") + strlen("scaleW=");
    index2 = line.find(' ', index);
    value = line.substr(index, index2-index);
    CCAssert(atoi(value.c_str()) <= CCConfiguration::sharedConfiguration()->getMaxTextureSize(), "CCLabelBMFont: page can't be larger than supported");
  
	// scaleH. sanity check // scaleH：图片高 
    index = line.find("scaleH=") + strlen("scaleH=");
    index2 = line.find(' ', index);
    value = line.substr(index, index2-index);
    CCAssert(atoi(value.c_str()) <= CCConfiguration::sharedConfiguration()->getMaxTextureSize(), "CCLabelBMFont: page can't be larger than supported");
  
	// pages. sanity check// pages:共有几张文字图片供使用 
    index = line.find("pages=") + strlen("pages=");
    index2 = line.find(' ', index);
    value = line.substr(index, index2-index);
    CCAssert(atoi(value.c_str()) == 1, "CCBitfontAtlas: only supports 1 page");

    // packed (ignore) What does this mean ??
}

//解析字符编码与UV对应信息块
void CCBMFontConfiguration::parseCharacterDefinition(std::string line, ccBMFontDef *characterDefinition)
{    
    //////////////////////////////////////////////////////////////////////////
    // line to parse:
    // char id=32   x=0     y=0     width=0     height=0     xoffset=0     yoffset=44    xadvance=14     page=0  chnl=0 
    //////////////////////////////////////////////////////////////////////////

    // Character ID //读取编码值
    int index = line.find("id=");
    int index2 = line.find(' ', index);
    std::string value = line.substr(index, index2-index);
    sscanf(value.c_str(), "id=%u", &characterDefinition->charID);

    // Character x // 在图块中的像素横向位置 
    index = line.find("x=");
    index2 = line.find(' ', index);
    value = line.substr(index, index2-index);
    sscanf(value.c_str(), "x=%f", &characterDefinition->rect.origin.x);

    // Character y// 在图块中的像素纵向位置 
    index = line.find("y=");
    index2 = line.find(' ', index);
    value = line.substr(index, index2-index);
    sscanf(value.c_str(), "y=%f", &characterDefinition->rect.origin.y);

    // Character width // 对应图块的宽
    index = line.find("width=");
    index2 = line.find(' ', index);
    value = line.substr(index, index2-index);
    sscanf(value.c_str(), "width=%f", &characterDefinition->rect.size.width);

    // Character height// 对应图块的高 
    index = line.find("height=");
    index2 = line.find(' ', index);
    value = line.substr(index, index2-index);
    sscanf(value.c_str(), "height=%f", &characterDefinition->rect.size.height);

    // Character xoffset// 当前字在绘制时的像素偏移横向位置 
    index = line.find("xoffset=");
    index2 = line.find(' ', index);
    value = line.substr(index, index2-index);
    sscanf(value.c_str(), "xoffset=%hd", &characterDefinition->xOffset);

    // Character yoffset// 当前字在绘制时的像素偏移纵向位置
    index = line.find("yoffset=");
    index2 = line.find(' ', index);
    value = line.substr(index, index2-index);
    sscanf(value.c_str(), "yoffset=%hd", &characterDefinition->yOffset);

    // Character xadvance// 绘制完当前字后，光标向后移多少像素以绘制下一个字 
    index = line.find("xadvance=");
    index2 = line.find(' ', index);
    value = line.substr(index, index2-index);
    sscanf(value.c_str(), "xadvance=%hd", &characterDefinition->xAdvance);
}

//解析字距调整信息 
void CCBMFontConfiguration::parseKerningEntry(std::string line)
{        
    //////////////////////////////////////////////////////////////////////////
    // line to parse:
    // kerning first=121  second=44  amount=-7
    //////////////////////////////////////////////////////////////////////////

    // first解析第一个字
    int first;
    int index = line.find("first=");
    int index2 = line.find(' ', index);
    std::string value = line.substr(index, index2-index);
    sscanf(value.c_str(), "first=%d", &first);

    // second
    int second;
    index = line.find("second=");
    index2 = line.find(' ', index);
    value = line.substr(index, index2-index);
    sscanf(value.c_str(), "second=%d", &second);

    // amount:解析要调整的像素个数，负值向左，正值向右
    int amount;
    index = line.find("amount=");
    index2 = line.find(' ', index);
    value = line.substr(index, index2-index);
    sscanf(value.c_str(), "amount=%d", &amount);
	//将对应关系存入哈希字典
    tCCKerningHashElement *element = (tCCKerningHashElement *)calloc( sizeof( *element ), 1 );
    element->amount = amount;
    element->key = (first<<16) | (second&0xffff);
    HASH_ADD_INT(m_pKerningDictionary,key, element);
}

//
//CCLabelBMFont
//

//LabelBMFont - Purge Cache 释放占用的内存 
void CCLabelBMFont::purgeCachedData()
{
    FNTConfigRemoveCache();
}


CCLabelBMFont * CCLabelBMFont::create()
{
    CCLabelBMFont * pRet = new CCLabelBMFont();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

CCLabelBMFont * CCLabelBMFont::create(const char *str, const char *fntFile, float width, CCTextAlignment alignment)
{
    return CCLabelBMFont::create(str, fntFile, width, alignment, CCPointZero);
}

CCLabelBMFont * CCLabelBMFont::create(const char *str, const char *fntFile, float width)
{
    return CCLabelBMFont::create(str, fntFile, width, kCCTextAlignmentLeft, CCPointZero);
}

//静态函数：由一个字符串和字体fnt文件创建图字 
CCLabelBMFont * CCLabelBMFont::create(const char *str, const char *fntFile)
{
    return CCLabelBMFont::create(str, fntFile, kCCLabelAutomaticWidth, kCCTextAlignmentLeft, CCPointZero);
}

//LabelBMFont - Creation & Init
CCLabelBMFont *CCLabelBMFont::create(const char *str, const char *fntFile, float width/* = kCCLabelAutomaticWidth*/, CCTextAlignment alignment/* = kCCTextAlignmentLeft*/, CCPoint imageOffset/* = CCPointZero*/)
{
	//new 出一个新的CCLabelBMFont
    CCLabelBMFont *pRet = new CCLabelBMFont();
	//调用其成员函数initWithString创建图字
    if(pRet && pRet->initWithString(str, fntFile, width, alignment, imageOffset))
    {
		//交由内存管理器进行内存管理。
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool CCLabelBMFont::init()
{
    return initWithString(NULL, NULL, kCCLabelAutomaticWidth, kCCTextAlignmentLeft, CCPointZero);
}

//由一个字符串和字体fnt文件创建图字
bool CCLabelBMFont::initWithString(const char *theString, const char *fntFile, float width/* = kCCLabelAutomaticWidth*/, CCTextAlignment alignment/* = kCCTextAlignmentLeft*/, CCPoint imageOffset/* = CCPointZero*/)
{
	//有效性判断
    CCAssert(!m_pConfiguration, "re-init is no longer supported");
    CCAssert( (theString && fntFile) || (theString==NULL && fntFile==NULL), "Invalid params for CCLabelBMFont");
    
    CCTexture2D *texture = NULL;
    
    if (fntFile)
    {
		//调用FNTConfigLoadFile读取Fnt文件并返回出信息的内存地址给指针变量m_pConfiguration
        CCBMFontConfiguration *newConf = FNTConfigLoadFile(fntFile);
        if (!newConf)
        {
            CCLOG("cocos2d: WARNING. CCLabelBMFont: Impossible to create font. Please check file: '%s'", fntFile);
            release();
            return false;
        }
        //用到它就给它的引用计数器加1 
        newConf->retain();
        CC_SAFE_RELEASE(m_pConfiguration);
        m_pConfiguration = newConf;
        
        m_sFntFile = fntFile;
        
        texture = CCTextureCache::sharedTextureCache()->addImage(m_pConfiguration->getAtlasName());
    }
    else 
    {
        texture = new CCTexture2D();
        texture->autorelease();
    }

    if (theString == NULL)
    {
        theString = "";
    }

    if (CCSpriteBatchNode::initWithTexture(texture, strlen(theString)))
    {
        m_fWidth = width;
        m_pAlignment = alignment;
        
        m_cDisplayedOpacity = m_cRealOpacity = 255;
		m_tDisplayedColor = m_tRealColor = ccWHITE;
        m_bCascadeOpacityEnabled = true;
        m_bCascadeColorEnabled = true;
        
        m_obContentSize = CCSizeZero;
        
        m_bIsOpacityModifyRGB = m_pobTextureAtlas->getTexture()->hasPremultipliedAlpha();
        m_obAnchorPoint = ccp(0.5f, 0.5f);
        
        m_tImageOffset = imageOffset;
        
        m_pReusedChar = new CCSprite();
        m_pReusedChar->initWithTexture(m_pobTextureAtlas->getTexture(), CCRectMake(0, 0, 0, 0), false);
        m_pReusedChar->setBatchNode(this);
        
        this->setString(theString, true);
        
        return true;
    }
    return false;
}

CCLabelBMFont::CCLabelBMFont()
: m_sString(NULL)
, m_sInitialString(NULL)
, m_pAlignment(kCCTextAlignmentCenter)
, m_fWidth(-1.0f)
, m_pConfiguration(NULL)
, m_bLineBreakWithoutSpaces(false)
, m_tImageOffset(CCPointZero)
, m_pReusedChar(NULL)
, m_cDisplayedOpacity(255)
, m_cRealOpacity(255)
, m_tDisplayedColor(ccWHITE)
, m_tRealColor(ccWHITE)
, m_bCascadeColorEnabled(true)
, m_bCascadeOpacityEnabled(true)
, m_bIsOpacityModifyRGB(false)
{

}

CCLabelBMFont::~CCLabelBMFont()
{
    CC_SAFE_RELEASE(m_pReusedChar);
    CC_SAFE_DELETE_ARRAY(m_sString);
    CC_SAFE_DELETE_ARRAY(m_sInitialString);
    CC_SAFE_RELEASE(m_pConfiguration);
}

// 由参1和参2组成key从哈希表中找到元素的数量
// LabelBMFont - Atlas generation
int CCLabelBMFont::kerningAmountForFirst(unsigned short first, unsigned short second)
{
    int ret = 0;
	//将两个字合并为双字做为key来进行哈希表查找 
    unsigned int key = (first<<16) | (second & 0xffff);
	//如果Fnt信息中有字距调整信息
    if( m_pConfiguration->m_pKerningDictionary ) {
		//使用哈希算法找到对应的字距信息值
        tCCKerningHashElement *element = NULL;
        HASH_FIND_INT(m_pConfiguration->m_pKerningDictionary, &key, element);  
		//如果找到返回
        if(element)
            ret = element->amount;
    }
    return ret;
}

//重点函数：由字符串和字编码纹理块对应信息设置到 
void CCLabelBMFont::createFontChars()
{
    int nextFontPositionX = 0;//下一个字的横向绘制位置
    int nextFontPositionY = 0;//下一个字的纵向绘制位置 
    unsigned short prev = -1;//上一个字编码 
    int kerningAmount = 0;//字间距调整的像素数量

    CCSize tmpSize = CCSizeZero;

    int longestLine = 0;// 最长的一行的宽度
    unsigned int totalHeight = 0;//字符串的总高度

    unsigned int quantityOfLines = 1;
    unsigned int stringLen = m_sString ? cc_wcslen(m_sString) : 0;
	//如果字符串长度为零直接返回 
    if (stringLen == 0)
    {
        return;
    }

    set<unsigned int> *charSet = m_pConfiguration->getCharacterSet();
	//统计行数
    for (unsigned int i = 0; i < stringLen - 1; ++i)
    {
        unsigned short c = m_sString[i];
        if (c == '\n')
        {
            quantityOfLines++;
        }
    }
	//由行高乘行数最得要显示的字符串占用的总高度
    totalHeight = m_pConfiguration->m_nCommonHeight * quantityOfLines;
	//由行高乘行数最得要显示的字符串占用的总高度 
    nextFontPositionY = 0-(m_pConfiguration->m_nCommonHeight - m_pConfiguration->m_nCommonHeight * quantityOfLines);
    
    CCRect rect;
    ccBMFontDef fontDef;
	//遍历所有的字
    for (unsigned int i= 0; i < stringLen; i++)
    {
        unsigned short c = m_sString[i];

        if (c == '\n')
        {
			//如果遇到换行符则进行换行处理
            nextFontPositionX = 0;
            nextFontPositionY -= m_pConfiguration->m_nCommonHeight;
            continue;
        }
        
        if (charSet->find(c) == charSet->end())
        {
            CCLOGWARN("cocos2d::CCLabelBMFont: Attempted to use character not defined in this bitmap: %d", c);
            continue;      
        }

		//根据上一个字与当前字进行间距调整信息哈希表的查找，返回调整的像素偏移量。
        kerningAmount = this->kerningAmountForFirst(prev, c);
        
        tCCFontDefHashElement *element = NULL;

        // unichar is a short, and an int is needed on HASH_FIND_INT
        unsigned int key = c;
        HASH_FIND_INT(m_pConfiguration->m_pFontDefDictionary, &key, element);
        if (! element)
        {
            CCLOGWARN("cocos2d::CCLabelBMFont: characer not found %d", c);
            continue;
        }
		//取得当前字在贴图上的像素矩形
        fontDef = element->fontDef;

        rect = fontDef.rect;
        rect = CC_RECT_PIXELS_TO_POINTS(rect);

        rect.origin.x += m_tImageOffset.x;
        rect.origin.y += m_tImageOffset.y;
		//取得对应当前字的精灵
        CCSprite *fontChar;

        bool hasSprite = true;
        fontChar = (CCSprite*)(this->getChildByTag(i));
        if(fontChar )
        {
            // Reusing previous Sprite
			//设置其显示
			fontChar->setVisible(true);
        }
        else
        {
            // New Sprite ? Set correct color, opacity, etc...
            if( 0 )
            {
				/* WIP: Doesn't support many features yet.
				 But this code is super fast. It doesn't create any sprite.
				 Ideal for big labels.
				 */
				fontChar = m_pReusedChar;
				fontChar->setBatchNode(NULL);
				hasSprite = false;
			}
            else
            {
				//如果找不到，则新建精灵，并将纹理上对应像素块信息传给精灵进行初始化。
                fontChar = new CCSprite();
                fontChar->initWithTexture(m_pobTextureAtlas->getTexture(), rect);
				//将精灵加入到子结点，设置i为附带查找唯一ID  
                addChild(fontChar, i, i);
                fontChar->release();
			}
            
            // Apply label properties
			fontChar->setOpacityModifyRGB(m_bIsOpacityModifyRGB);
            
			// Color MUST be set before opacity, since opacity might change color if OpacityModifyRGB is on
			fontChar->updateDisplayedColor(m_tDisplayedColor);
			fontChar->updateDisplayedOpacity(m_cDisplayedOpacity);
        }

        // updating previous sprite
        fontChar->setTextureRect(rect, false, rect.size);

        // See issue 1343. cast( signed short + unsigned integer ) == unsigned integer (sign is lost!)
        //设置字的纵向偏移 
		int yOffset = m_pConfiguration->m_nCommonHeight - fontDef.yOffset;
        //计算字的位置,注意：因为锚点设的精灵正中心，所以位置应该是左下角位置加上大小的一半再进行相关偏移和间距调整计算。
		CCPoint fontPos = ccp( (float)nextFontPositionX + fontDef.xOffset + fontDef.rect.size.width*0.5f + kerningAmount,
            (float)nextFontPositionY + yOffset - rect.size.height*0.5f * CC_CONTENT_SCALE_FACTOR() );
        fontChar->setPosition(CC_POINT_PIXELS_TO_POINTS(fontPos));

        // update kerning// 更新绘制下一个字的横向位置
        nextFontPositionX += fontDef.xAdvance + kerningAmount;
		//更新上一个字符供循环再次使用
        prev = c;

		//取得最长的一行的宽度 
        if (longestLine < nextFontPositionX)
        {
            longestLine = nextFontPositionX;
        }
        
        if (! hasSprite)
        {
            updateQuadFromSprite(fontChar, i);
        }
    }

    // If the last character processed has an xAdvance which is less that the width of the characters image, then we need
    // to adjust the width of the string to take this into account, or the character will overlap the end of the bounding
    // box
	//设置当前字符串在屏幕上占用的矩形位置
    if (fontDef.xAdvance < fontDef.rect.size.width)
    {
        tmpSize.width = longestLine + fontDef.rect.size.width - fontDef.xAdvance;
    }
    else
    {
        tmpSize.width = longestLine;
    }
    tmpSize.height = totalHeight;
	//调用基类CCNode的函数设置为原始大小
    this->setContentSize(CC_SIZE_PIXELS_TO_POINTS(tmpSize));
}

//LabelBMFont - CCLabelProtocol protocol
void CCLabelBMFont::setString(const char *newString)
{
    this->setString(newString, true);
}
//LabelBMFont重载基类 CCLabelProtocol的接口函数  
//设置字符串  
void CCLabelBMFont::setString(const char *newString, bool needUpdateLabel)
{
    if (newString == NULL) {
        newString = "";
    }
    if (needUpdateLabel) {
        m_sInitialStringUTF8 = newString;
    }
    unsigned short* utf16String = cc_utf8_to_utf16(newString);
    setString(utf16String, needUpdateLabel);
    CC_SAFE_DELETE_ARRAY(utf16String);
 }

void CCLabelBMFont::setString(unsigned short *newString, bool needUpdateLabel)
{
    if (!needUpdateLabel)
    {
        unsigned short* tmp = m_sString;
        m_sString = copyUTF16StringN(newString);
        CC_SAFE_DELETE_ARRAY(tmp);
    }
    else
    {
        unsigned short* tmp = m_sInitialString;
        m_sInitialString = copyUTF16StringN(newString);
        CC_SAFE_DELETE_ARRAY(tmp);
    }
    
    if (m_pChildren && m_pChildren->count() != 0)
    {
        CCObject* child;
        CCARRAY_FOREACH(m_pChildren, child)
        {
            CCNode* pNode = (CCNode*) child;
            if (pNode)
            {
                pNode->setVisible(false);
            }
        }
    }
    this->createFontChars();
    
    if (needUpdateLabel) {
        updateLabel();
    }
}

const char* CCLabelBMFont::getString(void)
{
    return m_sInitialStringUTF8.c_str();
}

void CCLabelBMFont::setCString(const char *label)
{
    setString(label);
}

//LabelBMFont - CCRGBAProtocol protocol
const ccColor3B& CCLabelBMFont::getColor()
{
    return m_tRealColor;
}

const ccColor3B& CCLabelBMFont::getDisplayedColor()
{
    return m_tDisplayedColor;
}

//LabelBMFont 重载基类 CCRGBAProtocol 的接口函数  
//设置颜色  
void CCLabelBMFont::setColor(const ccColor3B& color)
{
	m_tDisplayedColor = m_tRealColor = color;
	
	if( m_bCascadeColorEnabled ) {
		ccColor3B parentColor = ccWHITE;
        CCRGBAProtocol* pParent = dynamic_cast<CCRGBAProtocol*>(m_pParent);
        if (pParent && pParent->isCascadeColorEnabled())
        {
            parentColor = pParent->getDisplayedColor();
        }
        this->updateDisplayedColor(parentColor);
	}
}

GLubyte CCLabelBMFont::getOpacity(void)
{
    return m_cRealOpacity;
}

GLubyte CCLabelBMFont::getDisplayedOpacity(void)
{
    return m_cDisplayedOpacity;
}

/** Override synthesized setOpacity to recurse items *///设置透明度
void CCLabelBMFont::setOpacity(GLubyte opacity)
{
	m_cDisplayedOpacity = m_cRealOpacity = opacity;
    
	if( m_bCascadeOpacityEnabled ) {
		GLubyte parentOpacity = 255;
        CCRGBAProtocol* pParent = dynamic_cast<CCRGBAProtocol*>(m_pParent);
        if (pParent && pParent->isCascadeOpacityEnabled())
        {
            parentOpacity = pParent->getDisplayedOpacity();
        }
        this->updateDisplayedOpacity(parentOpacity);
	}
}

//设置是否使用透明度设置RGB值
void CCLabelBMFont::setOpacityModifyRGB(bool var)
{
    m_bIsOpacityModifyRGB = var;
    if (m_pChildren && m_pChildren->count() != 0)
    {
        CCObject* child;
        CCARRAY_FOREACH(m_pChildren, child)
        {
            CCNode* pNode = (CCNode*) child;
            if (pNode)
            {
				//因子结点即是精灵CCSprite，CCSprite又派生于CCRGBAProtocol，故进行转换
                CCRGBAProtocol *pRGBAProtocol = dynamic_cast<CCRGBAProtocol*>(pNode);
                if (pRGBAProtocol)
                {
					//调用setIsOpacityModifyRGB进行设置
                    pRGBAProtocol->setOpacityModifyRGB(m_bIsOpacityModifyRGB);
                }
            }
        }
    }
}

//取得是否使用透明度设置RGB值
bool CCLabelBMFont::isOpacityModifyRGB()
{
    return m_bIsOpacityModifyRGB;
}

void CCLabelBMFont::updateDisplayedOpacity(GLubyte parentOpacity)
{
	m_cDisplayedOpacity = m_cRealOpacity * parentOpacity/255.0;
    
	CCObject* pObj;
	CCARRAY_FOREACH(m_pChildren, pObj)
    {
        CCSprite *item = (CCSprite*)pObj;
		item->updateDisplayedOpacity(m_cDisplayedOpacity);
	}
}

void CCLabelBMFont::updateDisplayedColor(const ccColor3B& parentColor)
{
	m_tDisplayedColor.r = m_tRealColor.r * parentColor.r/255.0;
	m_tDisplayedColor.g = m_tRealColor.g * parentColor.g/255.0;
	m_tDisplayedColor.b = m_tRealColor.b * parentColor.b/255.0;
    
    CCObject* pObj;
	CCARRAY_FOREACH(m_pChildren, pObj)
    {
        CCSprite *item = (CCSprite*)pObj;
		item->updateDisplayedColor(m_tDisplayedColor);
	}
}

bool CCLabelBMFont::isCascadeColorEnabled()
{
    return false;
}

void CCLabelBMFont::setCascadeColorEnabled(bool cascadeColorEnabled)
{
    m_bCascadeColorEnabled = cascadeColorEnabled;
}

bool CCLabelBMFont::isCascadeOpacityEnabled()
{
    return false;
}

void CCLabelBMFont::setCascadeOpacityEnabled(bool cascadeOpacityEnabled)
{
    m_bCascadeOpacityEnabled = cascadeOpacityEnabled;
}

// LabelBMFont - AnchorPoint// 设置锚点
void CCLabelBMFont::setAnchorPoint(const CCPoint& point)
{
    if( ! point.equals(m_obAnchorPoint))
    {
        CCSpriteBatchNode::setAnchorPoint(point);
        updateLabel();
    }
}

// LabelBMFont - Alignment
void CCLabelBMFont::updateLabel()
{
    this->setString(m_sInitialString, false);

    if (m_fWidth > 0)
    {
        // Step 1: Make multiline
        vector<unsigned short> str_whole = cc_utf16_vec_from_utf16_str(m_sString);
        unsigned int stringLength = str_whole.size();
        vector<unsigned short> multiline_string;
        multiline_string.reserve( stringLength );
        vector<unsigned short> last_word;
        last_word.reserve( stringLength );

        unsigned int line = 1, i = 0;
        bool start_line = false, start_word = false;
        float startOfLine = -1, startOfWord = -1;
        int skip = 0;

        CCArray* children = getChildren();
        for (unsigned int j = 0; j < children->count(); j++)
        {
            CCSprite* characterSprite;
            unsigned int justSkipped = 0;
            
            while (!(characterSprite = (CCSprite*)this->getChildByTag(j + skip + justSkipped)))
            {
                justSkipped++;
            }
            
            skip += justSkipped;
            
            if (!characterSprite->isVisible())
                continue;

            if (i >= stringLength)
                break;

            unsigned short character = str_whole[i];

            if (!start_word)
            {
                startOfWord = getLetterPosXLeft( characterSprite );
                start_word = true;
            }
            if (!start_line)
            {
                startOfLine = startOfWord;
                start_line = true;
            }

            // Newline.
            if (character == '\n')
            {
                cc_utf8_trim_ws(&last_word);

                last_word.push_back('\n');
                multiline_string.insert(multiline_string.end(), last_word.begin(), last_word.end());
                last_word.clear();
                start_word = false;
                start_line = false;
                startOfWord = -1;
                startOfLine = -1;
                i+=justSkipped;
                line++;

                if (i >= stringLength)
                    break;

                character = str_whole[i];

                if (!startOfWord)
                {
                    startOfWord = getLetterPosXLeft( characterSprite );
                    start_word = true;
                }
                if (!startOfLine)
                {
                    startOfLine  = startOfWord;
                    start_line = true;
                }
            }

            // Whitespace.
            if (isspace_unicode(character))
            {
                last_word.push_back(character);
                multiline_string.insert(multiline_string.end(), last_word.begin(), last_word.end());
                last_word.clear();
                start_word = false;
                startOfWord = -1;
                i++;
                continue;
            }

            // Out of bounds.
            if ( getLetterPosXRight( characterSprite ) - startOfLine > m_fWidth )
            {
                if (!m_bLineBreakWithoutSpaces)
                {
                    last_word.push_back(character);

                    int found = cc_utf8_find_last_not_char(multiline_string, ' ');
                    if (found != -1)
                        cc_utf8_trim_ws(&multiline_string);
                    else
                        multiline_string.clear();

                    if (multiline_string.size() > 0)
                        multiline_string.push_back('\n');

                    line++;
                    start_line = false;
                    startOfLine = -1;
                    i++;
                }
                else
                {
                    cc_utf8_trim_ws(&last_word);

                    last_word.push_back('\n');
                    multiline_string.insert(multiline_string.end(), last_word.begin(), last_word.end());
                    last_word.clear();
                    start_word = false;
                    start_line = false;
                    startOfWord = -1;
                    startOfLine = -1;
                    line++;

                    if (i >= stringLength)
                        break;

                    if (!startOfWord)
                    {
                        startOfWord = getLetterPosXLeft( characterSprite );
                        start_word = true;
                    }
                    if (!startOfLine)
                    {
                        startOfLine  = startOfWord;
                        start_line = true;
                    }

                    j--;
                }

                continue;
            }
            else
            {
                // Character is normal.
                last_word.push_back(character);
                i++;
                continue;
            }
        }

        multiline_string.insert(multiline_string.end(), last_word.begin(), last_word.end());

        int size = multiline_string.size();
        unsigned short* str_new = new unsigned short[size + 1];

        for (int i = 0; i < size; ++i)
        {
            str_new[i] = multiline_string[i];
        }

        str_new[size] = '\0';

        this->setString(str_new, false);
        
        CC_SAFE_DELETE_ARRAY(str_new);
    }

    // Step 2: Make alignment
    if (m_pAlignment != kCCTextAlignmentLeft)
    {
        int i = 0;

        int lineNumber = 0;
        int str_len = cc_wcslen(m_sString);
        vector<unsigned short> last_line;
        for (int ctr = 0; ctr <= str_len; ++ctr)
        {
            if (m_sString[ctr] == '\n' || m_sString[ctr] == 0)
            {
                float lineWidth = 0.0f;
                unsigned int line_length = last_line.size();
				// if last line is empty we must just increase lineNumber and work with next line
                if (line_length == 0)
                {
                    lineNumber++;
                    continue;
                }
                int index = i + line_length - 1 + lineNumber;
                if (index < 0) continue;

                CCSprite* lastChar = (CCSprite*)getChildByTag(index);
                if ( lastChar == NULL )
                    continue;

                lineWidth = lastChar->getPosition().x + lastChar->getContentSize().width/2.0f;

                float shift = 0;
                switch (m_pAlignment)
                {
                case kCCTextAlignmentCenter:
                    shift = getContentSize().width/2.0f - lineWidth/2.0f;
                    break;
                case kCCTextAlignmentRight:
                    shift = getContentSize().width - lineWidth;
                    break;
                default:
                    break;
                }

                if (shift != 0)
                {
                    for (unsigned j = 0; j < line_length; j++)
                    {
                        index = i + j + lineNumber;
                        if (index < 0) continue;

                        CCSprite* characterSprite = (CCSprite*)getChildByTag(index);
                        characterSprite->setPosition(ccpAdd(characterSprite->getPosition(), ccp(shift, 0.0f)));
                    }
                }

                i += line_length;
                lineNumber++;

                last_line.clear();
                continue;
            }

            last_line.push_back(m_sString[ctr]);
        }
    }
}

// LabelBMFont - Alignment
void CCLabelBMFont::setAlignment(CCTextAlignment alignment)
{
    this->m_pAlignment = alignment;
    updateLabel();
}

void CCLabelBMFont::setWidth(float width)
{
    this->m_fWidth = width;
    updateLabel();
}

void CCLabelBMFont::setLineBreakWithoutSpace( bool breakWithoutSpace )
{
    m_bLineBreakWithoutSpaces = breakWithoutSpace;
    updateLabel();
}

void CCLabelBMFont::setScale(float scale)
{
    CCSpriteBatchNode::setScale(scale);
    updateLabel();
}

void CCLabelBMFont::setScaleX(float scaleX)
{
    CCSpriteBatchNode::setScaleX(scaleX);
    updateLabel();
}

void CCLabelBMFont::setScaleY(float scaleY)
{
    CCSpriteBatchNode::setScaleY(scaleY);
    updateLabel();
}

float CCLabelBMFont::getLetterPosXLeft( CCSprite* sp )
{
    return sp->getPosition().x * m_fScaleX - (sp->getContentSize().width * m_fScaleX * sp->getAnchorPoint().x);
}

float CCLabelBMFont::getLetterPosXRight( CCSprite* sp )
{
    return sp->getPosition().x * m_fScaleX + (sp->getContentSize().width * m_fScaleX * sp->getAnchorPoint().x);
}

// LabelBMFont - FntFile
void CCLabelBMFont::setFntFile(const char* fntFile)
{
    if (fntFile != NULL && strcmp(fntFile, m_sFntFile.c_str()) != 0 )
    {
        CCBMFontConfiguration *newConf = FNTConfigLoadFile(fntFile);

        CCAssert( newConf, "CCLabelBMFont: Impossible to create font. Please check file");

        m_sFntFile = fntFile;

        CC_SAFE_RETAIN(newConf);
        CC_SAFE_RELEASE(m_pConfiguration);
        m_pConfiguration = newConf;

        this->setTexture(CCTextureCache::sharedTextureCache()->addImage(m_pConfiguration->getAtlasName()));
        this->createFontChars();
    }
}

const char* CCLabelBMFont::getFntFile()
{
    return m_sFntFile.c_str();
}

CCBMFontConfiguration* CCLabelBMFont::getConfiguration() const
{
	return m_pConfiguration;
}

//LabelBMFont - Debug draw//绘制图字标签,Debug模式下手动调用绘制  
#if CC_LABELBMFONT_DEBUG_DRAW
void CCLabelBMFont::draw()
{
	//绘制图块集
    CCSpriteBatchNode::draw();
	//绘制边框
    const CCSize& s = this->getContentSize();
    CCPoint vertices[4]={
        ccp(0,0),ccp(s.width,0),
        ccp(s.width,s.height),ccp(0,s.height),
    };
    ccDrawPoly(vertices, 4, true);
}

#endif // CC_LABELBMFONT_DEBUG_DRAW

NS_CC_END
