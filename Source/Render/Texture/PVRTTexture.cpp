/******************************************************************************

 @File         PVRTTexture.cpp

 @Title        PVRTTexture

 @Version      

 @Copyright    Copyright (c) Imagination Technologies Limited.

 @Platform     ANSI compatible

 @Description  Texture loading.

******************************************************************************/
#include <string.h>
#include <stdlib.h>

#include "PVRTTexture.h"
//#include "PVRTMap.h"

/*****************************************************************************
** Functions
*****************************************************************************/
/*!***************************************************************************
@Function		ReadFromPtr
@Input			pDataCursor		The data to read
@Description	Reads from a pointer of memory in to the meta data block.
*****************************************************************************/
bool MetaDataBlock::ReadFromPtr(const unsigned char** pDataCursor)
{
	memcpy(&DevFOURCC,	 *pDataCursor, sizeof(PVRTuint32));		*pDataCursor += sizeof(PVRTuint32);
	memcpy(&u32Key,		 *pDataCursor, sizeof(PVRTuint32));		*pDataCursor += sizeof(PVRTuint32);
	memcpy(&u32DataSize, *pDataCursor, sizeof(PVRTuint32));		*pDataCursor += sizeof(PVRTuint32);
	if(u32DataSize > 0)
	{
		Data = new PVRTuint8[u32DataSize];
		memcpy(Data, *pDataCursor, u32DataSize);
		*pDataCursor += u32DataSize;
	}

	return true;
}

/*!***************************************************************************
@Function		PVRTTextureLoadTiled
@Modified		pDst			Texture to place the tiled data
@Input			nWidthDst		Width of destination texture
@Input			nHeightDst		Height of destination texture
@Input			pSrc			Texture to tile
@Input			nWidthSrc		Width of source texture
@Input			nHeightSrc		Height of source texture
@Input 			nElementSize	Bytes per pixel
@Input			bTwiddled		True if the data is twiddled
@Description	Needed by PVRTTextureTile() in the various PVRTTextureAPIs
*****************************************************************************/
void PVRTTextureLoadTiled(
	PVRTuint8		* const pDst,
	const unsigned int	nWidthDst,
	const unsigned int	nHeightDst,
	const PVRTuint8	* const pSrc,
	const unsigned int	nWidthSrc,
	const unsigned int	nHeightSrc,
	const unsigned int	nElementSize,
	const bool			bTwiddled)
{
	unsigned int nXs, nYs;
	unsigned int nXd, nYd;
	unsigned int nIdxSrc, nIdxDst;

	for(nIdxDst = 0; nIdxDst < nWidthDst*nHeightDst; ++nIdxDst)
	{
		if(bTwiddled)
		{
			PVRTTextureDeTwiddle(nXd, nYd, nIdxDst);
		}
		else
		{
			nXd = nIdxDst % nWidthDst;
			nYd = nIdxDst / nWidthDst;
		}

		nXs = nXd % nWidthSrc;
		nYs = nYd % nHeightSrc;

		if(bTwiddled)
		{
			PVRTTextureTwiddle(nIdxSrc, nXs, nYs);
		}
		else
		{
			nIdxSrc = nYs * nWidthSrc + nXs;
		}

		memcpy(pDst + nIdxDst*nElementSize, pSrc + nIdxSrc*nElementSize, nElementSize);
	}
}

/*!***************************************************************************
@Function		PVRTTextureCreate
@Input			w			Size of the texture
@Input			h			Size of the texture
@Input			wMin		Minimum size of a texture level
@Input			hMin		Minimum size of a texture level
@Input			nBPP		Bits per pixel of the format
@Input			bMIPMap		Create memory for MIP-map levels also?
@Return			Allocated texture memory (must be free()d)
@Description	Creates a PVRTextureHeaderV3 structure, including room for
				the specified texture, in memory.
*****************************************************************************/
PVRTextureHeaderV3 *PVRTTextureCreate(
	const unsigned int	w,
	const unsigned int	h,
	const unsigned int	wMin,
	const unsigned int	hMin,
	const unsigned int	nBPP,
	const bool			bMIPMap)
{
	size_t			len;
	unsigned char	*p;

	{
		unsigned int	wTmp = w, hTmp = h;

		len = 0;
		do
		{
			len += PVRT_MAX(wTmp, wMin) * PVRT_MAX(hTmp, hMin);
			wTmp >>= 1;
			hTmp >>= 1;
		}
		while(bMIPMap && (wTmp || hTmp));
	}

	len = (len * nBPP) / 8;
	len += PVRTEX3_HEADERSIZE;

	p = (unsigned char*)malloc(len);
	_ASSERT(p);

	if(p)
	{
		PVRTextureHeaderV3 * const psTexHeader = (PVRTextureHeaderV3*)p;

		*psTexHeader=PVRTextureHeaderV3();

		psTexHeader->u32Width=w;
		psTexHeader->u32Height=h;

		return psTexHeader;
	}
	else
	{
		return 0;
	}
}


/*!***************************************************************************
 @Function		PVRTTextureTwiddle
 @Output		a	Twiddled value
 @Input			u	Coordinate axis 0
 @Input			v	Coordinate axis 1
 @Description	Combine a 2D coordinate into a twiddled value
*****************************************************************************/
void PVRTTextureTwiddle(unsigned int &a, const unsigned int u, const unsigned int v)
{
	_ASSERT(!((u|v) & 0xFFFF0000));
	a = 0;
	for(int i = 0; i < 16; ++i)
	{
		a |= ((u & (1 << i)) << (i+1));
		a |= ((v & (1 << i)) << (i+0));
	}
}

/*!***************************************************************************
 @Function		PVRTTextureDeTwiddle
 @Output		u	Coordinate axis 0
 @Output		v	Coordinate axis 1
 @Input			a	Twiddled value
 @Description	Extract 2D coordinates from a twiddled value.
*****************************************************************************/
void PVRTTextureDeTwiddle(unsigned int &u, unsigned int &v, const unsigned int a)
{
	u = 0;
	v = 0;
	for(int i = 0; i < 16; ++i)
	{
		u |= (a & (1 << ((2*i)+1))) >> (i+1);
		v |= (a & (1 << ((2*i)+0))) >> (i+0);
	}
}

/*!***********************************************************************
 @Function		PVRTGetBitsPerPixel
 @Input			u64PixelFormat			A PVR Pixel Format ID.
 @Return		const PVRTuint32	Number of bits per pixel.
 @Description	Returns the number of bits per pixel in a PVR Pixel Format 
				identifier.
*************************************************************************/
PVRTuint32 PVRTGetBitsPerPixel(PVRTuint64 u64PixelFormat)
{
	if((u64PixelFormat&PVRTEX_PFHIGHMASK)!=0)
	{
		PVRTuint8* PixelFormatChar=(PVRTuint8*)&u64PixelFormat;
		return PixelFormatChar[4]+PixelFormatChar[5]+PixelFormatChar[6]+PixelFormatChar[7];
	}
	else
	{
		switch (u64PixelFormat)
		{
		case ePVRTPF_BW1bpp:
			return 1;
		case ePVRTPF_PVRTCI_2bpp_RGB:
		case ePVRTPF_PVRTCI_2bpp_RGBA:
		case ePVRTPF_PVRTCII_2bpp:
			return 2;
		case ePVRTPF_PVRTCI_4bpp_RGB:
		case ePVRTPF_PVRTCI_4bpp_RGBA:
		case ePVRTPF_PVRTCII_4bpp:
		case ePVRTPF_ETC1:
		case ePVRTPF_EAC_R11:
		case ePVRTPF_ETC2_RGB:	
		case ePVRTPF_ETC2_RGB_A1:
		case ePVRTPF_DXT1:
		case ePVRTPF_BC4:
			return 4;
		case ePVRTPF_DXT2:
		case ePVRTPF_DXT3:
		case ePVRTPF_DXT4:
		case ePVRTPF_DXT5:
		case ePVRTPF_BC5:
		case ePVRTPF_EAC_RG11:
		case ePVRTPF_ETC2_RGBA:
			return 8;
		case ePVRTPF_YUY2:
		case ePVRTPF_UYVY:
		case ePVRTPF_RGBG8888:
		case ePVRTPF_GRGB8888:
			return 16;
		case ePVRTPF_SharedExponentR9G9B9E5:
			return 32;
		case ePVRTPF_NumCompressedPFs:
			return 0;
		}
	}
	return 0;
}

/*!***********************************************************************
 @Function		PVRTGetFormatMinDims
 @Input			u64PixelFormat	A PVR Pixel Format ID.
 @Modified		minX			Returns the minimum width.
 @Modified		minY			Returns the minimum height.
 @Modified		minZ			Returns the minimum depth.
 @Description	Gets the minimum dimensions (x,y,z) for a given pixel format.
*************************************************************************/
void PVRTGetFormatMinDims(PVRTuint64 u64PixelFormat, PVRTuint32 &minX, PVRTuint32 &minY, PVRTuint32 &minZ)
{
	switch(u64PixelFormat)
	{
	case ePVRTPF_DXT1:
	case ePVRTPF_DXT2:
	case ePVRTPF_DXT3:
	case ePVRTPF_DXT4:
	case ePVRTPF_DXT5:
	case ePVRTPF_BC4:
	case ePVRTPF_BC5:
	case ePVRTPF_ETC1:
	case ePVRTPF_ETC2_RGB:
	case ePVRTPF_ETC2_RGBA:
	case ePVRTPF_ETC2_RGB_A1:
	case ePVRTPF_EAC_R11:
	case ePVRTPF_EAC_RG11:
		minX = 4;
		minY = 4;
		minZ = 1;
		break;
	case ePVRTPF_PVRTCI_4bpp_RGB:
	case ePVRTPF_PVRTCI_4bpp_RGBA:
		minX = 8;
		minY = 8;
		minZ = 1;
		break;
	case ePVRTPF_PVRTCI_2bpp_RGB:
	case ePVRTPF_PVRTCI_2bpp_RGBA:
		minX = 16;
		minY = 8;
		minZ = 1;
		break;
	case ePVRTPF_PVRTCII_4bpp:
		minX = 4;
		minY = 4;
		minZ = 1;
		break;
	case ePVRTPF_PVRTCII_2bpp:
		minX = 8;
		minY = 4;
		minZ = 1;
		break;
	case ePVRTPF_UYVY:
	case ePVRTPF_YUY2:
	case ePVRTPF_RGBG8888:
	case ePVRTPF_GRGB8888:
		minX = 2;
		minY = 1;
		minZ = 1;
		break;
	case ePVRTPF_BW1bpp:
		minX = 8;
		minY = 1;
		minZ = 1;
		break;
	default: //Non-compressed formats all return 1.
		minX = 1;
		minY = 1;
		minZ = 1;
		break;
	}
}

/*!***********************************************************************
@Function		PVRTGetTextureDataSize
@Input			iMipLevel	Specifies a mip level to check, 'PVRTEX_ALLMIPLEVELS'
							can be passed to get the size of all MIP levels. 
@Input			bAllSurfs	Size of all surfaces is calculated if true, 
							only a single surface if false.
@Input			bAllFaces	Size of all faces is calculated if true, 
							only a single face if false.
@Return			PVRTuint32		Size in BYTES of the specified texture area.
@Description	Gets the size in BYTES of the texture, given various input 
				parameters.	User can retrieve the size of either all 
				surfaces or a single surface, all faces or a single face and
				all MIP-Maps or a single specified MIP level.
*************************************************************************/
PVRTuint32 PVRTGetTextureDataSize(PVRTextureHeaderV3 sTextureHeader, PVRTint32 iMipLevel, bool bAllSurfaces, bool bAllFaces)
{
	//The smallest divisible sizes for a pixel format
	PVRTuint32 uiSmallestWidth=1;
	PVRTuint32 uiSmallestHeight=1;
	PVRTuint32 uiSmallestDepth=1;

	PVRTuint64 PixelFormatPartHigh = sTextureHeader.u64PixelFormat&PVRTEX_PFHIGHMASK;
	
	//If the pixel format is compressed, get the pixel format's minimum dimensions.
	if (PixelFormatPartHigh==0)
	{
		PVRTGetFormatMinDims((EPVRTPixelFormat)sTextureHeader.u64PixelFormat, uiSmallestWidth, uiSmallestHeight, uiSmallestDepth);
	}

	//Needs to be 64-bit integer to support 16kx16k and higher sizes.
	PVRTuint64 uiDataSize = 0;
	if (iMipLevel==-1)
	{
		for (PVRTuint8 uiCurrentMIP = 0; uiCurrentMIP<sTextureHeader.u32MIPMapCount; ++uiCurrentMIP)
		{
			//Get the dimensions of the current MIP Map level.
			PVRTuint32 uiWidth = PVRT_MAX(1,sTextureHeader.u32Width>>uiCurrentMIP);
			PVRTuint32 uiHeight = PVRT_MAX(1,sTextureHeader.u32Height>>uiCurrentMIP);
			PVRTuint32 uiDepth = PVRT_MAX(1,sTextureHeader.u32Depth>>uiCurrentMIP);

			//If pixel format is compressed, the dimensions need to be padded.
			if (PixelFormatPartHigh==0)
			{
				uiWidth=uiWidth+( (-1*uiWidth)%uiSmallestWidth);
				uiHeight=uiHeight+( (-1*uiHeight)%uiSmallestHeight);
				uiDepth=uiDepth+( (-1*uiDepth)%uiSmallestDepth);
			}

			//Add the current MIP Map's data size to the total.
			uiDataSize+=(PVRTuint64)PVRTGetBitsPerPixel(sTextureHeader.u64PixelFormat)*(PVRTuint64)uiWidth*(PVRTuint64)uiHeight*(PVRTuint64)uiDepth;
		}
	}
	else
	{
		//Get the dimensions of the specified MIP Map level.
		PVRTuint32 uiWidth = PVRT_MAX(1,sTextureHeader.u32Width>>iMipLevel);
		PVRTuint32 uiHeight = PVRT_MAX(1,sTextureHeader.u32Height>>iMipLevel);
		PVRTuint32 uiDepth = PVRT_MAX(1,sTextureHeader.u32Depth>>iMipLevel);

		//If pixel format is compressed, the dimensions need to be padded.
		if (PixelFormatPartHigh==0)
		{
			uiWidth=uiWidth+( (-1*uiWidth)%uiSmallestWidth);
			uiHeight=uiHeight+( (-1*uiHeight)%uiSmallestHeight);
			uiDepth=uiDepth+( (-1*uiDepth)%uiSmallestDepth);
		}

		//Work out the specified MIP Map's data size
		uiDataSize=PVRTGetBitsPerPixel(sTextureHeader.u64PixelFormat)*uiWidth*uiHeight*uiDepth;
	}
	
	//The number of faces/surfaces to register the size of.
	PVRTuint32 numfaces = ((bAllFaces)?(sTextureHeader.u32NumFaces):(1));
	PVRTuint32 numsurfs = ((bAllSurfaces)?(sTextureHeader.u32NumSurfaces):(1));

	//Multiply the data size by number of faces and surfaces specified, and return.
	return (PVRTuint32)(uiDataSize/8)*numsurfs*numfaces;
}

/*****************************************************************************
 End of file (PVRTTexture.cpp)
*****************************************************************************/
