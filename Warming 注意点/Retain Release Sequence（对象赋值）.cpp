2014-03-24
将一个对象赋值给某一指针作为引用的时候，为了遵循内存管理的原则，我们需要获得新对象的引用权，释放旧对象的引
用权。此时，release()和retain()的顺序是尤为重要的。
void SomeClass::setObject(CCObject* other) {  
	this->object->release();  
	other->retain();  
	this->object = other;  
} 
这里存在的隐患是，当other和object实际上指向同一个对象时，第一个release()可能会触发该对象的回收，这显然不
是我们想看到的局面，所以应该先执行retain()来保证other对象有效，然后再释放旧对象：
void SomeClass::setObject(CCObject* other) {  
	other->retain();  
	this->object->release();  
	this->object = other;  
} 
其他可行的解决方案也有很多，例如使用autorelease()方法来代替release()方法，或在赋值前判断两个对象是否相同。
在Google的Objective-C编程规范中，推荐使用autorelease()方法代替release()方法。