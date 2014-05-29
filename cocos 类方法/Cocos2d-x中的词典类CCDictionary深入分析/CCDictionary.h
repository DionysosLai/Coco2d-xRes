/****************************************************************************
Copyright (c) 2012 - 2013 cocos2d-x.org

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

#ifndef __CCDICTIONARY_H__
#define __CCDICTIONARY_H__
//需要哈希表的支持
#include "support/data_support/uthash.h"
#include "CCObject.h"
#include "CCArray.h"
#include "CCString.h"

NS_CC_BEGIN
//声明一下CCDictionary类，因为CCDictElement要用到CCDictionary指针。  
class CCDictionary;
//词典元素，或者简单理解就是词典中的一个词汇。我们从小查词典都知道，
//通过词汇名称或索引查找到对应的解释。解释与词汇名称或索引之间是一一对
//	应的关系。与这种关系相同，在这个词汇类中存储一个字符串名称或一个
//	索引以及与其相应的CCObject指针，这个CCObject指针就相当于是我们查
//	出来的解释一样与字符串名称或索引构成了对应关系。 
/**
 * @addtogroup data_structures
 * @{
 */


/**
 *  CCDictElement is used for traversing CCDictionary.
 *
 *  A CCDictElement is one element of CCDictionary, it contains two properties, key and object.
 *  Its key has two different type (integer and string).
 *
 *  @note The key type is unique, all the elements in CCDictionary has the same key type(integer or string).
 *  @code
 *  CCDictElement* pElement;
 *  CCDICT_FOREACH(dict, pElement)
 *  {
 *      const char*key = pElement->getStrKey();
 *      // You certainly know the type of value, so we assume that it's a CCSprite.
 *      CCSprite* pSprite = (CCSprite*)pElement->getObject();
 *      // ......
 *  }
 *  @endcode
 *  @js NA
 *  @lua NA
 */
class CC_DLL CCDictElement
{
private:
    /**
     *  Constructor of CCDictElement. It's only for internal usage. CCDictionary is its friend class.
     *  
     *  @param  pszKey    The string key of this element.
     *  @param  pObject   The object of this element.
     */
	//构造函数。  
//参1:字符串名称。  
//参2:对应的CCObject指针。
    CCDictElement(const char* pszKey, CCObject* pObject);

    /**
     *  Constructor of CCDictElement. It's only for internal usage. CCDictionary is its friend class.
     *
     *  @param  iKey    The integer key of this element.
     *  @param  pObject   The object of this element.
     */
	//构造函数  
    //参1:所在哈希表中的索引  
    //参2:对应的CCObject指针。
    CCDictElement(intptr_t iKey, CCObject* pObject);
    
public:
    /**
     *  The destructor of CCDictElement.
     */
    ~CCDictElement();

    // Inline functions need to be implemented in header file on Android.
    
    /**
     * Get the string key of this element.
     * @note    This method assumes you know the key type in the element. 
     *          If the element's key type is integer, invoking this method will cause an assert.
     *
     * @return  The string key of this element.
     */
	//取得名称字符串。
    inline const char* getStrKey() const
    {
        CCAssert(m_szKey[0] != '\0', "Should not call this function for integer dictionary");
        return m_szKey;
    }

    /**
     * Get the integer key of this element.
     * @note    This method assumes you know the key type in the element.
     *          If the element's key type is string, invoking this method will cause an assert.
     *
     * @return  The integer key of this element.
     */
	//取得哈希索引。
    inline intptr_t getIntKey() const
    {
        CCAssert(m_szKey[0] == '\0', "Should not call this function for string dictionary");
        return m_iKey;
    }
    
    /**
     * Get the object of this element.
     *
     * @return  The object of this element.
     */
	//取得CCObject指针。S
    inline CCObject* getObject() const { return m_pObject; }

private:
    // The max length of string key. 定义字符串名称的长度.
    #define   MAX_KEY_LEN   256
    // char array is needed for HASH_ADD_STR in UT_HASH.
    // So it's a pain that all elements will allocate 256 bytes for this array.
	//存储名称的字符数组。
    char      m_szKey[MAX_KEY_LEN];     // hash key of string type
	//哈希表索引 
    intptr_t  m_iKey;       // hash key of integer type
	//哈希值（CCObject指针）
    CCObject* m_pObject;    // hash value
public:
	//哈希表结构指针
    UT_hash_handle hh;      // makes this class hashable
	//词典为友元类 
    friend class CCDictionary; // declare CCDictionary as friend class
};

/** The macro for traversing dictionary
 *  
 *  @note It's faster than getting all keys and traversing keys to get objects by objectForKey.
 *        It's also safe to remove elements while traversing.
 */
//遍历词典中的所有词汇的一个宏，它内部调用HASH_ITER来进行for循环遍历链表。S
#define CCDICT_FOREACH(__dict__, __el__) \
    CCDictElement* pTmp##__dict__##__el__ = NULL; \
    if (__dict__) \
    HASH_ITER(hh, (__dict__)->m_pElements, __el__, pTmp##__dict__##__el__)



/**
 *  CCDictionary is a class like NSDictionary in Obj-C .
 *
 *  @note Only the pointer of CCObject or its subclass can be inserted to CCDictionary.
 *  @code
 *  // Create a dictionary, return an autorelease object.
 *  CCDictionary* pDict = CCDictionary::create();
 *
 *  // Insert objects to dictionary
 *  CCString* pValue1 = CCString::create("100");
 *  CCString* pValue2 = CCString::create("120");
 *  CCInteger* pValue3 = CCInteger::create(200);
 *  pDict->setObject(pValue1, "key1");
 *  pDict->setObject(pValue2, "key2");
 *  pDict->setObject(pValue3, "key3");
 *
 *  // Get the object for key
 *  CCString* pStr1 = (CCString*)pDict->objectForKey("key1");
 *  CCLog("{ key1: %s }", pStr1->getCString());
 *  CCInteger* pInteger = (CCInteger*)pDict->objectForKey("key3");
 *  CCLog("{ key3: %d }", pInteger->getValue());
 *  @endcode
 *  @js NA
 *
 */
 //词典类，由CCObject派生
class CC_DLL CCDictionary : public CCObject
{
public:
    /** 
     * The constructor of CCDictionary.
     * @lua NA
     */
    CCDictionary();

    /**
     * The destructor of CCDictionary
     * @lua NA
     */
    ~CCDictionary();

    /**
     *  Get the count of elements in CCDictionary.
     *
     *  @return  The count of elements.
     */
	//取得所有词汇的数量。
    unsigned int count();

    /**
     *  Return all keys of elements.
     *
     *  @return  The array contains all keys of elements. It's an autorelease object yet.
     */
	//返回所有的查询关键字。
    CCArray* allKeys();

    /** 
     *  Get all keys according to the specified object.
     *  @warning  We use '==' to compare two objects
     *  @return   The array contains all keys for the specified object. It's an autorelease object yet.
     */
	//取得对应CCObject指针的所有关键字或索引值。
    CCArray* allKeysForObject(CCObject* object);

    /**
     *  Get the object according to the specified string key.
     *
     *  @note The dictionary needs to use string as key. If integer is passed, an assert will appear.
     *  @param key  The string key for searching.
     *  @return The object matches the key. You need to force convert it to the type you know.
     *  @code
     *     // Assume that the elements are CCString* pointers. Convert it by following code.
     *     CCString* pStr = (CCString*)pDict->objectForKey("key1");
     *     // Do something about pStr.
     *     // If you don't know the object type, properly you need to use dynamic_cast<SomeType*> to check it.
     *     CCString* pStr2 = dynamic_cast<CCString*>(pDict->objectForKey("key1"));
     *     if (pStr2 != NULL) {
     *          // Do something about pStr2
     *     }
     *  @endcode
     *  @see objectForKey(intptr_t)
     */
	//通过查询关键字取得对应CCObject指针 
    CCObject* objectForKey(const std::string& key);
    
    /**
     *  Get the object according to the specified integer key.
     *
     *  @note The dictionary needs to use integer as key. If string is passed, an assert will appear.
     *  @param key  The integer key for searching.
     *  @return The object matches the key.
     *  @see objectForKey(const std::string&)
     */
	//通过哈希索引值取得对应CCObject指针
    CCObject* objectForKey(intptr_t key);
    
    /** Get the value according to the specified string key.
     *
     *  @note Be careful to use this function since it assumes the objects in the dictionary are CCString pointer.
     *  @param key  The string key for searching
     *  @return An instance of CCString.
     *          It will return an empty string if the objects aren't CCString pointer or the key wasn't found.
     *  @see valueForKey(intptr_t)
     */
	//通过查询关键字取得对应CCString指针
    const CCString* valueForKey(const std::string& key);
    
    /** Get the value according to the specified integer key.
     *
     *  @note Be careful to use this function since it assumes the objects in the dictionary are CCString pointer.
     *  @param key  The string key for searching.
     *  @return An instance of CCString.
     *          It will return an empty string if the objects aren't CCString pointer or the key wasn't found.
     *  @see valueForKey(intptr_t)
     */
	//通过哈希索引值取得对应CCString指针S
    const CCString* valueForKey(intptr_t key);

    /** Insert an object to dictionary, and match it with the specified string key.
     *
     *  @note Whe the first time this method is invoked, the key type will be set to string.
     *        After that you can't setObject with an integer key.
     *        If the dictionary contains the key you passed, the object matching the key will be released and removed from dictionary.
     *        Then the new object will be inserted after that.
     *
     *  @param pObject  The Object to be inserted.
     *  @param key      The string key for searching.
     *  @see setObject(CCObject*, intptr_t)
     */
	//设置一个CCObject和对应的名称存入词典。
    void setObject(CCObject* pObject, const std::string& key);
    
    /** Insert an object to dictionary, and match it with the specified string key.
     *
     *  @note Then the first time this method is invoked, the key type will be set to string.
     *        After that you can't setObject with an integer key.
     *        If the dictionary contains the key you passed, the object matching the key will be released and removed from dictionary.
     *        Then the new object will be inserted after that.
     *  @param pObject  The Object to be inserted.
     *  @param key      The string key for searching.
     *  @see setObject(CCObject*, const std::string&)
     */
	//设置一个CCObject和对应的哈希索引存入词典。
    void setObject(CCObject* pObject, intptr_t key);

    /** 
     *  Remove an object by the specified string key.
     *
     *  @param key  The string key for searching.
     *  @see removeObjectForKey(intptr_t), removeObjectsForKeys(CCArray*),
     *       removeObjectForElememt(CCDictElement*), removeAllObjects().
     */
	//按照查询关键字找到对应CCObject并删除。
    void removeObjectForKey(const std::string& key);
    
    /**
     *  Remove an object by the specified integer key.
     *
     *  @param key  The integer key for searching.
     *  @see removeObjectForKey(const std::string&), removeObjectsForKeys(CCArray*),
     *       removeObjectForElememt(CCDictElement*), removeAllObjects().
     */
	//按照哈希索引找到对应CCObject并删除。 
    void removeObjectForKey(intptr_t key);
    
    /**
     *  Remove objects by an array of keys.
     *
     *  @param pKeyArray  The array contains keys to be removed.
     *  @see removeObjectForKey(const std::string&), removeObjectForKey(intptr_t),
     *       removeObjectForElememt(CCDictElement*), removeAllObjects().
     */
	//按照容器中的查询关键字找到对应CCObject并删除。
    void removeObjectsForKeys(CCArray* pKeyArray);
    
    /**
     *  Remove an object by an element.
     *
     *  @param pElement  The element need to be removed.
     *  @see removeObjectForKey(const std::string&), removeObjectForKey(intptr_t),
     *       removeObjectsForKeys(CCArray*), removeAllObjects().
     *  @lua NA
     */
	//从词典中删除相应的词汇。
    void removeObjectForElememt(CCDictElement* pElement);
    
    /**
     *  Remove all objects in the dictionary.
     *
     *  @see removeObjectForKey(const std::string&), removeObjectForKey(intptr_t),
     *       removeObjectsForKeys(CCArray*), removeObjectForElememt(CCDictElement*).
     */
	//从词典中清空所有的词汇。
    void removeAllObjects();

    /// @{
    /// @name Function override
    /**
     *  This function is used for deepcopy elements from source dictionary to destination dictionary.
     *  You shouldn't invoke this function manually since it's called by CCObject::copy.
     *  @lua NA
     *  @js  NA
     */
	//重载CCObject的拷贝函数。产生一个一模一样的词典。
    virtual CCObject* copyWithZone(CCZone* pZone);
    /// @}
    
    /**
     *  Return a random object in the dictionary.
     *
     *  @return The random object. 
     *  @see objectForKey(intptr_t), objectForKey(const std::string&)
     *  @lua NA
     */
    CCObject* randomObject();
    
    /**
     *  Create a dictionary.
     *  @return A dictionary which is an autorelease object.
     *  @see createWithDictionary(CCDictionary*), createWithContentsOfFile(const char*), createWithContentsOfFileThreadSafe(const char*).
     */
	//静态函数，创建一个新词典  
    static CCDictionary* create();

    /**
     *  Create a dictionary with an existing dictionary.
     *
     *  @param srcDict The exist dictionary.
     *  @return A dictionary which is an autorelease object.
     *  @see create(), createWithContentsOfFile(const char*), createWithContentsOfFileThreadSafe(const char*).
     */
	//静态函数，取得一个指定词典的COPY。
    static CCDictionary* createWithDictionary(CCDictionary* srcDict);
    
    /**
     *  Create a dictionary with a plist file.
     *  @param  pFileName  The name of the plist file.
     *  @return A dictionary which is an autorelease object.
     *  @see create(), createWithDictionary(CCDictionary*), createWithContentsOfFileThreadSafe(const char*).
     */
	//静态函数:从一个plist文件中加载词典内容。
    static CCDictionary* createWithContentsOfFile(const char *pFileName);
    
    /**
     *  Write a dictionary to a plist file.
     *  @param fullPath The full path of the plist file. You can get writeable path by getWritablePath()
     *  @return true if successed, false if failed
     *  @lua NA
     */
    bool writeToFile(const char *fullPath);
     
    /**
     *  Create a dictionary with a plist file.
     *  
     *  @note the return object isn't an autorelease object.
     *        This can make sure not using autorelease pool in a new thread.
     *        Therefore, you need to manage the lifecycle of the return object.
     *        It means that when you don't need it, CC_SAFE_RELEASE needs to be invoked.
     *
     *  @param  pFileName  The name of the plist file.
     *  @return A dictionary which isn't an autorelease object.
     *  @lua NA
     */
	//静态函数：从一个plist文件中加载词典内容，但此函数是多线程安全的，另外此函数创建的词典需要手动release。
    static CCDictionary* createWithContentsOfFileThreadSafe(const char *pFileName);

    /* override functions 
     * @lua NA
     */
    virtual void acceptVisitor(CCDataVisitor &visitor);

private:
    /** 
     *  For internal usage, invoked by setObject.
     */
    void setObjectUnSafe(CCObject* pObject, const std::string& key);
    void setObjectUnSafe(CCObject* pObject, const intptr_t key);
    
public:
    /**
     *  All the elements in dictionary.
     * 
     *  @note For internal usage, we need to declare this member variable as public since it's used in UT_HASH.
     */
	//词汇的哈希表头部结构指针。
    CCDictElement* m_pElements;
private:
    
    /** The support type of dictionary, it's confirmed when setObject is invoked. */
	//词典查询类型。
    enum CCDictType
    {
        kCCDictUnknown = 0,
        kCCDictStr,//字符串名称 
        kCCDictInt//索引
    };
    
    /** 
     *  The type of dictionary, it's assigned to kCCDictUnknown by default.
     */
    CCDictType m_eDictType;//当前词典查询类型。一个词典实例要求只有一种固定词典查询类型。
};

// end of data_structure group
/// @}

NS_CC_END

#endif /* __CCDICTIONARY_H__ */
