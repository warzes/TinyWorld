#include "stdafx.h"
#include "GameAppBullet.h"

btDefaultCollisionConfiguration* collisionConfiguration;
btCollisionDispatcher* dispatcher;
btBroadphaseInterface* overlappingPairCache;
btSequentialImpulseConstraintSolver* solver;
btDiscreteDynamicsWorld* dynamicsWorld;

btPairCachingGhostObject* pairCachingGhostObject;
btKinematicCharacterController* kinematicCharacterController;

//keep track of the shapes, we release memory at exit.
//make sure to re-use collision shapes among rigid bodies whenever possible!
btAlignedObjectArray<btCollisionShape*> collisionShapes;

glm::mat4 btScalar2mat4(btScalar* matrix) {
	return glm::mat4(
		matrix[0], matrix[1], matrix[2], matrix[3],
		matrix[4], matrix[5], matrix[6], matrix[7],
		matrix[8], matrix[9], matrix[10], matrix[11],
		matrix[12], matrix[13], matrix[14], matrix[15]);
}


//-----------------------------------------------------------------------------
bool GameAppBullet::Create()
{
	// broad-фаза.
	// - btDbvtBroadphase используется в динамическом мире с множеством двигающихся объектов
	// - bt32BitAxisSweep3 вариант, возможно эффективней под мой проект - для более статичных кубов. У меня динамика не планируется. при этом размер региона известен (физ движок работает в текущем регионе)
	overlappingPairCache = new btDbvtBroadphase();
	//overlappingPairCache = new bt32BitAxisSweep3({ -100, -100, -100 }, { 100, 100, 100 });




	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	collisionConfiguration = new btDefaultCollisionConfiguration();
	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	solver = new btSequentialImpulseConstraintSolver;
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -9.8f, 0));

	// ground plane
	{
		btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(btScalar(0), btScalar(1), btScalar(0)), btScalar(0));
		collisionShapes.push_back(groundShape);

		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(0, 0, 0));

		btScalar mass(0.f);
		btVector3 localInertia(0, 0, 0);

		// using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		// add the body to the dynamics world
		dynamicsWorld->addRigidBody(body);
	}

	{
		btCollisionShape* colShape;
		btTransform startTransform;
		btScalar mass(1.0f);
		float rad = 12.0f;

		for (int j = 0; j < 24; j++)
		{
			for (int i = 0; i < 16; i++)
			{
				colShape = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
				collisionShapes.push_back(colShape);

				startTransform.setIdentity();

				// rigidbody is dynamic if and only if mass is non zero, otherwise static
				bool isDynamic = (mass != 0.f);

				btVector3 localInertia(0, 0, 0);
				if (isDynamic)
					colShape->calculateLocalInertia(mass, localInertia);

				startTransform.setOrigin(btVector3(rad * cos(2.0f * 3.14f * (i + static_cast<float>(j % 2) / 2.0f) / 16.0f), 1.0f + j * 2.0f, -rad * sin(2.0f * 3.14f * (i + static_cast<float>(j % 2) / 2.0f) / 16.0f)));
				startTransform.setRotation(btQuaternion(btVector3(btScalar(0), btScalar(1), btScalar(0)), btScalar((i + static_cast<float>(j % 2) / 2.0f) * 2.0f * 3.14f / 16.0f)));

				// using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
				btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
				btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
				btRigidBody* body = new btRigidBody(rbInfo);

				dynamicsWorld->addRigidBody(body);
			}
		}
	}

	{
		//create a dynamic rigidbody

		btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
		collisionShapes.push_back(colShape);

		// Create Dynamic Objects
		btTransform startTransform;
		startTransform.setIdentity();

		btScalar mass(1.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			colShape->calculateLocalInertia(mass, localInertia);

		startTransform.setOrigin(btVector3(2, 10, 0));

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		dynamicsWorld->addRigidBody(body);
	}


	{
#if 0
		btCollisionShape* capsuleShape = new btCapsuleShape(0.5f, 1.5f);
		collisionShapes.push_back(capsuleShape);
		btTransform startTransform;
		startTransform.setIdentity();
		btScalar mass(1.f);
		bool isDynamic = (mass != 0.f);
		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			capsuleShape->calculateLocalInertia(mass, localInertia);

		startTransform.setOrigin(btVector3(0, 10,-20));

		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, capsuleShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);
		body->setAngularFactor(btVector3(0, 0, 0));

		dynamicsWorld->addRigidBody(body);
#else
		btTransform startTransform;// начальное положение контроллера
		startTransform.setIdentity();
		startTransform.setOrigin(btVector3(0, 10, -20));
		startTransform.setRotation(btQuaternion(1, 0, 0, 1));

		btConvexShape* capsuleShape = new btCapsuleShape(0.5, 1.5f);
		collisionShapes.push_back(capsuleShape);

		// Создаем Character Controller
		pairCachingGhostObject = new btPairCachingGhostObject();
		pairCachingGhostObject->setWorldTransform(startTransform);
		dynamicsWorld->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
		pairCachingGhostObject->setCollisionShape(capsuleShape);
		pairCachingGhostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);

		btScalar stepHeight = btScalar(0.35);
		kinematicCharacterController = new btKinematicCharacterController(pairCachingGhostObject, capsuleShape, stepHeight, {0.0f, 1.0f, 0.0f});

		float jumpHeight = 1.5f; // Высота прыжка в метрах
		kinematicCharacterController->setMaxJumpHeight(jumpHeight); // Устанавливаем максимальную высоту прыжка

		const float STEP_HEIGHT = 0.4f;
		const float JUMP_HEIGHT = 2.0f;
		const float FALL_SPEED = 55.0f;
		const float JUMP_SPEED = 9.0f;
		const float MAX_SLOPE = 45.0f;
		const float DEFAULT_DAMPING = 0.2f;
		const btVector3 KINEMATIC_GRAVITY(0.0f, -14.0f, 0.0f);

		int colLayer_ = 1;
		int colMask_ = 0xffff;
		float stepHeight_ = STEP_HEIGHT;
		float maxJumpHeight_ = JUMP_HEIGHT;
		float jumpSpeed_ = JUMP_SPEED;
		float fallSpeed_ = FALL_SPEED;
		float maxSlope_ = MAX_SLOPE;
		float linearDamping_ = DEFAULT_DAMPING;
		float angularDamping_ = DEFAULT_DAMPING;
		btVector3 gravity_ = KINEMATIC_GRAVITY;
		//btVector3 position_;
		//btQuaternion rotation_;
		//btVector3 colShapeOffset_;
		bool reapplyAttributes_ = false;

		kinematicCharacterController->setGravity(gravity_);
		kinematicCharacterController->setLinearDamping(linearDamping_);
		kinematicCharacterController->setAngularDamping(angularDamping_);
		kinematicCharacterController->setStepHeight(stepHeight_);
		kinematicCharacterController->setMaxJumpHeight(maxJumpHeight_);
		kinematicCharacterController->setMaxSlope(glm::radians(maxSlope_));
		kinematicCharacterController->setJumpSpeed(jumpSpeed_);
		kinematicCharacterController->setFallSpeed(fallSpeed_);


		dynamicsWorld->addCollisionObject(pairCachingGhostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);

		// Добавляем контроллер в физический мир
		dynamicsWorld->addAction(kinematicCharacterController);
#endif
	}


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

	fragColor =  clamp((diffuseLightColor * lightIntensity), 0.9f, 1.0f);
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


	//glEnable(GL_CULL_FACE); // для теста - треугольник выше против часой стрелки

	auto& renderSystem = GetRenderSystem();

	m_shader = renderSystem.CreateShaderProgram({ vertexShaderText }, { fragmentShaderText });
	m_uniformProjectionMatrix = renderSystem.GetUniform(m_shader, "uProjection");
	m_uniformViewMatrix = renderSystem.GetUniform(m_shader, "uView");
	m_uniformWorldMatrix = renderSystem.GetUniform(m_shader, "uWorld");
	m_uniformLightDirection = renderSystem.GetUniform(m_shader, "LightDirection");

	m_geom = renderSystem.CreateGeometryBuffer(BufferUsage::StaticDraw, (unsigned)Countof(vert), (unsigned)sizeof(testVertex), vert, (unsigned)Countof(indices), IndexType::Uint32, indices, m_shader);

	m_texture = renderSystem.CreateTexture2D("../Data/textures/1mx1m.png");
	m_textureRed = renderSystem.CreateTexture2D("../Data/textures/1mx1m_red.png");

	GetInput().SetMouseLock(true);

	m_plane = GetGraphicsSystem().CreateModel("../Data/model/plane.obj");
	m_box = GetGraphicsSystem().CreateModel("../Data/model/box.obj");
	m_sphere = GetGraphicsSystem().CreateModel("../Data/model/sphere.obj");
	m_capsule = GetGraphicsSystem().CreateModel("../Data/model/capsule.obj");

	m_camera.Teleport(glm::vec3(0.0f, 3.0f, -9.0f));

	return true;
}
//-----------------------------------------------------------------------------
void GameAppBullet::Destroy()
{
	// cleanup in the reverse order of creation/initialization

	dynamicsWorld->removeAction(kinematicCharacterController);
	dynamicsWorld->removeCollisionObject(pairCachingGhostObject);
	delete kinematicCharacterController; kinematicCharacterController = nullptr;
	delete pairCachingGhostObject; pairCachingGhostObject = nullptr;

	// remove the rigidbodies from the dynamics world and delete them
	for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}

	// delete collision shapes
	for (int j = 0; j < collisionShapes.size(); j++)
	{
		btCollisionShape* shape = collisionShapes[j];
		collisionShapes[j] = 0;
		delete shape;
	}

	// delete dynamics world
	delete dynamicsWorld;

	// delete solver
	delete solver;

	// delete broadphase
	delete overlappingPairCache;

	// delete dispatcher
	delete dispatcher;

	delete collisionConfiguration;

	// next line is optional: it will be cleared by the destructor when the array goes out of scope
	collisionShapes.clear();


	GetInput().SetMouseLock(false);
	m_shader.reset();
	m_geom.reset();
	m_texture.reset();
}
//-----------------------------------------------------------------------------
void GameAppBullet::Render()
{
	auto& renderSystem = GetRenderSystem();

	if (m_windowWidth != GetWindowWidth() || m_windowHeight != GetWindowHeight())
	{
		m_windowWidth = GetWindowWidth();
		m_windowHeight = GetWindowHeight();
		m_perspective = glm::perspective(glm::radians(45.0f), GetWindowSizeAspect(), 0.01f, 100.f);
		renderSystem.SetViewport(m_windowWidth, m_windowHeight);
	}

	renderSystem.SetClearColor(glm::vec3(0.1f, 0.2f, 0.5f));
	renderSystem.ClearFrame();
	renderSystem.Bind(m_texture, 0);
	renderSystem.Bind(m_shader);
	renderSystem.SetUniform("DiffuseTexture", 0);
	renderSystem.SetUniform(m_uniformProjectionMatrix, m_perspective);
	renderSystem.SetUniform(m_uniformViewMatrix, m_camera.GetViewMatrix());
	renderSystem.SetUniform(m_uniformWorldMatrix, glm::mat4(1.0f));
	renderSystem.SetUniform(m_uniformLightDirection, m_camera.GetNormalizedViewVector());

	renderSystem.Draw(m_geom->vao);

	renderSystem.Bind(m_texture, 0);
	renderSystem.Bind(m_shader);
	renderSystem.SetUniform("DiffuseTexture", 0);
	renderSystem.SetUniform(m_uniformProjectionMatrix, m_perspective);
	renderSystem.SetUniform(m_uniformViewMatrix, m_camera.GetViewMatrix());
	renderSystem.SetUniform(m_uniformWorldMatrix, glm::scale(glm::mat4(1.0f), glm::vec3(50.0f, 1.0f, 50.0f)));
	renderSystem.SetUniform(m_uniformLightDirection, m_camera.GetNormalizedViewVector());
	GetGraphicsSystem().Draw(m_plane);

	renderSystem.Bind(m_texture, 0);
	renderSystem.Bind(m_shader);
	renderSystem.SetUniform("DiffuseTexture", 0);
	renderSystem.SetUniform(m_uniformProjectionMatrix, m_perspective);
	renderSystem.SetUniform(m_uniformViewMatrix, m_camera.GetViewMatrix());
	renderSystem.SetUniform(m_uniformLightDirection, m_camera.GetNormalizedViewVector());

	//print positions of all objects
	for (int j = dynamicsWorld->getNumCollisionObjects() - 2; j >= 0; j--)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
		btRigidBody* body = btRigidBody::upcast(obj);
		btTransform trans;
		if (body && body->getMotionState())
		{
			body->getMotionState()->getWorldTransform(trans);
		}
		else
		{
			trans = obj->getWorldTransform();
		}
		//printf("world pos object %d = %f,%f,%f\n", j, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));

		btScalar mat[16];
		trans.getOpenGLMatrix(mat);
		auto world = btScalar2mat4(mat);
		
		renderSystem.SetUniform(m_uniformWorldMatrix, world);
		GetGraphicsSystem().Draw(m_box);
	}


	{
		btTransform transform = pairCachingGhostObject->getWorldTransform();
		btVector3 position = transform.getOrigin();
		btQuaternion rotation = transform.getRotation();

		btScalar mat[16];
		transform.getOpenGLMatrix(mat);
		auto world = btScalar2mat4(mat);

		renderSystem.Bind(m_textureRed, 0);
		renderSystem.SetUniform(m_uniformWorldMatrix, world);
		GetGraphicsSystem().Draw(m_capsule);
	}
}
//-----------------------------------------------------------------------------
void GameAppBullet::Update(float deltaTime)
{
	if (GetInput().IsKeyDown(Input::KEY_ESCAPE))
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


	//glm::vec3 f = glm::vec3(0.0f);
	//if (GetInput().IsKeyDown(Input::KEY_J)) f.x -= 60.0 * deltaTime;
	//if (GetInput().IsKeyDown(Input::KEY_L)) f.x += 60.0* deltaTime;
	//if (GetInput().IsKeyDown(Input::KEY_I)) f.z += 60.0 * deltaTime;
	//if (GetInput().IsKeyDown(Input::KEY_K)) f.z -= 60.0 * deltaTime;

	btVector3 forwardVector(0, 0, 1);
	btVector3 rightVector(1, 0, 0);

	btVector3 walkDirection(0, 0, 0);
	if (GetInput().IsKeyDown(Input::KEY_I)) {
		walkDirection += forwardVector;
	}
	if (GetInput().IsKeyDown(Input::KEY_K)) {
		walkDirection -= forwardVector;
	}
	if (GetInput().IsKeyDown(Input::KEY_J)) {
		walkDirection -= rightVector;
	}
	if (GetInput().IsKeyDown(Input::KEY_L)) {
		walkDirection += rightVector;
	}

	if (!walkDirection.isZero())
	{
		walkDirection.normalize();
	}

	walkDirection *= 10 * deltaTime; // Устанавливаем скорость ходьбы
	kinematicCharacterController->setWalkDirection(walkDirection); // Устанавливаем направление и скорость ходьбы

	// Проверка нахождения контроллера на земле
	bool onGround = kinematicCharacterController->onGround();

	if (GetInput().IsKeyDown(Input::KEY_SPACE) && onGround)
	{
		kinematicCharacterController->jump(); // Вызываем метод jump() контроллера при нажатии на клавишу пробел
	}

	// просто пример как определить что персонаж дотронулся до стены (не проверенно)












	//btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[dynamicsWorld->getNumCollisionObjects() - 1];
	//btRigidBody* body = btRigidBody::upcast(obj);
	//if (f != glm::vec3(0.0f))
	//{
	//	body->activate();
	//	body->applyCentralImpulse({f.x, 0.0f, f.z});
	//	//body->applyCentralForce({f.x, 0.0f, f.z});
	//}
	//else
	//{
	//}


	//dynamicsWorld->stepSimulation(deltaTime);
	dynamicsWorld->stepSimulation(1.f / 60.0f, 10);
}
//-----------------------------------------------------------------------------