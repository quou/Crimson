#pragma once

namespace Crimson {
	class Game;

	class Layer {
	protected:
	public:
		Game* m_game;
		void* m_userData;

		virtual void OnInit() = 0;
		virtual void OnUpdate(float delta) = 0;
		virtual void OnExit() {}

		virtual ~Layer() {}
	};
}
