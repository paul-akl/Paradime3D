#include "OS_Layer.h"

namespace OS
{
	SDL_Event sdlEvent; // variable to detect SDL events
	SDL_Window *sdlWindow; 
	SDL_GLContext sdlContext;
	
			float vertical, horizontal;

	void initValues()
	{
		int numVideoDisplays = SDL_GetNumVideoDisplays();
		if(numVideoDisplays < 0)
			Message::show(MSG_ERROR, MSG_ENGINE, SDL_GetError());
		else
			if(numVideoDisplays < Config::window::default_display - 1)
				Config::window::default_display = numVideoDisplays;

		float textureAnisotropicFilterMax;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &textureAnisotropicFilterMax);
		if(Config::texture::gl_texture_anisotropy > textureAnisotropicFilterMax)
			Config::texture::gl_texture_anisotropy = (int)textureAnisotropicFilterMax;

		SDL_DisplayMode currentDisplayMode;
		SDL_GetCurrentDisplayMode(Config::window::default_display, &currentDisplayMode);	// Retrieve current maximum screen resolution, used to correct resolution from Config file
		
		Message::show(MSG_INFO, MSG_ENGINE, "Number of screens: " + Message::toString(numVideoDisplays) +  ".");
		Message::show(MSG_INFO, MSG_ENGINE, "Maximum screen resolution: " + Message::toString(currentDisplayMode.w) + "x" + Message::toString(currentDisplayMode.h) + ".");

		if(Config::window::size_fullscreen_x > currentDisplayMode.w)
			Config::window::size_fullscreen_x = currentDisplayMode.w;

		if(Config::window::size_fullscreen_y > currentDisplayMode.h)
			Config::window::size_fullscreen_y = currentDisplayMode.h;

		if(Config::window::size_windowed_x > currentDisplayMode.w)
			Config::window::size_windowed_x = currentDisplayMode.w;

		if(Config::window::size_windowed_y > currentDisplayMode.h)
			Config::window::size_windowed_y = currentDisplayMode.h;

		if(Config::window::fullscreen)
		{
			Config::currentScreenWidth = Config::window::size_fullscreen_x; 
			Config::currentScreenHeight = Config::window::size_fullscreen_y;
		}
		else
		{
			Config::currentScreenWidth = Config::window::size_windowed_x; 
			Config::currentScreenHeight = Config::window::size_windowed_y;
		}
	}
	void setupRC()
	{
		if (SDL_Init(SDL_INIT_VIDEO) < 0) // Initialize video
			Message::show(MSG_FATAL_ERROR, MSG_ENGINE, "Unable to initialize SDL");
		
		initValues();

		// If you request a context not supported by your drivers, no OpenGL context will be created
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, Config::engine::gl_context_major_version);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, Config::engine::gl_context_minor_version);

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // double buffering on
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8); // 8 bit alpha buffering
		
		// Enabling multisampling
		if(Config::engine::multisampling)
			glEnable(GL_MULTISAMPLE);

		// Turn on multisampling anti-aliasing (MSAA)
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, Config::engine::multisample_buffers);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, Config::engine::multisample_samples);
		
		// Create a window
		sdlWindow = SDL_CreateWindow(Config::window::name.c_str(),
			Config::window::position_x, Config::window::position_y,
		    Config::currentScreenWidth, Config::currentScreenHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
		
		if (!sdlWindow) // Check if the window has been created
			Message::show(MSG_FATAL_ERROR, MSG_ENGINE, "Unable to create window");
		else
			Message::show(MSG_INFO, MSG_ENGINE, "OpenGL context and SDL window has been created.");
	 
		sdlContext = SDL_GL_CreateContext(sdlWindow); // Create opengl context and attach to window
		
		if(Config::engine::depth_test)
		{
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(Config::engine::depth_test_func);
		}
		else
			Message::show(MSG_WARNING, MSG_ENGINE, "Depth test is disabled.");

		resetFaceCulling();

		setVerticalSync(Config::engine::vertical_sync); // Set swap buffers to sync with monitor's vertical refresh rate

		if(Config::window::mouse_clipped) // Display cursor if mouse clipping is turned on, and vice versa
			setMouseDisplay(false);
		else
			setMouseDisplay(true);
	}
	void handleEvents()
	{
		while (SDL_PollEvent(&sdlEvent)) 
		{
			if (sdlEvent.type == SDL_QUIT)
				Config::running = false;
			else
				handleSDLEvent(sdlEvent);
		}
	}
	void handleSDLEvent(SDL_Event const &sdlEvent)
	{
		switch(sdlEvent.type)
		{

		case SDL_WINDOWEVENT:

			switch(sdlEvent.window.event)
			{
			case SDL_WINDOWEVENT_RESIZED: // Remember window size, and take care of OpenGL View Port, when resized
				
				Config::resizeWindow = true;
				Config::window::size_windowed_x = sdlEvent.window.data1;	Config::currentScreenWidth = Config::window::size_windowed_x;
				Config::window::size_windowed_y = sdlEvent.window.data2;	Config::currentScreenHeight = Config::window::size_windowed_y;
				glViewport(0, 0, Config::currentScreenWidth, Config::currentScreenHeight);

				Message::show(MSG_INFO, MSG_ENGINE, "Window has been resized to: " + Message::toString(Config::currentScreenWidth) + "x" + Message::toString(Config::currentScreenHeight));

				break;

			case SDL_WINDOWEVENT_MOVED: // Remember window position

				Config::window::position_x = sdlEvent.window.data1;
				Config::window::position_y = sdlEvent.window.data2;
				
				break;
			}

		break;

		case SDL_MOUSEMOTION:
			
			Current::inputState->mouseEvent = true;

			Current::inputState->mousePosX = sdlEvent.motion.x;
			Current::inputState->mousePosY = sdlEvent.motion.y;

		break;

		case SDL_KEYDOWN: // When a key has been pressed down, change it's status to 'true'

			if(sdlEvent.key.keysym.scancode == Current::inputState->forwardKey.scancode)
			{
				Current::inputState->forwardKey.status = true;
				return;
			}

			if(sdlEvent.key.keysym.scancode == Current::inputState->backwardKey.scancode)
			{
				Current::inputState->backwardKey.status = true;
				return;
			}
			
			if(sdlEvent.key.keysym.scancode == Current::inputState->leftKey.scancode)
			{
				Current::inputState->leftKey.status = true;
				return;
			}
			
			if(sdlEvent.key.keysym.scancode == Current::inputState->rightKey.scancode)
			{
				Current::inputState->rightKey.status = true;
				return;
			}
			
			if(sdlEvent.key.keysym.scancode == Current::inputState->jumpKey.scancode)
			{
				Current::inputState->jumpKey.status = true;
				Config::texture::default_specular_intensity = 10.0f;
				return;
			}
			
			if(sdlEvent.key.keysym.scancode == Current::inputState->fullscreenKey.scancode)
			{
				if(Config::window::fullscreen)
				{
					Config::currentScreenWidth = Config::window::size_windowed_x;
					Config::currentScreenHeight = Config::window::size_windowed_y;
					
					setFullscreen(false);
					setWindowSize(Config::currentScreenWidth, Config::currentScreenHeight);

					Message::show(MSG_INFO, MSG_ENGINE, "Fullscreen - OFF, and window has been resized to: " + Message::toString(Config::currentScreenWidth) + "x" + Message::toString(Config::currentScreenHeight));
				}
				else
				{
					Config::currentScreenWidth = Config::window::size_fullscreen_x;
					Config::currentScreenHeight = Config::window::size_fullscreen_y;
					
					setWindowSize(Config::currentScreenWidth, Config::currentScreenHeight);
					setFullscreen(true);

					Message::show(MSG_INFO, MSG_ENGINE, "Fullscreen - ON, and window has been resized to: " + Message::toString(Config::currentScreenWidth) + "x" + Message::toString(Config::currentScreenHeight));
				}

				Config::window::fullscreen = !Config::window::fullscreen;
				return;
			}
			
			if(sdlEvent.key.keysym.scancode == Current::inputState->vsyncKey.scancode)
			{
				Config::engine::vertical_sync = !Config::engine::vertical_sync;
				setVerticalSync(Config::engine::vertical_sync);
				return;
			}

			if(sdlEvent.key.keysym.scancode == Current::inputState->clipMouseKey.scancode)
			{
				if(Config::window::mouse_clipped)
					setMouseDisplay(true);
				else
					setMouseDisplay(false);

				Config::window::mouse_clipped = !Config::window::mouse_clipped;
				return;
			}
			
			if(sdlEvent.key.keysym.scancode == Current::inputState->debug1Key.scancode)
			{
				//Config::drawDebugBuffers = !Config::drawDebugBuffers;
				//Current::scene->lighting->directionalLight->direction.rotate(5.0, Math3d::Vec3f(0.0, 0.0, 1.0));
				Current::scene->lighting->directionalLight->direction.z += 0.5;
				std::cout << Current::scene->lighting->directionalLight->direction.x << " ; " << Current::scene->lighting->directionalLight->direction.y 
						  << " ; " << Current::scene->lighting->directionalLight->direction.z << std::endl;
				Current::scene->lighting->directionalLight->directionUpdated = true;
				return;
			}
			
			if(sdlEvent.key.keysym.scancode == Current::inputState->debug2Key.scancode)
			{
				Config::drawDebugBuffers = !Config::drawDebugBuffers;
				return;
			}
			
			if(sdlEvent.key.keysym.scancode == Current::inputState->arrowUpKey.scancode)
			{
				Current::scene->lighting->directionalLight->direction.x += 0.5;
				std::cout << Current::scene->lighting->directionalLight->direction.x << " ; " << Current::scene->lighting->directionalLight->direction.y 
						  << " ; " << Current::scene->lighting->directionalLight->direction.z << std::endl;
				Current::scene->lighting->directionalLight->directionUpdated = true;

				
				//Current::scene->lighting->spotLightPool[0]->direction.y += 1.5;
				return;
			}
			
			if(sdlEvent.key.keysym.scancode == Current::inputState->arrowDownKey.scancode)
			{
				Current::scene->lighting->directionalLight->direction.x -= 0.5;
				std::cout << Current::scene->lighting->directionalLight->direction.x << " ; " << Current::scene->lighting->directionalLight->direction.y 
						  << " ; " << Current::scene->lighting->directionalLight->direction.z << std::endl;
				Current::scene->lighting->directionalLight->directionUpdated = true;

				
				//Current::scene->lighting->spotLightPool[0]->direction.y -= 1.5;
				return;
			}
			
			if(sdlEvent.key.keysym.scancode == Current::inputState->arrowLeftKey.scancode)
			{
				Current::scene->lighting->directionalLight->direction.y -= 1.5;
				std::cout << Current::scene->lighting->directionalLight->direction.x << " ; " << Current::scene->lighting->directionalLight->direction.y 
						  << " ; " << Current::scene->lighting->directionalLight->direction.z << std::endl;
				Current::scene->lighting->directionalLight->directionUpdated = true;

				//Current::scene->lighting->spotLightPool[0]->direction.x -= 1.5;
				return;
			}
			
			if(sdlEvent.key.keysym.scancode == Current::inputState->arrowRightKey.scancode)
			{
				Current::scene->lighting->directionalLight->direction.y += 1.5;
				std::cout << Current::scene->lighting->directionalLight->direction.x << " ; " << Current::scene->lighting->directionalLight->direction.y 
						  << " ; " << Current::scene->lighting->directionalLight->direction.z << std::endl;
				Current::scene->lighting->directionalLight->directionUpdated = true;

				//Current::scene->lighting->spotLightPool[0]->direction.x += 1.5;
				return;
			}

			break;

		case SDL_KEYUP: // When a key has been pressed down, change it's status to 'false'
			
			if(sdlEvent.key.keysym.scancode == Current::inputState->forwardKey.scancode)
			{
				Current::inputState->forwardKey.status = false;
				return;
			}
			
			if(sdlEvent.key.keysym.scancode == Current::inputState->backwardKey.scancode)
			{
				Current::inputState->backwardKey.status = false;
				return;
			}
			
			if(sdlEvent.key.keysym.scancode == Current::inputState->leftKey.scancode)
			{
				Current::inputState->leftKey.status = false;
				return;
			}
			
			if(sdlEvent.key.keysym.scancode == Current::inputState->rightKey.scancode)
			{
				Current::inputState->rightKey.status = false;
				return;
			}
			
			if(sdlEvent.key.keysym.scancode == Current::inputState->jumpKey.scancode)
			{
				Current::inputState->jumpKey.status = false;
				return;
			}

			break;
		}
	}
	void swapBuffers()
	{
		SDL_GL_SwapWindow(sdlWindow);
	}

	void setWindowTitle(std::string windowTitle_arg)
	{
		SDL_SetWindowTitle(sdlWindow, windowTitle_arg.c_str());
	}
	void setWindowSize(int width_arg, int height_arg)
	{
		SDL_SetWindowSize(sdlWindow, width_arg, height_arg);
		glViewport(0, 0, width_arg, height_arg);
		Config::resizeWindow = true;
		std::cout << "Windows Size Set " << width_arg << "x" << height_arg << std::endl;
	}
	void setVerticalSync(bool vsync_arg)
	{
		SDL_GL_SetSwapInterval(vsync_arg); // Set swap buffers to sync with monitor's vertical refresh rate
	}
	void setFullscreen(bool fullscreen_arg)
	{
		if(fullscreen_arg)
			SDL_SetWindowFullscreen(sdlWindow, SDL_TRUE);
		else
			SDL_SetWindowFullscreen(sdlWindow, SDL_FALSE);
		Config::resizeWindow = true;
	}
	void setMouseDisplay(bool mouseDisplay_arg)
	{
		if(mouseDisplay_arg)
			SDL_ShowCursor(SDL_ENABLE);
		else
			SDL_ShowCursor(SDL_FALSE);
	}
	void resetMousePosition()
	{
		SDL_WarpMouseInWindow(sdlWindow, Config::currentScreenWidth / 2, Config::currentScreenHeight / 2);
	}
	void resetFaceCulling()
	{
		if(Config::engine::face_culling)
		{
			glEnable(GL_CULL_FACE);
			glCullFace(Config::engine::face_culling_mode);
		}
		else
			glDisable(GL_CULL_FACE);
	}

	void getWindowSize(int *width_arg, int *height_arg)
	{
		SDL_GetWindowSize(sdlWindow, width_arg, height_arg);
	}
	void testWindowSize()
	{
		int width_arg, height_arg;
		SDL_GetWindowSize(sdlWindow, &width_arg, &height_arg);
		std::cout << width_arg << "x" << height_arg << std::endl;
	}
}