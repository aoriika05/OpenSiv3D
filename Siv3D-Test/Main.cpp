﻿
# include <Siv3D.hpp>
# include "Test/Siv3DTest.hpp"

void Main()
{
	//Cursor::Clip(Rect(640, 480));

	while (System::Update())
	{
		Window::SetTitle(Profiler::FPS(), L"fps");
		
		Graphics::SetBackground(HSV(Cursor::Pos().x * 0.5));
		
		if (KeyC.down())
		{
			Cursor::SetPos(320, 240);
		}
		
		if (KeyU.down())
		{
			Cursor::Clip(none);
		}

		if (KeyD.down())
		{
			Graphics::SetVSyncEnabled(false);
		}
		else if (KeyE.down())
		{
			Graphics::SetVSyncEnabled(true);
		}
	}
}
