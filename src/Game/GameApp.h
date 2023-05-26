#pragma once

#include "PhysicsSimulator.h"

class GameApp final : public IApp
{
	bool Create() final;
	void Destroy() final;

	void Render() final;
	void Update(float deltaTime) final;

private:
	int m_windowWidth = 0;
	int m_windowHeight = 0;

	ShaderProgramRef m_shader;
	Uniform m_uniformProjectionMatrix;
	Uniform m_uniformViewMatrix;
	Uniform m_uniformWorldMatrix;
	Uniform m_uniformLightDirection;

	GeometryBufferRef m_geom;
	Texture2DRef m_texture;
	Texture2DRef m_textureRed;

	ModelRef m_plane;
	ModelRef m_box;
	ModelRef m_sphere;
	ModelRef m_capsule;

	glm::mat4 m_perspective;
	Camera m_camera;

	// physics
	StaticPhysicsObjectRef m_planePh;
	std::vector<RigidBodyRef> m_boxPh;
	CharacterControllerRef m_playerPh;
};