CCLabelBMFont  相当于每次改变只改变了图片坐标，而CCLabelTTF要重新渲染.这个类使用之前，需要添加好字体文件，包括一个图片文件 (**.png) 和一个 字体坐标文件 （**.fnt）。

使用方法：
CCLabelBMFont* label = CCLabelBMFont::create("1", "Heiti16.fnt");
label->setPosition(ccp(100, 100));
label->setString("0");
addChild(label, 1);

可以看到 .fnt 文件定义了 图像文件的名称，以及每个字符对应的位置信息。
这个没办法指定字体的字号，但可以用  scale  属性进行缩放来调整大小。就当它是sprite。