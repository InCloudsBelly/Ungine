#pragma once
namespace U
{
	class PhysicsSettingsWindow
	{
	public:
		static void OnImGuiRender(bool* show);
		static void RenderLayerList();
		static void RenderSelectedLayer();
	};
}