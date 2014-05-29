#ifndef __CURL_GLOBAL_INITIATOR_H__
#define __CURL_GLOBAL_INITIATOR_H__

#include <string>

#include "cocos2d.h"
#include "curl/curl.h"

USING_NS_CC;
using namespace std;

class CURL_GLOBAL_INITIATOR
{
	CURL_GLOBAL_INITIATOR()
	{
		curl_global_init(CURL_GLOBAL_ALL);
	}

	static CURL_GLOBAL_INITIATOR curl_global_initiator;

public:
	~CURL_GLOBAL_INITIATOR()
	{
		curl_global_cleanup();
	}
};
	
CURL_GLOBAL_INITIATOR CURL_GLOBAL_INITIATOR::curl_global_initiator;


class NetworkAdaptor{
	string m_sBaseUrl;
public:
	NetworkAdaptor(const string& baseUrl);
	NetworkAdaptor(const char* baseUrl);
	bool sendValueForKey(const char* key, const char* _value, string& writeBackBuff);
	bool sendValuesForKey(const map<string, string>& values, string& writeBackBuff);

	void translate(const map<string, string>& values, string& sendoutMsg);
	size_t writer(char* data, size_t size, size_t nmemb, string* writerData);
};

typedef map<string, string> StringMap;
class AsynchronousNetworkAdaptor  
{
protected:
	struct RequestInfo
	{
		RequestInfo(const StringMap& _v, const string& _u, string& _b) : values(_v), url(_u), buffer(_b)
		{ }
		StringMap values;
		string url;
		string& buffer;
	};
	vector<RequestInfo> requests;
public:
	void sendValueForKeyToURL(const char* key, const char* _value,
		const string& url, string& writeBackBuff);
	void sendValuesForKeyToURL(const StringMap& values,
		const string& url, string& writeBackBuff);
	void flushSendRequest();
	CC_SYNTHESIZE_READONLY(int, m_iUnfinishedRequest, UnfinishedRequest);
};

#endif