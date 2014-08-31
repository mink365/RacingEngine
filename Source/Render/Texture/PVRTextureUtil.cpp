#include "PVRTextureUtil.h"

#include "FileSystem/FileSystem.h"
#include "Util/LogUtil.h"

#include "opengl.h"
#include <cstring>

#include "PVRTTexture.h"
#include "PVRTDecompress.h"

//#include "GLES2/gl2ext.h"
#include "gl2extimg.h"

#define CPVRTgles2Ext::IsGLExtensionSupported(X) true
#define PVRTErrorOutputDebug LOG_E
#define PVR_FAIL false
#define PVR_SUCCESS true

#define FREE(X)		{ if(X) { free(X); (X) = 0; } }

namespace re {

PVRTextureUtil::PVRTextureUtil()
{
}

/*!***********************************************************************
    @Function:		PVRTGetOGLESTextureFormat
    @Input:			sTextureHeader
    @Modified:		internalformat
    @Modified:		format
    @Modified:		type
    @Description:	Gets the OpenGLES equivalent values of internal format,
                    format and type for this texture header. This will return
                    any supported OpenGLES texture values, it is up to the user
                    to decide if these are valid for their current platform.
*************************************************************************/
static const void PVRTGetOGLES2TextureFormat(const PVRTextureHeaderV3& sTextureHeader, PVRTuint32& internalformat, PVRTuint32& format, PVRTuint32& type)
{
    PVRTuint64 PixelFormat = sTextureHeader.u64PixelFormat;
    EPVRTVariableType ChannelType = (EPVRTVariableType)sTextureHeader.u32ChannelType;
    EPVRTColourSpace ColourSpace = (EPVRTColourSpace)sTextureHeader.u32ColourSpace;

    //Initialisation. Any invalid formats will return 0 always.
    format = 0;
    type = 0;
    internalformat=0;

    //Get the last 32 bits of the pixel format.
    PVRTuint64 PixelFormatPartHigh = PixelFormat&PVRTEX_PFHIGHMASK;

    //Check for a compressed format (The first 8 bytes will be 0, so the whole thing will be equal to the last 32 bits).
    if (PixelFormatPartHigh==0)
    {
        //Format and type == 0 for compressed textures.
        switch (PixelFormat)
        {
        case ePVRTPF_PVRTCI_2bpp_RGB:
            {
                if (ColourSpace == ePVRTCSpacesRGB)
                {
                    internalformat=GL_COMPRESSED_SRGB_PVRTC_2BPPV1_EXT;
                }
                else
                {
                    internalformat=GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG;
                }
                return;
            }
        case ePVRTPF_PVRTCI_2bpp_RGBA:
            {
                if (ColourSpace == ePVRTCSpacesRGB)
                {
                    internalformat=GL_COMPRESSED_SRGB_ALPHA_PVRTC_2BPPV1_EXT;
                }
                else
                {
                    internalformat=GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;
                }
                return;
            }
        case ePVRTPF_PVRTCI_4bpp_RGB:
            {
                if (ColourSpace == ePVRTCSpacesRGB)
                {
                    internalformat=GL_COMPRESSED_SRGB_PVRTC_4BPPV1_EXT;
                }
                else
                {
                    internalformat=GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG;
                }
                return;
            }
        case ePVRTPF_PVRTCI_4bpp_RGBA:
            {
                if (ColourSpace == ePVRTCSpacesRGB)
                {
                    internalformat=GL_COMPRESSED_SRGB_ALPHA_PVRTC_4BPPV1_EXT;
                }
                else
                {
                    internalformat=GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
                }
                return;
            }
#ifndef TARGET_OS_IPHONE
        case ePVRTPF_PVRTCII_2bpp:
            {
                if (ColourSpace == ePVRTCSpacesRGB)
                {
                    internalformat=GL_COMPRESSED_SRGB_ALPHA_PVRTC_2BPPV2_IMG;
                }
                else
                {
                    internalformat=GL_COMPRESSED_RGBA_PVRTC_2BPPV2_IMG;
                }
                return;
            }
        case ePVRTPF_PVRTCII_4bpp:
            {
                if (ColourSpace == ePVRTCSpacesRGB)
                {
                    internalformat=GL_COMPRESSED_SRGB_ALPHA_PVRTC_4BPPV2_IMG;
                }
                else
                {
                    internalformat=GL_COMPRESSED_RGBA_PVRTC_4BPPV2_IMG;
                }
                return;
            }
        case ePVRTPF_ETC1:
            {
                internalformat=GL_ETC1_RGB8_OES;
                return;
            }
#endif
        default:
            return;
        }
    }
    else
    {
        switch (ChannelType)
        {
        case ePVRTVarTypeFloat:
            {
                switch (PixelFormat)
                {
                    //HALF_FLOAT_OES
                case PVRTGENPIXELID4('r','g','b','a',16,16,16,16):
                    {
                        type=GL_HALF_FLOAT_OES;
                        format = GL_RGBA;
                        internalformat=GL_RGBA;
                        return;
                    }
                case PVRTGENPIXELID3('r','g','b',16,16,16):
                    {
                        type=GL_HALF_FLOAT_OES;
                        format = GL_RGB;
                        internalformat=GL_RGB;
                        return;
                    }
                case PVRTGENPIXELID2('l','a',16,16):
                    {
                        type=GL_HALF_FLOAT_OES;
                        format = GL_LUMINANCE_ALPHA;
                        internalformat=GL_LUMINANCE_ALPHA;
                        return;
                    }
                case PVRTGENPIXELID1('l',16):
                    {
                        type=GL_HALF_FLOAT_OES;
                        format = GL_LUMINANCE;
                        internalformat=GL_LUMINANCE;
                        return;
                    }
                case PVRTGENPIXELID1('a',16):
                    {
                        type=GL_HALF_FLOAT_OES;
                        format = GL_ALPHA;
                        internalformat=GL_ALPHA;
                        return;
                    }
                    //FLOAT (OES)
                case PVRTGENPIXELID4('r','g','b','a',32,32,32,32):
                    {
                        type=GL_FLOAT;
                        format = GL_RGBA;
                        internalformat=GL_RGBA;
                        return;
                    }
                case PVRTGENPIXELID3('r','g','b',32,32,32):
                    {
                        type=GL_FLOAT;
                        format = GL_RGB;
                        internalformat=GL_RGB;
                        return;
                    }
                case PVRTGENPIXELID2('l','a',32,32):
                    {
                        type=GL_FLOAT;
                        format = GL_LUMINANCE_ALPHA;
                        internalformat=GL_LUMINANCE_ALPHA;
                        return;
                    }
                case PVRTGENPIXELID1('l',32):
                    {
                        type=GL_FLOAT;
                        format = GL_LUMINANCE;
                        internalformat=GL_LUMINANCE;
                        return;
                    }
                case PVRTGENPIXELID1('a',32):
                    {
                        type=GL_FLOAT;
                        format = GL_ALPHA;
                        internalformat=GL_ALPHA;
                        return;
                    }
                }
                break;
            }
        case ePVRTVarTypeUnsignedByteNorm:
            {
                type = GL_UNSIGNED_BYTE;
                switch (PixelFormat)
                {
                case PVRTGENPIXELID4('r','g','b','a',8,8,8,8):
                    {
                        format = internalformat = GL_RGBA;
                        return;
                    }
                case PVRTGENPIXELID3('r','g','b',8,8,8):
                    {
                        format = internalformat = GL_RGB;
                        return;
                    }
                case PVRTGENPIXELID2('l','a',8,8):
                    {
                        format = internalformat = GL_LUMINANCE_ALPHA;
                        return;
                    }
                case PVRTGENPIXELID1('l',8):
                    {
                        format = internalformat = GL_LUMINANCE;
                        return;
                    }
                case PVRTGENPIXELID1('a',8):
                    {
                        format = internalformat = GL_ALPHA;
                        return;
                    }
                case PVRTGENPIXELID4('b','g','r','a',8,8,8,8):
                    {
                        format = internalformat = GL_BGRA;
                        return;
                    }
                }
                break;
            }
        case ePVRTVarTypeUnsignedShortNorm:
            {
                switch (PixelFormat)
                {
                case PVRTGENPIXELID4('r','g','b','a',4,4,4,4):
                    {
                        type = GL_UNSIGNED_SHORT_4_4_4_4;
                        format = internalformat = GL_RGBA;
                        return;
                    }
                case PVRTGENPIXELID4('r','g','b','a',5,5,5,1):
                    {
                        type = GL_UNSIGNED_SHORT_5_5_5_1;
                        format = internalformat = GL_RGBA;
                        return;
                    }
                case PVRTGENPIXELID3('r','g','b',5,6,5):
                    {
                        type = GL_UNSIGNED_SHORT_5_6_5;
                        format = internalformat = GL_RGB;
                        return;
                    }
                }
                break;
            }
        default:
            return;
        }
    }
}

bool PVRTTextureLoadFromPointer(	const void* pointer,
                                        GLuint *const texName,
                                        const void *psTextureHeader,
                                        bool bAllowDecompress,
                                        const unsigned int nLoadFromLevel,
                                        const void * const texPtr,
                                        CPVRTMap<unsigned int, CPVRTMap<unsigned int, MetaDataBlock> > *pMetaData)
{
    //Compression bools
    bool bIsCompressedFormatSupported=false;
    bool bIsCompressedFormat=false;

    //Texture setup
    PVRTextureHeaderV3 sTextureHeader;
    PVRTuint8* pTextureData=NULL;

    //Just in case header and pointer for decompression.
    PVRTextureHeaderV3 sTextureHeaderDecomp;
    void* pDecompressedData=NULL;

    //Check if it's an old header format
    _ASSERT((*(PVRTuint32*)pointer)==PVRTEX3_IDENT);

    {
        //Get the header from the main pointer.
        sTextureHeader=*(PVRTextureHeaderV3*)pointer;

        //Get the texture data.
        pTextureData = texPtr? (PVRTuint8*)texPtr:(PVRTuint8*)pointer+PVRTEX3_HEADERSIZE+sTextureHeader.u32MetaDataSize;

        if (pMetaData)
        {
            //Read in all the meta data.
            PVRTuint32 metaDataSize=0;
            while (metaDataSize<sTextureHeader.u32MetaDataSize)
            {
                //Read the DevFourCC and advance the pointer offset.
                PVRTuint32 DevFourCC=*(PVRTuint32*)((PVRTuint8*)pointer+PVRTEX3_HEADERSIZE+metaDataSize);
                metaDataSize+=sizeof(DevFourCC);

                //Read the Key and advance the pointer offset.
                PVRTuint32 u32Key=*(PVRTuint32*)((PVRTuint8*)pointer+PVRTEX3_HEADERSIZE+metaDataSize);
                metaDataSize+=sizeof(u32Key);

                //Read the DataSize and advance the pointer offset.
                PVRTuint32 u32DataSize = *(PVRTuint32*)((PVRTuint8*)pointer+PVRTEX3_HEADERSIZE+metaDataSize);
                metaDataSize+=sizeof(u32DataSize);

                //Get the current meta data.
                MetaDataBlock& currentMetaData = (*pMetaData)[DevFourCC][u32Key];

                //Assign the values to the meta data.
                currentMetaData.DevFOURCC=DevFourCC;
                currentMetaData.u32Key=u32Key;
                currentMetaData.u32DataSize=u32DataSize;

                //Check for data, if there is any, read it into the meta data.
                if(u32DataSize > 0)
                {
                    //Allocate memory.
                    currentMetaData.Data = new PVRTuint8[u32DataSize];

                    //Copy the data.
                    memcpy(currentMetaData.Data, ((PVRTuint8*)pointer+PVRTEX3_HEADERSIZE+metaDataSize), u32DataSize);

                    //Advance the meta data size.
                    metaDataSize+=u32DataSize;
                }
            }
        }
    }

    //Return the PVRTextureHeader.
    if (psTextureHeader)
    {
        *(PVRTextureHeaderV3*)psTextureHeader=sTextureHeader;
    }

    //Setup GL Texture format values.
    GLenum eTextureFormat = 0;
    GLenum eTextureInternalFormat = 0;	// often this is the same as textureFormat, but not for BGRA8888 on iOS, for instance
    GLenum eTextureType = 0;

    //Get the OGLES format values.
    PVRTGetOGLES2TextureFormat(sTextureHeader,eTextureInternalFormat,eTextureFormat,eTextureType);

//    //Check supported texture formats.
//    bool bIsPVRTCSupported = CPVRTgles2Ext::IsGLExtensionSupported("GL_IMG_texture_compression_pvrtc");
//    bool bIsPVRTC2Supported = CPVRTgles2Ext::IsGLExtensionSupported("GL_IMG_texture_compression_pvrtc2");
//#ifndef TARGET_OS_IPHONE
//    bool bIsBGRA8888Supported  = CPVRTgles2Ext::IsGLExtensionSupported("GL_IMG_texture_format_BGRA8888");
//#else
//    bool bIsBGRA8888Supported  = CPVRTgles2Ext::IsGLExtensionSupported("GL_APPLE_texture_format_BGRA8888");
//#endif
//    bool bIsFloat16Supported = CPVRTgles2Ext::IsGLExtensionSupported("GL_OES_texture_half_float");
//    bool bIsFloat32Supported = CPVRTgles2Ext::IsGLExtensionSupported("GL_OES_texture_float");
//#ifndef TARGET_OS_IPHONE
//    bool bIsETCSupported = CPVRTgles2Ext::IsGLExtensionSupported("GL_OES_compressed_ETC1_RGB8_texture");
//#endif
    bool bIsPVRTCSupported = true;
    bool bIsPVRTC2Supported = true;
    bool bIsFloat16Supported = true;
    bool bIsFloat32Supported = true;
    bool bIsBGRA8888Supported = true;
    bool bIsETCSupported = true;

    //Check for compressed formats
    if (eTextureFormat==0 && eTextureType==0 && eTextureInternalFormat!=0)
    {
        if (eTextureInternalFormat>=GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG && eTextureInternalFormat<=GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG)
        {
            //Check for PVRTCI support.
            if(bIsPVRTCSupported)
            {
                bIsCompressedFormatSupported = bIsCompressedFormat = true;
            }
            else
            {
                //Try to decompress the texture.
                if(bAllowDecompress)
                {
                    //Output a warning.
                    PVRTErrorOutputDebug("PVRTTextureLoadFromPointer warning: PVRTC not supported. Converting to RGBA8888 instead.\n");

                    //Modify boolean values.
                    bIsCompressedFormatSupported = false;
                    bIsCompressedFormat = true;

                    //Check if it's 2bpp.
                    bool bIs2bppPVRTC = (eTextureInternalFormat==GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG || eTextureInternalFormat==GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG);

                    //Change texture format.
                    eTextureFormat = eTextureInternalFormat = GL_RGBA;
                    eTextureType = GL_UNSIGNED_BYTE;

                    //Create a near-identical texture header for the decompressed header.
                    sTextureHeaderDecomp = sTextureHeader;
                    sTextureHeaderDecomp.u32ChannelType=ePVRTVarTypeUnsignedByteNorm;
                    sTextureHeaderDecomp.u32ColourSpace=ePVRTCSpacelRGB;
                    sTextureHeaderDecomp.u64PixelFormat=PVRTGENPIXELID4('r','g','b','a',8,8,8,8);

                    //Allocate enough memory for the decompressed data. OGLES2, so only decompress one surface, but all faces.
                    pDecompressedData = malloc(PVRTGetTextureDataSize(sTextureHeaderDecomp, PVRTEX_ALLMIPLEVELS, false, true) );

                    //Check the malloc.
                    if (!pDecompressedData)
                    {
                        PVRTErrorOutputDebug("PVRTTextureLoadFromPointer error: Unable to allocate memory to decompress texture.\n");
                        return PVR_FAIL;
                    }

                    //Get the dimensions for the current MIP level.
                    PVRTuint32 uiMIPWidth = sTextureHeaderDecomp.u32Width>>nLoadFromLevel;
                    PVRTuint32 uiMIPHeight = sTextureHeaderDecomp.u32Height>>nLoadFromLevel;

                    //Setup temporary variables.
                    PVRTuint8* pTempDecompData = (PVRTuint8*)pDecompressedData;
                    PVRTuint8* pTempCompData = (PVRTuint8*)pTextureData;

                    {
                        //Decompress all the MIP levels.
                        for (PVRTuint32 uiMIPMap=nLoadFromLevel;uiMIPMap<sTextureHeader.u32MIPMapCount;++uiMIPMap)
                        {
                            //Get the face offset. Varies per MIP level.
                            PVRTuint32 decompressedFaceOffset = PVRTGetTextureDataSize(sTextureHeaderDecomp, uiMIPMap, false, false);
                            PVRTuint32 compressedFaceOffset = PVRTGetTextureDataSize(sTextureHeader, uiMIPMap, false, false);

                            for (PVRTuint32 uiFace=0;uiFace<sTextureHeader.u32NumFaces;++uiFace)
                            {
                                //Decompress the texture data.
                                PVRTDecompressPVRTC(pTempCompData,bIs2bppPVRTC?1:0,uiMIPWidth,uiMIPHeight,pTempDecompData);

                                //Move forward through the pointers.
                                pTempDecompData+=decompressedFaceOffset;
                                pTempCompData+=compressedFaceOffset;
                            }

                            //Work out the current MIP dimensions.
                            uiMIPWidth=PVRT_MAX(1,uiMIPWidth>>1);
                            uiMIPHeight=PVRT_MAX(1,uiMIPHeight>>1);
                        }
                    }
                }
                else
                {
                    PVRTErrorOutputDebug("PVRTTextureLoadFromPointer error: PVRTC not supported.\n");
                    return PVR_FAIL;
                }
            }
        }
#ifndef TARGET_OS_IPHONE //TODO
        else if (eTextureInternalFormat==GL_COMPRESSED_RGBA_PVRTC_4BPPV2_IMG || eTextureInternalFormat==GL_COMPRESSED_RGBA_PVRTC_2BPPV2_IMG)
        {
            //Check for PVRTCI support.
            if(bIsPVRTC2Supported)
            {
                bIsCompressedFormatSupported = bIsCompressedFormat = true;
            }
            else
            {
                PVRTErrorOutputDebug("PVRTTextureLoadFromPointer error: PVRTC not supported.\n");
                return PVR_FAIL;
            }
        }
        else if (eTextureInternalFormat==GL_ETC1_RGB8_OES)
        {
            if(bIsETCSupported)
            {
                bIsCompressedFormatSupported = bIsCompressedFormat = true;
            }
            else
            {
                if(bAllowDecompress)
                {
                    //Output a warning.
                    PVRTErrorOutputDebug("PVRTTextureLoadFromPointer warning: ETC not supported. Converting to RGBA8888 instead.\n");

                    //Modify boolean values.
                    bIsCompressedFormatSupported = false;
                    bIsCompressedFormat = true;

                    //Change texture format.
                    eTextureFormat = eTextureInternalFormat = GL_RGBA;
                    eTextureType = GL_UNSIGNED_BYTE;

                    //Create a near-identical texture header for the decompressed header.
                    sTextureHeaderDecomp = sTextureHeader;
                    sTextureHeaderDecomp.u32ChannelType=ePVRTVarTypeUnsignedByteNorm;
                    sTextureHeaderDecomp.u32ColourSpace=ePVRTCSpacelRGB;
                    sTextureHeaderDecomp.u64PixelFormat=PVRTGENPIXELID4('r','g','b','a',8,8,8,8);

                    //Allocate enough memory for the decompressed data. OGLES1, so only decompress one surface/face.
                    pDecompressedData = malloc(PVRTGetTextureDataSize(sTextureHeaderDecomp, PVRTEX_ALLMIPLEVELS, false, true) );

                    //Check the malloc.
                    if (!pDecompressedData)
                    {
                        PVRTErrorOutputDebug("PVRTTextureLoadFromPointer error: Unable to allocate memory to decompress texture.\n");
                        return PVR_FAIL;
                    }

                    //Get the dimensions for the current MIP level.
                    PVRTuint32 uiMIPWidth = sTextureHeaderDecomp.u32Width>>nLoadFromLevel;
                    PVRTuint32 uiMIPHeight = sTextureHeaderDecomp.u32Height>>nLoadFromLevel;

                    //Setup temporary variables.
                    PVRTuint8* pTempDecompData = (PVRTuint8*)pDecompressedData;
                    PVRTuint8* pTempCompData = (PVRTuint8*)pTextureData;

                    {
                        //Decompress all the MIP levels.
                        for (PVRTuint32 uiMIPMap=nLoadFromLevel;uiMIPMap<sTextureHeader.u32MIPMapCount;++uiMIPMap)
                        {
                            //Get the face offset. Varies per MIP level.
                            PVRTuint32 decompressedFaceOffset = PVRTGetTextureDataSize(sTextureHeaderDecomp, uiMIPMap, false, false);
                            PVRTuint32 compressedFaceOffset = PVRTGetTextureDataSize(sTextureHeader, uiMIPMap, false, false);

                            for (PVRTuint32 uiFace=0;uiFace<sTextureHeader.u32NumFaces;++uiFace)
                            {
                                //Decompress the texture data.
                                PVRTDecompressETC(pTempCompData,uiMIPWidth,uiMIPHeight,pTempDecompData,0);

                                //Move forward through the pointers.
                                pTempDecompData+=decompressedFaceOffset;
                                pTempCompData+=compressedFaceOffset;
                            }

                            //Work out the current MIP dimensions.
                            uiMIPWidth=PVRT_MAX(1,uiMIPWidth>>1);
                            uiMIPHeight=PVRT_MAX(1,uiMIPHeight>>1);
                        }
                    }
                }
                else
                {
                    PVRTErrorOutputDebug("PVRTTextureLoadFromPointer error: ETC not supported.\n");
                    return PVR_FAIL;
                }
            }
        }
#endif
    }

    //Check for BGRA support.
    if(eTextureFormat==GL_BGRA)
    {
#ifdef TARGET_OS_IPHONE
        eTextureInternalFormat = GL_RGBA;
#endif
        if(!bIsBGRA8888Supported)
        {
#ifdef TARGET_OS_IPHONE
            PVRTErrorOutputDebug("PVRTTextureLoadFromPointer failed: Unable to load GL_BGRA texture as extension GL_APPLE_texture_format_BGRA8888 is unsupported.\n");
#else
            PVRTErrorOutputDebug("PVRTTextureLoadFromPointer failed: Unable to load GL_BGRA texture as extension GL_IMG_texture_format_BGRA8888 is unsupported.\n");
#endif
            return PVR_FAIL;
        }
    }

    //Check for floating point textures
    if (eTextureType==GL_HALF_FLOAT_OES)
    {
        if(!bIsFloat16Supported)
        {
            PVRTErrorOutputDebug("PVRTTextureLoadFromPointer failed: Unable to load GL_HALF_FLOAT_OES texture as extension GL_OES_texture_half_float is unsupported.\n");
        }
    }
    if (eTextureType==GL_FLOAT)
    {
        if(!bIsFloat32Supported)
        {
            PVRTErrorOutputDebug("PVRTTextureLoadFromPointer failed: Unable to load GL_FLOAT texture as extension GL_OES_texture_float is unsupported.\n");
        }
    }

    //Deal with unsupported texture formats
    if (eTextureInternalFormat==0)
    {
        PVRTErrorOutputDebug("PVRTTextureLoadFromPointer failed: pixel type not supported.\n");
        return PVR_FAIL;
    }

    //PVR files are never row aligned.
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);

    //Generate a texture
    glGenTextures(1, texName);

    //Initialise a texture target.
    GLint eTarget=GL_TEXTURE_2D;

    if(sTextureHeader.u32NumFaces>1)
    {
        eTarget=GL_TEXTURE_CUBE_MAP;
    }

    //Check if this is a texture array.
    if(sTextureHeader.u32NumSurfaces>1)
    {
        //Not supported in OpenGLES 2.0
        PVRTErrorOutputDebug("PVRTTextureLoadFromPointer failed: Texture arrays are not available in OGLES2.0.\n");
        return PVR_FAIL;
    }

    //Bind the 2D texture
    glBindTexture(eTarget, *texName);

    if(glGetError())
    {
        PVRTErrorOutputDebug("PVRTTextureLoadFromPointer failed: glBindTexture() failed.\n");
        return PVR_FAIL;
    }

    //Initialise the current MIP size.
    PVRTuint32 uiCurrentMIPSize=0;

    //Loop through the faces
    //Check if this is a cube map.
    if(sTextureHeader.u32NumFaces>1)
    {
        eTarget=GL_TEXTURE_CUBE_MAP_POSITIVE_X;
    }

    //Initialise the width/height
    PVRTuint32 u32MIPWidth = sTextureHeader.u32Width;
    PVRTuint32 u32MIPHeight = sTextureHeader.u32Height;

    //Temporary data to save on if statements within the load loops.
    PVRTuint8* pTempData=NULL;
    PVRTextureHeaderV3 *psTempHeader=NULL;
    if (bIsCompressedFormat && !bIsCompressedFormatSupported)
    {
        pTempData=(PVRTuint8*)pDecompressedData;
        psTempHeader=&sTextureHeaderDecomp;
    }
    else
    {
        pTempData=pTextureData;
        psTempHeader=&sTextureHeader;
    }

    //Loop through all MIP levels.
    {
        for (PVRTuint32 uiMIPLevel=0; uiMIPLevel<psTempHeader->u32MIPMapCount; ++uiMIPLevel)
        {
            //Get the current MIP size.
            uiCurrentMIPSize=PVRTGetTextureDataSize(*psTempHeader,uiMIPLevel,false,false);

            GLint eTextureTarget=eTarget;

            for (PVRTuint32 uiFace=0; uiFace<psTempHeader->u32NumFaces; ++uiFace)
            {
                if (uiMIPLevel>=nLoadFromLevel)
                {
                    //Upload the texture
                    if (bIsCompressedFormat && bIsCompressedFormatSupported)
                    {
                        glCompressedTexImage2D(eTextureTarget,uiMIPLevel-nLoadFromLevel,eTextureInternalFormat,u32MIPWidth, u32MIPHeight, 0, uiCurrentMIPSize, pTempData);
                    }
                    else
                    {
                        glTexImage2D(eTextureTarget,uiMIPLevel-nLoadFromLevel,eTextureInternalFormat, u32MIPWidth, u32MIPHeight, 0, eTextureFormat, eTextureType, pTempData);
                    }
                }
                pTempData+=uiCurrentMIPSize;
                eTextureTarget++;
            }

            //Reduce the MIP Size.
            u32MIPWidth=PVRT_MAX(1,u32MIPWidth>>1);
            u32MIPHeight=PVRT_MAX(1,u32MIPHeight>>1);

            //Error check
            if(glGetError())
            {
                FREE(pDecompressedData);
                PVRTErrorOutputDebug("PVRTTextureLoadFromPointer failed: glTexImage2D() failed.\n");
                return PVR_FAIL;
            }
        }
    }

    FREE(pDecompressedData);

    if (eTarget!=GL_TEXTURE_2D)
    {
        eTarget=GL_TEXTURE_CUBE_MAP;
    }

    //Error check
    if(glGetError())
    {
        PVRTErrorOutputDebug("PVRTTextureLoadFromPointer failed: glTexImage2D() failed.\n");
        return PVR_FAIL;
    }

    //Set Minification and Magnification filters according to whether MIP maps are present.
    if(eTextureType==GL_FLOAT || eTextureType==GL_HALF_FLOAT_OES)
    {
        if(sTextureHeader.u32MIPMapCount==1)
        {	// Texture filter modes are limited to these for float textures
            glTexParameteri(eTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(eTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        }
        else
        {
            glTexParameteri(eTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
            glTexParameteri(eTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        }
    }
    else
    {
        if(sTextureHeader.u32MIPMapCount==1)
        {
            glTexParameteri(eTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(eTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
        else
        {
            glTexParameteri(eTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
            glTexParameteri(eTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
    }

    if(	(sTextureHeader.u32Width & (sTextureHeader.u32Width - 1)) | (sTextureHeader.u32Height & (sTextureHeader.u32Height - 1)))
    {
        /*
            NPOT textures requires the wrap mode to be set explicitly to
            GL_CLAMP_TO_EDGE or the texture will be inconsistent.
        */
        glTexParameteri(eTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(eTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    else
    {
        glTexParameteri(eTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(eTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    //Error check
    if(glGetError())
    {
        PVRTErrorOutputDebug("PVRTTextureLoadFromPointer failed: glTexParameter() failed.\n");
        return PVR_FAIL;
    }

    return PVR_SUCCESS;
}

bool PVRTTextureLoadFromPVR(	const char * const filename,
                                    GLuint * const texName,
                                    const void *psTextureHeader,
                                    bool bAllowDecompress,
                                    const unsigned int nLoadFromLevel,
                                    CPVRTMap<unsigned int, CPVRTMap<unsigned int, MetaDataBlock> > *pMetaData)
{
    //Attempt to open file.
    FilePtr texFile = FileSystem::getInstance().openFile(filename);
    Buffer::ptr buf = texFile->read();

//    //Check file opened successfully.
//    if (!TexFile.IsOpen())
//    {
//        return false;
//    }

    //Header size.
    PVRTuint32 u32HeaderSize=0;

    //Boolean whether to byte swap the texture data or not.
    bool bSwapDataEndianness=false;

    //Texture header to check against.
    PVRTextureHeaderV3 sTextureHeader;

    //The channel type for endian swapping.
    EPVRTVariableType u32CurrentChannelType=ePVRTVarTypeUnsignedByte;

    //Check the first word of the file and see if it's equal to the current identifier (or reverse identifier)
    if(*(PVRTuint32*)buf->getData()!=PVRTEX_CURR_IDENT
            && *(PVRTuint32*)buf->getData()!=PVRTEX_CURR_IDENT_REV)
    {
        //Swap the header bytes if necessary.
        if(!PVRTIsLittleEndian())
        {
            bSwapDataEndianness=true;
            PVRTuint32 u32HeaderSize=PVRTByteSwap32(*(PVRTuint32*)buf->getData());

            for (PVRTuint32 i=0; i<u32HeaderSize; ++i)
            {
                PVRTByteSwap( (PVRTuint8*)( ( (PVRTuint32*)buf->getData() )+i),sizeof(PVRTuint32) );
            }
        }

        //Get a pointer to the header.
        PVR_Texture_Header* sLegacyTextureHeader=(PVR_Texture_Header*)buf->getData();

        //Set the header size.
        u32HeaderSize=sLegacyTextureHeader->dwHeaderSize;

        //We only really need the channel type.
        PVRTuint64 tempFormat;
        EPVRTColourSpace tempColourSpace;
        bool tempIsPreMult;

        //Map the enum to get the channel type.
        PVRTMapLegacyTextureEnumToNewFormat( (PVRTPixelType)( sLegacyTextureHeader->dwpfFlags&0xff),tempFormat,tempColourSpace, u32CurrentChannelType, tempIsPreMult);
    }
    // If the header file has a reverse identifier, then we need to swap endianness
    else if(*(PVRTuint32*)buf->getData()==PVRTEX_CURR_IDENT_REV)
    {
        //Setup the texture header
        sTextureHeader=*(PVRTextureHeaderV3*)buf->getData();

        bSwapDataEndianness=true;
        PVRTextureHeaderV3* pTextureHeader=(PVRTextureHeaderV3*)buf->getData();

        pTextureHeader->u32ChannelType=PVRTByteSwap32(pTextureHeader->u32ChannelType);
        pTextureHeader->u32ColourSpace=PVRTByteSwap32(pTextureHeader->u32ColourSpace);
        pTextureHeader->u32Depth=PVRTByteSwap32(pTextureHeader->u32Depth);
        pTextureHeader->u32Flags=PVRTByteSwap32(pTextureHeader->u32Flags);
        pTextureHeader->u32Height=PVRTByteSwap32(pTextureHeader->u32Height);
        pTextureHeader->u32MetaDataSize=PVRTByteSwap32(pTextureHeader->u32MetaDataSize);
        pTextureHeader->u32MIPMapCount=PVRTByteSwap32(pTextureHeader->u32MIPMapCount);
        pTextureHeader->u32NumFaces=PVRTByteSwap32(pTextureHeader->u32NumFaces);
        pTextureHeader->u32NumSurfaces=PVRTByteSwap32(pTextureHeader->u32NumSurfaces);
        pTextureHeader->u32Version=PVRTByteSwap32(pTextureHeader->u32Version);
        pTextureHeader->u32Width=PVRTByteSwap32(pTextureHeader->u32Width);
        PVRTByteSwap((PVRTuint8*)&pTextureHeader->u64PixelFormat,sizeof(PVRTuint64));

        //Channel type.
        u32CurrentChannelType=(EPVRTVariableType)pTextureHeader->u32ChannelType;

        //Header size.
        u32HeaderSize=PVRTEX3_HEADERSIZE+sTextureHeader.u32MetaDataSize;
    }
    else
    {
        //Header size.
        u32HeaderSize=PVRTEX3_HEADERSIZE+sTextureHeader.u32MetaDataSize;
    }

    // Convert the data if needed
    if(bSwapDataEndianness)
    {
        //Get the size of the variables types.
        PVRTuint32 ui32VariableSize=0;
        switch(u32CurrentChannelType)
        {
        case ePVRTVarTypeFloat:
        case ePVRTVarTypeUnsignedInteger:
        case ePVRTVarTypeUnsignedIntegerNorm:
        case ePVRTVarTypeSignedInteger:
        case ePVRTVarTypeSignedIntegerNorm:
            {
                ui32VariableSize=4;
                break;
            }
        case ePVRTVarTypeUnsignedShort:
        case ePVRTVarTypeUnsignedShortNorm:
        case ePVRTVarTypeSignedShort:
        case ePVRTVarTypeSignedShortNorm:
            {
                ui32VariableSize=2;
                break;
            }
        case ePVRTVarTypeUnsignedByte:
        case ePVRTVarTypeUnsignedByteNorm:
        case ePVRTVarTypeSignedByte:
        case ePVRTVarTypeSignedByteNorm:
            {
                ui32VariableSize=1;
                break;
            }
        default:
            break;
        }

        //If the size of the variable type is greater than 1, then we need to byte swap.
        if (ui32VariableSize>1)
        {
            //Get the texture data.
            PVRTuint8* pu8OrigData = ( (PVRTuint8*)buf->getData() + u32HeaderSize);

            //Get the size of the texture data.
            PVRTuint32 ui32TextureDataSize = PVRTGetTextureDataSize(sTextureHeader);

            //Loop through and byte swap all the data. It's swapped in place so no need to do anything special.
            for(PVRTuint32 i = 0; i < ui32TextureDataSize; i+=ui32VariableSize)
            {
                PVRTByteSwap(pu8OrigData+i,ui32VariableSize);
            }
        }
    }

    return PVRTTextureLoadFromPointer(buf->getData(), texName, psTextureHeader, bAllowDecompress, nLoadFromLevel,NULL,pMetaData);
}

} // namespace re
