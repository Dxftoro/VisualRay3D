#include "vrpch.h"
#include "kernel.h"

#include "../logservice.h"
#include "../window_service/window.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "imgui_layer.h"

namespace vray {

	ImGuiLayer::ImGuiLayer(Window* _window) 
		: window(_window), time(glfwGetTime()), open(true) {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)window->getHandlerPtr(), true);
		ImGui_ImplOpenGL3_Init();
	}

	ImGuiLayer::~ImGuiLayer() {
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

		if (open) {
			ImGui::Begin("VisualRay debug", &open);
			ImVec2 vec = ImGui::GetWindowPos();
			
			ImGui::Text("CODECVT BECOME DEPRECATED!");
			if (ImGui::Button("Close")) { 
				open = false;
			}
			ImGui::End();
		}
		else {
			ImGui::ShowDemoWindow();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::onEvent(Event& evt) {
		VR_ENGINE_LOGINFO("Some event on ImGui layer!");
	}

}