2014-02-24
	一般都将触摸点都放在触摸类CCTouch中，公共函数见：
	class CC_DLL CCTouch : public CCObject
{
public:
    /**
     * @js ctor
     */
    CCTouch()
        : m_nId(0),
        m_startPointCaptured(false)
    {}

    /** returns the current touch location in OpenGL coordinates */
    CCPoint getLocation() const;
    /** returns the previous touch location in OpenGL coordinates */
    CCPoint getPreviousLocation() const;
    /** returns the start touch location in OpenGL coordinates */
    CCPoint getStartLocation() const;
    /** returns the delta of 2 current touches locations in screen coordinates */
    CCPoint getDelta() const;
    /** returns the current touch location in screen coordinates 
	* 获得点在画布中的位置
	*/
    CCPoint getLocationInView() const;
    /** returns the previous touch location in screen coordinates 
	* 之前在画布中的位置
	*/
    CCPoint getPreviousLocationInView() const;
    /** returns the start touch location in screen coordinates */
    CCPoint getStartLocationInView() const;
    
    void setTouchInfo(int id, float x, float y)
    {
        m_nId = id;
        m_prevPoint = m_point;
        m_point.x   = x;
        m_point.y   = y;
        if (!m_startPointCaptured)
        {
            m_startPoint = m_point;
            m_startPointCaptured = true;
        }
    }
    /**
     *  @js getId 获得ID号
     */
    int getID() const
    {
        return m_nId;
    }

private:
    int m_nId;
    bool m_startPointCaptured;
    CCPoint m_startPoint;
    CCPoint m_point;
    CCPoint m_prevPoint;
};
	CCTouch触点类封装了触摸点信息，在回调函数中，通过一系列函数操作获得我们需要的坐标值，如下：
	CCPoint point = touch->geiLocationInView();
	point = CCDirector::shareDirector()->converToGL(point);
	
	例子如下：
	void TouchesPerformTest1::onEnter()
{
    TouchesMainScene::onEnter();
    setTouchEnabled(true);
}

std::string TouchesPerformTest1::title()
{
    return "Targeted touches";
}

void TouchesPerformTest1::registerWithTouchDispatcher()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
	/// 获得触摸类并调用触摸代理addTargetedDelegate允许本布景层结束触摸事件
	/// 参数：代理对象 触摸的优先级 是否“吞噬”所有触摸点 true—>吞噬所有相应的触摸点
	/// ，那么比它权限低的以及CCStandardTouchDelegate标准触摸代理无论是多高的权限都收不到触摸分发。
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

bool TouchesPerformTest1::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    numberOfTouchesB++;
    return true;
}

void TouchesPerformTest1::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    numberOfTouchesM++;
}

void TouchesPerformTest1::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    numberOfTouchesE++;
}

void TouchesPerformTest1::ccTouchCancelled(CCTouch* touch, CCEvent* event)
{
    numberOfTouchesC++;
}
	
	
	如果在非布景层类和其子类想要使用触摸，那么继承相应代理即可，例如：
	class Paddle : public CCSprite, public CCTargetedTouchDelegate
{
    PaddleState        m_state;

public:
    Paddle(void);
    virtual ~Paddle(void);

    CCRect rect();
    bool initWithTexture(CCTexture2D* aTexture);
    virtual void onEnter();
    virtual void onExit();
    bool containsTouchLocation(CCTouch* touch);
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    virtual CCObject* copyWithZone(CCZone *pZone);

    virtual void touchDelegateRetain();
    virtual void touchDelegateRelease();

    static Paddle* paddleWithTexture(CCTexture2D* aTexture);
};
	然后在onEnter和onExit中注册和注销触摸的代理，如下：
	void Paddle::onEnter()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    CCSprite::onEnter();
}

void Paddle::onExit()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->removeDelegate(this);
    CCSprite::onExit();
}    
	
	
	