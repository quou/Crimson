#ifndef CRIMSON_H
#define CRIMSON_H

#include "Application.h"

/* Graphics */
#include "Graphics/Display.h"
#include "Graphics/Renderer.h"
#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Graphics/Texture.h"
#include "Graphics/Lighting/Light.h"
#include "Graphics/Lighting/Directional.h"
#include "Graphics/Camera.h"
#include "Graphics/Material.h"

/* Scene Management */
#include "SceneManagement/SceneSettings.h"
#include "SceneManagement/SceneManager.h"

/* ECS */
#include "SLECS.h"
#include "ComponentSystems/GraphicsSystems.h"
#include "ComponentSystems/Transform.h"

/* Utils */
#include "Utils/FileUtils.h"
#include "Utils/WorkingDir.h"

#endif /* end of include guard: CRIMSON_H */
