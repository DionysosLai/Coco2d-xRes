1． 图字的原理：将所要绘制的字绘制到图片上，通过编码取得对应的矩形块UV进行顶点缓冲的设置之后进行绘制。Cocos2d-x提供的两个类非常重要：
(1) CCTextureAtlas(2) CCSpriteBatchNode。

2． 图字的组成：一张纹理，一个纹理描述信息文件。缺一不可。要做好图字，必须深入理解图字纹理描述信息文件的格式,
可能看Cocos2d-x提供的类: CCBMFontConfiguration，并掌握图字工具的使用。
http://blog.csdn.net/honghaier/article/details/7931914