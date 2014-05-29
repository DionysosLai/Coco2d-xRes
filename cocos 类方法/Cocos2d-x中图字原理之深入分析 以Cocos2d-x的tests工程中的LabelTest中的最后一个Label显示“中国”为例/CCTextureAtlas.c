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
****************************************************************************/

// cocos2d
#include "CCTextureAtlas.h"
#include "CCTextureCache.h"
#include "ccMacros.h"
#include "shaders/CCGLProgram.h"
#include "shaders/ccGLStateCache.h"
#include "support/CCNotificationCenter.h"
#include "CCEventType.h"
#include "CCGL.h"
// support
#include "CCTexture2D.h"
#include "cocoa/CCString.h"
#include <stdlib.h>

//According to some tests GL_TRIANGLE_STRIP is slower, MUCH slower. Probably I'm doing something very wrong

// implementation CCTextureAtlas

NS_CC_BEGIN

CCTextureAtlas::CCTextureAtlas()
    :m_pIndices(NULL)
    ,m_bDirty(false)
    ,m_pTexture(NULL)
    ,m_pQuads(NULL)
{}

CCTextureAtlas::~CCTextureAtlas()
{
    CCLOGINFO("cocos2d: CCTextureAtlas deallocing %p.", this);

    CC_SAFE_FREE(m_pQuads);
    CC_SAFE_FREE(m_pIndices);

	/// 释放缓冲区对象
    glDeleteBuffers(2, m_pBuffersVBO);

#if CC_TEXTURE_ATLAS_USE_VAO
    glDeleteVertexArrays(1, &m_uVAOname);
    ccGLBindVAO(0);
#endif
    CC_SAFE_RELEASE(m_pTexture);

#if CC_ENABLE_CACHE_TEXTURE_DATA
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, EVENT_COME_TO_FOREGROUND);
#endif
}
//实现通过宏CC_PROPERTY_READONLY声明的函数  
//取得当前使用的图块的数量 
unsigned int CCTextureAtlas::getTotalQuads()
{
    return m_uTotalQuads;
}

unsigned int CCTextureAtlas::getCapacity()
{
    return m_uCapacity;
}
/// 取得大纹理
CCTexture2D* CCTextureAtlas::getTexture()
{
    return m_pTexture;
}
//设置大纹理  
void CCTextureAtlas::setTexture(CCTexture2D * var)
{
    CC_SAFE_RETAIN(var);
    CC_SAFE_RELEASE(m_pTexture);
    m_pTexture = var;
}
//取得使用此大图的图块的所有精灵的顶点缓冲信息数组
ccV3F_C4B_T2F_Quad* CCTextureAtlas::getQuads()
{
    //if someone accesses the quads directly, presume推定 that changes will be made
    m_bDirty = true;
    return m_pQuads;
}
//设置使用此大图的图块的所有精灵的顶点缓冲信息数组 
void CCTextureAtlas::setQuads(ccV3F_C4B_T2F_Quad *var)
{
    m_pQuads = var;
}

// TextureAtlas - alloc & init

CCTextureAtlas * CCTextureAtlas::create(const char* file, unsigned int capacity)
{
    CCTextureAtlas * pTextureAtlas = new CCTextureAtlas();
	//调用成员函数进行初始化
    if(pTextureAtlas && pTextureAtlas->initWithFile(file, capacity))
    {
        pTextureAtlas->autorelease();
        return pTextureAtlas;
    }
	//如果失败，释放后返回NULL
    CC_SAFE_DELETE(pTextureAtlas);
    return NULL;
}

CCTextureAtlas * CCTextureAtlas::createWithTexture(CCTexture2D *texture, unsigned int capacity)
{
    CCTextureAtlas * pTextureAtlas = new CCTextureAtlas();
	//调用成员函数进行初始化
    if (pTextureAtlas && pTextureAtlas->initWithTexture(texture, capacity))
    {
        pTextureAtlas->autorelease();
        return pTextureAtlas;
    }
	//如果失败，释放后返回NULL
    CC_SAFE_DELETE(pTextureAtlas);
    return NULL;
}
//非静态函数，从文件中创建纹理，并初始化图块容量
bool CCTextureAtlas::initWithFile(const char * file, unsigned int capacity)
{
    // retained in property// 由纹理管理器加载一个图片文件，返回生成的纹理
    CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage(file);

    if (texture)
    {//如果成功，使用纹理进行初始化
        return initWithTexture(texture, capacity);
    }
    else
    {
        CCLOG("cocos2d: Could not open file: %s", file);
        return false;
    }
}

bool CCTextureAtlas::initWithTexture(CCTexture2D *texture, unsigned int capacity)
{
	//纹理有效性判断，防止重复调用
//    CCAssert(texture != NULL, "texture should not be null");
    m_uCapacity = capacity;
    m_uTotalQuads = 0;

    // retained in property 设置纹理
    this->m_pTexture = texture;
    CC_SAFE_RETAIN(m_pTexture);

    // Re-initialization is not allowed// 判断是否重复调用 
    CCAssert(m_pQuads == NULL && m_pIndices == NULL, "");

    m_pQuads = (ccV3F_C4B_T2F_Quad*)malloc( m_uCapacity * sizeof(ccV3F_C4B_T2F_Quad) );
    m_pIndices = (GLushort *)malloc( m_uCapacity * 6 * sizeof(GLushort) );

    //如果失败，做相应处理  
    if( ! ( m_pQuads && m_pIndices) && m_uCapacity > 0) 
    {
        //CCLOG("cocos2d: CCTextureAtlas: not enough memory");
        CC_SAFE_FREE(m_pQuads);
        CC_SAFE_FREE(m_pIndices);

        // release texture, should set it to null, because the destruction will
        // release it too. see cocos2d-x issue #484
        CC_SAFE_RELEASE_NULL(m_pTexture);
        return false;
    }

    memset( m_pQuads, 0, m_uCapacity * sizeof(ccV3F_C4B_T2F_Quad) );
    memset( m_pIndices, 0, m_uCapacity * 6 * sizeof(GLushort) );

#if CC_ENABLE_CACHE_TEXTURE_DATA
    // listen the event when app go to background
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
                                                           callfuncO_selector(CCTextureAtlas::listenBackToForeground),
                                                           EVENT_COME_TO_FOREGROUND,
                                                           NULL);
#endif
    /// 初始化缓冲区
    this->setupIndices();

#if CC_TEXTURE_ATLAS_USE_VAO
    setupVBOandVAO();    
#else    
    setupVBO();
#endif

    m_bDirty = true;

    return true;
}

void CCTextureAtlas::listenBackToForeground(CCObject *obj)
{  
#if CC_TEXTURE_ATLAS_USE_VAO
    setupVBOandVAO();    
#else    
    setupVBO();
#endif
    
    // set m_bDirty to true to force it rebinding buffer
    m_bDirty = true;
}

const char* CCTextureAtlas::description()
{
    return CCString::createWithFormat("<CCTextureAtlas | totalQuads = %u>", m_uTotalQuads)->getCString();
}


void CCTextureAtlas::setupIndices()
{
	/// 容量直接为0，直接返回；
    if (m_uCapacity == 0)
        return;

    for( unsigned int i=0; i < m_uCapacity; i++)
    {
#if CC_TEXTURE_ATLAS_USE_TRIANGLE_STRIP
        m_pIndices[i*6+0] = i*4+0;
        m_pIndices[i*6+1] = i*4+0;
        m_pIndices[i*6+2] = i*4+2;        
        m_pIndices[i*6+3] = i*4+1;
        m_pIndices[i*6+4] = i*4+3;
        m_pIndices[i*6+5] = i*4+3;
#else
        m_pIndices[i*6+0] = i*4+0;
        m_pIndices[i*6+1] = i*4+1;
        m_pIndices[i*6+2] = i*4+2;

        // inverted index. issue #179
        m_pIndices[i*6+3] = i*4+3;
        m_pIndices[i*6+4] = i*4+2;
        m_pIndices[i*6+5] = i*4+1;        
#endif    
    }
}

//TextureAtlas - VAO / VBO specific

#if CC_TEXTURE_ATLAS_USE_VAO
void CCTextureAtlas::setupVBOandVAO()
{
    glGenVertexArrays(1, &m_uVAOname);
    ccGLBindVAO(m_uVAOname);

#define kQuadSize sizeof(m_pQuads[0].bl)

    glGenBuffers(2, &m_pBuffersVBO[0]);

    glBindBuffer(GL_ARRAY_BUFFER, m_pBuffersVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_pQuads[0]) * m_uCapacity, m_pQuads, GL_DYNAMIC_DRAW);

    // vertices
    glEnableVertexAttribArray(kCCVertexAttrib_Position);
    glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, kQuadSize, (GLvoid*) offsetof( ccV3F_C4B_T2F, vertices));

    // colors
    glEnableVertexAttribArray(kCCVertexAttrib_Color);
    glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (GLvoid*) offsetof( ccV3F_C4B_T2F, colors));

    // tex coords
    glEnableVertexAttribArray(kCCVertexAttrib_TexCoords);
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, kQuadSize, (GLvoid*) offsetof( ccV3F_C4B_T2F, texCoords));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pBuffersVBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_pIndices[0]) * m_uCapacity * 6, m_pIndices, GL_STATIC_DRAW);

    // Must unbind the VAO before changing the element buffer.
    ccGLBindVAO(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    CHECK_GL_ERROR_DEBUG();
}
#else // CC_TEXTURE_ATLAS_USE_VAO
void CCTextureAtlas::setupVBO()
{
    glGenBuffers(2, &m_pBuffersVBO[0]);

    mapBuffers();
}
#endif // ! // CC_TEXTURE_ATLAS_USE_VAO

void CCTextureAtlas::mapBuffers()
{
    // Avoid changing the element buffer for whatever VAO might be bound.
	ccGLBindVAO(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_pBuffersVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_pQuads[0]) * m_uCapacity, m_pQuads, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pBuffersVBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_pIndices[0]) * m_uCapacity * 6, m_pIndices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    CHECK_GL_ERROR_DEBUG();
}

// TextureAtlas - Update, Insert, Move & Remove
//通过索引值找到对应图块的顶点缓冲并用新数据修改它
void CCTextureAtlas::updateQuad(ccV3F_C4B_T2F_Quad *quad, unsigned int index)
{
    CCAssert( index >= 0 && index < m_uCapacity, "updateQuadWithTexture: Invalid index");

	/// 重新分配大小
    m_uTotalQuads = MAX( index+1, m_uTotalQuads);

    m_pQuads[index] = *quad;    


    m_bDirty = true;

}
//通过索引值找到对应图块的顶点缓冲，并在其位置之前插入一个新的图块。 
void CCTextureAtlas::insertQuad(ccV3F_C4B_T2F_Quad *quad, unsigned int index)
{
	/// 有效性判断
    CCAssert( index < m_uCapacity, "insertQuadWithTexture: Invalid index");
	/// 容量+1
    m_uTotalQuads++;
    CCAssert( m_uTotalQuads <= m_uCapacity, "invalid totalQuads");

    // issue #575. index can be > totalQuads
    unsigned int remaining = (m_uTotalQuads-1) - index;

    // last object doesn't need to be moved
    if( remaining > 0) 
    { //先将索引位置之后的数据整体后移一个位置，再用新数据填充索引位置的数据。实现插入操作。
        // texture coordinates 
        memmove( &m_pQuads[index+1],&m_pQuads[index], sizeof(m_pQuads[0]) * remaining );        
    }

    m_pQuads[index] = *quad;


    m_bDirty = true;

}
/// 通过索引值找到对应的图块顶点缓冲数据，并在其之前插入amount新的图块
void CCTextureAtlas::insertQuads(ccV3F_C4B_T2F_Quad* quads, unsigned int index, unsigned int amount)
{
    CCAssert(index + amount <= m_uCapacity, "insertQuadWithTexture: Invalid index + amount");

    m_uTotalQuads += amount;

    CCAssert( m_uTotalQuads <= m_uCapacity, "invalid totalQuads");

    // issue #575. index can be > totalQuads
    int remaining = (m_uTotalQuads-1) - index - amount;

    // last object doesn't need to be moved
    if( remaining > 0)
    {
        // tex coordinates
        memmove( &m_pQuads[index+amount],&m_pQuads[index], sizeof(m_pQuads[0]) * remaining );
    }


    unsigned int max = index + amount;
    unsigned int j = 0;
    for (unsigned int i = index; i < max ; i++)
    {
        m_pQuads[index] = quads[j];
        index++;
        j++;
    }

    m_bDirty = true;
}

// 移出某个位置图块，并在另一个位置插入该图块，这样做可以加快速度
void CCTextureAtlas::insertQuadFromIndex(unsigned int oldIndex, unsigned int newIndex)
{
	/// 有效性判断
    CCAssert( newIndex >= 0 && newIndex < m_uTotalQuads, "insertQuadFromIndex:atIndex: Invalid index");
    CCAssert( oldIndex >= 0 && oldIndex < m_uTotalQuads, "insertQuadFromIndex:atIndex: Invalid index");

    if( oldIndex == newIndex )
    {
        return;
    }
    // because it is ambiguous in iphone, so we implement abs ourselves
    // unsigned int howMany = abs( oldIndex - newIndex);
	//计算要移动的图块数量  
    unsigned int howMany = (oldIndex - newIndex) > 0 ? (oldIndex - newIndex) :  (newIndex - oldIndex);
    unsigned int dst = oldIndex;
    unsigned int src = oldIndex + 1;
    if( oldIndex > newIndex)
    {
        dst = newIndex+1;
        src = newIndex;
    }

    // texture coordinates
    ccV3F_C4B_T2F_Quad quadsBackup = m_pQuads[oldIndex];
	// 开始进行移动 
    memmove( &m_pQuads[dst],&m_pQuads[src], sizeof(m_pQuads[0]) * howMany );
	//更新新索引位置的数据 
    m_pQuads[newIndex] = quadsBackup;


    m_bDirty = true;

}
//移除指定位置的图块顶点缓冲 
void CCTextureAtlas::removeQuadAtIndex(unsigned int index)
{
    CCAssert( index < m_uTotalQuads, "removeQuadAtIndex: Invalid index");

    unsigned int remaining = (m_uTotalQuads-1) - index;

	//将索引图块后的所有图块向前移1个位置即可 
    // last object doesn't need to be moved
    if( remaining ) 
    {
        // texture coordinates
        memmove( &m_pQuads[index],&m_pQuads[index+1], sizeof(m_pQuads[0]) * remaining );
    }

    m_uTotalQuads--;


    m_bDirty = true;

}

void CCTextureAtlas::removeQuadsAtIndex(unsigned int index, unsigned int amount)
{
    CCAssert(index + amount <= m_uTotalQuads, "removeQuadAtIndex: index + amount out of bounds");

    unsigned int remaining = (m_uTotalQuads) - (index + amount);

    m_uTotalQuads -= amount;

    if ( remaining )
    {
        memmove( &m_pQuads[index], &m_pQuads[index+amount], sizeof(m_pQuads[0]) * remaining );
    }

    m_bDirty = true;
}
//移除所有的顶点缓冲数据
void CCTextureAtlas::removeAllQuads()
{//数量直接置0，这是最快的方式  
    m_uTotalQuads = 0;
}
// 重新设置图块顶点缓冲数组的容量  
// TextureAtlas - Resize
bool CCTextureAtlas::resizeCapacity(unsigned int newCapacity)
{
    if( newCapacity == m_uCapacity )
    {
        return true;
    }
    unsigned int uOldCapactiy = m_uCapacity; 
    // update capacity and totolQuads//确保当前绘制的精灵数量最大不能超过容量
    m_uTotalQuads = MIN(m_uTotalQuads, newCapacity);
	/// 更新容量
    m_uCapacity = newCapacity;

	//定义指针存放要申请的图块信息与索引数组的内存地址
    ccV3F_C4B_T2F_Quad* tmpQuads = NULL;
    GLushort* tmpIndices = NULL;
    
    // when calling initWithTexture(fileName, 0) on bada device, calloc(0, 1) will fail and return NULL,
    // so here must judge whether m_pQuads and m_pIndices is NULL.
	//为图块的顶点缓冲数组申请内存,注意：如果已占有内存则直接在原内存地址上进行内存长度调整 
    if (m_pQuads == NULL)
    {
        tmpQuads = (ccV3F_C4B_T2F_Quad*)malloc( m_uCapacity * sizeof(m_pQuads[0]) );
        if (tmpQuads != NULL)
        {
            memset(tmpQuads, 0, m_uCapacity * sizeof(m_pQuads[0]) );
        }
    }
    else
    {
		/// 重新调整
		tmpQuads = (ccV3F_C4B_T2F_Quad*)realloc( m_pQuads, sizeof(m_pQuads[0]) * m_uCapacity );
        if (tmpQuads != NULL && m_uCapacity > uOldCapactiy)
        {
            memset(tmpQuads+uOldCapactiy, 0, (m_uCapacity - uOldCapactiy)*sizeof(m_pQuads[0]) );
        }
    }
	//为图块的索引缓冲数组申请内存, 如果已占有内存则直接在原内存地址上进行内存长度调整
    if (m_pIndices == NULL)
    {    
        tmpIndices = (GLushort*)malloc( m_uCapacity * 6 * sizeof(m_pIndices[0]) );
        if (tmpIndices != NULL)
        {
            memset( tmpIndices, 0, m_uCapacity * 6 * sizeof(m_pIndices[0]) );
        }
        
    }
    else
    {
        tmpIndices = (GLushort*)realloc( m_pIndices, sizeof(m_pIndices[0]) * m_uCapacity * 6 );
        if (tmpIndices != NULL && m_uCapacity > uOldCapactiy)
        {
            memset( tmpIndices+uOldCapactiy, 0, (m_uCapacity-uOldCapactiy) * 6 * sizeof(m_pIndices[0]) );
        }
    }
	//如果内存申请失败的处理
    if( ! ( tmpQuads && tmpIndices) ) {
        CCLOG("cocos2d: CCTextureAtlas: not enough memory");
        CC_SAFE_FREE(tmpQuads);
        CC_SAFE_FREE(tmpIndices);
        CC_SAFE_FREE(m_pQuads);
        CC_SAFE_FREE(m_pIndices);
        m_uCapacity = m_uTotalQuads = 0;
        return false;
    }
	//如果成功，则将内存地址赋给相应成员指针变量
    m_pQuads = tmpQuads;
    m_pIndices = tmpIndices;

	//初始化索引缓冲 
    setupIndices();
    mapBuffers();

    m_bDirty = true;

    return true;
}

void CCTextureAtlas::increaseTotalQuadsWith(unsigned int amount)
{
    m_uTotalQuads += amount;
}

void CCTextureAtlas::moveQuadsFromIndex(unsigned int oldIndex, unsigned int amount, unsigned int newIndex)
{
    CCAssert(newIndex + amount <= m_uTotalQuads, "insertQuadFromIndex:atIndex: Invalid index");
    CCAssert(oldIndex < m_uTotalQuads, "insertQuadFromIndex:atIndex: Invalid index");

    if( oldIndex == newIndex )
    {
        return;
    }
    //create buffer
    size_t quadSize = sizeof(ccV3F_C4B_T2F_Quad);
    ccV3F_C4B_T2F_Quad* tempQuads = (ccV3F_C4B_T2F_Quad*)malloc( quadSize * amount);
    memcpy( tempQuads, &m_pQuads[oldIndex], quadSize * amount );

    if (newIndex < oldIndex)
    {
        // move quads from newIndex to newIndex + amount to make room for buffer
        memmove( &m_pQuads[newIndex], &m_pQuads[newIndex+amount], (oldIndex-newIndex)*quadSize);
    }
    else
    {
        // move quads above back
        memmove( &m_pQuads[oldIndex], &m_pQuads[oldIndex+amount], (newIndex-oldIndex)*quadSize);
    }
    memcpy( &m_pQuads[newIndex], tempQuads, amount*quadSize);

    free(tempQuads);

    m_bDirty = true;
}

void CCTextureAtlas::moveQuadsFromIndex(unsigned int index, unsigned int newIndex)
{
    CCAssert(newIndex + (m_uTotalQuads - index) <= m_uCapacity, "moveQuadsFromIndex move is out of bounds");

    memmove(m_pQuads + newIndex,m_pQuads + index, (m_uTotalQuads - index) * sizeof(m_pQuads[0]));
}

void CCTextureAtlas::fillWithEmptyQuadsFromIndex(unsigned int index, unsigned int amount)
{
    ccV3F_C4B_T2F_Quad quad;
    memset(&quad, 0, sizeof(quad));

    unsigned int to = index + amount;
    for (unsigned int i = index ; i < to ; i++)
    {
        m_pQuads[i] = quad;
    }
}

// TextureAtlas - Drawing

void CCTextureAtlas::drawQuads()
{
    this->drawNumberOfQuads(m_uTotalQuads, 0);
}

void CCTextureAtlas::drawNumberOfQuads(unsigned int n)
{
    this->drawNumberOfQuads(n, 0);
}
//绘制从指定的图块顶点缓冲数据起后面的N个图块顶点缓冲数据 
void CCTextureAtlas::drawNumberOfQuads(unsigned int n, unsigned int start)
{    
    if (0 == n) 
    {
        return;
    }
	//设置使用纹理 
    ccGLBindTexture2D(m_pTexture->getName());

#if CC_TEXTURE_ATLAS_USE_VAO

    //
    // Using VBO and VAO
    //
	//在绘制时如果遇到需要更新标记，则重新将顶点缓冲数据更新到绑定的对象上。 
    // XXX: update is done in draw... perhaps it should be done in a timer
    if (m_bDirty) 
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_pBuffersVBO[0]);
        // option 1: subdata
        //glBufferSubData(GL_ARRAY_BUFFER, sizeof(m_pQuads[0])*start, sizeof(m_pQuads[0]) * n , &m_pQuads[start] );
		
		// option 2: data
        //		glBufferData(GL_ARRAY_BUFFER, sizeof(quads_[0]) * (n-start), &quads_[start], GL_DYNAMIC_DRAW);
		
		// option 3: orphaning + glMapBuffer
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_pQuads[0]) * (n-start), NULL, GL_DYNAMIC_DRAW);
		void *buf = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		memcpy(buf, m_pQuads, sizeof(m_pQuads[0])* (n-start));
		glUnmapBuffer(GL_ARRAY_BUFFER);
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);

        m_bDirty = false;
    }

    ccGLBindVAO(m_uVAOname);

#if CC_REBIND_INDICES_BUFFER
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pBuffersVBO[1]);
#endif

#if CC_TEXTURE_ATLAS_USE_TRIANGLE_STRIP
    glDrawElements(GL_TRIANGLE_STRIP, (GLsizei) n*6, GL_UNSIGNED_SHORT, (GLvoid*) (start*6*sizeof(m_pIndices[0])) );
#else
    glDrawElements(GL_TRIANGLES, (GLsizei) n*6, GL_UNSIGNED_SHORT, (GLvoid*) (start*6*sizeof(m_pIndices[0])) );
#endif // CC_TEXTURE_ATLAS_USE_TRIANGLE_STRIP

#if CC_REBIND_INDICES_BUFFER
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#endif

//    glBindVertexArray(0);

#else // ! CC_TEXTURE_ATLAS_USE_VAO

    //
    // Using VBO without VAO
    //

#define kQuadSize sizeof(m_pQuads[0].bl)
    glBindBuffer(GL_ARRAY_BUFFER, m_pBuffersVBO[0]);

    // XXX: update is done in draw... perhaps it should be done in a timer
    if (m_bDirty) 
    {
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(m_pQuads[0])*start, sizeof(m_pQuads[0]) * n , &m_pQuads[start] );
        m_bDirty = false;
    }

    ccGLEnableVertexAttribs(kCCVertexAttribFlag_PosColorTex);

    // vertices
    glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, kQuadSize, (GLvoid*) offsetof(ccV3F_C4B_T2F, vertices));

    // colors
    glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (GLvoid*) offsetof(ccV3F_C4B_T2F, colors));

    // tex coords
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, kQuadSize, (GLvoid*) offsetof(ccV3F_C4B_T2F, texCoords));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pBuffersVBO[1]);

#if CC_TEXTURE_ATLAS_USE_TRIANGLE_STRIP
    glDrawElements(GL_TRIANGLE_STRIP, (GLsizei)n*6, GL_UNSIGNED_SHORT, (GLvoid*) (start*6*sizeof(m_pIndices[0])));
#else
    glDrawElements(GL_TRIANGLES, (GLsizei)n*6, GL_UNSIGNED_SHORT, (GLvoid*) (start*6*sizeof(m_pIndices[0])));
#endif // CC_TEXTURE_ATLAS_USE_TRIANGLE_STRIP

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

#endif // CC_TEXTURE_ATLAS_USE_VAO

    CC_INCREMENT_GL_DRAWS(1);
    CHECK_GL_ERROR_DEBUG();
}


NS_CC_END

