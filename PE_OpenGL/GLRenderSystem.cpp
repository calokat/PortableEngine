#include "GLRenderSystem.h"
#include <imgui.h>
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
	for (int i = 0; i < (unsigned long)ShaderAttributes::__COUNT__; ++i)
	{
		glRenderer->shaderProgram.attributes[i].value = glGetAttribLocation(glRenderer->shaderProgram.programID, glRenderer->shaderProgram.attributes[i].name);
	}
	for (int i = 0; i < (unsigned long)VertexUniforms::__COUNT__; ++i)
	{
		glRenderer->shaderProgram.vertexUniforms[i].value = glGetUniformLocation(glRenderer->shaderProgram.programID, glRenderer->shaderProgram.vertexUniforms[i].name);
	}
	for (int i = 0; i < (unsigned long)FragmentUniforms::__COUNT__; ++i)
	{
		glRenderer->shaderProgram.fragmentUniforms[i].value = glGetUniformLocation(glRenderer->shaderProgram.programID, glRenderer->shaderProgram.fragmentUniforms[i].name);
	}
	for (int i = 0; i < (unsigned long)DirectionalLightUniforms::__COUNT__; ++i)
	{
		glRenderer->shaderProgram.dirLightLightData[i].value = glGetUniformLocation(glRenderer->shaderProgram.programID, glRenderer->shaderProgram.dirLightLightData[i].name);
	}
	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < (unsigned long)PointLightUniforms::__COUNT__; ++j)
		{
			glRenderer->shaderProgram.pointLightData[i][j].value = glGetUniformLocation(glRenderer->shaderProgram.programID, glRenderer->shaderProgram.pointLightData[i][j].name);
		}
	}
	glEnableVertexAttribArray(glRenderer->shaderProgram.attributes[(unsigned long)ShaderAttributes::Position].value);
	SetupAttribute(glRenderer->shaderProgram.attributes[(unsigned long)ShaderAttributes::Position].value, 3, GL_FLOAT, Vertex, Position);
	if (glRenderer->shaderProgram.propertyFlags & ShaderProgramProperties::Textured)
	{
		glEnableVertexAttribArray(glRenderer->shaderProgram.attributes[(unsigned long)ShaderAttributes::TextureCoordinate].value);
		SetupAttribute(glRenderer->shaderProgram.attributes[(unsigned long)ShaderAttributes::TextureCoordinate].value, 2, GL_FLOAT, Vertex, UV);
	}
	if (glRenderer->shaderProgram.propertyFlags & ShaderProgramProperties::Lit)
	{
		glEnableVertexAttribArray(glRenderer->shaderProgram.attributes[(unsigned long)ShaderAttributes::Normal].value);
		SetupAttribute(glRenderer->shaderProgram.attributes[(unsigned long)ShaderAttributes::Normal].value, 3, GL_FLOAT, Vertex, Normal);
	}
	if (glRenderer->shaderProgram.propertyFlags & ShaderProgramProperties::Normal)
	{
		glEnableVertexAttribArray(glRenderer->shaderProgram.attributes[(unsigned long)ShaderAttributes::Tangent].value);
		SetupAttribute(glRenderer->shaderProgram.attributes[(unsigned long)ShaderAttributes::Tangent].value, 3, GL_FLOAT, Vertex, Tangent);
	}
}

void GLRenderSystem::BindRenderer(IRenderer* renderer)
{
	GLRenderer* glRenderer = (GLRenderer*)renderer;
	glBindVertexArray(glRenderer->shaderProgram.vao.u);
	if (glRenderer->shaderProgram.propertyFlags & ShaderProgramProperties::Textured)
	{
		glActiveTexture(GL_TEXTURE0);
		OpenGLImageGraphicsData* glGraphicsData = (OpenGLImageGraphicsData*)glRenderer->textures[TextureType::DiffuseTexture].imageGraphicsData.get();
		glBindTexture(GL_TEXTURE_2D, glGraphicsData->texture);
	}
	if (glRenderer->shaderProgram.propertyFlags & ShaderProgramProperties::Normal)
	{
		glActiveTexture(GL_TEXTURE1);
		OpenGLImageGraphicsData* glGraphicsData = (OpenGLImageGraphicsData*)glRenderer->textures[TextureType::NormalTexture].imageGraphicsData.get();
		glBindTexture(GL_TEXTURE_2D, glGraphicsData->texture);
	}
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

void GLRenderSystem::UpdateRenderer(IRenderer* renderer, Transform meshTransform, Camera camera, DirectionalLight dirLight, PointLight pointLights[MAX_POINT_LIGHTS])
{
	GLenum err = glGetError();
	GLRenderer* glRenderer = (GLRenderer*)renderer;
	glUseProgram(glRenderer->shaderProgram.programID);
	if (glRenderer->shaderProgram.propertyFlags & ShaderProgramProperties::Fully_3D)
	{
		glUniformMatrix4fv(glRenderer->shaderProgram.vertexUniforms[(unsigned long)VertexUniforms::View].value, 1, GL_FALSE, glm::value_ptr(camera.view));
		glUniformMatrix4fv(glRenderer->shaderProgram.vertexUniforms[(unsigned long)VertexUniforms::Projection].value, 1, GL_FALSE, glm::value_ptr(camera.projection));
		glUniformMatrix4fv(glRenderer->shaderProgram.vertexUniforms[(unsigned long)VertexUniforms::Model].value, 1, GL_FALSE, glm::value_ptr(meshTransform.worldMatrix));
	}
	if (glRenderer->shaderProgram.propertyFlags & ShaderProgramProperties::Colored)
	{
		glUniform4f(glRenderer->shaderProgram.vertexUniforms[(unsigned long)VertexUniforms::Color].value, glRenderer->vertexColor.x, glRenderer->vertexColor.y, glRenderer->vertexColor.z, glRenderer->vertexColor.w);
	}
	err = glGetError();
	if (glRenderer->shaderProgram.propertyFlags & ShaderProgramProperties::Lit)
	{
		glUniform4f(glRenderer->shaderProgram.dirLightLightData[(unsigned long)DirectionalLightUniforms::AmbientColor].value, dirLight.AmbientColor.x, dirLight.AmbientColor.y, dirLight.AmbientColor.z, dirLight.AmbientColor.w);
		err = glGetError();
		glUniform4f(glRenderer->shaderProgram.dirLightLightData[(unsigned long)DirectionalLightUniforms::DiffuseColor].value, dirLight.DiffuseColor.x, dirLight.DiffuseColor.y, dirLight.DiffuseColor.z, dirLight.DiffuseColor.w);
		glUniform4f(glRenderer->shaderProgram.dirLightLightData[(unsigned long)DirectionalLightUniforms::Direction].value, dirLight.Direction.x, dirLight.Direction.y, dirLight.Direction.z, dirLight.Direction.w);
		glUniform4f(glRenderer->shaderProgram.pointLightData[0][(unsigned long)PointLightUniforms::AmbientColor].value, pointLights[0].AmbientColor.x, pointLights[0].AmbientColor.y, pointLights[0].AmbientColor.z, pointLights[0].AmbientColor.w);
		glUniform4f(glRenderer->shaderProgram.pointLightData[0][(unsigned long)PointLightUniforms::DiffuseColor].value, pointLights[0].DiffuseColor.x, pointLights[0].DiffuseColor.y, pointLights[0].DiffuseColor.z, pointLights[0].DiffuseColor.w);
		glUniform4f(glRenderer->shaderProgram.pointLightData[0][(unsigned long)PointLightUniforms::Position].value, pointLights[0].Position.x, pointLights[0].Position.y, pointLights[0].Position.z, pointLights[0].Position.w);
		glm::mat4 inverseView = glm::inverse(camera.view);
		glUniform3f(glRenderer->shaderProgram.fragmentUniforms[(unsigned long)FragmentUniforms::CameraPosition].value, inverseView[3].x, inverseView[3].y, inverseView[3].z);
		glUniform1f(glRenderer->shaderProgram.fragmentUniforms[(unsigned long)FragmentUniforms::SpecularIntensity].value, 16);
	}
	for (auto texIt = glRenderer->textures.begin(); texIt != glRenderer->textures.end(); ++texIt)
	{
		if (texIt->second.pathChanged)
		{
			LoadTexture(texIt->second, texIt->second.path);
			texIt->second.pathChanged = false;
		}
	}
	if (glRenderer->shaderProgram.propertyFlags & ShaderProgramProperties::Textured)
	{
		glUniform1i(glRenderer->shaderProgram.fragmentUniforms[(unsigned long)FragmentUniforms::DiffuseTextureID].value, 0);
	}
	if (glRenderer->shaderProgram.propertyFlags & ShaderProgramProperties::Normal)
	{
		if (glRenderer->shaderProgram.fragmentUniforms[(unsigned long)FragmentUniforms::NormalTextureID].value < 0)
		{
			glRenderer->shaderProgram.fragmentUniforms[(unsigned long)FragmentUniforms::NormalTextureID].value = glGetUniformLocation(glRenderer->shaderProgram.programID, "normalTexture");
		}
		glUniform1i(glRenderer->shaderProgram.fragmentUniforms[(unsigned long)FragmentUniforms::NormalTextureID].value, 1);
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

void GLRenderSystem::LoadTexture(IRenderer* renderer, std::map<TextureType, const char*> imagePaths)
{
	GLRenderer* glRenderer = (GLRenderer*)renderer;
	if (glRenderer->shaderProgram.propertyFlags & ShaderProgramProperties::Textured)
	{
		LoadTexture(glRenderer->textures[TextureType::DiffuseTexture], imagePaths[TextureType::DiffuseTexture]);
	}
	if (glRenderer->shaderProgram.propertyFlags & ShaderProgramProperties::Normal)
	{
		LoadTexture(glRenderer->textures[TextureType::NormalTexture], imagePaths[TextureType::NormalTexture]);
	}
}

GLRenderSystem::GLRenderSystem(IPlatform* plat) : platform(plat)
{
}

void GLRenderSystem::BindTexture(GLRenderer& renderer)
{
}

void GLRenderSystem::LoadTexture(PEImage& texture, std::string imagePath)
{
	
	texture = { imagePath };
	unsigned char* imageData = nullptr;
	bool createImageSuccess = ImageSystem::CreateImage(texture, &imageData);
	if (!createImageSuccess) return;
	CreateTexture(texture);
	OpenGLImageGraphicsData* glImageGraphicsData = (OpenGLImageGraphicsData*)texture.imageGraphicsData.get();
	glBindTexture(GL_TEXTURE_2D, glImageGraphicsData->texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

#ifdef __EMSCRIPTEN__
	// Power of two check source: https://www.geeksforgeeks.org/cpp-program-to-find-whether-a-no-is-power-of-two/
	if (ceil(log2(texture.width)) == floor(log2(texture.width)) && ceil(log2(texture.height)) == floor(log2(texture.height)))
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
	int imgFormat = (texture.numChannels > 3) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, imgFormat, texture.width, texture.height, 0, imgFormat, GL_UNSIGNED_BYTE, imageData);
	glGenerateMipmap(GL_TEXTURE_2D);

	ImageSystem::DestroyImageData(imageData);

}
