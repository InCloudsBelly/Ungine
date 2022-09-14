#include <Ungine.h>
#include <Ungine/EntryPoint.h>

#include "EditorLayer.h"

class EditiorApplication : public U::Application
{
public:
	EditiorApplication(const U::ApplicationProps& props)
		: Application(props)
	{
	}

	virtual void OnInit() override
	{
		PushLayer(new U::EditorLayer());
	}
};

U::Application* U::CreateApplication()
{
	return new EditiorApplication({"Hazelnut", 1600, 900});
}