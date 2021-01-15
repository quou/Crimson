#pragma once

/* Crimson main header - to be included by Crimson applications */

/* Basic */
#include "logger.h"
#include "application.h"
#include "assets.h"
#include "memory.h"

/* Math */
#include "math/vec2.h"
#include "math/vec3.h"
#include "math/vec4.h"
#include "math/mat4.h"

/* Renderer */
#include "rendering/renderer.h"
#include "rendering/shader.h"
#include "rendering/window.h"
#include "rendering/mesh.h"
#include "rendering/meshfactory.h"
#include "rendering/model.h"
#include "rendering/camera.h"
#include "rendering/pbrmaterial.h"

/* Scene Management */
#include "entity/entity.h"
#include "entity/scene.h"
#include "entity/components/renderable.h"
#include "entity/components/transform.h"
#include "entity/components/lights.h"

/* Dear ImGui */
#include "imgui/imguimanager.h"