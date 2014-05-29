第一行是对字体的介绍。



info face="华康海报体W12(P)" size=32 bold=0italic=0 charset="" unicode=0stretchH=100smooth=1 aa=1 padding=0,0,0,0 spacing=1,1



解释：

face="华康海报体W12(P)":字体为”华康海报体W12(P)”,

size=32:大小为32像素

bold=0 :不加粗

italic=0:不使用斜体

charset="": charset是编码字符集，这里没有填写值即使用默认，

unicode=0:不使用Unicode

stretchH=100:纵向缩放百分比

smooth=1 :开启平滑

aa=1:开启抗锯齿

padding=0,0,0,0:内边距，文字与边框的空隙。

spacing=1,1 :外边距，就是相临边缘的距离。

 

第二行是对应所有字贴图的公共信息

common lineHeight=37 base=28 scaleW=512 scaleH=512pages=1 packed=0

解释：

lineHeight=37：行高，如果遇到换行符时，绘制字的位置坐标的Y值在换行后增加的像素值。

base=28 :字的基本大小

scaleW=512 :图片大小

scaleH=512:图片大小

pages=1 :此种字体共用到几张图。

packed=0:图片不压缩

 

第三行是对应当前字贴图的信息

//第一页，文件名称是”bitmapFontChinese.png”

page id=0 file="bitmapFontChinese.png"

 

第四行是当前贴图中所容纳的文字数量

chars count=204

 

第五行起把当前贴图中所用到的所有文字的编码以及对应在图片上的矩形位置，偏移等列出来

第一个字符编码为32,也就是空格，位置为0,0,宽高为0,0, 绘制到屏幕的相应位置时，像素偏移（0，28），
绘制完后相应位置的x往后移15像素再画下一个字符，字的图块在第1页上

char id=32  x=0     y=0     width=0    height=0     xoffset=0     yoffset=28    xadvance=15     page=0 chnl=0

第一个字符编码为汉字”象”,也就是空格，位置为0,0,宽为33,高为36, 绘制到屏幕的相应位置时，像素偏移（0，-1），
绘制完后相应位置的x往后移36像素再画下一个字，字的图块在第1页上

char id=35937  x=0     y=0     width=33    height=36     xoffset=0     yoffset=-1    xadvance=36     page=0 chnl=0

char id=26696  x=33     y=0     width=35     height=36     xoffset=-1     yoffset=-1    xadvance=36     page=0 chnl=0

char id=26071  x=68     y=0     width=35     height=36     xoffset=-1    yoffset=-1    xadvance=36     page=0 chnl=0

…

 

再后面是描述两个字在进行组合绘制时字距调整的相关信息，这里没有要进行间距调整的字组合所以为设-1。
对于字组合间距调整可以看此示例图：http://www.blueidea.com/articleimg/2007/12/5160/01s.jpg

kernings count=-1

这个数字代表参与字组合间距调整的字的数量。

如果kernings count大于零，后面会有类似这样的描述：

kerning first=102  second=41 amount=2

也就是’f’与’)’进行组合显示’f)’时，’)’向右移2像素防止粘在一起。

 

通过上面这些信息，引擎可以通过编码找到相应的文字并取出对应的纹理块。
