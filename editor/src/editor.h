#include <crimson.h>

#include "panel.h"

namespace Crimson {
	class Editor : public Application {
	private:
		ref<Scene> m_scene;
		std::string m_currentSave;

		bool m_isRunning = false;

		ref<PanelManager> m_panelManager;

		friend class Heirarchy;

		void Save();
		void SaveAs();
		void Open();
		void New();

		std::string m_currentSavePath;
		std::string m_currentSaveFile;
	public:
		void OnInit();
		void OnUpdate(float delta);
		void OnExit();
	};

}