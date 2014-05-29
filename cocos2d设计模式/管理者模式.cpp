管理者模式
1.应用场景：
在cocos2d-x里面有大量的应用，比如之前在介绍单例模式中提到的CCTextureCache, CCSpriteFrameCache, CCAnimationCache和CCShaderCache类。
这些管理者一般被设计成单例类。

为什么管理者类要设计成单例呢？因为管理者一般会采用key-value的形式来管理其它类的实例，每当需要获取一个管理者中的实例时，只需要提供一个惟一的键值字符串就可以得到一个与之对应的惟一实例。如果允许存在多个管理者实例的话，那么每个管理者都会维护各自的key-value pairs。这样显然就不能通过键值字符串来获得惟一对象实例了。

CCSpriteFrameCache类通过定制的plist文件来实例化一系列相关的CCSpriteFrame实例，然后只需要提供精灵帧的名字就可以得到相应的CCSpriteFrame实例了。从这个意义上来说，CCSpriteFrameCache类也可以说是一个工厂类，专门负责生产CCSpriteFrame实例。同时，如果精灵帧名字相同的话，那么获取的精灵帧实例也是相同的。

2.使用管理者模式的优缺点。
优点：为一组相关的对象提供一个统一的全局访问点，同时可以提供一些简洁的接口来获取和操作这些对象。同时，使用此模式来缓存游戏中的常用资源，可以提高游戏运行时性能。

缺点：由于管理者大多采用单例模式，所以，它继承了单例模式所有的缺点，这里就不再赘述了。

3.管理者模式的定义
管理者类（cache类）可以简化一些可以重用的资源（比如字体、纹理、精灵帧等）的创建和管理工作。管理者模式其实是个混合模式，它综合了单例模式、外观模式和工厂模式。该模式在游戏开发中比较常见，很多需要提升游戏运行性能的场合都运用了此模式。
此模式的动机：提供一个统一的接口来管理一组相关对象的实例化和访问。
它的一般实现如下：

class TestManager{
public:
    static TestManager *sharedTestManager(){
        if (NULL == m_psManager) {
            m_psManager = new TestManager;
            instanceTable = CCDictionary::create();
            instanceTable->retain();
        }
        return m_psManager;
    }
    void purge(){
        CC_SAFE_DELETE(m_psManager);
        CC_SAFE_RELEASE_NULL(instanceTable);
    }
    void registeInstance(const string& key,CCObject *obj){
        instanceTable->setObject(ojb,key);
    }
    CCObject* getInstance(const string& key){
        return instanceTable->objectForKey(key);
    }
private:
    static TestManager* m_psManager;
    CCDictionary *instanceTable; //用来管理其它类的一组实例
};
TestManager* TestManager::m_psManager = NULL;
4.游戏开发中如何运用此模式
在cocos2dx游戏开发中，经常需要使用CCAnimate动作来播放动画，这些动作的创建运行时开销是比较大的，一般采用的方式都是在node的init方法中创建好，然后retain。之后需要使用的时候直接引用此动作即可，前提是你得声明许多CCAnimate对象的弱引用。这里，我们可以为之创建一个CCAnimateCache类，专门用来管理这些动画动作实例。这样对于游戏中经常变换状态需要更换不同的动画时，可以从此CCAniamteCache类中获取相应动画引用，非常方便，同时可以提高游戏性能。相应的，也可以为Action创建相应的类。

引申：但凡那些对象，在运行时创建的时间开销特别大时，都可以采取此模式来提高运行时性能。

5.此模式经常与单例模式配合使用，它的一些设计思想也掺合了外观模式和工厂模式。