#include "GLRenderSystem.h"
#include <imgui.h>
#include <ImGuizmo.h>
#include "CameraSystem.h"
#include "OpenGLImageGraphicsData.h"
#include "ImageSystem.h"

static glm::mat4 newView(1.0f);

IRenderer& GLRenderSystem::CreateRenderer(entt::registry& reg, entt::entity& e)
{
	#ifdef __EMSCRIPTEN__
	IRenderer& rendererRef = reg.emplace<GLRenderer>(e, platform->GetAssetManager()->GetAssetPath("../../Shaders/GLSL-ES/vertex.glsl"), platform->GetAssetManager()->GetAssetPath("../../Shaders/GLSL-ES/fragment.glsl"));
	#else
	IRenderer& rendererRef = reg.emplace<GLRenderer>(e, platform->GetAssetManager()->GetAssetPath("../../Shaders/GLSL/vertex.glsl"), platform->GetAssetManager()->GetAssetPath("../../Shaders/GLSL/fragment.glsl"));
	#endif
	return rendererRef;
}

void GLRenderSystem::Load(IRenderer* renderer, Camera& camera)
{
	GLRenderer* glRenderer = (GLRenderer*)renderer;
	glGenVertexArrays(1, &glRenderer->vao);
	glBindVertexArray(glRenderer->vao);
	glGenBuffers(1, &glRenderer->vbo);
	glGenBuffers(1, &glRenderer->ibo);
	glBindBuffer(GL_ARRAY_BUFFER, glRenderer->vbo);
	glRenderer->program = glCreateProgram();
	glAttachShader(glRenderer->program, glRenderer->vertex.GetId());
	glAttachShader(glRenderer->program, glRenderer->pixel.GetId());
	glLinkProgram(glRenderer->program);
	glUseProgram(glRenderer->program);
	glRenderer->projLoc = glGetUniformLocation(glRenderer->program, "projection");
	glRenderer->viewLoc = glGetUniformLocation(glRenderer->program, "view");
	glRenderer->modelLoc = glGetUniformLocation(glRenderer->program, "model");
	glRenderer->colorLoc = glGetUniformLocation(glRenderer->program, "in_color");
	glm::mat4 model(1.0f);
	glUniformMatrix4fv(glRenderer->modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glRenderer->viewLoc, 1, GL_FALSE, glm::value_ptr(camera.view));
	glUniformMatrix4fv(glRenderer->projLoc, 1, GL_FALSE, glm::value_ptr(camera.projection));
	glUniform4f(glRenderer->colorLoc, glRenderer->vertexColor.x, glRenderer->vertexColor.y, glRenderer->vertexColor.z, glRenderer->vertexColor.w);
}

void GLRenderSystem::BindRenderer(IRenderer* renderer)
{
	GLRenderer* glRenderer = (GLRenderer*)renderer;
	glBindVertexArray(glRenderer->vao);
	glBindBuffer(GL_ARRAY_BUFFER, glRenderer->vbo);
	GLint posAttrib = glGetAttribLocation(glRenderer->program, "in_position");
	glEnableVertexAttribArray(posAttrib);
	SetupAttribute(posAttrib, 3, GL_FLOAT, Vertex, Position);

	GLint uvAttrib = glGetAttribLocation(glRenderer->program, "aTexCoord");
	glEnableVertexAttribArray(uvAttrib);
	SetupAttribute(uvAttrib, 2, GL_FLOAT, Vertex, UV);

	//GLint vertColorAttrib = glGetAttribLocation(glRenderer->program, "in_color");
	GLint vertColorAttrib = glGetUniformLocation(glRenderer->program, "in_color");
	//glEnableVertexAttribArray(vertColorAttrib);

	//SetupAttribute(vertColorAttrib, 3, GL_FLOAT, Vertex, Color);
	//glVertexAttribPointer(vertColorAttrib, 3, GL_FLOAT, 0, sizeof(Vertex), (void*)offsetof(structure, element));
}

void GLRenderSystem::LoadMesh(IRenderer* renderer, Mesh& mesh)
{
	GLRenderer* glRenderer = (GLRenderer*)renderer;
	glBindVertexArray(glRenderer->vao);
	glBufferData(GL_ARRAY_BUFFER, mesh.rawVertices.size() * sizeof(Vertex), mesh.rawVertices.data(), GL_STATIC_DRAW);
	GLint posAttrib = glGetAttribLocation(glRenderer->program, "in_position");
	glEnableVertexAttribArray(posAttrib);
	SetupAttribute(posAttrib, 3, GL_FLOAT, Vertex, Position);

	GLint uvAttrib = glGetAttribLocation(glRenderer->program, "aTexCoord");
	glEnableVertexAttribArray(uvAttrib);
	SetupAttribute(uvAttrib, 2, GL_FLOAT, Vertex, UV);

	//GLint vertColorAttrib = glGetAttribLocation(glRenderer->program, "in_color");
	GLint vertColorAttrib = glGetUniformLocation(glRenderer->program, "in_color");
	//glEnableVertexAttribArray(vertColorAttrib);

	//SetupAttribute(vertColorAttrib, 3, GL_FLOAT, Vertex, Color);
	//glVertexAttribPointer(vertColorAttrib, 3, GL_FLOAT, 0, sizeof(Vertex), (void*)offsetof(structure, element));
	glRenderer->numVertices = mesh.rawVertices.size();
}

void GLRenderSystem::Draw(IRenderer* renderer)
{
	GLRenderer* glRenderer = (GLRenderer*)renderer;
	glBindVertexArray(glRenderer->vao);
	BindTexture(*glRenderer);
	if (glRenderer->numVertices == 2)
	{
		glDrawArrays(GL_LINES, 0, glRenderer->numVertices);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, glRenderer->numVertices);
	}
}

void GLRenderSystem::DrawWireframe(IRenderer* renderer)
{
	GLRenderer* glRenderer = (GLRenderer*)renderer;
	glBindVertexArray(glRenderer->vao);
	glUseProgram(glRenderer->program);
	glUniform4f(glRenderer->colorLoc, 1, 1, 1, 1);
	glDrawArrays(GL_LINES, 0, glRenderer->numVertices);
}

void GLRenderSystem::UpdateRenderer(IRenderer* renderer, Transform meshTransform, Camera camera)
{
	GLRenderer* glRenderer = (GLRenderer*)renderer;
	glUseProgram(glRenderer->program);
	glUniformMatrix4fv(glRenderer->viewLoc, 1, GL_FALSE, glm::value_ptr(camera.view));
	glUniformMatrix4fv(glRenderer->projLoc, 1, GL_FALSE, glm::value_ptr(camera.projection));
	glUniformMatrix4fv(glRenderer->modelLoc, 1, GL_FALSE, glm::value_ptr(meshTransform.worldMatrix));
	glUniform4f(glRenderer->colorLoc, glRenderer->vertexColor.x, glRenderer->vertexColor.y, glRenderer->vertexColor.z, glRenderer->vertexColor.w);
}

void GLRenderSystem::DrawGizmo(Camera camera)
{
	ImGuiIO& io = ImGui::GetIO();
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
	ImGuizmo::Manipulate(glm::value_ptr(camera.view), glm::value_ptr(camera.projection), ImGuizmo::TRANSLATE, ImGuizmo::WORLD, glm::value_ptr(newView));
}

void GLRenderSystem::CreateTexture(PEImage& img)
{
	std::shared_ptr<OpenGLImageGraphicsData> glImageGraphicsData = std::make_shared<OpenGLImageGraphicsData>();
	img.imageGraphicsData = glImageGraphicsData;
	glGenTextures(1, &glImageGraphicsData->texture);
	
}

void GLRenderSystem::LoadTexture(IRenderer* renderer, std::string imagePath)
{
	GLRenderer* glRenderer = (GLRenderer*)renderer;
	glRenderer->diffuseTexture = PEImage(imagePath);
	ImageSystem::CreateImage(glRenderer->diffuseTexture);
	CreateTexture(glRenderer->diffuseTexture);
	std::shared_ptr<OpenGLImageGraphicsData> glImageGraphicsData = std::dynamic_pointer_cast<OpenGLImageGraphicsData>(glRenderer->diffuseTexture.imageGraphicsData);
	glBindTexture(GL_TEXTURE_2D, glImageGraphicsData->texture);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#ifdef __EMSCRIPTEN__
	// Power of two check source: https://www.geeksforgeeks.org/cpp-program-to-find-whether-a-no-is-power-of-two/
	if (ceil(log2(glRenderer->diffuseTexture.width)) == floor(log2(glRenderer->diffuseTexture.width)) && ceil(log2(glRenderer->diffuseTexture.height)) == floor(log2(glRenderer->diffuseTexture.height)))
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // This is required on WebGL for non power-of-two textures
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Same
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
#else
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
#endif

	int imgFormat = (glRenderer->diffuseTexture.numChannels > 3) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, imgFormat, glRenderer->diffuseTexture.width, glRenderer->diffuseTexture.height, 0, imgFormat, GL_UNSIGNED_BYTE, glRenderer->diffuseTexture.data);
	glGenerateMipmap(GL_TEXTURE_2D);

	ImageSystem::DestroyImage(glRenderer->diffuseTexture);
}

void GLRenderSystem::LoadTexture(PEImage& img)
{
	CreateTexture(img);
	std::shared_ptr<OpenGLImageGraphicsData> glImageGraphicsData = std::dynamic_pointer_cast<OpenGLImageGraphicsData>(img.imageGraphicsData);
	glBindTexture(GL_TEXTURE_2D, glImageGraphicsData->texture);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#ifdef __EMSCRIPTEN__
	// Power of two check source: https://www.geeksforgeeks.org/cpp-program-to-find-whether-a-no-is-power-of-two/
	if (ceil(log2(img.width)) == floor(log2(img.width)) && ceil(log2(img.height)) == floor(log2(img.height)))
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // This is required on WebGL for non power-of-two textures
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Same
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
#else
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
#endif

	int imgFormat = (img.numChannels > 3) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, imgFormat, img.width, img.height, 0, imgFormat, GL_UNSIGNED_BYTE, img.data);
	glGenerateMipmap(GL_TEXTURE_2D);

	ImageSystem::DestroyImage(img);
}

GLRenderSystem::GLRenderSystem(IPlatform* plat) : platform(plat)
{
}

void GLRenderSystem::BindTexture(GLRenderer& renderer)
{
	std::shared_ptr<OpenGLImageGraphicsData> glImageData = std::dynamic_pointer_cast<OpenGLImageGraphicsData>(renderer.diffuseTexture.imageGraphicsData);
	glBindTexture(GL_TEXTURE_2D, glImageData->texture);
}
