void glReadPixels (GLint x, GLint y,  
      GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid* pixels); 
  这个函数将当前屏幕上的像素读取到一个内存块pixels中，且pixels指针指向的内存必须足够大。为此，我们设计一个
函数saveScreenToCCImage来实现截图功能，


unsigned char screenBuffer[1024 * 1024 * 8];
CCImage* saveScreenToCCImage(bool upsidedown)
{  
	CCSize winSize = CCDirector::sharedDirector()->getWinSizeInPixels();
	int w = winSize.width;
	int h = winSize.height;
	int  myDataLength = w * h * 4;

	GLubyte* buffer = screenBuffer;
	glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

	CCImage* image = new CCImage();
	if(upsidedown)
	{
		GLubyte* buffer2 = (GLubyte*) malloc(myDataLength);
		for(int y = 0; y <h; y++)
		{
			for(int x = 0; x <w * 4; x++)
			{
				buffer2[(h - 1 - y) * w * 4 + x] = buffer[y * 4 * w + x];
			}
		}
		bool ok = image->initWithImageData(buffer2, myDataLength,
		CCImage::kFmtRawData, w, h);
		free(buffer2);
	}
	else 
	{
		bool ok = image->initWithImageData(buffer, myDataLength,
		CCImage::kFmtRawData, w, h);
	}
	return image;
}
void saveScreen(/*CCObject* sender*/)
{
	CCImage* image = saveScreenToCCImage();
	image->saveToFile("screen.png");
	image->release();
}