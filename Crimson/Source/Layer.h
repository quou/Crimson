#pragma once

namespace Crimson {
	class Layer {
	public:
		virtual void OnInit() = 0;
		virtual void OnUpdate(float delta) = 0;
	};
}
