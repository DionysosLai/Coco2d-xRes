	为什么要设置抗锯齿呢？因为受分辨率的制约，在渲染物体时，被绘制的物体边缘总
会或多或少地呈现三角形的锯齿，抗锯齿对图像边缘进行柔化处理，使图像边缘看起来更平
滑，更接近实物的物体。因为默认是抗锯齿，所以抗锯齿不用设置，锯齿需要在进入和出场
景时设置。

	设置精灵抗锯齿与非抗锯齿：
    sprite->getTexture()->setAliasTexParameters();///< 非抗锯齿
	
	sprite->getTexture()->setAntiAliasTexParameters();		///< 抗锯齿