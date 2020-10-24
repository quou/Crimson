#pragma once

#include <tuple>
#include <memory>
#include <vector>

struct GLFWwindow;

#include "Layer.h"

#include "Renderer/Renderer.h"

namespace Crimson {
	class Game {
	private:
		GLFWwindow* m_window;

		std::vector<std::shared_ptr<Layer>> m_layers;

	protected:
		std::shared_ptr<Renderer> m_renderer;

		virtual void OnInit() = 0;
		virtual void OnUpdate(float delta) = 0;
		virtual void OnExit() = 0;

		std::pair<int, int> GetWindowSize();
	public:
		void Run(const char* windowTitle, std::pair<int, int> windowSize);

		template <typename T, typename... Args>
		void AddLayer(Args... constructorArgs) {
			m_layers.push_back(std::make_shared<T>(constructorArgs...));
		}
	};
}
