2014-02-28
1. 创建世界对象
 1). 首先，定义整个世界的重力系统，包括重力大小和方向，方向符合基本坐标轴方向。
然后以重力为参数定义世界。代码如下： Box2dTest.cpp	--initPhysics
     b2Vec2 gravity;
    gravity.Set(0.0f, -10.0f);
    world = new b2World(gravity);
 2). 设置是否允许世界中的刚体休眠的参数，休眠允许系统模拟时跳过某些不需要处理的刚体。
当施加某一物体上的力小于临界值一段时间后，这个刚体会进入休眠阶段。即某个刚体一段时间
不动后，会自动休眠，不再对它进行计算、这样可提高引擎效率。
     // Do we want to let bodies sleep?
    world->SetAllowSleeping(true);

    world->SetContinuousPhysics(true);

	
3. 创建地面盒
	地面盒是一个物体。物体，即刚体，也就是物理学的质点。
	可以分为几类：
	1：静态刚体, 没有质量，没有速度，只有手动来改变它的位置；
	2：棱柱刚体，没有质量，可以有速度，可以自己更新位置；
	3：动态刚体：有质量，也有速度。
	物理引擎首先需要一个描述类，然后根据描述类通过世界来创建世界。创建刚体时需要两个
步骤：
	1). 生产一个刚体定义。
	2). 根据刚体定义生产刚体。
	在刚体定义定义中的信息会被复制，也就是说创建完成后刚体只要没被释放掉，就可以重复使用。
	在Box2D中新建一个物体需要如下步骤：
	1). 使用位置和阻尼等参数定义物体。
	2). 使用世界对象创建物体。
	3). 使用几何结构、摩擦和密度等定义对象。
	4). 调整物体质量和形状相匹配。
	定义物体，eg：
	// Define the ground body.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0, 0); // bottom-left corner
	定义物体，即定义b2BodyDef结构，它作为创建物体的参数将被传入物体创建中，eg：
	// Call the body factory which allocates memory for the ground body
    // from a pool and creates the ground box shape (also from a pool).
    // The body is also added to the world.
    b2Body* groundBody = world->CreateBody(&groundBodyDef);
	之后定义物体边界，
	    // Define the ground box shape.
    b2EdgeShape groundBox;

    // bottom
    groundBox.Set(b2Vec2(VisibleRect::leftBottom().x/PTM_RATIO,VisibleRect::leftBottom().y/PTM_RATIO), b2Vec2(VisibleRect::rightBottom().x/PTM_RATIO,VisibleRect::rightBottom().y/PTM_RATIO));
    groundBody->CreateFixture(&groundBox,0);

    // top
    groundBox.Set(b2Vec2(VisibleRect::leftTop().x/PTM_RATIO,VisibleRect::leftTop().y/PTM_RATIO), b2Vec2(VisibleRect::rightTop().x/PTM_RATIO,VisibleRect::rightTop().y/PTM_RATIO));
    groundBody->CreateFixture(&groundBox,0);

    // left
    groundBox.Set(b2Vec2(VisibleRect::leftTop().x/PTM_RATIO,VisibleRect::leftTop().y/PTM_RATIO), b2Vec2(VisibleRect::leftBottom().x/PTM_RATIO,VisibleRect::leftBottom().y/PTM_RATIO));
    groundBody->CreateFixture(&groundBox,0);

    // right
    groundBox.Set(b2Vec2(VisibleRect::rightBottom().x/PTM_RATIO,VisibleRect::rightBottom().y/PTM_RATIO), b2Vec2(VisibleRect::rightTop().x/PTM_RATIO,VisibleRect::rightTop().y/PTM_RATIO));
    groundBody->CreateFixture(&groundBox,0);
	先分别定义4条边，最后把它定义到物体中。
	和定义物体一样，只是把数据复制到物体中，这个形状的定义也可以重新被使用。定义物体和形状必须绑在一起。形状才有一样，
	也就是说形状是依附于物体存在的。