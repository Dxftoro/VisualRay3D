#include "vrpch.h"
#include "game.h"

#include "../logservice.h"
#include "../window_service/window.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "GLFW/glfw3.h"
#include "imgui.h"

#include "fps_counter.h"
#include "debugger.h"

namespace vray {

	template<>
	void DebugDisplay<int>::display(const std::string& name, const int* data) {
		ImGui::Text(name.c_str(), *data);
	}

	template<>
	void DebugDisplay<float>::display(const std::string& name, const float* data) {
		ImGui::Text(name.c_str(), *data);

	}

	template<>
	void VRAYLIB DebugDisplay<double>::display(const std::string& name, const double* data) {
		ImGui::Text(name.c_str(), *data);

	}

	template<>
	void DebugDisplay<bool>::display(const std::string& name, const bool* data) {
		ImGui::Text(name.c_str(), *data);
	}

	template<>
	void DebugDisplay<glm::vec3>::display(const std::string& name, const glm::vec3* data) {
		ImGui::Text(name.c_str(), data->x, data->y, data->z);
	}

	Debugger::Debugger(Window* _window) : window(_window), time(glfwGetTime()), open(true) {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)window->getHandlerPtr(), true);
		ImGui_ImplOpenGL3_Init();

		fpsCounter.start(window);
	}

	Debugger::~Debugger() {
		fpsCounter.stop();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void Debugger::removeVariable(const std::string& name) {
		varTable.erase(name);
	}

	void Debugger::update() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)window->getWidth(), (float)window->getHeight());

		float newTime = glfwGetTime();
		io.DeltaTime = (float)(newTime - time);
		time = newTime;

		//ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
		//ImGui::SetNextWindowSize(ImVec2(80, 50), ImGuiCond_Always);

		ImGui::Begin("Debug", &open);
		ImGui::Text("FPS: %d", fpsCounter.getFps());

		for (auto& it : varTable) {
			it.second.display(it.first.c_str(), it.second.data);
		}

		ImGui::End();

		//else {
			//ImGui::ShowDemoWindow();
		//}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	}

}