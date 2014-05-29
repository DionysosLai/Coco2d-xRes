#include "CURL_GLOBAL_INITIATOR.h"


///@brief 向一个预设的URL传输一个字典中的所有键值对。
///
/// 用curl_easy_setopt设置了CURL的几个关键参数：设置发送POST请求、设置上传的数据、设置回调
///函数，以及设置缓冲对象。最后便是执行和清理了。
///@param[in|out] 
///@pre 
///@return 
///@retval 
///@post 
///*@version 1.0
///*@data 2014/04/10
bool NetworkAdaptor::sendValuesForKey(const map<string,string>& values, string& writeBackBuff)
{
	CURL *curl = curl_easy_init();

	string sendout;
	translate(values, sendout);
	curl_easy_setopt(curl, CURLOPT_URL, m_sBaseUrl.c_str());
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, sendout.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &writeBackBuff);

	int res = curl_easy_perform(curl);
	curl_easy_cleanup(curl);

	if (res == 0)
	{
		CCLOG("get data from  server : %s", writeBackBuff.c_str());
		return true;
	}
	else
	{
		CCLOG("curl post error!");
		return false;
	}
}

///@brief 向一个预设的URL传输一个字典中的所有键值对。
///
/// 
///@param[in|out] 
///@pre 
///@return 
///@retval 
///@post 
///*@version 1.0
///*@data 2014/04/10
void NetworkAdaptor::translate(const map<string, string>& values, string& sendoutMsg)
{
	sendoutMsg = "";

	for(map<string, string>::const_iterator it = values.begin();
		it != values.end(); ++it)
	{
		sendoutMsg += (it->first + " = " + it->second);
	}
}

///@brief 
///
///
///@param[in|out] 
///@pre 
///@return 
///@retval 
///@post 
///*@version 1.0
///*@data 2014/04/10
size_t NetworkAdaptor::writer(char* data, size_t size, size_t nmemb, string* writerData)
{
//	LOG_FUNCTION_LIFE;
	if(writerData == NULL)
		return 0;
	writerData->append(data, size * nmemb);
	return size * nmemb;
}

void AsynchronousNetworkAdaptor::sendValuesForKeyToURL(const StringMap& values, const string& url, string& writeBackBuff)
{
	RequestInfo info(values, url, writeBackBuff);
	requests.push_back(info);
}

void AsynchronousNetworkAdaptor::flushSendRequest()
{
	CURLM* backUrl = curl_multi_init();
	for(int i = 0; i < requests.size(); i++)
	{
		CURL *curl = curl_easy_init();
		curl_easy_setopt(curl, CURLOPT_URL, requests[i].url);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_POST,1);
		string sendout;
		translate(requests[i].values, sendout);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS,sendout.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &requests[i].buffer);
		curl_multi_add_handle(backUrl, curl);
		curl_easy_cleanup(curl);
	}
	url_multi_perform(backUrl, &m_iUnfinishedRequest);
}