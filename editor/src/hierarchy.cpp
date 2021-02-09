#include <typeinfo>

#include <imgui.h>
#include <imgui_internal.h>

#include "hierarchy.h"
#include "editor.h"

namespace Crimson {
	void Hierarchy::DrawEntityNode(const ref<Entity>& ent) {
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;

		if (m_selectionContext == ent.get()) {
			flags |= ImGuiTreeNodeFlags_Selected;
		}

		bool open = ImGui::TreeNodeEx((void*)(uint64_t)ent->GetID(), flags, "%s", ent->m_name.c_str());

		if (ImGui::IsItemClicked()) {
			m_selectionContext = ent.get();
		}

		if (open) {
			ImGui::TreePop();
		}
	}


	template <typename T, typename ComponentDrawFunction>
	static void DrawComponent(const std::string& componentName, Entity* ent, ComponentDrawFunction drawFunction, bool canRemove = true) {
		if (ent == NULL) { return; }

		const ImGuiTreeNodeFlags treeNodeFlags = 
		ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap | 
		ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;

		if (ent->HasComponent<T>()) {
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));

			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImVec2 contentRegionAvail = ImGui::GetContentRegionAvail();

			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, "%s", componentName.c_str());
			ImGui::PopStyleVar();

			ImGui::SameLine(contentRegionAvail.x - lineHeight * 0.5f);
			if (ImGui::Button("+", ImVec2(lineHeight, lineHeight))) {
				ImGui::OpenPopup("ComponentSettings");
			}

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings")) {
				if (ImGui::MenuItem("Remove Component", "", false, canRemove) && canRemove) {
					removeComponent = true;
				}
				ImGui::EndPopup();
			}

			if (open) {
				drawFunction(ent->GetComponent<T>());

				ImGui::TreePop();
			}

			if (removeComponent) {
				ent->RemoveComponent<T>();
			}
		}
	}

	void Hierarchy::OnDraw(const ref<Scene>& scene, float delta) {
		Editor* editor = (Editor*)m_userData;
		
		/* Heriarchy */
		ImGui::Begin("hierarchy");

		if (!editor->m_isRunning) {
			if (ImGui::Button("create entity")) {
				scene->CreateEntity();
			}

			// Right click on blank space
			if (ImGui::BeginPopupContextWindow(0, 1, false)) {
				if (ImGui::BeginMenu("create...")) {
					if (ImGui::MenuItem("empty entity")) {
						scene->CreateEntity();
					}

					if (ImGui::MenuItem("spatial")) {
						Entity* e = scene->CreateEntity();
						e->AddComponent<TransformComponent>();
					}

					if (ImGui::MenuItem("cube")) {
						Entity* e = scene->CreateEntity("cube");
						e->AddComponent<TransformComponent>();
						Crimson::ref<Crimson::Material> material(new Crimson::PhongMaterial("standard", Crimson::vec3(1.0f, 1.0f, 1.0f), 32.0f));
						Crimson::ref<Crimson::Model> model(new Crimson::Model());
						model->AddMesh(Crimson::MeshFactory::NewCubeMesh(material));
						e->AddComponent<Crimson::RenderableComponent>(model);
					}

					if (ImGui::MenuItem("sphere")) {
						Entity* e = scene->CreateEntity("sphere");
						e->AddComponent<TransformComponent>();
						Crimson::ref<Crimson::Material> material(new Crimson::PhongMaterial("standard", Crimson::vec3(1.0f, 1.0f, 1.0f), 32.0f));
						Crimson::ref<Crimson::Model> model(new Crimson::Model());
						model->AddMesh(Crimson::MeshFactory::NewSphereMesh(material));
						e->AddComponent<Crimson::RenderableComponent>(model);
					}

					if (ImGui::MenuItem("sun")) {
						if (!scene->GetSun()) {
							Entity* e = scene->CreateEntity("sun");
							e->AddComponent<SkyLightComponent>(vec3(1,1,1), 0.1f);
							e->AddComponent<SunComponent>(vec3(-45.0f, 36.0f, 34.0f), vec3(1.0f), 1.0f);
						} else {
							Log(LogType::WARNING, "scene already contains a sun");
						}
					}

					if (ImGui::MenuItem("lamp")) {
						Entity* e = scene->CreateEntity("lamp");
						e->AddComponent<TransformComponent>();
						e->AddComponent<PointLightComponent>(vec3(1.0f), 1.0f);
					}

					if (ImGui::MenuItem("camera")) {
						Entity* e = scene->CreateEntity("camera");
						e->AddComponent<CameraComponent>(true);
					}

					ImGui::EndMenu();
				}

				ImGui::EndPopup();
			}

			for (const ref<Entity>& ent : scene->GetEntities()) {
				DrawEntityNode(ent);
			}

			if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
				m_selectionContext = NULL;
			}
		} else {
			m_selectionContext = NULL;
		}

		ImGui::End();

		/* Properties */
		if (m_selectionContext != NULL) {
			std::string propertiesTitle = m_selectionContext->m_name + " properties";
			ImGui::Begin((propertiesTitle + "###PROPERTIES").c_str());

			if (ImGui::Button("add component")) {
				ImGui::OpenPopup("addcomponent");
			}
			
			ImGui::SameLine();
			
			bool shouldDestroy = false;
			if (ImGui::Button("delete entity")) {
				shouldDestroy = true;
			}

			DrawTextControl("name", m_selectionContext->m_name);

			if (shouldDestroy) {
				m_selectionContext->Destroy();
				m_selectionContext = NULL;
			}
		} else {
			ImGui::Begin("properties###PROPERTIES");
		}

		if (!editor->m_isRunning) {
			if (ImGui::BeginPopup("addcomponent")) {
				if (ImGui::MenuItem("transform")) {
						m_selectionContext->AddComponent<TransformComponent>();
				}

				if (ImGui::MenuItem("script")) {
					m_selectionContext->AddComponent<ScriptComponent>("~");
				}

				if (ImGui::BeginMenu("rendering")) {
					if (ImGui::MenuItem("point light")) {
						m_selectionContext->AddComponent<PointLightComponent>(vec3(1.0f), 1.0f);
					}

					if (ImGui::MenuItem("sky light")) {
						m_selectionContext->AddComponent<SkyLightComponent>(vec3(1,1,1), 0.1f);
					}

					if (ImGui::MenuItem("sun")) {
						if (!scene->GetSun()) {
							m_selectionContext->AddComponent<SunComponent>(vec3(-45.0f, 36.0f, 34.0f), vec3(1.0f), 1.0f);
						} else {
							Log(LogType::WARNING, "scene already contains a sun");
						}
					}

					if (ImGui::MenuItem("camera")) {
						m_selectionContext->AddComponent<CameraComponent>(true);
					}

					if (ImGui::MenuItem("renderable")) {
						Crimson::ref<Crimson::Material> material(new Crimson::PhongMaterial("standard", Crimson::vec3(1.0f, 1.0f, 1.0f), 32.0f));
						Crimson::ref<Crimson::Model> model(new Crimson::Model());
						model->AddMesh(Crimson::MeshFactory::NewSphereMesh(material));
						m_selectionContext->AddComponent<Crimson::RenderableComponent>(model);
					}

					ImGui::EndMenu();
				}

				ImGui::EndPopup();
			}

			DrawComponent<TransformComponent>("transform", m_selectionContext, [](void* component){
				TransformComponent* tc = (TransformComponent*)component;

				DrawVec3Control("translation", tc->translation);
				DrawVec3Control("rotation", tc->rotation);
				DrawVec3Control("scale", tc->scale);
			}, true);

			DrawComponent<PointLightComponent>("point light", m_selectionContext, [](void* component){
				PointLightComponent* plc = (PointLightComponent*)component;

				DrawColorControl("colour", plc->color);
				DrawFloatControl("intensity", &plc->intensity);
				DrawFloatControl("constant", &plc->constant);
				DrawFloatControl("linear", &plc->linear);
				DrawFloatControl("quadratic", &plc->quadratic);
			}, true);

			DrawComponent<SkyLightComponent>("sky light", m_selectionContext, [](void* component){
				SkyLightComponent* slc = (SkyLightComponent*)component;

				DrawColorControl("colour", slc->color);
				DrawFloatControl("intensity", &slc->intensity);
			}, true);

			DrawComponent<SunComponent>("sun", m_selectionContext, [](void* component){
				SunComponent* sc = (SunComponent*)component;

				DrawVec3Control("direction", sc->direction);
				DrawColorControl("colour", sc->color);
				DrawFloatControl("intensity", &sc->intensity, 0.001f);
				DrawBoolControl("cast shadows", &sc->castShadows);
			}, true);
			
			DrawComponent<CameraComponent>("camera", m_selectionContext, [](void* component){
				CameraComponent* cc = (CameraComponent*)component;

				DrawBoolControl("active", &cc->active);
			}, true);

			DrawComponent<ScriptComponent>("script", m_selectionContext, [&](void* component){
				ScriptComponent* slc = (ScriptComponent*)component;

				bool exists = scene->m_scriptManager->CompiliationSuccess() && scene->m_scriptManager->CheckBehaviourExistance(slc->m_behaviourDecl.c_str());

				if (scene->m_scriptManager->CompiliationSuccess()) {
					if (exists) {
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
						ImGui::Text(ICON_FK_CHECK " valid behaviour");
					} else if (slc->m_behaviourDecl == "~") {
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 0.15f, 1.0f));
						ImGui::Text(ICON_FK_EXCLAMATION_TRIANGLE " no behaviour");
					} else {
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
						ImGui::Text(ICON_FK_TIMES " invalid behaviour");
					}
				} else {
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 0.15f, 1.0f));
					ImGui::Text(ICON_FK_EXCLAMATION_TRIANGLE " behaviours not compiled");
				}
				ImGui::PopStyleColor();

				std::string selectedScript = slc->m_behaviourDecl;
				DrawComboBox("behaviour", [&](){
					if (ImGui::BeginCombo("###SCRIPTCOMBO", selectedScript.c_str())) {
						if (ImGui::Selectable("~", "~" == selectedScript)) {
							slc->m_behaviourDecl = "~";
						}

						for (const std::string& t : scene->m_scriptManager->GetBehaviourNames()) {
							if (ImGui::Selectable(t.c_str(), t == selectedScript)) {
								slc->m_dontCallInit = true;
								slc->ClearSerialisableValues();
								slc->m_behaviourDecl = t;
								slc->OnDestroy();
								slc->OnInit();
							}
						}

						ImGui::EndCombo();
					}
				});

				if (exists) {
					ImGui::Separator();

					for (const BehaviourField& f : scene->m_scriptManager->GetBehaviourFields(slc->GetInstance())) {
						std::string name;
						for (auto c : f.name) {
							if (std::isupper(c)) {
								name += " ";
								name += (std::tolower(c));
							} else if (c == '_') {
								name += " ";
							} else {
								name += c;
							}
						}

						if (scene->m_scriptManager->IsFloat(f)) {
							float v = scene->m_scriptManager->GetFloatProperty(slc->GetInstance(), f);
							DrawFloatControl(name.c_str(), &v);
							scene->m_scriptManager->SetFloatProperty(slc->GetInstance(), f, v);
							slc->m_serialisableFloats[f] = v;
						} else if (scene->m_scriptManager->IsInt(f)) {
							int v = scene->m_scriptManager->GetIntProperty(slc->GetInstance(), f);
							DrawIntControl(name.c_str(), &v);
							scene->m_scriptManager->SetIntProperty(slc->GetInstance(), f, v);
							slc->m_serialisableInts[f] = v;
						} else if (scene->m_scriptManager->IsString(f)) {
							std::string v = scene->m_scriptManager->GetStringProperty(slc->GetInstance(), f);
							DrawTextControl(name.c_str(), v);
							scene->m_scriptManager->SetStringProperty(slc->GetInstance(), f, v);
							slc->m_serialisableStrings[f] = v;
						}
					}
				}
			}, true);

			DrawComponent<RenderableComponent>("renderable", m_selectionContext, [](void* component){
				RenderableComponent* rc = (RenderableComponent*)component;

				ref<Model>& m = rc->m_model;

				ref<Mesh>& firstMesh = m->GetFirstMesh();
				std::string selectedMeshType;
				if (!m->IsFromFile()) {
					selectedMeshType = firstMesh->GetFactoryType() == Mesh::CUBE ? "cube" : "sphere";
				} else {
					selectedMeshType = m->GetPath();
				}

				DrawComboBox("mesh", [&](){
					if (ImGui::BeginCombo("###MESHCOMBO", selectedMeshType.c_str())) {
						if (ImGui::Selectable("new sphere mesh", "sphere" == selectedMeshType)) {
							m->ClearMeshes();
							m->AddMesh(MeshFactory::NewSphereMesh(ref<Material>(new PhongMaterial("standard", vec3(1.0f), 32.0f))));
						}

						if (ImGui::Selectable("new cube mesh", "cube" == selectedMeshType)) {
							m->ClearMeshes();
							m->AddMesh(MeshFactory::NewCubeMesh(ref<Material>(new PhongMaterial("standard", vec3(1.0f), 32.0f))));
						}

						for (const auto& e : AssetManager::GetDir()) {
							if (e.second == "fbx") {
								if (ImGui::Selectable(e.first.c_str(), e.first == m->GetPath())) {
									m = ref<Model>(new Model(e.first.c_str()));
								}
							}
						}

						ImGui::EndCombo();
					}
				});

				int i = 0;
				for (ref<Mesh>& mesh : m->GetMeshList()) {
					std::string meshName = "mesh " + std::to_string(i);
					if (ImGui::TreeNode(meshName.c_str())) {
						ref<Material> material = mesh->GetMaterial();

						if (material->m_type == "phong") {
							if (ImGui::TreeNodeEx((void*)(unsigned long)i, ImGuiTreeNodeFlags_OpenOnArrow, "phong material")) {
								std::string selectedShader = material->m_shader;

								DrawComboBox("shader", [&](){
									if (ImGui::BeginCombo("###SHADERCOMBO", selectedShader.c_str())) {
										if (ImGui::Selectable("standard", "standard" == selectedShader)) {
											material->m_shader = "standard";
										}

										for (const auto& e : AssetManager::GetDir()) {
											if (e.second == "glsl") {
												if (ImGui::Selectable(e.first.c_str(), e.first == selectedShader)) {
													material->m_shader = e.first;
												}
											}
										}

										ImGui::EndCombo();
									}
								});

								PhongMaterial* phongMat = (PhongMaterial*)material.get();

								DrawColorControl("color", phongMat->color);
								DrawFloatControl("shininess", &phongMat->shininess);
							
								ImGui::TreePop();
							}
						}

						ImGui::TreePop();
					}

					i++;
				}
			}, true);
		}
		ImGui::End();
	}
}