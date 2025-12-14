#include "vrpch.h"
#include "kernel.h"

#include "../logservice.h"
#include "../window_service/window.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "imgui_layer.h"
#include "GLFW/glfw3.h"
#include "imgui.h"

namespace vray {

	ImGuiLayer::ImGuiLayer(Window* _window) 
		: window(_window), time(glfwGetTime()), open(true),
		fps(1.0f), minFps(1000), maxFps(0.0f) {

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)window->getHandlerPtr(), true);
		ImGui_ImplOpenGL3_Init();

		fpsCounter = new std::thread([this] {
			while (!window->isClosed()) {
				fps = (int)(1.0f / Game::deltaTime());

				//std::ofstream fout("D:/fps_profiling.txt", std::ios::app);
				//fout << fps << std::endl;
				//fout.close();

				if (fps < minFps) minFps = fps;
				if (fps > maxFps) maxFps = fps;
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}
		});
	}

	ImGuiLayer::~ImGuiLayer() {
		fpsCounter->join();
		delete[] fpsCounter;

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::onPush() {
		VR_ENGINE_LOGINFO("ImGui layer pushed!");
	}

	void ImGuiLayer::onPop() {
		VR_ENGINE_LOGINFO("ImGui layer popped!");
	}

	void ImGuiLayer::onUpdate() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)window->getWidth(), (float)window->getHeight());

		float newTime = glfwGetTime();
		io.DeltaTime = (float)(newTime - time);
		time = newTime;

		ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(80, 50), ImGuiCond_Always);

		ImGui::Begin("FPS", &open);
		ImGui::Text("FPS: %d", fps);
		ImGui::End();

		//else {
		//	ImGui::ShowDemoWindow();
		//}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::onEvent(Event& evt) {
		VR_ENGINE_LOGINFO("Some event on ImGui layer!");
	}

}