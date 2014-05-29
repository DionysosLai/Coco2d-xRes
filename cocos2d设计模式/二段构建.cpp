二段构建
所谓二段构建，就是指创建对象时不是直接通过构建函数来分配内存并完成初始化操作。取而代之的是，构造函数只
负责分配内存，而初始化的工作则由一些名为initXXX的成员方法来完成。然后再定义一些静态类方法把这两个阶段组
合起来，完成最终对象的构建。因为在《Cocoa设计模式》一书中，把此惯用法称之为“Two Stage Creation”，即“二段
构建”。因为此模式在cocos2d里面被广泛使用，所以把该模式也引入过来了。


1.应用场景：
二段构建在cocos2d-x里面随处可见，自从2.0版本以后，所有的二段构建方法的签名都改成create了。这样做的好处是
一方面统一接口，方便记忆，另一方面是以前的类似Cocoa的命名规范不适用c++，容易引起歧义。下面以CCSprite为类，
来具体阐述二段构建的过程，请看下列代码：

//此方法现在已经不推荐使用了，将来可能会删除
CCSprite* CCSprite::spriteWithFile(const char *pszFileName)
{
    return CCSprite::create(pszFileName);
}
 
CCSprite* CCSprite::create(const char *pszFileName)
{
    CCSprite *pobSprite = new CCSprite();  //1.第一阶段，分配内存
    if (pobSprite && pobSprite->initWithFile(pszFileName))  //2.第二阶段，初始化
    {
        pobSprite->autorelease();  //！！！额外做了内存管理的工作。
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}
如上面代码中的注释所示，创建一个sprite明显被分为两个步骤：1.使用new来创建内存；2.使用initXXX方法来完成初始化。
因为CCSprite的构造函数也有初始化的功能，所以，我们再来看看CCSprite的构建函数实现：

CCSprite::CCSprite(void)
: m_pobTexture(NULL)
, m_bShouldBeHidden(false)
{
}
很明显，这个构建函数所做的初始化工作非常有限，仅仅是在初始化列表里面初始化了m_pobTexture和m_bShouldBeHidden两
个变量。实际的初始化工作大部分都放在initXXX系列方法中，大家可以动手去查看源代码。

2.分析为什么要使用此模式？
这种二段构建对于C++程序员来说，其实有点别扭。因为c++的构造函数在设计之初就是用来分配内存+初始化对象的。如果再
搞个二段构建，实则是多此一举。但是，在objective-c里面是没有构造函数这一说的，所以，在Cocoa的编程世界里，二段
构建被广泛采用。而cocos2d-x当初是从cocos2d-iphone移植过来了，为了保持最大限度的代码一致性，所以保留了这种二段
构建方式。这样可以方便移植cocos2d-iphone的游戏，同时也方便cocos2d-iphone的程序员快速上手cocos2d-x。

不过在后来，由于c++天生不具备oc那种可以指定每一个参数的名称的能力，所以，cocos2d-x的设计者决定使用c++的函数重
载来解决这个问题。这也是后来为什么2.0版本以后，都使用create函数的重载版本了。

虽然接口签名改掉了，但是本质并没有变化，还是使用的二段构建。二段构建并没有什么不好，只是更加突出了对象需要初
始化。在某种程度上也可以说是一种设计强化。因为忘记初始化是一切莫名其妙的bug的罪魁祸首。同时，二段构建出来的对
象都是autorelease的对象，而autorelease对象是使用引用计数来管理内存的。客户端程序员在使用此接口创建对象的时候，
无需关心具体实现细节，只要知道使用create方法可以创建并初始化一个自动释放内存的对象即可。

在一点，在《Effective Java》一书中，也有提到。为每一个类提供一个静态工厂方法来代替构造函数，它有以下三个优点：
1.与构造函数不同，静态方法有名字，而构造函数只能通过参数重载。
2.它每次被调用的时候，不一定都创建一个新的对象。比如Boolean.valueOf(boolean)。
3.它还可以返回原类型的子类型对象。

因此，使用二段构建的原因有二：
1.兼容性、历史遗留原因。（这也再次印证了一句话，一切系统都是遗留系统，呵呵）
2.二段构建有其自身独有的优势。

3.使用此模式的优缺点是什么？
优点：
1.显示分开内存分配和初始化阶段，让初始化地位突出。因为程序员一般不会忘记分配内存，但却常常忽略初始化的作用。
2.见上面分析《Effective Java》的第1条：“为每一个类提供一个静态工厂方法来代替构造函数”
3.除了完成对象构建，还可以管理对象内存。
缺点：
1.不如直接使用构造函数来得直白、明了，违反直觉，但这个是相对的。

4.此模式的定义及一般实现
定义：
将一个对象的构建分为两个步骤来进行：1.分配内存 2.初始化
它的一般实现如下：

class Test
{
public:
    //静态工厂方法
    static Test* create()
    {
        Test *pTest = new Test;
        if (pTest && pTest->init()) {
            //这里还可以做其它操作，比如cocos2d-x里面管理内存
            return pTest;
        }
        return NULL;
    }
    //
    Test()
    {
        //分配成员变量的内存，但不初始化
    }
    bool init(){
        //这里初始化对象成员
        return true;
    }
private:
    //这里定义数据成员
     
};
5.在游戏开发中如何运用此模式
这个也非常简单，就是今后在使用cocos2d-x的时候，如果你继承CCSprite实现自定义的精灵，你也需要按照“二段构建”
的方式，为你的类提供一个静态工厂方法，同时编写相应的初始化方法。当然，命名规范最好和cocos2d-x统一，即静态
工厂方法为create，而初始化方法为initXXXX。

6.此模式经常与哪些模式配合使用
由于此模式在GoF的设计模式中并未出现，所以暂时不讨论与其它模式的关系。

最后看看cocos2d-x创始人王哲对于为什么要设计成二段构建的看法：
“其实我们设计二段构造时首先考虑其优势而非兼容cocos2d-iphone. 初始化时会遇到图片资源不存在等异常，而C++
构造函数无返回值，只能用try-catch来处理异常，启用try-catch会使编译后二进制文件大不少，故需要init返回bool
值。Symbian, Bada SDK，objc的alloc + init也都是二阶段构造”。