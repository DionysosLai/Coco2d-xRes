/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2009-2010 Ricardo Quesada
Copyright (C) 2009      Matt Oswald
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

#ifndef __CC_SPRITE_BATCH_NODE_H__
#define __CC_SPRITE_BATCH_NODE_H__

#include "base_nodes/CCNode.h"
#include "CCProtocols.h"
#include "textures/CCTextureAtlas.h"
#include "ccMacros.h"
#include "cocoa/CCArray.h"

NS_CC_BEGIN

/**
 * @addtogroup sprite_nodes
 * @{
 */

#define kDefaultSpriteBatchCapacity   29

class CCSprite;

/** CCSpriteBatchNode is like a batch node: if it contains children, it will draw them in 1 single OpenGL call
* (often known as "batch draw").
*
* A CCSpriteBatchNode can reference one and only one texture (one image file, one texture atlas).
* Only the CCSprites that are contained in that texture can be added to the CCSpriteBatchNode.
* All CCSprites added to a CCSpriteBatchNode are drawn in one OpenGL ES draw call.
* If the CCSprites are not added to a CCSpriteBatchNode then an OpenGL ES draw call will be needed for each one, which is less efficient.
*
*
* Limitations:
*  - The only object that is accepted as child (or grandchild, grand-grandchild, etc...) is CCSprite or any subclass of CCSprite. eg: particles, labels and layer can't be added to a CCSpriteBatchNode.
*  - Either all its children are Aliased or Antialiased. It can't be a mix. This is because "alias" is a property of the texture, and all the sprites share the same texture.
* 
* @since v0.7.1
*/
class CC_DLL CCSpriteBatchNode : public CCNode, public CCTextureProtocol
{
public:
    /**
     *  @js ctor
     */
    CCSpriteBatchNode();
    /**
     * @js NA
     * @lua NA
     */
    ~CCSpriteBatchNode();

    // property
    
    // retain// 取得纹理图块顶点缓冲管理器。
    inline CCTextureAtlas* getTextureAtlas(void) { return m_pobTextureAtlas; }
	//设置纹理图块顶点缓冲管理器  
    inline void setTextureAtlas(CCTextureAtlas* textureAtlas) 
    {  //如果当前使用的纹理图块顶点缓冲管理器与要设置的纹理图块顶点缓冲管理器不同，
/*		先释放原顶点缓冲管理器，再将当前使用的顶点缓冲管理器指针指向要设置顶点缓冲管理器。*/
        if (textureAtlas != m_pobTextureAtlas)
        {
            CC_SAFE_RETAIN(textureAtlas);
            CC_SAFE_RELEASE(m_pobTextureAtlas);
            m_pobTextureAtlas = textureAtlas;
        }
    }
	//取得存放所有使用此图块集的CCSprite指针数组 
    inline CCArray* getDescendants(void) { return m_pobDescendants; }

	//静态函数：通过纹理指针按照默认的图块数量创建一个CCSpriteBatch实例对象。
// 	注意：如果游戏运行中图块数量超过这个数值，则数量递增33%以满足需要 
    /** creates a CCSpriteBatchNode with a texture2d and capacity of children.
    The capacity will be increased in 33% in runtime if it run out of space.
    */
    static CCSpriteBatchNode* createWithTexture(CCTexture2D* tex, unsigned int capacity);
	//静态函数：通过纹理指针按照设定的图块数量创建一个CCSpriteBatch实例对象。
    static CCSpriteBatchNode* createWithTexture(CCTexture2D* tex) {
        return CCSpriteBatchNode::createWithTexture(tex, kDefaultSpriteBatchCapacity);
    }

    /** creates a CCSpriteBatchNode with a file image (.png, .jpeg, .pvr, etc) and capacity of children.
    The capacity will be increased in 33% in runtime if it run out of space.
    The file will be loaded using the TextureMgr.
    */
	//静态函数：通过图片名称按照设定的图块数量创建一个CCSpriteBatch实例对象。
    static CCSpriteBatchNode* create(const char* fileImage, unsigned int capacity);
	//静态函数：通过图片名称按照默认的图块数量29创建一个CCSpriteBatch实例对象。
    static CCSpriteBatchNode* create(const char* fileImage) {
        return CCSpriteBatchNode::create(fileImage, kDefaultSpriteBatchCapacity);
    }

    /** initializes a CCSpriteBatchNode with a texture2d and capacity of children.
    The capacity will be increased in 33% in runtime if it run out of space.
    */
	//通过纹理指针和图块数量进行初始化
    bool initWithTexture(CCTexture2D *tex, unsigned int capacity);
    /** initializes a CCSpriteBatchNode with a file image (.png, .jpeg, .pvr, etc) and a capacity of children.
    The capacity will be increased in 33% in runtime if it run out of space.
    The file will be loaded using the TextureMgr.
    */
	//通过图片名称和图块数量进行初始化。
    bool initWithFile(const char* fileImage, unsigned int capacity);
    bool init();

	//扩容，递增1/3的图块数量  
    void increaseAtlasCapacity();

    /** removes a child given a certain index. It will also cleanup the running actions depending on the cleanup parameter.
    @warning Removing a child from a CCSpriteBatchNode is very slow
    *///通过索引移除一个子结点，参数doCleanUp决定移除的同时是否对其进行释放。
    void removeChildAtIndex(unsigned int index, bool doCleanup);
	//将一个指定的CCSprite指针插入到子节点指针数组的指定位置 
    void insertChild(CCSprite *child, unsigned int index);
    void appendChild(CCSprite* sprite);
	//移除一个指定的CCSprite指针
    void removeSpriteFromAtlas(CCSprite *sprite);
	//重新构建指定的CCSprite指针下的所有子结点的索引 
    unsigned int rebuildIndexInOrder(CCSprite *parent, unsigned int index);
	//取得指定CCSprite的所有节点的最大图块索引  
    unsigned int highestAtlasIndexInChild(CCSprite *sprite);
	//取得指定CCSprite的所有节点的最小图块索引  
    unsigned int lowestAtlasIndexInChild(CCSprite *sprite);
	//取得指定CCSprite的Z顺序之上或之下的最近的图块索引  
    unsigned int atlasIndexForChild(CCSprite *sprite, int z);
    /* Sprites use this to start sortChildren, don't call this manually */
    void reorderBatch(bool reorder);

	// 实现CCTextureProtocol的接口函数  
	// 取得纹理  
    // CCTextureProtocol
    virtual CCTexture2D* getTexture(void);
	// 设置纹理  
    virtual void setTexture(CCTexture2D *texture);
	//为纹理设置使用的混合状态  
    virtual void setBlendFunc(ccBlendFunc blendFunc);
    /**
     * @js NA
     *///取得混合状态  
    virtual ccBlendFunc getBlendFunc(void);


	//重载CCNode的相关函数  
	//每一帧要被调用的函数 
    virtual void visit(void);
	//将一个CCNode指针做为自已的子结点  
    virtual void addChild(CCNode * child);
	//将一个CCNode指针以指定的Z顺序做为自已的子结点 
    virtual void addChild(CCNode * child, int zOrder);
	//将一个CCNode指针以指定的Z顺序并附带一个参数值做为自已的子结点  
    virtual void addChild(CCNode * child, int zOrder, int tag);
	//将一个子结点重新设置Z顺序  
    virtual void reorderChild(CCNode * child, int zOrder);
     //将一个子结点删除，并根所参数决定是否释放元素
    virtual void removeChild(CCNode* child, bool cleanup);
	//将所有的子结点移除，并根所参数决定是否释放元素  
    virtual void removeAllChildrenWithCleanup(bool cleanup);
    virtual void sortAllChildren();
	//绘制函数  
    virtual void draw(void);

protected:
    /** Inserts a quad at a certain index into the texture atlas. The CCSprite won't be added into the children array.
     This method should be called only when you are dealing with very big AtlasSrite and when most of the CCSprite won't be updated.
     For example: a tile map (CCTMXMap) or a label with lots of characters (CCLabelBMFont)
     */
	//从精灵数据中在图块管理器中插入一个新的图块，注意：并不将精灵放入子结点。
    void insertQuadFromSprite(CCSprite *sprite, unsigned int index);
    /** Updates a quad at a certain index into the texture atlas. The CCSprite won't be added into the children array.
     This method should be called only when you are dealing with very big AtlasSrite and when most of the CCSprite won't be updated.
     For example: a tile map (CCTMXMap) or a label with lots of characters (CCLabelBMFont)
     */
    void updateQuadFromSprite(CCSprite *sprite, unsigned int index);
    /* This is the opposite of "insertQuadFromSprite.
    It add the sprite to the children and descendants array, but it doesn't update add it to the texture atlas
    *///在子结点数组指定位置插入一个新的精灵。注意：并不新建图块。
    CCSpriteBatchNode * addSpriteWithoutQuad(CCSprite*child, unsigned int z, int aTag);

private:
    void updateAtlasIndex(CCSprite* sprite, int* curIndex);
    void swap(int oldIndex, int newIndex);
	//更新混合状态 
    void updateBlendFunc();

protected:
	//使用的纹理图块顶点缓冲管理器 
    CCTextureAtlas *m_pobTextureAtlas;
	//混合状态  
    ccBlendFunc m_blendFunc;
	//指向存放所有使用此纹理中的图块的CCSprite指针数组的指针 
    // all descendants: children, gran children, etc...
    CCArray* m_pobDescendants;
};

// end of sprite_nodes group
/// @}

NS_CC_END

#endif // __CC_SPRITE_BATCH_NODE_H__
