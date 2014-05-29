	第一个陷阱是接收触摸的对象并不一定正显示在屏幕上。触摸分发器和引擎中的绘图是相互独立的，所以并不关心触摸代
理是否处于屏幕上。因此，在实际使用中，应该在不需要的时候及时从分发器中移除触摸代理，尤其是自定义的触摸对象。
而CCLayer也仅仅会在切换场景时将自己从分发器中移除，所以同场景内手动切换CCLayer的时候，也需要注意禁用触摸
来从分发器移除自己。
例如在lay类中这样写：
bool TouchLayer::init()  
  {  
      bool bRet = false;  
      do {  
          CC_BREAK_IF(!CCLayer::init());  
   
          this->registerWithTouchDispatcher();  
          this->setDelegate(NULL);  
          bRet = true;  
      } while (0);  
   
      return bRet;  
  }  
   
  void TouchLayer::onExit()  
  {  
      CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);  
  }  
	另一个陷阱出自CCTargetedTouchDelegate。尽管每次只传入一个触摸点，也只有在开始阶段被声明过的触摸点后续才会传
入，但是这并不意味着只会接收一个触摸点：只要被声明过的触摸点都会传入，而且可能是乱序的。因此，一个良好的习
惯是，如果使用CCTargeted- TouchDelegate，那么只声明一个触摸，针对一个触摸作处理即可。