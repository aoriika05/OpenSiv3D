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

# pragma once
# include "Common.hpp"

namespace s3d
{
	enum class PPMType : uint8
	{
		P1,
		P2,
		P3,
		P4,
		P5,
		P6,
		Ascii01		= P1,
		AsciiGray	= P2,
		AsciiRGB	= P3,
		Binary01	= P4,
		BinaryGray	= P5,
		BinaryRGB	= P6
	};
}
