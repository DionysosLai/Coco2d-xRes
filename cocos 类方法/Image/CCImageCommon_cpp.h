/****************************************************************************
Copyright (c) 2010 cocos2d-x.org
Copyright (c) Microsoft Open Technologies, Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#ifndef __CC_PLATFORM_IMAGE_CPP__
#error "CCFileUtilsCommon_cpp.h can only be included for CCFileUtils.cpp in platform/win32(android,...)"
#endif /* __CC_PLATFORM_IMAGE_CPP__ */

#include "CCImage.h"
#include "CCCommon.h"
#include "CCStdC.h"
#include "CCFileUtils.h"
//libpng库的头文件
#include "png.h"
//libjpg库的头文件  
#include "jpeglib.h"
//libtiff库的头文件 
#include "tiffio.h"
//如果没有定义基于平台的CCImage的CPP标记宏，编译时打印出错。
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
#include "CCFreeTypeFont.h"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/CCFileUtilsAndroid.h"
#endif

#include <string>
#include <ctype.h>

#ifdef EMSCRIPTEN
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#endif // EMSCRIPTEN
//使用Cocos2d命名空间
NS_CC_BEGIN

// premultiply alpha, or the effect will wrong when want to use other pixel format in CCTexture2D,
// such as RGB888, RGB5A1//定义宏从RGB888或RGB5A1像素格式数据中返回一个RGBA8888的像素格式数据。
#define CC_RGB_PREMULTIPLY_ALPHA(vr, vg, vb, va) \
    (unsigned)(((unsigned)((unsigned char)(vr) * ((unsigned char)(va) + 1)) >> 8) | \
    ((unsigned)((unsigned char)(vg) * ((unsigned char)(va) + 1) >> 8) << 8) | \
    ((unsigned)((unsigned char)(vb) * ((unsigned char)(va) + 1) >> 8) << 16) | \
    ((unsigned)(unsigned char)(va) << 24))

// on ios, we should use platform/ios/CCImage_ios.mm instead
//图片文件数据的信息结构
typedef struct 
{
    unsigned char* data;
    int size;
    int offset;
}tImageSource;
//读取PNG文件数据的回调函数  
//参1:PNG文件数据指针  
//参2:返回的图片数据地址  
//参3:要从PNG文件中读取的图片数据的长度，其值 = 每像素字节数X图片的宽X图片的高。
static void pngReadCallback(png_structp png_ptr, png_bytep data, png_size_t length)
{
	//从一个PNG文件数据指针指针中返回图片文件数据的信息结构
    tImageSource* isource = (tImageSource*)png_get_io_ptr(png_ptr);
	//如果要读取的长度有效。则将相应长度的图像数据拷贝到返回的图片数据地址中。
    if((int)(isource->offset + length) <= isource->size)
    {
        memcpy(data, isource->data+isource->offset, length);
        isource->offset += length;
    }
    else
    {
        png_error(png_ptr, "pngReaderCallback failed");
    }
}

//////////////////////////////////////////////////////////////////////////
// Implement CCImage
//////////////////////////////////////////////////////////////////////////

CCImage::CCImage()
: m_nWidth(0)
, m_nHeight(0)
, m_nBitsPerComponent(0)
, m_pData(0)
, m_bHasAlpha(false)
, m_bPreMulti(false)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
    m_ft = nullptr;
#endif
}

CCImage::~CCImage()
{
    CC_SAFE_DELETE_ARRAY(m_pData);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
    CC_SAFE_DELETE(m_ft);
#endif
}
//从指定的路径载入一个所支持的格式的图片文件。
bool CCImage::initWithImageFile(const char * strPath, EImageFormat eImgFmt/* = eFmtPng*/)
{
    bool bRet = false;

#ifdef EMSCRIPTEN
    // Emscripten includes a re-implementation of SDL that uses HTML5 canvas
    // operations underneath. Consequently, loading images via IMG_Load (an SDL
    // API) will be a lot faster than running libpng et al as compiled with
    // Emscripten.
    SDL_Surface *iSurf = IMG_Load(strPath);

    int size = 4 * (iSurf->w * iSurf->h);
    bRet = _initWithRawData((void*)iSurf->pixels, size, iSurf->w, iSurf->h, 8, true);

    unsigned int *tmp = (unsigned int *)m_pData;
    int nrPixels = iSurf->w * iSurf->h;
    for(int i = 0; i < nrPixels; i++)
    {
        unsigned char *p = m_pData + i * 4;
        tmp[i] = CC_RGB_PREMULTIPLY_ALPHA( p[0], p[1], p[2], p[3] );
    }

    SDL_FreeSurface(iSurf);
#else
    unsigned long nSize = 0;
    std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(strPath);
    unsigned char* pBuffer = CCFileUtils::sharedFileUtils()->getFileData(fullPath.c_str(), "rb", &nSize);
    if (pBuffer != NULL && nSize > 0)
    {
        bRet = initWithImageData(pBuffer, nSize, eImgFmt);
    }
    CC_SAFE_DELETE_ARRAY(pBuffer);
#endif // EMSCRIPTEN

    return bRet;
}
//从指定的路径载入一个所支持的格式的图片文件,但它是线程安全的,因此可以用在多线程加载图片。 S
bool CCImage::initWithImageFileThreadSafe(const char *fullpath, EImageFormat imageType)
{
    bool bRet = false;
    unsigned long nSize = 0;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	//调用文件操作函数库中的函数读取相应路径的文件到内存中，并返回内存的地址给指针变量pBuffer。 
    CCFileUtilsAndroid *fileUitls = (CCFileUtilsAndroid*)CCFileUtils::sharedFileUtils();
    unsigned char *pBuffer = fileUitls->getFileDataForAsync(fullpath, "rb", &nSize);
#else
    unsigned char *pBuffer = CCFileUtils::sharedFileUtils()->getFileData(fullpath, "rb", &nSize);
#endif
    if (pBuffer != NULL && nSize > 0)
    {
		 //如果读取成功，则将内存地址做为参数调用initWithImageData函数来加载图片数据。
        bRet = initWithImageData(pBuffer, nSize, imageType);
    }
	//释放读取文件所创建的内存。
    CC_SAFE_DELETE_ARRAY(pBuffer);
    return bRet;
}
//从内存中加载图片数据。  
//参1:指向图片数据所处内存地址的指针。  
//参2:图片数据长度  
//参3:数据对应图片的格式，  
//参4:数据对应图片的宽度  
//参5:数据对应图片的高度  
//参6:每像素的字节位数,即色深。  
bool CCImage::initWithImageData(void * pData, 
                                int nDataLen, 
                                EImageFormat eFmt/* = eSrcFmtPng*/, 
                                int nWidth/* = 0*/,
                                int nHeight/* = 0*/,
                                int nBitsPerComponent/* = 8*/)
{
    bool bRet = false;
    do 
    {
		//参数有效性判断
        CC_BREAK_IF(! pData || nDataLen <= 0);
		//根据不同的图片数据格式调用不同的函数创建相应的图片数据。
        if (kFmtPng == eFmt)
        {
			//读取PNG格式的图片数据。
            bRet = _initWithPngData(pData, nDataLen);
            break;
        }
        else if (kFmtJpg == eFmt)
        {
			//读取JPG格式的图片数据。
            bRet = _initWithJpgData(pData, nDataLen);
            break;
        }
        else if (kFmtTiff == eFmt)
        {
			//读取TIFF格式的图片数据。
            bRet = _initWithTiffData(pData, nDataLen);
            break;
        }
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT) && (CC_TARGET_PLATFORM != CC_PLATFORM_WP8)
       else if (kFmtWebp == eFmt)
        {
			
            bRet = _initWithWebpData(pData, nDataLen);
            break;
        }
#endif
        else if (kFmtRawData == eFmt)
        {
			//读取RGBA8888格式的图片数据。
            bRet = _initWithRawData(pData, nDataLen, nWidth, nHeight, nBitsPerComponent, false);
            break;
        }
        else
        {
            // if it is a png file buffer.
			// 如果未指定数据的格式.则通过对比相应格式的文件头信息判断格式。  
           //判断是否是PNG 
            if (nDataLen > 8)
            {
                unsigned char* pHead = (unsigned char*)pData;
                if (   pHead[0] == 0x89
                    && pHead[1] == 0x50
                    && pHead[2] == 0x4E
                    && pHead[3] == 0x47
                    && pHead[4] == 0x0D
                    && pHead[5] == 0x0A
                    && pHead[6] == 0x1A
                    && pHead[7] == 0x0A)
                {
					//通过对比如果是属于PNG格式则读取PNG格式的图片数据 
                    bRet = _initWithPngData(pData, nDataLen);
                    break;
                }
            }

            // if it is a tiff file buffer.
			//判断是否是TIFF
            if (nDataLen > 2)
            {
                unsigned char* pHead = (unsigned char*)pData;
                if (  (pHead[0] == 0x49 && pHead[1] == 0x49)
                    || (pHead[0] == 0x4d && pHead[1] == 0x4d)
                    )
                {
                    bRet = _initWithTiffData(pData, nDataLen);
                    break;
                }
            }

            // if it is a jpeg file buffer.
			//判断是否是JPG 
            if (nDataLen > 2)
            {
                unsigned char* pHead = (unsigned char*)pData;
                if (   pHead[0] == 0xff
                    && pHead[1] == 0xd8)
                {
                    bRet = _initWithJpgData(pData, nDataLen);
                    break;
                }
            }
        }
    } while (0);
    return bRet;
}

/*
 * ERROR HANDLING:
 *
 * The JPEG library's standard error handler (jerror.c) is divided into
 * several "methods" which you can override individually.  This lets you
 * adjust the behavior without duplicating a lot of code, which you might
 * have to update with each future release.
 *
 * We override the "error_exit" method so that control is returned to the
 * library's caller when a fatal error occurs, rather than calling exit()
 * as the standard error_exit method does.
 *
 * We use C's setjmp/longjmp facility to return control.  This means that the
 * routine which calls the JPEG library must first execute a setjmp() call to
 * establish the return point.  We want the replacement error_exit to do a
 * longjmp().  But we need to make the setjmp buffer accessible to the
 * error_exit routine.  To do this, we make a private extension of the
 * standard JPEG error handler object.  (If we were using C++, we'd say we
 * were making a subclass of the regular error handler.)
 *
 * Here's the extended error handler struct:
 */

struct my_error_mgr {
  struct jpeg_error_mgr pub;	/* "public" fields */

  jmp_buf setjmp_buffer;	/* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;

/*
 * Here's the routine that will replace the standard error_exit method:
 */

METHODDEF(void)
my_error_exit (j_common_ptr cinfo)
{
  /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
  my_error_ptr myerr = (my_error_ptr) cinfo->err;

  /* Always display the message. */
  /* We could postpone this until after returning, if we chose. */
  (*cinfo->err->output_message) (cinfo);

  /* Return control to the setjmp point */
  longjmp(myerr->setjmp_buffer, 1);
}

//读取JPG格式的图片数据。
bool CCImage::_initWithJpgData(void * data, int nSize)
{
    /* these are standard libjpeg structures for reading(decompression) */
	//此处使用libjpeg库来读取JPG，这里需要建立相应的结构变量。
    struct jpeg_decompress_struct cinfo;
    /* We use our private extension JPEG error handler.
	 * Note that this struct must live as long as the main JPEG parameter
	 * struct, to avoid dangling-pointer problems.
	 */
	struct my_error_mgr jerr;
    /* libjpeg data structure for storing one row, that is, scanline of an image */
    JSAMPROW row_pointer[1] = {0};
    unsigned long location = 0;
    unsigned int i = 0;

    bool bRet = false;
    do 
    {
        /* We set up the normal JPEG error routines, then override error_exit. */
		//下面是使用libjpeg来进行JPG格式数据的读取。
		cinfo.err = jpeg_std_error(&jerr.pub);
		jerr.pub.error_exit = my_error_exit;
		/* Establish the setjmp return context for my_error_exit to use. */
		// JPG只能支持RGB的像素格式
		if (setjmp(jerr.setjmp_buffer)) {
			/* If we get here, the JPEG code has signaled an error.
			 * We need to clean up the JPEG object, close the input file, and return.
			 */
			CCLog("%d", bRet);
			jpeg_destroy_decompress(&cinfo);
			break;
		}
		//开始解压JPG。
        /* setup decompression process and source, then read JPEG header */
        jpeg_create_decompress( &cinfo );

        jpeg_mem_src( &cinfo, (unsigned char *) data, nSize );

        /* reading the image header which contains image information */
#if (JPEG_LIB_VERSION >= 90)
        // libjpeg 0.9 adds stricter types.
        jpeg_read_header( &cinfo, TRUE );
#else
        jpeg_read_header( &cinfo, true );
#endif

        // we only support RGB or grayscale
        if (cinfo.jpeg_color_space != JCS_RGB)
        {
            if (cinfo.jpeg_color_space == JCS_GRAYSCALE || cinfo.jpeg_color_space == JCS_YCbCr)
            {
                cinfo.out_color_space = JCS_RGB;
            }
        }
        else
        {
            break;
        }

        /* Start decompression jpeg here */
        jpeg_start_decompress( &cinfo );

        /* init image info */
		//设置相应成员变量。
        m_nWidth  = (short)(cinfo.output_width);
        m_nHeight = (short)(cinfo.output_height);
        m_bHasAlpha = false;
        m_bPreMulti = false;
        m_nBitsPerComponent = 8;
        row_pointer[0] = new unsigned char[cinfo.output_width*cinfo.output_components];
        CC_BREAK_IF(! row_pointer[0]);
		//为图片数据指针申请相应大小的内存。
        m_pData = new unsigned char[cinfo.output_width*cinfo.output_height*cinfo.output_components];
        CC_BREAK_IF(! m_pData);

        /* now actually read the jpeg into the raw buffer */
        /* read one scan line at a time */
		//将像素信息读取到图片数据指针指向的内存中。
        while( cinfo.output_scanline < cinfo.output_height )
        {
			//每次读取一个扫描行的像素信息
            jpeg_read_scanlines( &cinfo, row_pointer, 1 );
            for( i=0; i<cinfo.output_width*cinfo.output_components;i++) 
            {
				//将读取到的像素信息存入相应的内存中。
                m_pData[location++] = row_pointer[0][i];
            }
        }

		/* When read image file with broken data, jpeg_finish_decompress() may cause error.
		 * Besides, jpeg_destroy_decompress() shall deallocate and release all memory associated
		 * with the decompression object.
		 * So it doesn't need to call jpeg_finish_decompress().
		 */
		//jpeg_finish_decompress( &cinfo );
		//完成解压
        jpeg_destroy_decompress( &cinfo );
        /* wrap up decompression, destroy objects, free pointers and close open files */        
        bRet = true;
    } while (0);
	//释放申请的内存S
    CC_SAFE_DELETE_ARRAY(row_pointer[0]);
    return bRet;
}

//读取PNG格式的图片数据。
bool CCImage::_initWithPngData(void * pData, int nDatalen)
{
// length of bytes to check if it is a valid png file
	// PNG文件头信息长度值 
#define PNGSIGSIZE  8
    bool bRet = false;
	//定义存储PNG文件头信息的BYTE数组
    png_byte        header[PNGSIGSIZE]   = {0}; 
	//PNG的读取说明结构，这里是libpng用到的结构体。
    png_structp     png_ptr     =   0;
	 //PNG的信息结构
    png_infop       info_ptr    = 0;

    do 
    {
        // png header len is 8 bytes
		// PNG文件头有效性判断
        CC_BREAK_IF(nDatalen < PNGSIGSIZE);

        // check the data is png or not
		// 存储文件头信息
        memcpy(header, pData, PNGSIGSIZE);
        CC_BREAK_IF(png_sig_cmp(header, 0, PNGSIGSIZE));

        // init png_struct
		//初始化PNG的读取说明结构 
        png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
        CC_BREAK_IF(! png_ptr);

        // init png_info
		// 初始化 PNG信息结构 
        info_ptr = png_create_info_struct(png_ptr);
        CC_BREAK_IF(!info_ptr);

#if (CC_TARGET_PLATFORM != CC_PLATFORM_BADA && CC_TARGET_PLATFORM != CC_PLATFORM_NACL)
        CC_BREAK_IF(setjmp(png_jmpbuf(png_ptr)));
#endif

        // set the read call back function
		//图片文件数据的信息结构
        tImageSource imageSource;
        imageSource.data    = (unsigned char*)pData;
        imageSource.size    = nDatalen;
        imageSource.offset  = 0;
		//设置读取PNG的回调函数
        png_set_read_fn(png_ptr, &imageSource, pngReadCallback);

        // read png header info
        
        // read png file info
		//读取PNG信息结构 
        png_read_info(png_ptr, info_ptr);
        //取得图片数据的相关属性。
        m_nWidth = png_get_image_width(png_ptr, info_ptr);
        m_nHeight = png_get_image_height(png_ptr, info_ptr);
        m_nBitsPerComponent = png_get_bit_depth(png_ptr, info_ptr);
        png_uint_32 color_type = png_get_color_type(png_ptr, info_ptr);

        //CCLOG("color type %u", color_type);
        
        // force palette images to be expanded to 24-bit RGB
        // it may include alpha channel
		// 如果是调色板格式的PNG，将其转为RGB888的像素格式。
        if (color_type == PNG_COLOR_TYPE_PALETTE)
        {
            png_set_palette_to_rgb(png_ptr);
        }
        // low-bit-depth grayscale images are to be expanded to 8 bits
		// 如果是像素格式少于1字节长度的灰度图，将其转为每像素占1字节的像素格式。
        if (color_type == PNG_COLOR_TYPE_GRAY && m_nBitsPerComponent < 8)
        {
            png_set_expand_gray_1_2_4_to_8(png_ptr);
        }
        // expand any tRNS chunk data into a full alpha channel
		// 将RNS块数据信息扩展为完整的ALPHA通道信息
        if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
        {
            png_set_tRNS_to_alpha(png_ptr);
        }  
        // reduce images with 16-bit samples to 8 bits
        if (m_nBitsPerComponent == 16)
        {
            png_set_strip_16(png_ptr);            
        } 
        // expand grayscale images to RGB// 将灰度图格式扩展成RGB
        if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        {
            png_set_gray_to_rgb(png_ptr);
        }

        // read png data// 读取PNG数据
        // m_nBitsPerComponent will always be 8
        m_nBitsPerComponent = 8;
        png_uint_32 rowbytes;
		//后面读取PNG信息是按行读取，将每一行的像素数据读取到相应内存块中，
		//下面这个BYTE指针数组就是为了存储每行图片像素信息读取到的相应内存块位置。
        png_bytep* row_pointers = (png_bytep*)malloc( sizeof(png_bytep) * m_nHeight );
        
        png_read_update_info(png_ptr, info_ptr);
        //取得图片每一行像素的字节数量
        rowbytes = png_get_rowbytes(png_ptr, info_ptr);
        //为图片数据申请内存。
        m_pData = new unsigned char[rowbytes * m_nHeight];
        CC_BREAK_IF(!m_pData);
        //将申请到的内存地址按行放入相应的读取结构中
        for (unsigned short i = 0; i < m_nHeight; ++i)
        {
            row_pointers[i] = m_pData + i*rowbytes;
        }
		//将图片文件数据读取到相应的内存地址。
        png_read_image(png_ptr, row_pointers);
        //结束读取。
        png_read_end(png_ptr, NULL);
        //计算每像素占字节数。不管是RGB888还是RGBA8888的像素格式，
		//其实际每像素占用的字节数均是4，只不过RGB888中多余的1字节不被用到。
        png_uint_32 channel = rowbytes/m_nWidth;
        if (channel == 4)
        {
			//设置为带ALPHA通道
            m_bHasAlpha = true;
			//定义指针变量tmp指向图像数据地址。用于在后面存放图像数据。
            unsigned int *tmp = (unsigned int *)m_pData;
			//双循环遍历像素数据
            for(unsigned short i = 0; i < m_nHeight; i++)
            {
                for(unsigned int j = 0; j < rowbytes; j += 4)
                {
					//将R,G,B,A四个BYTE值组成一个DWORD值。 
                    *tmp++ = CC_RGB_PREMULTIPLY_ALPHA( row_pointers[i][j], row_pointers[i][j + 1], 
                                                      row_pointers[i][j + 2], row_pointers[i][j + 3] );
                }
            }
            //设置使用渐变ALPHA
            m_bPreMulti = true;
        }
		//释放row_pointer
        CC_SAFE_FREE(row_pointers);

        bRet = true;
    } while (0);

    if (png_ptr)
    {
		//释放png_ptr
        png_destroy_read_struct(&png_ptr, (info_ptr) ? &info_ptr : 0, 0);
    }
    return bRet;
}
//读取TIFF图片数据时的回调函数。  
//参1:文件数据内存。  
//参2:输出参数，读取到的图像数据复制到对应的内存地址中。  
//参3:图片数据长度。
static tmsize_t _tiffReadProc(thandle_t fd, void* buf, tmsize_t size)
{
	//将fd转化为图片文件数据的信息结构指针。
    tImageSource* isource = (tImageSource*)fd;
    uint8* ma;
    uint64 mb;
	//定义一次可以读取的数据长度。 
    unsigned long n;
	//定义变量o统计每次循环读取的数据长度。
    unsigned long o;
	//定义变量统计读取完的数据长度。
    tmsize_t p;
	//让前面定义的uint8类型指针变量ma指向buf。用于在后面存放图像数据。
    ma=(uint8*)buf;
	//让前面定义的变量mb来统计剩余未读取的数据长度。
    mb=size;
    p=0;
    while (mb>0)
    {
        n=0x80000000UL;
        if ((uint64)n>mb)
            n=(unsigned long)mb;

		//如果尚未读完所有数据，则继续读取，否则出错返回0
        if((int)(isource->offset + n) <= isource->size)
        {
            memcpy(ma, isource->data+isource->offset, n);
            isource->offset += n;
            o = n;
        }
        else
        {
            return 0;
        }
		//读取完长度为o的数据，则对指针进行相应的偏移操作供下次进行读取操作。
        ma+=o;
		//更新未读取的剩余长度 
        mb-=o;
		//更新读取完的数量长度
        p+=o;
		//下面这个if比较奇怪，因为是不可能为true的。在上一个if判断中已经设置了o=n。
        if (o!=n)
        {
            break;
        }
    }
    return p;
}
//将数据保存为tiff图像文件所调用的回调函数。这里未用。
static tmsize_t _tiffWriteProc(thandle_t fd, void* buf, tmsize_t size)
{
    CC_UNUSED_PARAM(fd);
    CC_UNUSED_PARAM(buf);
    CC_UNUSED_PARAM(size);
    return 0;
}

//在对TIFF图像文件进行解析时进行重定位时调用的回调函数。
static uint64 _tiffSeekProc(thandle_t fd, uint64 off, int whence)
{
	//将fd转化为图片文件数据的信息结构指针。
    tImageSource* isource = (tImageSource*)fd;
    uint64 ret = -1;
    do 
    {
		//如果定位方式为从头开始计算
        if (whence == SEEK_SET)
        {
            CC_BREAK_IF(off >= (uint64)isource->size);
            ret = isource->offset = (uint32)off;
        }
        else if (whence == SEEK_CUR)
        {
			//如果定位方式为从当前位置开始计算
            CC_BREAK_IF(isource->offset + off >= (uint64)isource->size);
            ret = isource->offset += (uint32)off;
        }
        else if (whence == SEEK_END)
        {
			//如果定位方工业从文件尾部开始计算
            CC_BREAK_IF(off >= (uint64)isource->size);
            ret = isource->offset = (uint32)(isource->size-1 - off);
        }
        else
        {
			//其它方式也按照从头开始计算
            CC_BREAK_IF(off >= (uint64)isource->size);
            ret = isource->offset = (uint32)off;
        }
    } while (0);

    return ret;
}
//取得tiff图片文件大小的回调函数。
static uint64 _tiffSizeProc(thandle_t fd)
{
    tImageSource* pImageSrc = (tImageSource*)fd;
    return pImageSrc->size;
}
//关闭tiff图片文件读取的回调函数。
static int _tiffCloseProc(thandle_t fd)
{
    CC_UNUSED_PARAM(fd);
    return 0;
}
//将tiff图片文件映射到内存时调用的回调函数。
static int _tiffMapProc(thandle_t fd, void** pbase, toff_t* psize)
{
    CC_UNUSED_PARAM(fd);
    CC_UNUSED_PARAM(pbase);
    CC_UNUSED_PARAM(psize);
    return 0;
}
//解除tiff图片映射到内存的回调函数。
static void _tiffUnmapProc(thandle_t fd, void* base, toff_t size)
{
    CC_UNUSED_PARAM(fd);
    CC_UNUSED_PARAM(base);
    CC_UNUSED_PARAM(size);
}
//使用LibTiff读取TIFF格式的图片数据。
bool CCImage::_initWithTiffData(void* pData, int nDataLen)
{
    bool bRet = false;
    do 
    {
        // set the read call back function//设置图片文件数据的信息结构 
        tImageSource imageSource;
        imageSource.data    = (unsigned char*)pData;
        imageSource.size    = nDataLen;
        imageSource.offset  = 0;
		//使用libtiff打开一个tif文件，设置对其进行操作的各行为的回调函数。如果成功打开文件返回一个TIFF结构指针。
        TIFF* tif = TIFFClientOpen("file.tif", "r", (thandle_t)&imageSource, 
            _tiffReadProc, _tiffWriteProc,
            _tiffSeekProc, _tiffCloseProc, _tiffSizeProc,
            _tiffMapProc,
            _tiffUnmapProc);
		//有效性判断。
        CC_BREAK_IF(NULL == tif);

        uint32 w = 0, h = 0;
        uint16 bitsPerSample = 0, samplePerPixel = 0, planarConfig = 0;
		//定义变量nPixels存储图像数据像素数量。
        size_t npixels = 0;
        //读取相应的图片属性信息。  
        //图片宽度。
        TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &w);
		//图片高度。
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &h);
		//图片色深。 
        TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bitsPerSample);
		//每像素数据占的字节数 
        TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samplePerPixel);
		//图像的平面配置 
        TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &planarConfig);
		//取得像素数量 
        npixels = w * h;
        //设置带ALPHA通道。
        m_bHasAlpha = true;
        m_nWidth = w;
        m_nHeight = h;
        m_nBitsPerComponent = 8;
		//申请相应的内存用来存储像素数据。
        m_pData = new unsigned char[npixels * sizeof (uint32)];
		//申请临时内存进行TIFF数据读取。
        uint32* raster = (uint32*) _TIFFmalloc(npixels * sizeof (uint32));
        if (raster != NULL) 
        {
			//读取TIFF数据
           if (TIFFReadRGBAImageOriented(tif, w, h, raster, ORIENTATION_TOPLEFT, 0))
           {
                /* the raster data is pre-multiplied by the alpha component 
                   after invoking TIFFReadRGBAImageOriented
                unsigned char* src = (unsigned char*)raster;
                unsigned int* tmp = (unsigned int*)m_pData;

                for(int j = 0; j < m_nWidth * m_nHeight * 4; j += 4)
                {
                    *tmp++ = CC_RGB_PREMULTIPLY_ALPHA( src[j], src[j + 1], 
                        src[j + 2], src[j + 3] );
                }
                */
			   //ALPHA通道有效
                m_bPreMulti = true;
				//上面循环将组合后的DWORD值写入图像数据太慢，这里直接进行内存拷贝可以达到同样目的。
               memcpy(m_pData, raster, npixels*sizeof (uint32));
           }
		   //释放临时申请的内存。
          _TIFFfree(raster);
        }
        
		//关闭TIFF文件读取。
        TIFFClose(tif);

        bRet = true;
    } while (0);
    return bRet;
}
//读取RGBA8888格式的图片数据。  
//参1:数据地址  
//参2:数据长度  
//参3:图片宽度  
//参4:图片高度  
//参5:图片色深  
bool CCImage::_initWithRawData(void * pData, int nDatalen, int nWidth, int nHeight, int nBitsPerComponent, bool bPreMulti)
{
    bool bRet = false;
    do 
    {
		//宽高有效性判断 
        CC_BREAK_IF(0 == nWidth || 0 == nHeight);
		//保存相关属性 
        m_nBitsPerComponent = nBitsPerComponent;
        m_nHeight   = (short)nHeight;
        m_nWidth    = (short)nWidth;
        m_bHasAlpha = true;
        m_bPreMulti = bPreMulti;

        // only RGBA8888 supported 只支持 RGBA8888 格式
        int nBytesPerComponent = 4;
        int nSize = nHeight * nWidth * nBytesPerComponent;
		//为图像数据申请相应内存，将地址返回给m_pData。
        m_pData = new unsigned char[nSize];
		//内存申请成败判断 
        CC_BREAK_IF(! m_pData);
		//将参数数据拷贝到m_pData指向的内存地址中。
        memcpy(m_pData, pData, nSize);

        bRet = true;
    } while (0);

    return bRet;
}

//将图像数据保存为图片文件，目前只支持PNG和JPG  
//参1:文件路径  
//参2:是否是RGB的像素格式 
bool CCImage::saveToFile(const char *pszFilePath, bool bIsToRGB)
{
    bool bRet = false;

    do 
    {
		//参数有效性判断 
        CC_BREAK_IF(NULL == pszFilePath);
		//通过是否有扩展名判断参数有效性。
        std::string strFilePath(pszFilePath);
        CC_BREAK_IF(strFilePath.size() <= 4);
		//将路径名转为小写字符串 
        std::string strLowerCasePath(strFilePath);
        for (unsigned int i = 0; i < strLowerCasePath.length(); ++i)
        {
            strLowerCasePath[i] = tolower(strFilePath[i]);
        }
		//通过扩展名转成相应的图片文件  
        //PNG 
        if (std::string::npos != strLowerCasePath.find(".png"))
        {
            CC_BREAK_IF(!_saveImageToPNG(pszFilePath, bIsToRGB));
        }
		//JPG 
        else if (std::string::npos != strLowerCasePath.find(".jpg"))
        {
            CC_BREAK_IF(!_saveImageToJPG(pszFilePath));
        }
        else
        {
			//不支持其它格式
            break;
        }

        bRet = true;
    } while (0);

    return bRet;
}

//将图像数据保存为PNG图片
bool CCImage::_saveImageToPNG(const char * pszFilePath, bool bIsToRGB)
{
    bool bRet = false;
    do 
    {
		//参数有效性判断 
        CC_BREAK_IF(NULL == pszFilePath);
		//使用libpng来写PNG文件。  
        //定义文件指针变量用于写文件 
        FILE *fp;
		//定义libpng所用的一些信息结构
        png_structp png_ptr;
        png_infop info_ptr;
        png_colorp palette;
        png_bytep *row_pointers;
		//打开文件开始写入 
        fp = fopen(pszFilePath, "wb");
        CC_BREAK_IF(NULL == fp);
		//创建写PNG的文件结构体,将其结构指针返回给png_ptr
        png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		//指针有效性判断 
        if (NULL == png_ptr)
        {
            fclose(fp);
            break;
        }
		//创建PNG的信息结构体，将其结构指针返回给info_ptr。
        info_ptr = png_create_info_struct(png_ptr);
        if (NULL == info_ptr)
        {
            fclose(fp);
            png_destroy_write_struct(&png_ptr, NULL);
            break;
        }
#if (CC_TARGET_PLATFORM != CC_PLATFORM_BADA && CC_TARGET_PLATFORM != CC_PLATFORM_NACL)
        if (setjmp(png_jmpbuf(png_ptr)))
        {
            fclose(fp);
            png_destroy_write_struct(&png_ptr, &info_ptr);
            break;
        }
#endif
		//初始化png_ptr
        png_init_io(png_ptr, fp);
		//根据是否有ALPHA来写入相应的头信息
        if (!bIsToRGB && m_bHasAlpha)
        {
            png_set_IHDR(png_ptr, info_ptr, m_nWidth, m_nHeight, 8, PNG_COLOR_TYPE_RGB_ALPHA,
                PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
        } 
        else
        {
            png_set_IHDR(png_ptr, info_ptr, m_nWidth, m_nHeight, 8, PNG_COLOR_TYPE_RGB,
                PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
        }
		//创建调色板S
        palette = (png_colorp)png_malloc(png_ptr, PNG_MAX_PALETTE_LENGTH * sizeof (png_color));
		//设置调色板
        png_set_PLTE(png_ptr, info_ptr, palette, PNG_MAX_PALETTE_LENGTH);
		//写入info_ptr
        png_write_info(png_ptr, info_ptr);
        png_set_packing(png_ptr);
		//申请临时存储m_pData中每一行像素数据地址的内存空间，将申请到的内存地址返回给row_pointers。
        row_pointers = (png_bytep *)malloc(m_nHeight * sizeof(png_bytep));
        if(row_pointers == NULL)
        {
            fclose(fp);
            png_destroy_write_struct(&png_ptr, &info_ptr);
            break;
        }
		//根据是否有ALPHA分别处理写入像素数据到文件中。 
        if (!m_bHasAlpha)
        {
			//如果没有ALPHA，只是RGB，这里将m_pData中数据，遍历每一行，将每一行的起始内存地址放入row_pointers指针数组中。
            for (int i = 0; i < (int)m_nHeight; i++)
            {
                row_pointers[i] = (png_bytep)m_pData + i * m_nWidth * 3;
            }
			//将row_pointers中指向的每一行数据写入文件。
            png_write_image(png_ptr, row_pointers);
			//释放内存 
            free(row_pointers);
            row_pointers = NULL;
        }
        else
        {
			//如果带ALPHA通道。对是否是RGB格式又进行分别处理。  
           //如果是RGB888格式
            if (bIsToRGB)
            {
				//创建临时的内存存放像素数据。每个像素3字节，分别存R,G,B值。
                unsigned char *pTempData = new unsigned char[m_nWidth * m_nHeight * 3];
                if (NULL == pTempData)
                {
                    fclose(fp);
                    png_destroy_write_struct(&png_ptr, &info_ptr);
                    break;
                }
				//双循环遍历每个像素，将R,G,B值保存到数组中。
                for (int i = 0; i < m_nHeight; ++i)
                {
                    for (int j = 0; j < m_nWidth; ++j)
                    {
                        pTempData[(i * m_nWidth + j) * 3] = m_pData[(i * m_nWidth + j) * 4];
                        pTempData[(i * m_nWidth + j) * 3 + 1] = m_pData[(i * m_nWidth + j) * 4 + 1];
                        pTempData[(i * m_nWidth + j) * 3 + 2] = m_pData[(i * m_nWidth + j) * 4 + 2];
                    }
                }
				//将数组中保存的每行像素的内存地址存入row_pointers数组中。
                for (int i = 0; i < (int)m_nHeight; i++)
                {
                    row_pointers[i] = (png_bytep)pTempData + i * m_nWidth * 3;
                }
				//将row_pointers中指向的每一行数据写入文件。 
                png_write_image(png_ptr, row_pointers);
				//释放内存
                free(row_pointers);
                row_pointers = NULL;

                CC_SAFE_DELETE_ARRAY(pTempData);
            } 
            else
            {
				//如果是RGBA8888格式  
               //将数组中保存的每行像素的内存地址存入row_pointers数组中。
                for (int i = 0; i < (int)m_nHeight; i++)
                {
                    row_pointers[i] = (png_bytep)m_pData + i * m_nWidth * 4;
                }
				//将row_pointers中指向的每一行数据写入文件。
                png_write_image(png_ptr, row_pointers);
				//释放内存 
                free(row_pointers);
                row_pointers = NULL;
            }
        }
		//结束写PNG文件
        png_write_end(png_ptr, info_ptr);
		//释放相应的信息结构
        png_free(png_ptr, palette);
        palette = NULL;

        png_destroy_write_struct(&png_ptr, &info_ptr);

        fclose(fp);

        bRet = true;
    } while (0);
    return bRet;
}

//将图像数据保存为JPG文件 
bool CCImage::_saveImageToJPG(const char * pszFilePath)
{
    bool bRet = false;
    do 
    {
		//参数有效性判断 
        CC_BREAK_IF(NULL == pszFilePath);
		//使用libjpg库要用到的相关结构。
        struct jpeg_compress_struct cinfo;
        struct jpeg_error_mgr jerr;
        FILE * outfile;                 /* target file */
        JSAMPROW row_pointer[1];        /* pointer to JSAMPLE row[s] */
        int     row_stride;          /* physical row width in image buffer */
		//初始化相关结构 
        cinfo.err = jpeg_std_error(&jerr);
        /* Now we can initialize the JPEG compression object. */
        jpeg_create_compress(&cinfo);
		//开始写入文件
        CC_BREAK_IF((outfile = fopen(pszFilePath, "wb")) == NULL);
        //写入JPG头文件基本信息
        jpeg_stdio_dest(&cinfo, outfile);
		//填充JPG图像的属性信息结构
        cinfo.image_width = m_nWidth;    /* image width and height, in pixels */
        cinfo.image_height = m_nHeight;
        cinfo.input_components = 3;       /* # of color components per pixel */
        cinfo.in_color_space = JCS_RGB;       /* colorspace of input image */
		//将信息结构来设置JPG图像
        jpeg_set_defaults(&cinfo);
		//开始进行数据压缩输出
        jpeg_start_compress(&cinfo, TRUE);
		//设置每行的字节长度 
        row_stride = m_nWidth * 3; /* JSAMPLEs per row in image_buffer */
		//跟据图像数据是否有ALPHA通道来进行分别处理
        if (m_bHasAlpha)
        {
			//创建内存来存放图像的像素数据。
            unsigned char *pTempData = new unsigned char[m_nWidth * m_nHeight * 3];
            if (NULL == pTempData)
            {
                jpeg_finish_compress(&cinfo);
                jpeg_destroy_compress(&cinfo);
                fclose(outfile);
                break;
            }
			//双循环遍历每一个图像像素的数据，取R,G,B值存放到临时申请的内存地址中,A值弃之不取。
            for (int i = 0; i < m_nHeight; ++i)
            {
                for (int j = 0; j < m_nWidth; ++j)

                {
					//因图像数据有A通道，所以找相应的像素地址时会由像素索引x4。 
                    pTempData[(i * m_nWidth + j) * 3] = m_pData[(i * m_nWidth + j) * 4];
                    pTempData[(i * m_nWidth + j) * 3 + 1] = m_pData[(i * m_nWidth + j) * 4 + 1];
                    pTempData[(i * m_nWidth + j) * 3 + 2] = m_pData[(i * m_nWidth + j) * 4 + 2];
                }
            }
			//将扫描行的数据写入JPG文件
            while (cinfo.next_scanline < cinfo.image_height) {
                row_pointer[0] = & pTempData[cinfo.next_scanline * row_stride];
                (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
            }

            CC_SAFE_DELETE_ARRAY(pTempData);
        } 
        else
        {
			//将扫描行的数据写入JPG文件
            while (cinfo.next_scanline < cinfo.image_height) {
                row_pointer[0] = & m_pData[cinfo.next_scanline * row_stride];
                (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
            }
        }
		//结束数据压缩，关闭文件并释放相应信息结构。
        jpeg_finish_compress(&cinfo);
        fclose(outfile);
        jpeg_destroy_compress(&cinfo);
        
        bRet = true;
    } while (0);
    return bRet;
}

NS_CC_END

