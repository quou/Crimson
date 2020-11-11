#pragma once


namespace Crimson {
	class Game;

	class Layer {
	protected:
	public:
		Game* m_game;

		virtual void OnInit() = 0;
		virtual void OnUpdate(float delta) = 0;

		virtual ~Layer() {}
	};
}
