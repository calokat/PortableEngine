#include "GLRenderSystem.h"
#include <imgui.h>
#include <ImGuizmo.h>
#include "CameraSystem.h"
#include "OpenGLImageGraphicsData.h"
#include "ImageSystem.h"

IRenderer& GLRenderSystem::CreateRenderer(entt::registry& reg, entt::entity& e, ShaderType type)
{
	IRenderer& rendererRef = reg.emplace<GLRenderer>(e, platform->GetAssetManager()->GetAssetPath(typeToVertexPath[type]), platform->GetAssetManager()->GetAssetPath(typeToPixelPath[type]), type);
	return rendererRef;
}

void GLRenderSystem::Load(IRenderer* renderer, Camera& camera)
{
	GLRenderer* glRenderer = (GLRenderer*)renderer;
	glGenVertexArrays(1, &glRenderer->shaderProgram.vao.u);
	glBindVertexArray(glRenderer->shaderProgram.vao.u);
	glGenBuffers(1, &glRenderer->shaderProgram.vbo.u);
	glGenBuffers(1, &glRenderer->shaderProgram.ibo.u);
	glBindBuffer(GL_ARRAY_BUFFER, glRenderer->shaderProgram.vbo.u);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glRenderer->shaderProgram.ibo.u);
	glRenderer->shaderProgram.programID = glCreateProgram();
	glAttachShader(glRenderer->shaderProgram.programID, glRenderer->shaderProgram.vertex.GetId());
	glAttachShader(glRenderer->shaderProgram.programID, glRenderer->shaderProgram.pixel.GetId());
	glLinkProgram(glRenderer->shaderProgram.programID);
	glUseProgram(glRenderer->shaderProgram.programID);
	for (auto attrIt = glRenderer->shaderProgram.attributes.begin(); attrIt != glRenderer->shaderProgram.attributes.end(); ++attrIt)
	{
		glRenderer->shaderProgram.attributes[attrIt->first].value.u = glGetAttribLocation(glRenderer->shaderProgram.programID, attrIt->first);
	}
	for (auto uniIt = glRenderer->shaderProgram.uniforms.begin(); uniIt != glRenderer->shaderProgram.uniforms.end(); ++uniIt)
	{
		glRenderer->shaderProgram.uniforms[uniIt->first].value.s = glGetUniformLocation(glRenderer->shaderProgram.programID, uniIt->first);
	}

	glEnableVertexAttribArray(glRenderer->shaderProgram.attributes["in_position"].value.u);
	SetupAttribute(glRenderer->shaderProgram.attributes["in_position"].value.u, 3, GL_FLOAT, Vertex, Position);
	if (glRenderer->shaderProgram.propertyFlags & ShaderProgramProperties::Textured)
	{
		glEnableVertexAttribArray(glRenderer->shaderProgram.attributes["aTexCoord"].value.u);
		SetupAttribute(glRenderer->shaderProgram.attributes["aTexCoord"].value.u, 2, GL_FLOAT, Vertex, UV);
	}
}

void GLRenderSystem::BindRenderer(IRenderer* renderer)
{
	GLRenderer* glRenderer = (GLRenderer*)renderer;
	glBindVertexArray(glRenderer->shaderProgram.vao.u);
}

void GLRenderSystem::LoadMesh(IRenderer* renderer, Mesh& mesh)
{
	GLRenderer* glRenderer = (GLRenderer*)renderer;
	glBindVertexArray(glRenderer->shaderProgram.vao.u);
	glBufferData(GL_ARRAY_BUFFER, mesh.rawVertices.size() * sizeof(Vertex), mesh.rawVertices.data(), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.rawIndices.size() * sizeof(unsigned int), mesh.rawIndices.data(), GL_STATIC_DRAW);
	glRenderer->numVertices = mesh.rawVertices.size();
	glRenderer->numIndices = mesh.rawIndices.size();
}

void GLRenderSystem::Draw(IRenderer* renderer)
{
	GLRenderer* glRenderer = (GLRenderer*)renderer;
	glBindVertexArray(glRenderer->shaderProgram.vao.u);
	if (glRenderer->numVertices == 2)
	{
		glDrawElements(GL_LINES, glRenderer->numIndices, GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawElements(GL_TRIANGLES, glRenderer->numIndices, GL_UNSIGNED_INT, 0);
	}
}

void GLRenderSystem::DrawWireframe(IRenderer* renderer)
{
}

void GLRenderSystem::UpdateRenderer(IRenderer* renderer, Transform meshTransform, Camera camera)
{
	GLRenderer* glRenderer = (GLRenderer*)renderer;
	glUseProgram(glRenderer->shaderProgram.programID);
	if (glRenderer->shaderProgram.propertyFlags & ShaderProgramProperties::Fully_3D)
	{
		glUniformMatrix4fv(glRenderer->shaderProgram.uniforms["view"].value.u, 1, GL_FALSE, glm::value_ptr(camera.view));
		glUniformMatrix4fv(glRenderer->shaderProgram.uniforms["projection"].value.u, 1, GL_FALSE, glm::value_ptr(camera.projection));
		glUniformMatrix4fv(glRenderer->shaderProgram.uniforms["model"].value.u, 1, GL_FALSE, glm::value_ptr(meshTransform.worldMatrix));
	}
	if (glRenderer->shaderProgram.propertyFlags & ShaderProgramProperties::Colored)
	{
		glUniform4f(glRenderer->shaderProgram.uniforms["in_color"].value.u, glRenderer->vertexColor.x, glRenderer->vertexColor.y, glRenderer->vertexColor.z, glRenderer->vertexColor.w);
	}
}

void GLRenderSystem::DrawGizmo(Camera camera)
{
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
