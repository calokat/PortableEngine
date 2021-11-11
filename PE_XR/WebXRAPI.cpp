//#ifdef __EMSCRIPTEN__
#include "WebXRAPI.h"
#include "GLRenderer.h"
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>

WebXRAPI* WebXRAPI::staticThis;

WebXRAPI::WebXRAPI(IGraphicsAPI* graph, IRenderSystem* rs)
{
	this->graphics = graph;
	this->renderSystem = rs;
	//glGenTextures(2, viewTextures);
	WebXR_Eyebuffer_Init(eyeBuffer);
	WebXR_Eyebuffer_Init(blankBuffer);
	if (staticThis == nullptr)
	{
		staticThis = this;
	}
}

void WebXRAPI::PollEvents()
{

}

bool WebXRAPI::IsSessionRunning()
{
	return isSessionRunning;
}

void WebXRAPI::Frame(entt::registry& reg, IRenderSystem* renderSystem)
{
	//GLfloat red[] = { 1, 0, 0, 1 };
	//glClearBufferfv(GL_COLOR, 0, red);
	//glClearBufferfi(GL_DEPTH_STENCIL, 0, 1, 1);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, this->blankBuffer.frameBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->eyeBuffer.frameBuffer);
	glBlitFramebuffer(0, 0, this->eyeWidth * 2, this->eyeHeight, 0, 0, this->eyeWidth * 2, this->eyeHeight, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);
	for (int i = 0; i < 2; ++i)
	{
		glViewport(eyeWidth * i, 0, eyeWidth, eyeHeight);
		RenderEye(reg, renderSystem, viewCameras[i], eyeBuffer, i);
	}
}

void WebXRAPI::RenderEye(entt::registry& reg, IRenderSystem* renderSystem, Camera& viewCam, WebXR_Eyebuffer& eyeBuffer, unsigned int index)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, eyeBuffer.frameBuffer);
	auto renderableView = reg.view<GLRenderer, Transform>();
	//glViewport(eyeWidth * index, 0, eyeWidth * (index + 1), eyeHeight);
	//glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//entt::entity cameraEntity = reg.view<Camera, Transform>().front();
	//Camera& viewCam = reg.get<Camera>(cameraEntity);
	//graphics->ClearScreen();
	for (auto renderable : renderableView)
	{
		GLRenderer& renderer = reg.get<GLRenderer>(renderable);
		Mesh& mesh = reg.get<Mesh>(renderable);
		Transform& meshTransform = reg.get<Transform>(renderable);
		//renderSystem->LoadMesh(&renderer, mesh);
		renderSystem->BindRenderer(&renderer);

		renderSystem->UpdateRenderer(&renderer, meshTransform, viewCam);
		renderSystem->Draw(&renderer);
	}


	//ImGui::Render();
	//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

extern "C" {

	void webxr_clear_framebuffer()
	{
		GLenum colorAttachment = GL_COLOR_ATTACHMENT0;
		glDrawBuffers(1, &colorAttachment);
		glClearColor(1, 0, 0, 1);
		glClearDepth(1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void webxr_set_eye_measurements(int w, int h)
	{
		if (WebXRAPI::staticThis->eyeWidth != 0 && WebXRAPI::staticThis->eyeHeight != 0) return;
		WebXRAPI::staticThis->eyeWidth = w;
		WebXRAPI::staticThis->eyeHeight = h;
		WebXR_Eyebuffer_AllocateRenderbuffers(WebXRAPI::staticThis->blankBuffer, w, h);
		WebXR_Eyebuffer_BindRenderBuffersToFrameBuffer(WebXRAPI::staticThis->blankBuffer);
		WebXR_Eyebuffer_AllocateRenderbuffers(WebXRAPI::staticThis->eyeBuffer, w, h);
		WebXR_Eyebuffer_BindRenderBuffersToFrameBuffer(WebXRAPI::staticThis->eyeBuffer);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			printf("I screwed up the framebuffer!\n");
		}
		else
		{
			WebXRAPI::staticThis->isSessionRunning = true;
		}
	}

	void webxr_render_scene()
	{
		//WebXRAPI::staticThis->graphics->ClearScreen();
		glBindFramebuffer(GL_READ_FRAMEBUFFER, WebXRAPI::staticThis->eyeBuffer.frameBuffer);
		int width = WebXRAPI::staticThis->eyeWidth;
		int height = WebXRAPI::staticThis->eyeHeight;
		glBlitFramebuffer(0, 0, width * 2, height, 0, 0, width * 2, height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);
		
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0, 0, width * 2, height, 0, 0, width * 2, height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);

		//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		//glBindFramebuffer(GL_READ_FRAMEBUFFER, WebXRAPI::staticThis->eyeBuffer.frameBuffer);
		//glBlitFramebuffer(0, 0, width * 2, height, 0, 0, width * 2, height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);
		//WebXRAPI::staticThis->graphics->_SwapBuffers();

		//GLenum colorAttachment = GL_COLOR_ATTACHMENT0;
		//glDrawBuffers(1, &colorAttachment);
		//glClearColor(1, 0, 0, 1);
		//glClearDepth(1);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	}
	
	void webxr_set_camera_matrices(float* matricesPtr)
	{
		Camera* viewCameras = WebXRAPI::staticThis->viewCameras;
		const int offset = sizeof(float) * 16;
		memcpy(glm::value_ptr(viewCameras[0].view), matricesPtr, offset);
		memcpy(glm::value_ptr(viewCameras[0].projection), matricesPtr + 16, offset);

		memcpy(glm::value_ptr(viewCameras[1].view), matricesPtr + (16 * 2), offset);
		memcpy(glm::value_ptr(viewCameras[1].projection), matricesPtr + (16 * 3), offset);

		
		//for (int i = 0; i < 4; ++i)
		//{
		//	printf("View 0 [%f %f %f %f]\n", viewCameras[0].view[i][0], viewCameras[0].view[i][1], viewCameras[0].view[i][2], viewCameras[0].view[i][3]);
		//}


		//for (int i = 0; i < 4; ++i)
		//{
		//	printf("Projection 0 [%f %f %f %f]\n", viewCameras[0].projection[i][0], viewCameras[0].projection[i][1], viewCameras[0].projection[i][2], viewCameras[0].projection[i][3]);
		//}

		//for (int i = 0; i < 4; ++i)
		//{
		//	printf("View 1 [%f %f %f %f]\n", viewCameras[1].view[i][0], viewCameras[1].view[i][1], viewCameras[1].view[i][2], viewCameras[1].view[i][3]);
		//}


		//for (int i = 0; i < 4; ++i)
		//{
		//	printf("Projection 1 [%f %f %f %f]\n", viewCameras[1].projection[i][0], viewCameras[1].projection[i][1], viewCameras[1].projection[i][2], viewCameras[1].projection[i][3]);
		//}

	}
}

void WebXR_Eyebuffer_Init(WebXR_Eyebuffer& buffer)
{
	glGenFramebuffers(1, &buffer.frameBuffer);
	glGenRenderbuffers(1, &buffer.colorBuffer);
	glGenRenderbuffers(1, &buffer.depthStencilBuffer);
}

void WebXR_Eyebuffer_AllocateRenderbuffers(WebXR_Eyebuffer buffer, unsigned int w, unsigned int h)
{
	glBindRenderbuffer(GL_RENDERBUFFER, buffer.colorBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA32F, w * 2, h);
	
	glBindRenderbuffer(GL_RENDERBUFFER, buffer.depthStencilBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w * 2, h);
}

void WebXR_Eyebuffer_BindRenderBuffersToFrameBuffer(WebXR_Eyebuffer buffer)
{
	glBindFramebuffer(GL_FRAMEBUFFER, buffer.frameBuffer);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, buffer.colorBuffer);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, buffer.depthStencilBuffer);
}
//#endif