#include <glad/glad.h>

#include "renderer.h"
#include "logger.h"

namespace Crimson {
	static void MessageCallback(unsigned int source, unsigned int type,
			unsigned int id, unsigned int severity,
			int length, const char* message,
			const void* userPointer) {
		/* Ignore messages that aren't important */
		if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

		const char* sourceString;
		if (source == GL_DEBUG_SOURCE_SHADER_COMPILER) {
			sourceString = "(from shader compiler)";
		}
		
		LogType logType = LogType::INFO;
		if (severity == GL_DEBUG_SEVERITY_HIGH || 
				severity == GL_DEBUG_SEVERITY_MEDIUM) {
			logType = LogType::ERROR;
		} else if (severity == GL_DEBUG_SEVERITY_LOW) {
			logType = LogType::WARNING;
		}

		Log(logType, "%s %s", sourceString, message);

	}
	
	void Renderer::Init(const ref<Window>& window) {
		instance().m_window = window;
#ifdef DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
		glDebugMessageCallback(MessageCallback, nullptr);
    		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, 
				GL_DONT_CARE, 0, nullptr, GL_TRUE);
#endif
	}

	void Renderer::Clear(float r, float g, float b) {
		Renderer& i = instance();
		
		glClearColor(r, g, b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		if (i.m_window) {
			glViewport(0, 0, i.m_window->GetWidth(), i.m_window->GetHeight());
		}
	}
}
