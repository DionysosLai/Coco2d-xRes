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

#ifndef __CCTEXTURE_ATLAS_H__
#define __CCTEXTURE_ATLAS_H__

#include "ccTypes.h"
#include "cocoa/CCObject.h"
#include "ccConfig.h"
#include <string>

NS_CC_BEGIN

class CCTexture2D;

/**
 * @addtogroup textures
 * @{
 */

/** @brief A class that implements a Texture Atlas.
Supported features:
* The atlas file can be a PVRTC, PNG or any other format supported by Texture2D
* Quads can be updated in runtime
* Quads can be added in runtime
* Quads can be removed in runtime
* Quads can be re-ordered in runtime
* The TextureAtlas capacity can be increased or decreased in runtime
* OpenGL component: V3F, C4B, T2F.
The quads are rendered using an OpenGL ES VBO.
To render the quads using an interleaved vertex array list, you should modify the ccConfig.h file 
*/
/// CCTextureAtlas由CCObject派生而来  
class CC_DLL CCTextureAtlas : public CCObject 
{
protected:
	//使用此大图中的图块的精灵对应的三角形索引数组的指针
    GLushort*           m_pIndices;
#if CC_TEXTURE_ATLAS_USE_VAO
    GLuint              m_uVAOname;
#endif
	//如果使用Vertex Buffer Object(VBO:使用显存而非内存存储顶点缓冲数据，大大提高效率)，
	/*	建立VBO句柄数组，第一个元素存顶点数组的句柄，第二个元素存索引数组句柄 */
    GLuint              m_pBuffersVBO[2]; //0: vertex  1: indices
	//标记是否更新需要更新的图块信息。当你新加入了图块或者修改了图块，需要设置为true。
    bool                m_bDirty; //indicates whether or not the array buffer of the VBO needs to be updated


	// CC_PROPERTY_READONLY宏为类定义变量及增加相应的get函数。  
	//当前使用图块的数量 
    /** quantity of quads that are going to be drawn */
    CC_PROPERTY_READONLY(unsigned int, m_uTotalQuads, TotalQuads)
    /** quantity of quads that can be stored with the current texture atlas size //存储图块信息的数组容量  */
    CC_PROPERTY_READONLY(unsigned int, m_uCapacity, Capacity)
    /** Texture of the texture atlas  //设置所使用的大图纹理 */
    CC_PROPERTY(CCTexture2D *, m_pTexture, Texture)
    /** Quads that are going to be rendered  //使用此大图的图块的所有精灵的顶点缓冲信息数组  */
    CC_PROPERTY(ccV3F_C4B_T2F_Quad *, m_pQuads, Quads)

public:
    /**
     * @js ctor
     */
    CCTextureAtlas();
    /**
     *  @js NA
     *  @lua NA
     */
    virtual ~CCTextureAtlas();
    /**
     *  @js NA
     *  @lua NA
     */
    const char* description();

    /** creates a TextureAtlas 图片集基类 with an filename and with an initial capacity for Quads.
    * The TextureAtlas capacity can be increased in runtime. 从文件中创建纹理，并初始化图块量大小；
    */
    static CCTextureAtlas* create(const char* file , unsigned int capacity);

    /** initializes a TextureAtlas with a filename and with a certain capacity for Quads.
    * The TextureAtlas capacity can be increased in runtime.
    *
    * WARNING: Do not reinitialize 重新启动the TextureAtlas because it will leak memory (issue #706) ->不要重复调用
    */
    bool initWithFile(const char* file, unsigned int capacity);

   /** creates a TextureAtlas with a previously initialized Texture2D object, and
    * with an initial capacity for n Quads. 从贴图中创建纹理
    * The TextureAtlas capacity can be increased in runtime.
    */S
    static CCTextureAtlas* createWithTexture(CCTexture2D *texture, unsigned int capacity);


    /** initializes a TextureAtlas with a previously initialized Texture2D object, and
    * with an initial capacity for Quads. 
    * The TextureAtlas capacity can be increased in runtime.
    *
    * WARNING: Do not reinitialize the TextureAtlas because it will leak memory (issue #706)
    */
    bool initWithTexture(CCTexture2D *texture, unsigned int capacity);

    /** updates a Quad (texture, vertex and color) at a certain index
    * index must be between 0 and the atlas capacity - 1通过索引值找到对应的图块顶点缓冲数据并用新数据修改它，
	由CCSprite实例对象在变换顶点信息时调用。  
    @since v0.8 
    */
    void updateQuad(ccV3F_C4B_T2F_Quad* quad, unsigned int index);

    /** Inserts a Quad (texture, vertex and color) at a certain index
    index must be between 0 and the atlas capacity - 1
    @since v0.8
    */
    void insertQuad(ccV3F_C4B_T2F_Quad* quad, unsigned int index);

    /** Inserts a c array of quads at a given index 通过索引值找到对应的图块顶点缓冲数据，并在其之前插入一个新的图块
     index must be between 0 and the atlas capacity - 1
     this method doesn't enlarge the array when amount + index > totalQuads
     @since v1.1
    */
    void insertQuads(ccV3F_C4B_T2F_Quad* quads, unsigned int index, unsigned int amount);

    /** Removes the quad that is located at a certain index and inserts it at a new index
    This operation is faster than removing and inserting in a quad in 2 different steps
    @since v0.7.2 移出某个位置图块，并在另一个位置插入，这样做可以加快速度
    */
    void insertQuadFromIndex(unsigned int fromIndex, unsigned int newIndex);

    /** removes a quad at a given index number.
    The capacity remains the same, but the total number of quads to be drawn is reduced in 1
    @since v0.7.2
    */
    void removeQuadAtIndex(unsigned int index);

    /** removes a amount of quads starting from index 移除从指定点开始的，总数amout的图块
        @since 1.1
     */
    void removeQuadsAtIndex(unsigned int index, unsigned int amount);
    /** removes all Quads.
    The TextureAtlas capacity remains untouched. No memory is freed.
    The total number of quads to be drawn will be 0
    @since v0.7.2
    */
    void removeAllQuads();


    /** resize the capacity of the CCTextureAtlas.
    * The new capacity can be lower or higher than the current one
    * It returns YES if the resize was successful.
    * If it fails to resize the capacity it will return NO with a new capacity of 0.
    */
    bool resizeCapacity(unsigned int n);

    /**
     Used internally by CCParticleBatchNode
     don't use this unless you know what you're doing
     @since 1.1
    */
    void increaseTotalQuadsWith(unsigned int amount);

    /** Moves an amount of quads from oldIndex at newIndex
     @since v1.1
     */
    void moveQuadsFromIndex(unsigned int oldIndex, unsigned int amount, unsigned int newIndex);

    /**
     Moves quads from index till totalQuads to the newIndex
     Used internally by CCParticleBatchNode
     This method doesn't enlarge the array if newIndex + quads to be moved > capacity
     @since 1.1
    */
    void moveQuadsFromIndex(unsigned int index, unsigned int newIndex);

    /**
     Ensures that after a realloc quads are still empty
     Used internally by CCParticleBatchNode
     @since 1.1
    */
    void fillWithEmptyQuadsFromIndex(unsigned int index, unsigned int amount);

    /** draws n quads
    * n can't be greater than the capacity of the Atlas
    */
    void drawNumberOfQuads(unsigned int n);

    /** draws n quads from an index (offset).
    n + start can't be greater than the capacity of the atlas

    @since v1.0
    */
    void drawNumberOfQuads(unsigned int n, unsigned int start);

    /** draws all the Atlas's Quads
    */
    void drawQuads();
    /** listen the event that coming to foreground on Android
     */
    void listenBackToForeground(CCObject *obj);

    /** whether or not the array buffer of the VBO needs to be updated*/
    inline bool isDirty(void) { return m_bDirty; }
    /** specify if the array buffer of the VBO needs to be updated */
    inline void setDirty(bool bDirty) { m_bDirty = bDirty; }

private:
    void setupIndices();
    void mapBuffers();
#if CC_TEXTURE_ATLAS_USE_VAO
    void setupVBOandVAO();
#else
    void setupVBO();
#endif
};

// end of textures group
/// @}

NS_CC_END

#endif //__CCTEXTURE_ATLAS_H__


