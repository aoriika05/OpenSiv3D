﻿//-----------------------------------------------
//
//	This file is part of the Siv3D Engine.
//
//	Copyright (c) 2008-2021 Ryo Suzuki
//	Copyright (c) 2016-2021 OpenSiv3D Project
//
//	Licensed under the MIT License.
//
//-----------------------------------------------

# include <Siv3D/ImageFormat/WebPEncoder.hpp>
# include <Siv3D/BinaryWriter.hpp>
# include <Siv3D/Image.hpp>
# include <Siv3D/EngineLog.hpp>

namespace s3d
{
	/*
	StringView WebPEncoder::name() const
	{
		return U"WebP"_sv;
	}

	const Array<String>& WebPEncoder::possibleExtensions() const
	{
		static const Array<String> extensions = { U"gif" };

		return extensions;
	}

	bool WebPEncoder::save(const Image& image, const FilePathView path) const
	{
		BinaryWriter writer{ path };

		if (not writer)
		{
			return false;
		}

		return encode(image, writer);
	}

	bool WebPEncoder::encode(const Image& image, IWriter& writer) const
	{
		if (not writer.isOpen())
		{
			return false;
		}

		const int32 width = image.width();
		const int32 height = image.height();
		const int32 num_pixels = image.num_pixels();
		Array<uint8> rBuffer(num_pixels);
		Array<uint8> gBuffer(num_pixels);
		Array<uint8> bBuffer(num_pixels);

		uint8* rDst = rBuffer.data();
		uint8* gDst = gBuffer.data();
		uint8* bDst = bBuffer.data();
		{
			const Color* pSrc = image.data();
			const Color* const pSrcEnd = pSrc + num_pixels;

			while (pSrc != pSrcEnd)
			{
				*rDst++ = pSrc->r;
				*gDst++ = pSrc->g;
				*bDst++ = pSrc->b;
				++pSrc;
			}
		}

		bool hasTransparency = false;
		{
			const Color* pSrc = image.data();
			const Color* const pSrcEnd = pSrc + num_pixels;

			while (pSrc != pSrcEnd)
			{
				if (pSrc->a == 0)
				{
					hasTransparency = true;
					break;
				}

				++pSrc;
			}
		}

		int32 colorMapSize = hasTransparency ? 255 : 256;
		GifColorType colors[256] = {};

		ColorMapObject colorMap;
		colorMap.ColorCount = colorMapSize;
		colorMap.BitsPerPixel = 8;
		colorMap.Colors = colors;
		Array<GifByteType> outputBuffer(num_pixels);

		GifQuantizeBuffer(width, height, &colorMapSize,
			rBuffer.data(), gBuffer.data(), bBuffer.data(), outputBuffer.data(), colorMap.Colors);

		int32 transparencyIndex = -1;

		if (hasTransparency)
		{
			transparencyIndex = colorMap.ColorCount;

			++colorMap.ColorCount;
			colorMap.Colors[transparencyIndex] = { 0,0,0 };

			const Color* pSrc = image.data();
			const Color* const pSrcEnd = pSrc + num_pixels;
			GifByteType* pDst = outputBuffer.data();

			while (pSrc != pSrcEnd)
			{
				if (pSrc->a == 0)
				{
					*pDst = 255;
				}

				++pSrc;
				++pDst;
			}
		}

		int error = 0;
		GifFileType* gif = EGifOpen(&writer, detail::GifWriteCallback, &error);

		EGifSetGifVersion(gif, true);

		if (EGifPutScreenDesc(gif, width, height, 8, 0, nullptr) == WebP_ERROR)
		{
			EGifCloseFile(gif, &error);
			return false;
		}

		GraphicsControlBlock controlBlock;
		controlBlock.DisposalMode = DISPOSAL_UNSPECIFIED;
		controlBlock.UserInputFlag = false;
		controlBlock.DelayTime = 100;
		controlBlock.TransparentColor = hasTransparency ? transparencyIndex : NO_TRANSPARENT_COLOR;

		GifByteType ext[4];
		EGifGCBToExtension(&controlBlock, ext);

		if (EGifPutExtension(gif, GRAPHICS_EXT_FUNC_CODE, sizeof(ext), ext) == WebP_ERROR)
		{
			EGifCloseFile(gif, &error);
			return false;
		}

		int r = EGifPutImageDesc(gif, 0, 0, width, height, false, &colorMap);

		if (r != WebP_OK)
		{
			EGifCloseFile(gif, &error);
			return false;
		}

		for (int32 y = 0; y < height; ++y)
		{
			if (EGifPutLine(gif, &outputBuffer[y * width], width) == WebP_ERROR)
			{
				EGifCloseFile(gif, &error);
				return false;
			}
		}

		EGifCloseFile(gif, &error);

		return true;
	}

	Blob WebPEncoder::encode(const Image& image) const
	{
		const int32 width = image.width();
		const int32 height = image.height();
		const int32 num_pixels = image.num_pixels();
		Array<uint8> rBuffer(num_pixels);
		Array<uint8> gBuffer(num_pixels);
		Array<uint8> bBuffer(num_pixels);

		uint8* rDst = rBuffer.data();
		uint8* gDst = gBuffer.data();
		uint8* bDst = bBuffer.data();
		{
			const Color* pSrc = image.data();
			const Color* const pSrcEnd = pSrc + num_pixels;

			while (pSrc != pSrcEnd)
			{
				*rDst++ = pSrc->r;
				*gDst++ = pSrc->g;
				*bDst++ = pSrc->b;
				++pSrc;
			}
		}

		bool hasTransparency = false;
		{
			const Color* pSrc = image.data();
			const Color* const pSrcEnd = pSrc + num_pixels;

			while (pSrc != pSrcEnd)
			{
				if (pSrc->a == 0)
				{
					hasTransparency = true;
					break;
				}

				++pSrc;
			}
		}

		int32 colorMapSize = hasTransparency ? 255 : 256;
		GifColorType colors[256] = {};

		ColorMapObject colorMap;
		colorMap.ColorCount = colorMapSize;
		colorMap.BitsPerPixel = 8;
		colorMap.Colors = colors;
		Array<GifByteType> outputBuffer(num_pixels);

		GifQuantizeBuffer(width, height, &colorMapSize,
			rBuffer.data(), gBuffer.data(), bBuffer.data(), outputBuffer.data(), colorMap.Colors);

		int32 transparencyIndex = -1;

		if (hasTransparency)
		{
			transparencyIndex = colorMap.ColorCount;

			++colorMap.ColorCount;
			colorMap.Colors[transparencyIndex] = { 0,0,0 };

			const Color* pSrc = image.data();
			const Color* const pSrcEnd = pSrc + num_pixels;
			GifByteType* pDst = outputBuffer.data();

			while (pSrc != pSrcEnd)
			{
				if (pSrc->a == 0)
				{
					*pDst = 255;
				}

				++pSrc;
				++pDst;
			}
		}

		Blob blob;
		int error = 0;
		GifFileType* gif = EGifOpen(&blob, detail::GifBlobWriteCallback, &error);

		EGifSetGifVersion(gif, true);

		if (EGifPutScreenDesc(gif, width, height, 8, 0, nullptr) == WebP_ERROR)
		{
			EGifCloseFile(gif, &error);
			return{};
		}

		GraphicsControlBlock controlBlock;
		controlBlock.DisposalMode = DISPOSAL_UNSPECIFIED;
		controlBlock.UserInputFlag = false;
		controlBlock.DelayTime = 100;
		controlBlock.TransparentColor = hasTransparency ? transparencyIndex : NO_TRANSPARENT_COLOR;

		GifByteType ext[4];
		EGifGCBToExtension(&controlBlock, ext);

		if (EGifPutExtension(gif, GRAPHICS_EXT_FUNC_CODE, sizeof(ext), ext) == WebP_ERROR)
		{
			EGifCloseFile(gif, &error);
			return{};
		}

		int r = EGifPutImageDesc(gif, 0, 0, width, height, false, &colorMap);

		if (r != WebP_OK)
		{
			EGifCloseFile(gif, &error);
			return{};
		}

		for (int32 y = 0; y < height; ++y)
		{
			if (EGifPutLine(gif, &outputBuffer[y * width], width) == WebP_ERROR)
			{
				EGifCloseFile(gif, &error);
				return{};
			}
		}

		EGifCloseFile(gif, &error);

		return blob;
	}
	*/
}
