#pragma once

#include <Crimson.h>

class EditorLayer : public Crimson::Layer {
private:
	Crimson::RenderTarget* m_renderTarget;
public:
	EditorLayer(Crimson::RenderTarget* renderTarget);

	void OnInit() override;
	void OnUpdate(float delta) override;
};
