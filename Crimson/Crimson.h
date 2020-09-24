// To be included by Crimson applications

/*
 --- CRIMSON GAME ENGINE

Copyright (c) 2020 George Lamb

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef CRIMSON_H
#define CRIMSON_H

#include "Application.h"

/* Graphics */
#include "Graphics/Display.h"
#include "Graphics/Renderer.h"
#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Graphics/Texture.h"
#include "Graphics/Camera.h"
#include "Graphics/Material.h"
#include "Graphics/RenderTarget.h"
#include "Graphics/Cubemap.h"
#include "Graphics/Lighting/DirectionalLight.h"
#include "Graphics/Lighting/PointLight.h"

/* Scripting */
#include "Scripting/ScriptComponent.h"
#include "Scripting/ScriptStandardFunctions.h"
#include "Scripting/ScriptWrapper.h"

/* Scene Management */
#include "SceneManagement/SceneSettings.h"
#include "SceneManagement/SceneManager.h"

/* ECS */
#include "ECS.h"
#include "ComponentSystems/GraphicsSystems.h"
#include "ComponentSystems/Transform.h"
#include "ComponentSystems/ScriptSystems.h"
#include "ComponentSystems/PrefabSystems.h"
#include "ComponentSystems/CameraSystems.h"

/* Utils */
#include "Utils/FileUtils.h"
#include "Utils/WorkingDir.h"

#endif /* end of include guard: CRIMSON_H */
