#include "AppBase.hpp"

class MyApp : public AppBase
{
public:
	MyApp()  {}
	~MyApp() = default;

	virtual void Setup() final
	{

	}

	virtual void Loop() final
	{
		ImGui::ShowDemoWindow(&show_demo_window);
	}
private:
	// Our state
	bool show_demo_window = true;
	bool show_another_window = false;

};

int main(int, char**)
{
	MyApp app;
	app.Run();
	return 0;
}