2014-02-27
	Cocos2D-x中支持的游戏存档类CCUserDefault，可以作为一个轻量化的数据库来使用。
支持的数据库包括bool int float double string 
/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org

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
#ifndef __SUPPORT_CCUSERDEFAULT_H__
#define __SUPPORT_CCUSERDEFAULT_H__

#include "platform/CCPlatformMacros.h"
#include <string>

NS_CC_BEGIN

/**
 * @addtogroup data_storage
 * @{
 */

/**
 * CCUserDefault acts as a tiny database. You can save and get base type values by it.
 * For example, setBoolForKey("played", true) will add a bool value true into the database.
 * Its key is "played". You can get the value of the key by getBoolForKey("played").
 * 
 * It supports the following base types:
 * bool, int, float, double, string
 */
class CC_DLL CCUserDefault
{
public:
    ~CCUserDefault();

    // get value methods

    /**
    @brief Get bool value by key, if the key doesn't exist, a default value will return.
     You can set the default value, or it is false.
	 * 根据传入的键值参数返回相应的布尔值，第二个参数作为默认值，一旦该键值的值不存在，就返回默认值
    */
    bool    getBoolForKey(const char* pKey);
    bool    getBoolForKey(const char* pKey, bool defaultValue);
    /**
    @brief Get integer value by key, if the key doesn't exist, a default value will return.
     You can set the default value, or it is 0.
	  * 根据传入的键值参数返回相应的整数值，第二个参数作为默认值，一旦该键值的值不存在，就返回默认值
    */
    int     getIntegerForKey(const char* pKey);
    int     getIntegerForKey(const char* pKey, int defaultValue);
    /**
    @brief Get float value by key, if the key doesn't exist, a default value will return.
     You can set the default value, or it is 0.0f.
	  * 根据传入的键值参数返回相应的浮点值，第二个参数作为默认值，一旦该键值的值不存在，就返回默认值
    */
    float    getFloatForKey(const char* pKey);
    float    getFloatForKey(const char* pKey, float defaultValue);
    /**
    @brief Get double value by key, if the key doesn't exist, a default value will return.
     You can set the default value, or it is 0.0.
	 * 根据传入的键值参数返回相应的双精度浮点值，第二个参数作为默认值，一旦该键值的值不存在，就返回默认值
    */
    double  getDoubleForKey(const char* pKey);
    double  getDoubleForKey(const char* pKey, double defaultValue);
    /**
    @brief Get string value by key, if the key doesn't exist, a default value will return.
    You can set the default value, or it is "".
	 * 根据传入的键值参数返回相应的字符串，第二个参数作为默认值，一旦该键值的值不存在，就返回默认值
    */
    std::string getStringForKey(const char* pKey);
    std::string getStringForKey(const char* pKey, const std::string & defaultValue);

    // set value methods

    /**
    @brief Set bool value by key.第一个参数为键，第二个参数为对应的值，布尔型存档
    */
    void    setBoolForKey(const char* pKey, bool value);
    /**
    @brief Set integer value by key.第一个参数为键，第二个参数为对应的值，整型存档
    */
    void    setIntegerForKey(const char* pKey, int value);
    /**
    @brief Set float value by key.第一个参数为键，第二个参数为对应的值，浮点值存档
    */
    void    setFloatForKey(const char* pKey, float value);
    /**
    @brief Set double value by key.第一个参数为键，第二个参数为对应的值，双精度浮点值存档
    */
    void    setDoubleForKey(const char* pKey, double value);
    /**
    @brief Set string value by key.第一个参数为键，第二个参数为对应的值，字符串型存档
    */
    void    setStringForKey(const char* pKey, const std::string & value);
    /**
     @brief Save content to xml file 存为xml文档
     */
    void    flush();

    static CCUserDefault* sharedUserDefault();
    static void purgeSharedUserDefault();
    const static std::string& getXMLFilePath();
    static bool isXMLFileExist();

private:
    CCUserDefault();
    static bool createXMLFile();
    static void initXMLFilePath();
    
    static CCUserDefault* m_spUserDefault;
    static std::string m_sFilePath;
    static bool m_sbIsFilePathInitialized;
};

// end of data_storage group
/// @}

NS_CC_END

#endif // __SUPPORT_CCUSERDEFAULT_H__
使用例子：
void UserDefaultTest::doTest()
{
    CCLOG("********************** init value ***********************");

    // set default value

    CCUserDefault::sharedUserDefault()->setStringForKey("string", "value1");
    CCUserDefault::sharedUserDefault()->setIntegerForKey("integer", 10);
    CCUserDefault::sharedUserDefault()->setFloatForKey("float", 2.3f);
    CCUserDefault::sharedUserDefault()->setDoubleForKey("double", 2.4);
    CCUserDefault::sharedUserDefault()->setBoolForKey("bool", true);

    // print value

    string ret = CCUserDefault::sharedUserDefault()->getStringForKey("string");
    CCLOG("string is %s", ret.c_str());

    double d = CCUserDefault::sharedUserDefault()->getDoubleForKey("double");
    CCLOG("double is %f", d);

    int i = CCUserDefault::sharedUserDefault()->getIntegerForKey("integer");
    CCLOG("integer is %d", i);

    float f = CCUserDefault::sharedUserDefault()->getFloatForKey("float");
    CCLOG("float is %f", f);

    bool b = CCUserDefault::sharedUserDefault()->getBoolForKey("bool");
    if (b)
    {
        CCLOG("bool is true");
    }
    else
    {
        CCLOG("bool is false");
    }
    
    //CCUserDefault::sharedUserDefault()->flush();

    CCLOG("********************** after change value ***********************");

    // change the value

    CCUserDefault::sharedUserDefault()->setStringForKey("string", "value2");
    CCUserDefault::sharedUserDefault()->setIntegerForKey("integer", 11);
    CCUserDefault::sharedUserDefault()->setFloatForKey("float", 2.5f);
    CCUserDefault::sharedUserDefault()->setDoubleForKey("double", 2.6);
    CCUserDefault::sharedUserDefault()->setBoolForKey("bool", false);

    CCUserDefault::sharedUserDefault()->flush();

    // print value

    ret = CCUserDefault::sharedUserDefault()->getStringForKey("string");
    CCLOG("string is %s", ret.c_str());

    d = CCUserDefault::sharedUserDefault()->getDoubleForKey("double");
    CCLOG("double is %f", d);

    i = CCUserDefault::sharedUserDefault()->getIntegerForKey("integer");
    CCLOG("integer is %d", i);

    f = CCUserDefault::sharedUserDefault()->getFloatForKey("float");
    CCLOG("float is %f", f);

    b = CCUserDefault::sharedUserDefault()->getBoolForKey("bool");
    if (b)
    {
        CCLOG("bool is true");
    }
    else
    {
        CCLOG("bool is false");
    }
}	
	例子是把：把一组值存入CCUserDefault类中，然后获得值，并把它们输出，之后修改之前存入的那组值，然后再次
获得值并输出
	
	注意：所哟的存档操作在调用flush后才有效。
	注意，CCUserDefault类是一个单例模式，通过sharedUserDefault函数获得并进行操作。