#include "stdafx.h"
#include "GameApp.h"
//-----------------------------------------------------------------------------
bool GameApp::Create()
{
	const char* vertexShaderText = R"(
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aColor;
layout(location = 3) in vec2 aTexCoord;

uniform mat4 uWorld;
uniform mat4 uView;
uniform mat4 uProjection;

out vec3 fNormal;
out vec3 fColor;
out vec2 fTexCoord;

void main()
{
	gl_Position = uProjection * uView * uWorld * vec4(aPos, 1.0);
	fNormal = normalize(mat3(uWorld) * aNormal);
	fColor = aColor;
	fTexCoord = aTexCoord;
}
)";

	const char* fragmentShaderText = R"(
#version 330 core

in vec3 fNormal;
in vec3 fColor;
in vec2 fTexCoord;

uniform sampler2D DiffuseTexture;
uniform vec3 LightDirection;

out vec4 fragColor;

void main()
{
	vec4 textureColor = texture(DiffuseTexture, fTexCoord) * vec4(fColor, 1.0);
	//if (textureColor.a < 0.02) discard;

	vec3 lightDir = -LightDirection;
	float lightIntensity = clamp(dot(fNormal, lightDir), 0.0f, 1.0f);
	vec4 diffuseLightColor = vec4(1.0, 1.0, 1.0, 1.0);

	fragColor =  clamp((diffuseLightColor * lightIntensity), 0.0f, 1.0f);
	fragColor = fragColor * textureColor;
}
)";

	struct testVertex
	{
		glm::vec3 pos;
		glm::vec3 normal;
		glm::vec3 color;
		glm::vec2 texCoord;
	}
	vert[] =
	{
		{{-0.5f,  0.5f, 2.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}, // top left
		{{ 0.5f,  0.5f, 2.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}}, // top right
		{{ 0.5f, -0.5f, 2.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}}, // bottom right
		{{-0.5f, -0.5f, 2.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}}, // bottom left
	};

	unsigned int indices[] = {  // note that we start from 0!
		0, 3, 2,   // first triangle
		2, 1, 0    // second triangle
	};


	glEnable(GL_CULL_FACE); // ��� ����� - ����������� ���� ������ ����� �������

	auto& renderSystem = GetRenderSystem();

	m_shader = renderSystem.CreateShaderProgram({ vertexShaderText }, { fragmentShaderText });
	m_uniformProjectionMatrix = renderSystem.GetUniform(m_shader, "uProjection");
	m_uniformViewMatrix = renderSystem.GetUniform(m_shader, "uView");
	m_uniformWorldMatrix = renderSystem.GetUniform(m_shader, "uWorld");
	m_uniformLightDirection = renderSystem.GetUniform(m_shader, "LightDirection");

	m_geom = renderSystem.CreateGeometryBuffer(BufferUsage::StaticDraw, (unsigned)Countof(vert), (unsigned)sizeof(testVertex), vert, (unsigned)Countof(indices), IndexType::Uint32, indices, m_shader);

	m_texture = renderSystem.CreateTexture2D("../Data/textures/1mx1m.png");

	GetInput().SetMouseLock(true);

	return true;
}
//-----------------------------------------------------------------------------
void GameApp::Destroy()
{
	GetInput().SetMouseLock(false);
	m_shader.reset();
	m_geom.reset();
	m_texture.reset();
}
//-----------------------------------------------------------------------------
void GameApp::Render()
{
	auto& renderSystem = GetRenderSystem();

	if (m_windowWidth != GetWindowWidth() || m_windowHeight != GetWindowHeight())
	{
		m_windowWidth = GetWindowWidth();
		m_windowHeight = GetWindowHeight();
		m_perspective = glm::perspective(glm::radians(45.0f), GetWindowSizeAspect(), 0.01f, 100.f);
		renderSystem.SetViewport(m_windowWidth, m_windowHeight);
	}

	renderSystem.ClearFrame();
	renderSystem.Bind(m_texture, 0);
	renderSystem.Bind(m_shader);
	renderSystem.SetUniform("DiffuseTexture", 0);
	renderSystem.SetUniform(m_uniformProjectionMatrix, m_perspective);
	renderSystem.SetUniform(m_uniformViewMatrix, m_camera.GetViewMatrix());
	renderSystem.SetUniform(m_uniformWorldMatrix, glm::mat4(1.0f));
	renderSystem.SetUniform(m_uniformLightDirection, m_camera.GetNormalizedViewVector());


	renderSystem.Draw(m_geom->vao);
}
//-----------------------------------------------------------------------------
void GameApp::Update(float deltaTime)
{
	if( GetInput().IsKeyDown(Input::KEY_ESCAPE) )
	{
		ExitRequest();
		return;
	}

	const float mouseSensitivity = 10.0f * deltaTime;
	const float moveSpeed = 10.0f * deltaTime;
	const glm::vec3 oldCameraPos = m_camera.position;

	if (GetInput().IsKeyDown(Input::KEY_W)) m_camera.MoveBy(moveSpeed);
	if (GetInput().IsKeyDown(Input::KEY_S)) m_camera.MoveBy(-moveSpeed);
	if (GetInput().IsKeyDown(Input::KEY_A)) m_camera.StrafeBy(moveSpeed);
	if (GetInput().IsKeyDown(Input::KEY_D)) m_camera.StrafeBy(-moveSpeed);

	glm::vec2 delta = GetInput().GetMouseDeltaPosition();
	if (delta.x != 0.0f)  m_camera.RotateLeftRight(delta.x * mouseSensitivity);
	if (delta.y != 0.0f)  m_camera.RotateUpDown(-delta.y * mouseSensitivity);
}
//-----------------------------------------------------------------------------