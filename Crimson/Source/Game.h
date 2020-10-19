#pragma once

#include <tuple>

namespace Crimson {
	class Game {
	private:
	protected:
		virtual void OnInit() = 0;
		virtual void OnUpdate(float delta) = 0;
	public:
		void Run(const char* windowTitle, std::pair<int, int> windowSize);
	};
}
