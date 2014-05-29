1.基本动作
     Cocos2d提供的基本动作:瞬时动作、延时动作、运作速度。
     瞬时动作：就是不需要时间,马上就完成的动作。瞬时动作的共同基类是 InstantAction。
     Cocos2d提供以下瞬时动作：
  放置 – Place
            效果类似于 node.Position = ccp(x, y)。之所以作为一个动作来实现是为了可以与其他动作形成一个连续动作。
  隐藏 – Hide
           效果类似于 [node setVisible:NO]. 之所以作为一个劢作来实现是为了可以与其他动作形成一个连续动作。
 显示 – Show
           效果类似于 [node setVisible:YES]. 之所以作为一个动作来实现是为了可以与其他动作形成一个连续动作。
可见切换 – ToggleVisibility

2.延时动作
      延时动作就是指动作的完成需要一定时间。因此actionWithDuration 是延时动作执行时的第一个参数,延时动作的共同基类是 CCIntervalAction(包含了组合动作类)。
      Cocos2d提供以下瞬时动作（函数命名规则是：XxxxTo: 意味着运动到指定的位置, XxxxBy:意味着运动到按照指定的 x、y 增量的位置。[x、y 可以是负值]）：
 移动到 – CCMoveTo
移动– CCMoveBy
跳跃到 – CCJumpTo   设置终点位置和跳跃的高度和次数。
跳跃 – CCJumpBy   设置终点位置和跳跃的高度和次数。
贝塞尔 – CCBezierBy  支持 3 次贝塞尔曲线:P0-起点,P1-起点切线方向,P2-终点切线方向,P3-终点。
放大到 – CCScaleTo   设置放大倍数,是浮点型。
放大 – CCScaleBy
旋转到 – CCRotateTo
旋转 – CCRotateBy
闪烁 – CCBlink   设定闪烁次数
色调变化到 – CCTintTo
色调变换 – CCTintBy
变暗到 – CCFadeTo
由无变亮 – CCFadeIn
由亮变无 – CCFadeOut

3.组合动作
   按照一定的次序将上述基本动作组合起来,形成连贯的一套组合动作。组合动作包括以下几类:
 序列 – CCSequence
      Sequence 的使用非常简单,该类也从 CCIntervalAction 派生,本身就可以被 CocosNode 对象执行。该类的作用就是线序排列若干个动作,然后按先后次序逐个执行。
同步 – Spawn
      Spawn 的使用非常简单,该类也从 IntervalAction 派生,本身就可以被CocosNode 对象执行。该类的作用就是同时并列执行若干个动作,但要求动作都必须是可以同时执行的。比如:移动式翻转、变色、变大小等。
      需要特别注意的是,同步执行最后的完成时间由基本动作中用时最大者决定。
重复有线次数 – Repeate
       重复有限次数的动作,该类也从 IntervalAction 派生,可以被 CocosNode 对象执行。
反动作 – Reverse
       反动作就是反向(逆向)执行某个动作,支持针对动作序列的反动作序列。反动作不是一个专门的类,而是 CCFiniteAction 引入的一个接口。不是所有的类都支持反动作,XxxxTo 类通常不支持反动作,XxxxBy 类通常支持。
 动画 – Animation
      动画就是让精灵自身连续执行一段影像,形成模拟运动的效果:行走时的精灵状态、打斗时的状态等。
无限重复 – RepeatForever
      RepeatForever 是从 Action 类直接派生的,因此无法参与序列和同步;自身也无法反向执行。该类的作用就是无限期执行某个动作或动作序列,直到被停止。

4.速度变化
      基本动作和组合动作实现了针对精灵的各种运动、动画效果的改变,但这样的改变的速度是不变的,通过 CCEaseAction 为基类的类系和 CCSpped 类我们可以很方便的修改精灵执行劢作的速度:由快至慢还是由慢至快。
EaseIn 由慢至快。
EaseOut 由快至慢
EaseInOut 由慢至快再由快至慢。
EaseSineIn
由慢至快。
EaseSineOut 由快至慢
EaseSineInOut 由慢至快再由快至慢。
EaseExponentialIn 由慢至极快。
EaseExponentialOut 由极快至慢。
EaseExponentialInOut 由慢至极快再由极快至慢。
Speed 人工设定速度,还可通过 SetSpeed 不断调整。
5.扩展动作
   延时动作 – Delay ，比如在动作序列中增加一个时间间歇
函数调用
函数
          在动作序列中间或者结束调用某个函数,执行任何需要执行的任务:动作、状态修改等。id acf = [CCCallFunc actionWithTarget:self selector:@selector(CallBack1)];对应的函数为:- (void) CallBack1 {
[sprite runAction:[CCTintBy actionWithDuration:0.5 red:255 green:0 blue:255]]; }
带对象参数--调用自定义函数时,传递当前对象。id acf = [CallFuncN actionWithTarget:self selector:@selector(CallBack2:)];对应的自定义函数:(这里,我们直接使用了该对象)
- (void) CallBack2:(id)sender {
[sender runAction:[CCTintBy actionWithDuration:1 red:255 green:0 blue:255]]; }
带 对象、数据参数--用自定义函数时,传递当前对象和一个常量(也可以是指针)。id acf = [CCCallFuncND actionWithTarget:self selector:@selector(CallBack3:data:) data:(void*)2];对应的自定义函数,我们使用了传递的对象和数据:
-(void) CallBack3:(id)sender data:(void*)data {
[sender runAction:[CCTintBy actionWithDuration:(NSInteger)data red:255 green:0 blue:255]]; }