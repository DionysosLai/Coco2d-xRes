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

#include "CCDictionary.h"
#include "CCString.h"
#include "CCInteger.h"
#include "platform/CCFileUtils.h"

using namespace std;

NS_CC_BEGIN

// -----------------------------------------------------------------------
// CCDictElement

CCDictElement::CCDictElement(const char* pszKey, CCObject* pObject)
{
    CCAssert(pszKey && strlen(pszKey) > 0, "Invalid key value.");
    m_iKey = 0;
    const char* pStart = pszKey;
     //字符串的字节长度 
    int len = strlen(pszKey);
    if (len > MAX_KEY_LEN )
    {
		//如果长度大于MAX_KEY_LEN，截取后面MAX_KEY_LEN长度字符串。
        char* pEnd = (char*)&pszKey[len-1];
        pStart = pEnd - (MAX_KEY_LEN-1);
    }
    //字符串COPY
    strcpy(m_szKey, pStart);
    
    m_pObject = pObject;
    memset(&hh, 0, sizeof(hh));
}

CCDictElement::CCDictElement(intptr_t iKey, CCObject* pObject)
{
    m_szKey[0] = '\0';
    m_iKey = iKey;
    m_pObject = pObject;
    memset(&hh, 0, sizeof(hh));
}

CCDictElement::~CCDictElement()
{

}

// -----------------------------------------------------------------------
// CCDictionary

CCDictionary::CCDictionary()
: m_pElements(NULL)
, m_eDictType(kCCDictUnknown)
{

}

CCDictionary::~CCDictionary()
{
	//请空词汇，释放所有词汇占用的内存。
    removeAllObjects();
}
//取得词典中的所有词汇数量。 
unsigned int CCDictionary::count()
{
	//通过HASH_CONT宏来取得哈希表的元素数量。  
    return HASH_COUNT(m_pElements);
}

//返回所有的查询关键字。  
CCArray* CCDictionary::allKeys()
{
	//取得词汇的数量
    int iKeyCount = this->count();
    if (iKeyCount <= 0) return NULL;
	//创建一个iKeyCount大小的CCArray
    CCArray* pArray = CCArray::createWithCapacity(iKeyCount);
	//定义临时词汇指针变量。
    CCDictElement *pElement, *tmp;
    if (m_eDictType == kCCDictStr)
    {
		//如果当前词典查询类型是通过名称字符串。  
        //遍历所有词汇。
        HASH_ITER(hh, m_pElements, pElement, tmp) 
        {
			//取得每一个词汇的名称字符串放入CCArray中。
            CCString* pOneKey = new CCString(pElement->m_szKey);
            pArray->addObject(pOneKey);
            CC_SAFE_RELEASE(pOneKey);
        }
    }
    else if (m_eDictType == kCCDictInt)
    {
		//如果当前词典查询类型是通过索引。  
        //遍历所有词汇。
        HASH_ITER(hh, m_pElements, pElement, tmp) 
        {
			 //取得每一个词汇的名称字符串放入CCArray中。
            CCInteger* pOneKey = new CCInteger(pElement->m_iKey);
            pArray->addObject(pOneKey);
            CC_SAFE_RELEASE(pOneKey);
        }
    }
    
    return pArray;
}
//取得对应CCObject指针的所有关键字或索引值。
CCArray* CCDictionary::allKeysForObject(CCObject* object)
{
	//取得词汇的数量
    int iKeyCount = this->count();
    if (iKeyCount <= 0) return NULL;
	//创建一个CCArray。
	CCArray* pArray = CCArray::create();
	//定义临时词汇指针变量。
    CCDictElement *pElement, *tmp;

    if (m_eDictType == kCCDictStr)
    {
		//如果当前词典查询类型是通过名称字符串。  
        //遍历所有词汇。
        HASH_ITER(hh, m_pElements, pElement, tmp) 
        {
            if (object == pElement->m_pObject)
            {
				//如果与指定的词汇相同，将其名称字符串放入CCArray中。
                CCString* pOneKey = new CCString(pElement->m_szKey);
                pArray->addObject(pOneKey);
                CC_SAFE_RELEASE(pOneKey);
            }
        }
    }
    else if (m_eDictType == kCCDictInt)
    {
		//如果当前词典查询类型是通过索引。  
        //遍历所有词汇。
        HASH_ITER(hh, m_pElements, pElement, tmp) 
        {
			//如果与指定的词汇相同，将其名称字符串放入CCArray中。
            if (object == pElement->m_pObject)
            {
                CCInteger* pOneKey = new CCInteger(pElement->m_iKey);
                pArray->addObject(pOneKey);
                CC_SAFE_RELEASE(pOneKey);
            }
        }
    }
    return pArray;
}
//通过查询关键字取得对应CCObject指针
CCObject* CCDictionary::objectForKey(const std::string& key)
{
    // if dictionary wasn't initialized, return NULL directly.
	//当前词典查询类型值有效性判断。此处有错，应该改为:
	//if (m_eDictType == kCCDictUnknown || m_eDictType == kCCDictInt) return NULL; 
    if (m_eDictType == kCCDictUnknown) return NULL;
    // CCDictionary only supports one kind of key, string or integer.
    // This method uses string as key, therefore we should make sure that the key type of this CCDictionary is string.
   //要求当前词典查询类型为按字符串查询。
	CCAssert(m_eDictType == kCCDictStr, "this dictionary does not use string as key.");
	//定义临时词汇指针变量。
    CCObject* pRetObject = NULL;
    CCDictElement *pElement = NULL;
	//通过名称字符串查询哈希表中的相应词汇
    HASH_FIND_STR(m_pElements, key.c_str(), pElement);
    if (pElement != NULL)
    {
		//如果查询到词汇，返回其对应的CCObject指针 
        pRetObject = pElement->m_pObject;
    }
    return pRetObject;
}
//通过查询索引取得对应CCObject指针 
CCObject* CCDictionary::objectForKey(intptr_t key)
{
    // if dictionary wasn't initialized, return NULL directly.
	//当前词典查询类型值有效性判。此处有错，应该改为:
	//if (m_eDictType == kCCDictUnknown || m_eDictType == kCCDictStr) return NULL;  
    if (m_eDictType == kCCDictUnknown) return NULL;
    // CCDictionary only supports one kind of key, string or integer.
    // This method uses integer as key, therefore we should make sure that the key type of this CCDictionary is integer.
    //要求当前词典查询类型为按字符串查询。
	CCAssert(m_eDictType == kCCDictInt, "this dictionary does not use integer as key.");
	//定义临时词汇指针变量。
    CCObject* pRetObject = NULL;
    CCDictElement *pElement = NULL;
	//通过索引查询哈希表中的相应词汇
    HASH_FIND_PTR(m_pElements, &key, pElement);
    if (pElement != NULL)
    {
		//如果查询到词汇，返回其对应的CCObject指针
        pRetObject = pElement->m_pObject;
    }
    return pRetObject;
}

//通过查询关键字取得对应CCString指针,其实即要求存入词汇的CCObject指针是CCString实例对象指针。
const CCString* CCDictionary::valueForKey(const std::string& key)
{
	//将通过查询关键字取得对应CCString指针强转为CCString指针。
    CCString* pStr = dynamic_cast<CCString*>(objectForKey(key));
    if (pStr == NULL)
    {
		//如果没找到，返回空字符串
        pStr = CCString::create("");
    }
    return pStr;
}
//通过查询索引取得对应CCString指针,即要求存入词汇的CCObject指针是CCString实例对象指针。
const CCString* CCDictionary::valueForKey(intptr_t key)
{
	//将通过查询索引取得对应CCString指针强转为CCString指针。
    CCString* pStr = dynamic_cast<CCString*>(objectForKey(key));
    if (pStr == NULL)
    {
		//如果没找到，返回空字符串
        pStr = CCString::create("");
    }
    return pStr;
}

//设置一个CCObject和对应的名称存入词典。
void CCDictionary::setObject(CCObject* pObject, const std::string& key)
{
	//参数有效性判断 
    CCAssert(key.length() > 0 && pObject != NULL, "Invalid Argument!");
	//如果是第一次存入，记录查询类型为字符串类型。
    if (m_eDictType == kCCDictUnknown)
    {
        m_eDictType = kCCDictStr;
    }
	//将当前词典查询类型设为字符串查询类型。这个变量是可以省略的，因为要求词典查询类型为固定。只用m_eOldDictType就可以了。
    CCAssert(m_eDictType == kCCDictStr, "this dictionary doesn't use string as key.");
	//定义临时指针变量从词典中取得对应名称的词汇。 
    CCDictElement *pElement = NULL;
    HASH_FIND_STR(m_pElements, key.c_str(), pElement);
    if (pElement == NULL)
    {
		//如果词典中没有此词汇，将此新词汇放入词典。
        setObjectUnSafe(pObject, key);
    }
    else if (pElement->m_pObject != pObject)
    {
		//如果词典中已有此词汇，则删除老词汇放入新词汇。
        CCObject* pTmpObj = pElement->m_pObject;
		//此处调用retain对引用计数器加1可以避免在后面的删除函数中释放pTmpObj指向的CCObject。
        pTmpObj->retain();
		//删除此词汇 
        removeObjectForElememt(pElement);
		//放入新词汇。
        setObjectUnSafe(pObject, key);
		//因为之前retain对引用计数器加1一次，所以必须release对引用计数器减1一次才能保证由
		//内存管理器来进行内存释放时，pTempObj指向的CCObject可以正确的被释放掉。
        pTmpObj->release();
    }
}
//设置一个CCObject和对应的哈希索引存入词典。
void CCDictionary::setObject(CCObject* pObject, intptr_t key)
{
    CCAssert(pObject != NULL, "Invalid Argument!");
	//如果是第一次存入，记录查询类型为索引类型。
    if (m_eDictType == kCCDictUnknown)
    {	
		//将当前词典查询类型设为索引查询类型。这个变量是可以省略的，因为要求词典查询类型为固定。只用m_eOldDictType就可以了。
		m_eDictType = kCCDictInt;
    }
	//一致性判断  
    CCAssert(m_eDictType == kCCDictInt, "this dictionary doesn't use integer as key.");
	//定义临时指针变量从词典中取得对应名称的词汇。
    CCDictElement *pElement = NULL;
    HASH_FIND_PTR(m_pElements, &key, pElement);
    if (pElement == NULL)
    {
		//如果词典中没有此词汇，将此新词汇放入词典。
        setObjectUnSafe(pObject, key);
    }
    else if (pElement->m_pObject != pObject)
    {
		//如果词典中已有此词汇，则删除老词汇放入新词汇。 
        CCObject* pTmpObj = pElement->m_pObject;
		//此处调用retain对引用计数器加1可以避免在后面的删除函数中释放pTmpObj指向的CCObject。
        pTmpObj->retain();
		//删除此词汇
        removeObjectForElememt(pElement);
		//放入新词汇。
        setObjectUnSafe(pObject, key);
		//因为之前retain对引用计数器加1一次，所以必须release对引用计数器减1一次才能保证
		//由内存管理器来进行内存释放时，pTempObj指向的CCObject可以正确的被释放掉。  
        pTmpObj->release();
    }

}

//按照查询关键字找到对应CCObject并删除。
void CCDictionary::removeObjectForKey(const std::string& key)
{
	//当前词典是否有效
    if (m_eDictType == kCCDictUnknown)
    {
        return;
    }
    //当前词典的查询类型是否为字符串名称查询方式 
    CCAssert(m_eDictType == kCCDictStr, "this dictionary doesn't use string as its key");
	//参数有效性判断 
    CCAssert(key.length() > 0, "Invalid Argument!");
	//定义临时指针变量从词典中取得对应名称的词汇。
    CCDictElement *pElement = NULL;
	//从词典中删除相应的词汇。 
    HASH_FIND_STR(m_pElements, key.c_str(), pElement);
    removeObjectForElememt(pElement);
}

void CCDictionary::removeObjectForKey(intptr_t key)
{
	//当前词典是否有效 
    if (m_eDictType == kCCDictUnknown)
    {
        return;
    }
    //当前词典的查询类型是否为索引查询方式  
    CCAssert(m_eDictType == kCCDictInt, "this dictionary doesn't use integer as its key");
	//定义临时指针变量从词典中取得对应索引的词汇。
    CCDictElement *pElement = NULL;
    HASH_FIND_PTR(m_pElements, &key, pElement);
	//从词典中删除相应的词汇。
    removeObjectForElememt(pElement);
}
//将CCObject实例指针与对应的字符串名称存入哈希表。 
void CCDictionary::setObjectUnSafe(CCObject* pObject, const std::string& key)
{
	//对pObject指向的实例对像引用计数器加1,即告诉其被词典使用.避免万一其的其它使用者都不再使用时被内存管理器释放. 
    pObject->retain();
	//由pObject和名称字符串产生一个新的词汇。
    CCDictElement* pElement = new CCDictElement(key.c_str(), pObject);
	//将新的词汇放入哈希表中。
    HASH_ADD_STR(m_pElements, m_szKey, pElement);
}
//将CCObject实例指针与对应的索引存入哈希表。
void CCDictionary::setObjectUnSafe(CCObject* pObject, const intptr_t key)
{
	//对pObject指向的实例对像引用计数器加1,即告诉其被词典使用.避免万一其的其它使用者都不再使用时被内存管理器释放.
    pObject->retain();
	//由pObject和名称字符串产生一个新的词汇。
    CCDictElement* pElement = new CCDictElement(key, pObject);
	//将新的词汇放入哈希表中。
    HASH_ADD_PTR(m_pElements, m_iKey, pElement);
}

//按照容器中的查询关键字找到对应CCObject并删除。
void CCDictionary::removeObjectsForKeys(CCArray* pKeyArray)
{
	//遍历CCArray实例对像的所有名称字符串，查询与之对应的词汇。并删除。 
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(pKeyArray, pObj)
    {
        CCString* pStr = (CCString*)pObj;
        removeObjectForKey(pStr->getCString());
    }
}
//从词典中删除相应的词汇。
void CCDictionary::removeObjectForElememt(CCDictElement* pElement)
{
	//参数有效性判断 
    if (pElement != NULL)
    {
		//从哈希表中删除pElement指向的词汇
        HASH_DEL(m_pElements, pElement);
		//前面在将词汇加入词典时对引用计数器加1，这里删除词汇是就应该对引用计数器减1。 
        pElement->m_pObject->release();
		//释放词汇
        CC_SAFE_DELETE(pElement);
    }
}

//从词典中清空所有的词汇。
void CCDictionary::removeAllObjects()
{
	//定义遍历哈希表所用的指针变量
    CCDictElement *pElement, *tmp;
	//遍历哈希表
    HASH_ITER(hh, m_pElements, pElement, tmp) 
    {
		//删除词汇并释放 
        HASH_DEL(m_pElements, pElement);
        pElement->m_pObject->release();
        CC_SAFE_DELETE(pElement);

    }
}

//重载CCObject的拷贝函数。产生一个一模一样的词典。
CCObject* CCDictionary::copyWithZone(CCZone* pZone)
{
	//参数有效性判断
    CCAssert(pZone == NULL, "CCDictionary should not be inherited.");
	//创建一个新的词典 
    CCDictionary* pNewDict = new CCDictionary();
	//定义用来遍历的临时变量
    CCDictElement* pElement = NULL;
    CCObject* pTmpObj = NULL;
	//如果是索引查询方式 
    if (m_eDictType == kCCDictInt)
    {
		//遍历所有词汇
        CCDICT_FOREACH(this, pElement)
        {
			//产生遍历词汇对应的CCObject的COPY,生成新的词汇放入新的词典中. 
            pTmpObj = pElement->getObject()->copy();
            pNewDict->setObject(pTmpObj, pElement->getIntKey());
            pTmpObj->release();
        }
    }
    else if (m_eDictType == kCCDictStr)
    {
		//如果是名称字符串查询方式.
		//遍历所有词汇 
        CCDICT_FOREACH(this, pElement)
        {
			//产生遍历词汇对应的CCObject的COPY,生成新的词汇放入新的词典中.
            pTmpObj = pElement->getObject()->copy();
            pNewDict->setObject(pTmpObj, pElement->getStrKey());
            pTmpObj->release();
        }
    }

    return pNewDict;
}

CCObject* CCDictionary::randomObject()
{
    if (m_eDictType == kCCDictUnknown)
    {
        return NULL;
    }
    
    CCObject* key = allKeys()->randomObject();
    
    if (m_eDictType == kCCDictInt)
    {
        return objectForKey(((CCInteger*)key)->getValue());
    }
    else if (m_eDictType == kCCDictStr)
    {
        return objectForKey(((CCString*)key)->getCString());
    }
    else
    {
        return NULL;
    }
}
//静态函数，取得单例的词典
CCDictionary* CCDictionary::create()
{
	//创建一个新的词典
    CCDictionary* pRet = new CCDictionary();
    if (pRet != NULL)
    {
		//将其设为由引用计数器来判断释放时机.交由内存管理器进行管理.
        pRet->autorelease();
    }
	//返回新创建的词典指针
    return pRet;
}
//静态函数，取得一个指定词典的COPY. 
CCDictionary* CCDictionary::createWithDictionary(CCDictionary* srcDict)
{
	//查生一个指定词典的COPY.
    CCDictionary* pNewDict = (CCDictionary*)srcDict->copy();
    pNewDict->autorelease();
    return pNewDict;
}
//声明静态函数：从一个plist文件中加载词典内容，此函数是多线程安全的，其内部调用 createWithContentsOfFileThreadSafe函数。
CCDictionary* CCDictionary::createWithContentsOfFileThreadSafe(const char *pFileName)
{
    return CCFileUtils::sharedFileUtils()->createCCDictionaryWithContentsOfFile(pFileName);
}

void CCDictionary::acceptVisitor(CCDataVisitor &visitor)
{
    return visitor.visit(this);
}

CCDictionary* CCDictionary::createWithContentsOfFile(const char *pFileName)
{
    CCDictionary* pRet = createWithContentsOfFileThreadSafe(pFileName);
    pRet->autorelease();
    return pRet;
}

bool CCDictionary::writeToFile(const char *fullPath)
{
    return CCFileUtils::sharedFileUtils()->writeToFile(this, fullPath);
}


NS_CC_END
