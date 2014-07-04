cocos2d-x 获取图片的某像素点的RGBA颜色  

2013-08-19 15:34:12|  分类： cocos2d-x |举报|字号 订阅
ccColor4B c = {0, 0, 0, 0};
CCPoint pt = ccp(240,160);//要获取的点
unsigned int x = pt.x, y = pt.y;

CCImage * myimg = new CCImage();
//加载 图片
myimg->initWithImageFileThreadSafe("HelloWorld1.png");
unsigned char *data_=myimg->getData();
unsigned int *pixel = (unsigned int *)data_;
pixel = pixel + (y * 480) + x;        //480 是图片的宽
c.r = *pixel & 0xff;
c.g = (*pixel >> 8) & 0xff;
c.b = (*pixel >> 16) & 0xff;
c.a = (*pixel >> 24) & 0xff;        //这个值  就是透明度