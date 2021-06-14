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

# include <Siv3D/FontAsset.hpp>
# include <Siv3D/Asset/IAsset.hpp>
# include <Siv3D/Common/Siv3DEngine.hpp>

namespace s3d
{
	namespace detail
	{
		[[nodiscard]]
		static Font FromAsset(const IAsset* asset)
		{
			if (const FontAssetData* fontAssetData = dynamic_cast<const FontAssetData*>(asset))
			{
				return fontAssetData->font;
			}

			return{};
		}
	}

	FontAsset::FontAsset(const AssetNameView name)
		: Font{ detail::FromAsset(SIV3D_ENGINE(Asset)->getAsset(AssetType::Font, name)) } {}

	bool FontAsset::Register(const AssetName& name, const int32 fontSize, const FilePathView path, const FontStyle style)
	{
		return Register(name, FontMethod::Bitmap, fontSize, path, 0, style);
	}

	bool FontAsset::Register(const AssetName& name, const int32 fontSize, const FilePathView path, const size_t faceIndex, const FontStyle style)
	{
		return Register(name, FontMethod::Bitmap, fontSize, path, faceIndex, style);
	}

	bool FontAsset::Register(const AssetName& name, const int32 fontSize, const Typeface typeface, const FontStyle style)
	{
		return Register(name, FontMethod::Bitmap, fontSize, typeface, style);
	}

	bool FontAsset::Register(const AssetName& name, const FontMethod fontMethod, const int32 fontSize, const FilePathView path, const FontStyle style)
	{
		return Register(name, fontMethod, fontSize, path, 0, style);
	}

	bool FontAsset::Register(const AssetName& name, const FontMethod fontMethod, const int32 fontSize, const FilePathView path, const size_t faceIndex, const FontStyle style)
	{
		std::unique_ptr<FontAssetData> data = std::make_unique<FontAssetData>(fontMethod, fontSize, path, faceIndex, style);
	
		return Register(name, std::move(data));
	}

	bool FontAsset::Register(const AssetName& name, const FontMethod fontMethod, const int32 fontSize, const Typeface typeface, const FontStyle style)
	{
		std::unique_ptr<FontAssetData> data = std::make_unique<FontAssetData>(fontMethod, fontSize, typeface, style);

		return Register(name, std::move(data));
	}

	bool FontAsset::Register(const AssetName& name, std::unique_ptr<FontAssetData>&& data)
	{
		return SIV3D_ENGINE(Asset)->registerAsset(AssetType::Font, name, std::move(data));
	}

	bool FontAsset::IsRegistered(const AssetNameView name)
	{
		return SIV3D_ENGINE(Asset)->isRegistered(AssetType::Font, name);
	}
}
