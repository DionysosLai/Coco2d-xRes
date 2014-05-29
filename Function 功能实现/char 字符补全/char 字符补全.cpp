char str[64] = {0};  
for(int i = 1; i <=12; i++)   
{  
	sprintf(str, "run%04d.png", i);  
	CCSpriteFrame* frame = cache->spriteFrameByName( str );  
	animFrames->addObject(frame);  
} 