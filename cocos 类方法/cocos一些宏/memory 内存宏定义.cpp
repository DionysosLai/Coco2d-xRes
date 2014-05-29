2014-04-08
1. 使用delete操作符删除一个C++对象p，如果p为NULL，则不进行操作
CC_SAFE_DELETE
#define CC_SAFE_DELETE(p)            do { if(p) { delete (p); (p) = 0; } } while(0)
CC_SAFE_DELETE(pSprite); 

2. 使用delete[]操作符删除一个C++数组p，如果p为NULL，则不进行操作
CC_SAFE_DELETE_ARRAY
#define CC_SAFE_DELETE_ARRAY(p)     do { if(p) { delete[] (p); (p) = 0; } } while(0)

3. 释放节点；使用free()函数删除p，如果p为NULL，则不进行操作
CC_SAFE_FREE
#define CC_SAFE_FREE(p)                do { if(p) { free(p); (p) = 0; } } while(0)

4. 释放节点；使用release()方法释放Cocos2d-x对象p的一次引用，如果p为NULL，则不进行操作
CC_SAFE_RELEASE
#define CC_SAFE_RELEASE(p)            do { if(p) { (p)->release(); } } while(0)

5. 使用release()方法释放Cocos2d-x对象p的一次引用，再把p赋值为NULL。如果p已经为NULL，则不进行操作
CC_SAFE_RELEASE_NULL
#define CC_SAFE_RELEASE_NULL(p)        do { if(p) { (p)->release(); (p) = 0; } } while(0)

6. 使用reatin()方法增加Cocos2d-x对象p的一次引用。如果p为NULL，则不进行操作
CC_SAFE_RETAIN
#define CC_SAFE_RETAIN(p)            do { if(p) { (p)->retain(); } } while(0)