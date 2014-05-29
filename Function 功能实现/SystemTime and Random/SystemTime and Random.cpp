bool HelloWorld::init()
{
    bool bRet = false;
    do
    {
 
        CC_BREAK_IF(! CCLayer::init());
        //获取系统时间
        //time_t是long类型，精确到秒，通过time()函数可以获得当前时间和1970年1月1日零点时间的差
        time_t tt;
        time(&tt);
        struct tm * now;
        //获得本地时间
        now = localtime(&tt);
        CCLog("%d/%d/%d %d:%d:%d",now->tm_year+1900,now->tm_mon+1,
            now->tm_mday,now->tm_hour,now->tm_min,now->tm_sec);
 
        //产生随机数
        //CCRANDOM_0_1()获取一个0-1之间的float类型数，包括0和1
        //通过CCRANDOM_0_1()获得的随机数其实并不是随机的，要想获取接近真实的随机数需要设置一个种子
        //比较常用的方法是以当前时间为种子，在cocos2d-x中提供了获取时间的方法，而且获取的时间更精确
        //cc_timeval是个结构体，里边有俩个变量，一个是以秒为单位的，一个是以微妙为单位的
        cc_timeval tv;
        CCTime::gettimeofdayCocos2d(&tv,NULL);
        //都转化为毫秒
        unsigned long reed = tv.tv_sec*1000+tv.tv_usec/1000;
        //srand()中传入一个随机数种子
        srand(reed);
 
        this->schedule(schedule_selector(HelloWorld::random),1.0);
 
        bRet = true;
    } while (0);
 
    return bRet;
}
 
//如果要获得a~b之间的一个随机数，则应该是CCRANDOM_0_1*(b-a)+a，传入的参数就是a和b
int HelloWorld::getRandomNumber(int start,int end)
{
    return CCRANDOM_0_1()*(end-start)+start;
}
 
void HelloWorld::random(float tm)
{
    int random = this->getRandomNumber(0,10);
    CCLog("random = %d",random);
}