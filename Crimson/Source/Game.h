#pragma once

#include <tuple>
#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include "Layer.h"

struct GLFWwindow;

namespace Crimson {
	class Game {
	private:
		GLFWwindow* m_window;

		std::vector<std::shared_ptr<Layer>> m_layers;

	protected:
		virtual void OnInit() = 0;
		virtual void OnUpdate(float delta) = 0;
		virtual void OnExit() = 0;

		std::pair<int, int> GetWindowSize();
	public:
		void SetCursorPosition(glm::vec2 newPos);

		void Run(const char* windowTitle, std::pair<int, int> windowSize);

		template <typename T, typename... Args>
		void AddLayer(void* userData, Args... constructorArgs) {
			m_layers.push_back(std::make_shared<T>(constructorArgs...));

			if (userData) {
				m_layers.at(m_layers.size() - 1)->m_userData = userData;
			}
		}
	};
}
