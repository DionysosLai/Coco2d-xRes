void releaseCaches()
{
	CCAnimationCache::purgeSharedAnimationCache();
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
	CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}

值得注意的是清理的顺序，应该先清理动画缓存，然后清理精灵帧，最后是纹理。按照引用层级由高到低，以保证释放引
用有效。