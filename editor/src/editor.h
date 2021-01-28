#include <crimson.h>

namespace Crimson {
	class Editor : public Application {
	private:
		ref<Scene> m_scene;
		std::string m_currentSave;

		bool m_isRunning = false;
		
		Camera m_camera;

		ref<PanelManager> m_panelManager;

		friend class Heirarchy;
	public:
		void OnInit();
		void OnUpdate(float delta);
		void OnExit();
	};

}