/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2009-2010 Ricardo Quesada
Copyright (c) 2009      Matt Oswald
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
#include "CCSpriteBatchNode.h"
#include "ccConfig.h"
#include "CCSprite.h"
#include "effects/CCGrid.h"
#include "draw_nodes/CCDrawingPrimitives.h"
#include "textures/CCTextureCache.h"
#include "support/CCPointExtension.h"
#include "shaders/CCShaderCache.h"
#include "shaders/CCGLProgram.h"
#include "shaders/ccGLStateCache.h"
#include "CCDirector.h"
#include "support/TransformUtils.h"
#include "support/CCProfiling.h"
// external
#include "kazmath/GL/matrix.h"

NS_CC_BEGIN

/*
* creation with CCTexture2D
*/
//静态函数：通过纹理指针按照设定的图块数量创建一个CCSpriteBatch实例对象。
CCSpriteBatchNode* CCSpriteBatchNode::createWithTexture(CCTexture2D* tex, unsigned int capacity/* = kDefaultSpriteBatchCapacity*/)
{
	//使用new创建一个CCSpriteBatchNode实例对象。
    CCSpriteBatchNode *batchNode = new CCSpriteBatchNode();
	//使用纹理指针和默认图块数量对CCSpriteBatchNode实例对象进行初始化。
    batchNode->initWithTexture(tex, capacity);
	//设置其由内存管理器进行内存释放。
    batchNode->autorelease();
	//返回新创建的CCSpriteBatchNode实例对象  
    return batchNode;
}

/*
* creation with File Image
*/

CCSpriteBatchNode* CCSpriteBatchNode::create(const char *fileImage, unsigned int capacity/* = kDefaultSpriteBatchCapacity*/)
{
    CCSpriteBatchNode *batchNode = new CCSpriteBatchNode();
    batchNode->initWithFile(fileImage, capacity);
    batchNode->autorelease();

    return batchNode;
}

/*
* init with CCTexture2D
*///通过纹理指针和图块数量进行初始化。注意：这个函数才是真正进行初始化的实现过程。
bool CCSpriteBatchNode::initWithTexture(CCTexture2D *tex, unsigned int capacity)
{
	//设置混合系数，源混合系数为CC_BLEND_SRC，目标混合系数为CC_BLEND_DST，
// 	则图像绘制时按照纹理或色彩的Alpha值与背景进行混合。
    m_blendFunc.src = CC_BLEND_SRC;
    m_blendFunc.dst = CC_BLEND_DST;
	//创建一个图片集管理器
    m_pobTextureAtlas = new CCTextureAtlas();

	/// 如果容量为0，则表示容量默认为29
    if (0 == capacity)
    {
        capacity = kDefaultSpriteBatchCapacity;
    }
    //将纹理和图块数量做为参数初始化图片集,这里可知图块集还是依靠CCTextureAtlas类。
    m_pobTextureAtlas->initWithTexture(tex, capacity);
	//更新混合状态
    updateBlendFunc();

    // no lazy alloc in this node// 新建一个CCArray实例，将m_pChildren指向它 
    m_pChildren = new CCArray();
    m_pChildren->initWithCapacity(capacity);

    m_pobDescendants = new CCArray();
    m_pobDescendants->initWithCapacity(capacity);

    setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor));
    return true;
}

bool CCSpriteBatchNode::init()
{
    CCTexture2D * texture = new CCTexture2D();
    texture->autorelease();
    return this->initWithTexture(texture, 0);
}

/*
* init with FileImage
*/
bool CCSpriteBatchNode::initWithFile(const char* fileImage, unsigned int capacity)
{
    CCTexture2D *pTexture2D = CCTextureCache::sharedTextureCache()->addImage(fileImage);
    return initWithTexture(pTexture2D, capacity);
}

CCSpriteBatchNode::CCSpriteBatchNode()
: m_pobTextureAtlas(NULL)
, m_pobDescendants(NULL)
{
}

CCSpriteBatchNode::~CCSpriteBatchNode()
{
	/// 释放图片集
    CC_SAFE_RELEASE(m_pobTextureAtlas);
    CC_SAFE_RELEASE(m_pobDescendants);
}

// override visit
// don't call visit on it's children
// 是基类CCNode虚函数，是每帧会被调用到的函数。
void CCSpriteBatchNode::visit(void)
{
    CC_PROFILER_START_CATEGORY(kCCProfilerCategoryBatchSprite, "CCSpriteBatchNode - visit");

    // CAREFUL:
    // This visit is almost identical to CocosNode#visit
    // with the exception that it doesn't call visit on it's children
    //
    // The alternative is to have a void CCSprite#visit, but
    // although this is less maintainable, is faster
    //
	// 如果不显示，直接返回
    if (! m_bVisible)
    {
        return;
    }
	//矩阵压栈，保存渲染此结点前的所有OpenGL所需矩阵的值 
    kmGLPushMatrix();

    if (m_pGrid && m_pGrid->isActive())
    {
        m_pGrid->beforeDraw();
        transformAncestors();
    }

    sortAllChildren();
	//矩阵变量
    transform();
	//基类CCNode虚函数，用于实现当前CCNode的绘制
    draw();

    if (m_pGrid && m_pGrid->isActive())
    {
        m_pGrid->afterDraw(this);
    }
	//矩阵出栈。恢复渲染此结点前的所有OpenGL所需矩阵的值
    kmGLPopMatrix();
    setOrderOfArrival(0);

    CC_PROFILER_STOP_CATEGORY(kCCProfilerCategoryBatchSprite, "CCSpriteBatchNode - visit");

}
//将一个CCSprite指针以指定的Z顺序并附带一个参数值做为子结点 
void CCSpriteBatchNode::addChild(CCNode *child, int zOrder, int tag)
{
    CCAssert(child != NULL, "child should not be null");
    CCAssert(dynamic_cast<CCSprite*>(child) != NULL, "CCSpriteBatchNode only supports CCSprites as children");

	//将结点转为CCSprite 
    CCSprite *pSprite = (CCSprite*)(child);
    // check CCSprite is using the same texture id
	//确保pSprite的纹理与图片集的纹理相同才可以放为子结点 
    CCAssert(pSprite->getTexture()->getName() == m_pobTextureAtlas->getTexture()->getName(), "CCSprite is not using the same texture id");

    CCNode::addChild(child, zOrder, tag);

    appendChild(pSprite);
}

void CCSpriteBatchNode::addChild(CCNode *child)
{
    CCNode::addChild(child);
}

void CCSpriteBatchNode::addChild(CCNode *child, int zOrder)
{
    CCNode::addChild(child, zOrder);
}

// override reorderChild
//将子结点重新设置Z顺序。
void CCSpriteBatchNode::reorderChild(CCNode *child, int zOrder)
{
    CCAssert(child != NULL, "the child should not be null");
	//确保此结点是挂在子结点树上。
    CCAssert(m_pChildren->containsObject(child), "Child doesn't belong to Sprite");

	//如果顺序已经与结点的Z顺序相等则直接返回 
    if (zOrder == child->getZOrder())
    {
        return;
    }

    //set the z-order and sort later
    CCNode::reorderChild(child, zOrder);
}

// override remove child
 //将一个CCNode指针从子结点数组中删除，并根所参数决定是否释放元素  
void CCSpriteBatchNode::removeChild(CCNode *child, bool cleanup)
{
    CCSprite *pSprite = (CCSprite*)(child);

    // explicit null handling
    if (pSprite == NULL)
    {
        return;
    }
	//确保子结点树中有pSprite 
    CCAssert(m_pChildren->containsObject(pSprite), "sprite batch node should contain the child");

    // cleanup before removing//移除一个指定的CCSprite指针 
    removeSpriteFromAtlas(pSprite);
	//调用基类的移除子结点函数
    CCNode::removeChild(pSprite, cleanup);
}
//通过索引移除一个子结点，参数doCleanUp决定移除的同时是否对其进行释放。
void CCSpriteBatchNode::removeChildAtIndex(unsigned int uIndex, bool bDoCleanup)
{
    removeChild((CCSprite*)(m_pChildren->objectAtIndex(uIndex)), bDoCleanup);
}
//将所有的子结点数组元素移除，并根所参数决定是否释放元素
void CCSpriteBatchNode::removeAllChildrenWithCleanup(bool bCleanup)
{
    // Invalidate atlas index. issue #569
    // useSelfRender should be performed on all descendants. issue #1216
    arrayMakeObjectsPerformSelectorWithObject(m_pobDescendants, setBatchNode, NULL, CCSprite*);

    CCNode::removeAllChildrenWithCleanup(bCleanup);

    m_pobDescendants->removeAllObjects();
    m_pobTextureAtlas->removeAllQuads();
}

//override sortAllChildren
void CCSpriteBatchNode::sortAllChildren()
{
    if (m_bReorderChildDirty)
    {
        int i = 0,j = 0,length = m_pChildren->data->num;
        CCNode ** x = (CCNode**)m_pChildren->data->arr;
        CCNode *tempItem = NULL;

        //insertion sort
        for(i=1; i<length; i++)
        {
            tempItem = x[i];
            j = i-1;

            //continue moving element downwards while zOrder is smaller or when zOrder is the same but orderOfArrival is smaller
            while(j>=0 && ( tempItem->getZOrder() < x[j]->getZOrder() || ( tempItem->getZOrder() == x[j]->getZOrder() && tempItem->getOrderOfArrival() < x[j]->getOrderOfArrival() ) ) )
            {
                x[j+1] = x[j];
                j--;
            }

            x[j+1] = tempItem;
        }

        //sorted now check all children
        if (m_pChildren->count() > 0)
        {
            //first sort all children recursively based on zOrder
            arrayMakeObjectsPerformSelector(m_pChildren, sortAllChildren, CCSprite*);

            int index=0;

            CCObject* pObj = NULL;
            //fast dispatch, give every child a new atlasIndex based on their relative zOrder (keep parent -> child relations intact)
            // and at the same time reorder descendants and the quads to the right index
            CCARRAY_FOREACH(m_pChildren, pObj)
            {
                CCSprite* pChild = (CCSprite*)pObj;
                updateAtlasIndex(pChild, &index);
            }
        }

        m_bReorderChildDirty=false;
    }
}

void CCSpriteBatchNode::updateAtlasIndex(CCSprite* sprite, int* curIndex)
{
    unsigned int count = 0;
    CCArray* pArray = sprite->getChildren();
    if (pArray != NULL)
    {
        count = pArray->count();
    }
    
    int oldIndex = 0;

    if( count == 0 )
    {
        oldIndex = sprite->getAtlasIndex();
        sprite->setAtlasIndex(*curIndex);
        sprite->setOrderOfArrival(0);
        if (oldIndex != *curIndex){
            swap(oldIndex, *curIndex);
        }
        (*curIndex)++;
    }
    else
    {
        bool needNewIndex=true;

        if (((CCSprite*) (pArray->data->arr[0]))->getZOrder() >= 0)
        {
            //all children are in front of the parent
            oldIndex = sprite->getAtlasIndex();
            sprite->setAtlasIndex(*curIndex);
            sprite->setOrderOfArrival(0);
            if (oldIndex != *curIndex)
            {
                swap(oldIndex, *curIndex);
            }
            (*curIndex)++;

            needNewIndex = false;
        }

        CCObject* pObj = NULL;
        CCARRAY_FOREACH(pArray,pObj)
        {
            CCSprite* child = (CCSprite*)pObj;
            if (needNewIndex && child->getZOrder() >= 0)
            {
                oldIndex = sprite->getAtlasIndex();
                sprite->setAtlasIndex(*curIndex);
                sprite->setOrderOfArrival(0);
                if (oldIndex != *curIndex) {
                    this->swap(oldIndex, *curIndex);
                }
                (*curIndex)++;
                needNewIndex = false;

            }

            updateAtlasIndex(child, curIndex);
        }

        if (needNewIndex)
        {//all children have a zOrder < 0)
            oldIndex=sprite->getAtlasIndex();
            sprite->setAtlasIndex(*curIndex);
            sprite->setOrderOfArrival(0);
            if (oldIndex!=*curIndex) {
                swap(oldIndex, *curIndex);
            }
            (*curIndex)++;
        }
    }
}

void CCSpriteBatchNode::swap(int oldIndex, int newIndex)
{
    CCObject** x = m_pobDescendants->data->arr;
    ccV3F_C4B_T2F_Quad* quads = m_pobTextureAtlas->getQuads();

    CCObject* tempItem = x[oldIndex];
    ccV3F_C4B_T2F_Quad tempItemQuad=quads[oldIndex];

    //update the index of other swapped item
    ((CCSprite*) x[newIndex])->setAtlasIndex(oldIndex);

    x[oldIndex]=x[newIndex];
    quads[oldIndex]=quads[newIndex];
    x[newIndex]=tempItem;
    quads[newIndex]=tempItemQuad;
}

void CCSpriteBatchNode::reorderBatch(bool reorder)
{
    m_bReorderChildDirty=reorder;
}

// draw
void CCSpriteBatchNode::draw(void)
{
    CC_PROFILER_START("CCSpriteBatchNode - draw");

    // Optimization: Fast Dispatch
	// 如果图块集为空直接返回  
    if( m_pobTextureAtlas->getTotalQuads() == 0 )
    {
        return;
    }

    CC_NODE_DRAW_SETUP();

    arrayMakeObjectsPerformSelector(m_pChildren, updateTransform, CCSprite*);

    ccGLBlendFunc( m_blendFunc.src, m_blendFunc.dst );

    m_pobTextureAtlas->drawQuads();

    CC_PROFILER_STOP("CCSpriteBatchNode - draw");
}
//扩容，递增1/3的图块数量 
void CCSpriteBatchNode::increaseAtlasCapacity(void)
{
    // if we're going beyond the current TextureAtlas's capacity,
    // all the previously initialized sprites will need to redo their texture coords
    // this is likely computationally expensive
	// 如果当前需要的图块数量比实际图块数量大，则将图块的数量增加1/3。 
    unsigned int quantity = (m_pobTextureAtlas->getCapacity() + 1) * 4 / 3;
	// 打印图块数量的变化信息
    CCLOG("cocos2d: CCSpriteBatchNode: resizing TextureAtlas capacity from [%lu] to [%lu].",
        (long)m_pobTextureAtlas->getCapacity(),
        (long)quantity);
	//图块集重新设置数量，resizeCapacity按新的图块集数量重新计算每个图块的纹理UV 
    if (! m_pobTextureAtlas->resizeCapacity(quantity))
    {
        // serious problems
        CCLOGWARN("cocos2d: WARNING: Not enough memory to resize the atlas");
        CCAssert(false, "Not enough memory to resize the atlas");
    }
}
//重新构建指定的CCSprite指针下的所有子结点的索引 
unsigned int CCSpriteBatchNode::rebuildIndexInOrder(CCSprite *pobParent, unsigned int uIndex)
{
	//取得参数pobParent的所有子结点数组 
    CCArray *pChildren = pobParent->getChildren();
	//如果子结点数组不为空
    if (pChildren && pChildren->count() > 0)
    {
		//遍历子结点数组并
        CCObject* pObject = NULL;
        CCARRAY_FOREACH(pChildren, pObject)
        {
            CCSprite* pChild = (CCSprite*) pObject;
            if (pChild && (pChild->getZOrder() < 0))
            {
                uIndex = rebuildIndexInOrder(pChild, uIndex);
            }
        }
    }    

    // ignore self (batch node)
    if (! pobParent->isEqual(this))
    {
        pobParent->setAtlasIndex(uIndex);
        uIndex++;
    }

	//如果子结点数组不为空
    if (pChildren && pChildren->count() > 0)
    {
        CCObject* pObject = NULL;
        CCARRAY_FOREACH(pChildren, pObject)
        {
            CCSprite* pChild = (CCSprite*) pObject;
            if (pChild && (pChild->getZOrder() >= 0))
            {
                uIndex = rebuildIndexInOrder(pChild, uIndex);
            }
        }
    }

    return uIndex;
}

//取得指定CCSprite的所有节点的最大图块索引  
unsigned int CCSpriteBatchNode::highestAtlasIndexInChild(CCSprite *pSprite)
{
	//取得指定CCSprite的子结点数组 
    CCArray *pChildren = pSprite->getChildren();
	//如果没有子结点，直接返回指定CCSprite的图块索引  
    if (! pChildren || pChildren->count() == 0)
    {
        return pSprite->getAtlasIndex();
    }
    else
    {
        return highestAtlasIndexInChild((CCSprite*)(pChildren->lastObject()));
    }
}

//取得指定CCSprite的所有节点的最小图块索引
unsigned int CCSpriteBatchNode::lowestAtlasIndexInChild(CCSprite *pSprite)
{	
	//取得指定CCSprite的子结点数
    CCArray *pChildren = pSprite->getChildren();

    if (! pChildren || pChildren->count() == 0)
    {
		//取得指定CCSprite的子结点数组
        return pSprite->getAtlasIndex();
    }
    else
    {
        return lowestAtlasIndexInChild((CCSprite*)(pChildren->objectAtIndex(0)));
    }
}

//取得对应的子结点的Z顺序之上或之下的最近图块索引
unsigned int CCSpriteBatchNode::atlasIndexForChild(CCSprite *pobSprite, int nZ)
{
	//取得pobSprite的父节点的所有子结点数组。
    CCArray *pBrothers = pobSprite->getParent()->getChildren();
	//查询pobSprite在所有子结点数组中的索引
    unsigned int uChildIndex = pBrothers->indexOfObject(pobSprite);

    // ignore parent Z if parent is spriteSheet
	// 判断当前节点是否是pobSprite的父结点，将结果存入bIgnoreParent。
    bool bIgnoreParent = (CCSpriteBatchNode*)(pobSprite->getParent()) == this;
	// 新建指针变量pPrevious，用于存储pobSprite的前一个结点。
    CCSprite *pPrevious = NULL;
    if (uChildIndex > 0 &&
        uChildIndex < UINT_MAX)
    {
		//取得pobSprite在所有子结点中的位置前一个节点。
        pPrevious = (CCSprite*)(pBrothers->objectAtIndex(uChildIndex - 1));
    }

	// 如果当前节点是pobSprite的父结点。
    // first child of the sprite sheet
    if (bIgnoreParent)
    {
		//如果pobSprite就是第一个子结点。则返回0。  
        if (uChildIndex == 0)
        {
            return 0;
        }
		//如果pobSprite不是第一个子结点，则返回pobSprite的前一个结点的最大图块索引，加1返回。  
        return highestAtlasIndexInChild(pPrevious) + 1;
    }

    // parent is a CCSprite, so, it must be taken into account

    // first child of an CCSprite ?
	// 当前节点不是pobSprite的父结点。  
	// 如果pobSprite是其父节点的第一子结点 
    if (uChildIndex == 0)
    {
		//新建指针变量p，指向pobSprite的父结点。
        CCSprite *p = (CCSprite*)(pobSprite->getParent());

		// less than parent and brothers  
		// 如果nZ小于零,即在其之下的最近索引
        // less than parent and brothers
        if (nZ < 0)
        {
			//返回pobSprite的父结点的图块索引。
            return p->getAtlasIndex();
        }
        else
        {
			//如果nZ大于零，即在其之上的最近索引  
			//返回pobSprite的父结点的图块索引值加1。
            return p->getAtlasIndex() + 1;
        }
    }
    else
    {
		//如果pobSprite不是其父节点的第一子结点
        // previous & sprite belong to the same branch
        if ((pPrevious->getZOrder() < 0 && nZ < 0) || (pPrevious->getZOrder() >= 0 && nZ >= 0))
        {
			//返回pobSprite前一个节点的的最大图块索引加1 
            return highestAtlasIndexInChild(pPrevious) + 1;
        }
		// 否则返回其父节点的图块索引值加1 
        // else (previous < 0 and sprite >= 0 )
        CCSprite *p = (CCSprite*)(pobSprite->getParent());
        return p->getAtlasIndex() + 1;
    }
	// 如果进行到这一步，已经出错了。打印出错信息。
    // Should not happen. Error calculating Z on SpriteSheet
    CCAssert(0, "should not run here");
    return 0;
}

// add child helper

//将一个CCSprite插入到子节点指针数组的指定位置 
void CCSpriteBatchNode::insertChild(CCSprite *pSprite, unsigned int uIndex)
{
	//pobSprite应用此图片集
    pSprite->setBatchNode(this);
	//设置图块集索引值  
    pSprite->setAtlasIndex(uIndex);
	//设置pobSprite在绘制时需要进行绑定图块的更新 
    pSprite->setDirty(true);

	//如果图块集已经达到最大容量则进行扩容
    if(m_pobTextureAtlas->getTotalQuads() == m_pobTextureAtlas->getCapacity())
    {
        increaseAtlasCapacity();
    }
	//在图块集管理器中新增加一个图块，并指定其索引 
    ccV3F_C4B_T2F_Quad quad = pSprite->getQuad();
	//将pobSprite放入到使用此图集子结点数组的对应位置
    m_pobTextureAtlas->insertQuad(&quad, uIndex);

    ccArray *descendantsData = m_pobDescendants->data;

    ccArrayInsertObjectAtIndex(descendantsData, pSprite, uIndex);

    // update indices 跟新索引
    unsigned int i = uIndex+1;
    
    CCSprite* pChild = NULL;
    for(; i<descendantsData->num; i++){
        pChild = (CCSprite*)descendantsData->arr[i];
        pChild->setAtlasIndex(pChild->getAtlasIndex() + 1);
    }

    // add children recursively
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(pSprite->getChildren(), pObj)
    {
        pChild = (CCSprite*)pObj;
        unsigned int idx = atlasIndexForChild(pChild, pChild->getZOrder());
        insertChild(pChild, idx);
    }
}

// addChild helper, faster than insertChild
void CCSpriteBatchNode::appendChild(CCSprite* sprite)
{
    m_bReorderChildDirty=true;
    sprite->setBatchNode(this);
    sprite->setDirty(true);

    if(m_pobTextureAtlas->getTotalQuads() == m_pobTextureAtlas->getCapacity()) {
        increaseAtlasCapacity();
    }

    ccArray *descendantsData = m_pobDescendants->data;

    ccArrayAppendObjectWithResize(descendantsData, sprite);

    unsigned int index=descendantsData->num-1;

    sprite->setAtlasIndex(index);

    ccV3F_C4B_T2F_Quad quad = sprite->getQuad();
    m_pobTextureAtlas->insertQuad(&quad, index);

    // add children recursively
    
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(sprite->getChildren(), pObj)
    {
        CCSprite* child = (CCSprite*)pObj;
        appendChild(child);
    }
}

//移除一个CCSprite
void CCSpriteBatchNode::removeSpriteFromAtlas(CCSprite *pobSprite)
{
    // remove from TextureAtlas // 从图块管理器中移除目标精灵pobSprite所用的图块信息
    m_pobTextureAtlas->removeQuadAtIndex(pobSprite->getAtlasIndex());

    // Cleanup sprite. It might be reused (issue #569)
    pobSprite->setBatchNode(NULL);

	//取得目标精灵pobSprite在CCSpriteBatchNode的子结点中数组中的位置
    unsigned int uIndex = m_pobDescendants->indexOfObject(pobSprite);
	//如果找到后，从子结点数组中移除它
    if (uIndex != UINT_MAX)
    {
        m_pobDescendants->removeObjectAtIndex(uIndex);
		
        // update all sprites beyond this one
        unsigned int count = m_pobDescendants->count();
        // 取得子结点的数组元素数量，遍历大于当前索引的子结点，向前移一个位置。注意：这个for循环从uIndex开始计数。
        for(; uIndex < count; ++uIndex)
        {
            CCSprite* s = (CCSprite*)(m_pobDescendants->objectAtIndex(uIndex));
            s->setAtlasIndex( s->getAtlasIndex() - 1 );
        }
    }

	// 取得目标精灵的子结点数组，遍历并从当前CCSpriteBatchNode的子结点数组中移除。  
    // remove children recursively
    CCArray *pChildren = pobSprite->getChildren();
    if (pChildren && pChildren->count() > 0)
    {
        CCObject* pObject = NULL;
        CCARRAY_FOREACH(pChildren, pObject)
        {
            CCSprite* pChild = (CCSprite*) pObject;
            if (pChild)
            {
                removeSpriteFromAtlas(pChild);
            }
        }
    }
}

//更新Alpha混合状态 
void CCSpriteBatchNode::updateBlendFunc(void)
{
	//如果当前图块集对应的纹理有Alpha通道
    if (! m_pobTextureAtlas->getTexture()->hasPremultipliedAlpha())
    {
		//设置GL_SRC_ALPHA表示使用源颜色的alpha值来作源混合因子。
        m_blendFunc.src = GL_SRC_ALPHA;
		// GL_ONE_MINUS_SRC_ALPHA表示用1.0减去源颜色的alpha值来作为目标混合因子。
        m_blendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
    }
}

// CocosNodeTexture protocol
void CCSpriteBatchNode::setBlendFunc(ccBlendFunc blendFunc)
{
    m_blendFunc = blendFunc;
}

ccBlendFunc CCSpriteBatchNode::getBlendFunc(void)
{
    return m_blendFunc;
}

CCTexture2D* CCSpriteBatchNode::getTexture(void)
{
    return m_pobTextureAtlas->getTexture();
}

void CCSpriteBatchNode::setTexture(CCTexture2D *texture)
{
    m_pobTextureAtlas->setTexture(texture);
    updateBlendFunc();
}


// CCSpriteSheet Extension
//implementation CCSpriteSheet (TMXTiledMapExtension)

//从精灵数据中在图块管理器中插入一个新的图块，注意：并不将精灵放入子结点。
void CCSpriteBatchNode::insertQuadFromSprite(CCSprite *sprite, unsigned int index)
{
    CCAssert( sprite != NULL, "Argument must be non-NULL");
    CCAssert( dynamic_cast<CCSprite*>(sprite), "CCSpriteBatchNode only supports CCSprites as children");

	//如果指定的索引大于图块集的数量或者图块集的数量已经达到最大值，则需要对图块集进行扩容。
    // make needed room
    while(index >= m_pobTextureAtlas->getCapacity() || m_pobTextureAtlas->getCapacity() == m_pobTextureAtlas->getTotalQuads())
    {
        this->increaseAtlasCapacity();
    }
    //
    // update the quad directly. Don't add the sprite to the scene graph
    //
    sprite->setBatchNode(this);
    sprite->setAtlasIndex(index);
	//新建一个图块顶点信息放入图块信息管理器中。
    ccV3F_C4B_T2F_Quad quad = sprite->getQuad();
    m_pobTextureAtlas->insertQuad(&quad, index);

    // XXX: updateTransform will update the textureAtlas too, using updateQuad.
    // XXX: so, it should be AFTER the insertQuad
	//设置在缓制sprite时更新一下图块信息 
    sprite->setDirty(true);
	//将sprite的顶点位置信息填充入图块顶点缓冲管理器中的顶点数据中。
    sprite->updateTransform();
}


void CCSpriteBatchNode::updateQuadFromSprite(CCSprite *sprite, unsigned int index)
{
    CCAssert(sprite != NULL, "Argument must be non-nil");
    CCAssert(dynamic_cast<CCSprite*>(sprite) != NULL, "CCSpriteBatchNode only supports CCSprites as children");
    
	// make needed room
	while (index >= m_pobTextureAtlas->getCapacity() || m_pobTextureAtlas->getCapacity() == m_pobTextureAtlas->getTotalQuads())
    {
		this->increaseAtlasCapacity();
    }
    
	//
	// update the quad directly. Don't add the sprite to the scene graph
	//
	sprite->setBatchNode(this);
    sprite->setAtlasIndex(index);
    
	sprite->setDirty(true);
	
	// UpdateTransform updates the textureAtlas quad
	sprite->updateTransform();
}

//在子结点数组指定位置插入一个新的精灵。注意：并不新建图块。
CCSpriteBatchNode * CCSpriteBatchNode::addSpriteWithoutQuad(CCSprite*child, unsigned int z, int aTag)
{
    CCAssert( child != NULL, "Argument must be non-NULL");
    CCAssert( dynamic_cast<CCSprite*>(child), "CCSpriteBatchNode only supports CCSprites as children");

    // quad index is Z
    child->setAtlasIndex(z);

    // XXX: optimize with a binary search
	//遍历子结点数组查看当前Z值应该排在哪个位置。
    int i=0;
 
    CCObject* pObject = NULL;
    CCARRAY_FOREACH(m_pobDescendants, pObject)
    {
        CCSprite* pChild = (CCSprite*) pObject;
        if (pChild && (pChild->getAtlasIndex() >= z))
        {
            ++i;
        }
    }
    //取得位置i，插入child到数组中。 
    m_pobDescendants->insertObject(child, i);

    // IMPORTANT: Call super, and not self. Avoid adding it to the texture atlas array
    CCNode::addChild(child, z, aTag);
    //#issue 1262 don't use lazy sorting, tiles are added as quads not as sprites, so sprites need to be added in order
    reorderBatch(false);

    return this;
}

NS_CC_END
