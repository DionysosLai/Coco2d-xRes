2014-03-28
1、使用CCSprite创建1000个Icon.png到场景中，这样渲染批次就是1000（暂且不考虑其他的精灵）
for(int i = 0;i < 1000;++i){  
        int x = arc4random()%960;  
        int y = arc4random()%640;  
        CCSprite* testIcon = CCSprite::create("Icon.png");  
        testIcon->setPosition( ccp(x, y) );  
        addChild(testIcon);  
}  
创建了1000个Icon到场景中，这是的FPS是22，渲染批次是1000次。

2、使用CCSpriteBatchNode批量渲染，一次渲染就把所有的CCSprite绘制出来。大大降低渲染批次。
1、使用CCSprite创建1000个Icon.png到场景中，但是这里利用了CCSpriteBatchNode批量渲染。这时的渲染批次、FPS如何呢？
[cpp] view plaincopy
CCSpriteBatchNode* batchNode = CCSpriteBatchNode::create("Icon.png", 1000);  
batchNode->setPosition(CCPointZero);  
this->addChild(batchNode);  
      
for(int i = 0;i < 1000;++i){  
        int x = arc4random()%960;  
        int y = arc4random()%640;  
        CCSprite* testIcon = CCSprite::createWithTexture(batchNode->getTexture());  
        testIcon->setPosition( ccp(x, y) );  
        batchNode->addChild(testIcon);  
}  

创建了1000个Icon到场景中，这是的FPS是39.9，渲染批次是1次。


1、
CCSpriteBatchNode::create(const char *fileImage);//利用贴图创建，默认子节点数量29.（数量不够时，系统会自己增加）
CCSpriteBatchNode* batchNode = CCSpriteBatchNode::create(const char *fileImage, unsigned int capacity);//利用贴图创建,并指定子节点数量

2、使用CCSpriteBatchNode时，所使用的贴图必须是同一张图片，也不能指定精灵的深度。所有的精灵都必须在同一渲染层。

3、但是项目中总不可能局限于一张贴图上，所以你可以把多张贴图合并成一张大贴图（合并的工具很多，我不介绍了）。所以利用合成后的大贴图创建一个CCSpriteBatchNode。
然后创建CCSprite的时候，设置贴图的区域就可以了。