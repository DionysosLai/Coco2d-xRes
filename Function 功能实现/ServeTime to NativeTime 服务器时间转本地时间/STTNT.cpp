做网络游戏时，服务器的时间在本地显示的时候要转成本地时间，可以利用系统函数转化。


[cpp] view plaincopy
<span style="font-size:14px;">long long int timeSec = "1353735416";//假如这是服务器的时间  
char str[64] = {0};  
time_t timep = timeSec;  
sprintf(str, "%s",asctime(gmtime(&timep)));</span>  



str是 星期+日期+时间
 
如果希望自己定义格式，用下面的办法也可以。

[cpp] view plaincopy
<span style="font-size:14px;">long long int timeSec = "1353735416";//假如这是服务器的时间  
char str[64] = {0};  
time_t timep = timeSec;  
tm* timeStuct = gmtime(&timep);  
sprintf(str, "%02d/%02d/%02d %02d:%02d",  
timeStuct->tm_year + 1900,  
timeStuct->tm_mon + 1,  
timeStuct->tm_mday,  
timeStuct->tm_hour,  
timeStuct->tm_min);</span>  