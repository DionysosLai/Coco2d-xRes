#include "HelloWorldScene.h"
#include "AppMacros.h"

#include "cocos2d.h"
#include "../external/json/document.h"
#include "../external/json/writer.h"
#include "../external/json/stringbuffer.h"
#include "../external/json/rapidjson.h"

using namespace  rapidjson;

USING_NS_CC;


Scene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

	//////////////////////////////////////////////////////////////////////////
	/// 数据读取
	SSIZE_T size;
/*	FILE* file1 = fopen("test.json", "wr");*/
	unsigned char* ch = FileUtils::getInstance()->getFileData("test.json","r", &size);
	std::string data = std::string((const char* )ch, size);
	//////////////////////////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////////////////////////
	/// 数据解析成json格式数据
	Document doc;		///< 创建一个Document对象 rapidJson的相关操作都在Document类中
	doc.Parse<0>(data.c_str());	///< 通过Parse方法将Json数据解析出来
	if (doc.HasParseError()) 
	{ 
		CCLOG("GetParseError %s\n",doc.GetParseError());
	} 
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	/// Json数据读取和更改-----对值操作
	rapidjson::Value& valString = doc["hello1"];		///< 读取键“hello”的值，根据我们的json文档，是一个字符串
	if (valString.IsString())	///< 判断是否是字符串
	{
		const char* ch = valString.GetString();
		log(ch);
		log(valString.GetString());
		valString.SetString("newString");
		log(valString.GetString());
	}
	rapidjson::Value& valArray = doc["a"];			///< 读取键“a”值，根据我们的json文档，是一个数组
	if (valArray.IsArray())							///< 判断val的类型 是否为数组 我们的Tollgate键对应的value实际为数组
	{
		for (int i = 0; i < valArray.Capacity(); ++i)
		{
			rapidjson::Value& first	= valArray[i];	///< 获取到val中的第i个元素 根据我们这里的json文件 val中共有4个元素
			CCLOG("%f", first.GetDouble());			///< 将value转换成Double类型打印出来 结果为0.5
			first.SetDouble(10.f);
			CCLOG("%f", first.GetDouble());			///< 将value转换成Double类型打印出来 结果为0.5S
		}
	}
	//////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	/// json数据操作---对键操作之添加成员对象
	/// 添加一个String对象；	
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();	///< 获取最初数据的分配器
	rapidjson::Value strObject(rapidjson::kStringType);					///< 添加字符串方法1
	strObject.SetString("love");
	doc.AddMember("hello1", strObject, allocator);
/*	doc.AddMember("hello1", "love you", allocator);						///< 添加字符串方法2:往分配器中添加一个对象*/

	/// 添加一个null对象
	rapidjson::Value nullObject(rapidjson::kNullType);
	doc.AddMember("null", nullObject, allocator);						///< 往分配器中添加一个对象

	/// 添加一个数组对象
	rapidjson::Value array(rapidjson::kArrayType);		///< 创建一个数组对象
	rapidjson::Value object(rapidjson::kObjectType);	///< 创建数组里面对象。
	object.AddMember("id", 1, allocator);
	object.AddMember("name", "lai", allocator);
	object.AddMember("age", "12", allocator);
	object.AddMember("low", true, allocator);
	array.PushBack(object, allocator);
	doc.AddMember("player", array, allocator);			///< 将上述的数组内容添加到一个名为“player”的数组中

	/// 在已有的数组中添加一个成员对象
	rapidjson::Value& aArray1 = doc["a"];
	aArray1.PushBack(2.0, allocator);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	/// json数据操作---对键操作之删除成员对象
	/// 删除数组成员对象里面元素
	rapidjson::Value& aArray2 = doc["a"];	///< 读取键“a”值，根据我们的json文档，是一个数组
	aArray2.PopBack();		///< 删除数组最后一个成员对象

	if (doc.RemoveMember("i"))				///< 删除键为“i”的成员变量
	{
		log("delet i member ok!");
	}
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	/// 将json数据重新写入文件中---先将文件删除，再写入内容
	rapidjson::StringBuffer  buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);		

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	system("del E:\cocos2d-x-3.2rc0\tests\cpp-empty-test\Resources\test.josn");		///< 先将文件删除掉---之前重这个文件读取数据，因此确保这个文件存在了
	FILE* file = fopen("test.json", "wb");

	if (file)
	{
		fputs(buffer.GetString(), file);
		fclose(file);
	}
#else if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	/// 原理差不多，就是先将文件清空，在写入。这里就不写了。
#endif
	//////////////////////////////////////////////////////////////////////////    
    return true;
}

