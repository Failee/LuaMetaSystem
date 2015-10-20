#ifndef WINDOW_H
#define WINDOW_H

#include "extern/input.h"
#include "extern/gl.h"


	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	enum WindowOptions{
		RGB16 = 1 << 0,
		RGB24 = 1 << 1,
		RGB32 = 1 << 2,
		RGB64 = 1 << 3,

		ALPHA4 = 1 << 4,
		ALPHA8 = 1 << 5,
		ALPHA12 = 1 << 6,
		ALPHA16 = 1 << 7,

		STENCIL4 = 1 << 8,
		STENCIL8 = 1 << 9,
		STENCIL16 = 1 << 10,

		DEPTH8 = 1 << 11,
		DEPTH16 = 1 << 12,
		DEPTH24 = 1 << 13,
		DEPTH32 = 1 << 14,

		MULTISAMPLE2 = 1 << 15,
		MULTISAMPLE4 = 1 << 16,
		MULTISAMPLE8 = 1 << 17,
		MULTISAMPLE16 = 1 << 18,
		MULTISAMPLE32 = 1 << 19,

		FRAME_SYNC = 1 << 20,
        FULLSCREEN = 1 << 21,
		ADAPTIVE_FRAME_SYNC = 1 << 22,
		STEREO = 1 << 23,
		DOUBLE_BUFFER = 1 << 24,
        DEBUG_CONTEXT = 1 << 25,
		CORE_PROFILE = 1 << 26
	};
	
	class Window{
	public:
		Window(const char* _name, int32 _width, int32 _height, int32 _xpos, int32 _ypos, uint32 _options);
		Window() : m_inputHandler(new InputHandler()), m_initialized(false), m_active(false) {}

		~Window();

		virtual bool32 processAllMessages() const;
		virtual void swapBuffers() const { SwapBuffers(m_hdc); }

		virtual void windowResolution(int& _width, int& _height) const;
		virtual void desktopResolution(int& _width, int& _height) const;

		virtual bool32 isFullscreen() const;
		virtual bool32 setFullscreen(bool32 _fullscreen);
		virtual bool32 isMinimized() const { return IsIconic(m_hwnd); };
		virtual bool32 setMinimized(bool32 _minimized) { return ShowWindow(m_hwnd, (_minimized ? SW_MINIMIZE : SW_SHOW)); };

		bool32 initialized() const { return m_initialized; }
		bool32 active() const { return m_active; }
		void deactivate() { m_active = false; }

		InputHandler::ButtonState getMouseButton(InputHandler::MouseButton _button) const { return m_inputHandler->queryMouseButton(_button); }
		InputHandler::ButtonState getKey(char _character) const { return m_inputHandler->queryKeyboardButton(_character); };
		/**
		* Returns the Mouse Position in OpenGL Viewport Coordinates
		*/
		void getMousePos(float32& _mousePosX, float32& _mousePosY) const {
			int32 mw, mh;
			m_inputHandler->queryMousePosition(mw, mh);
			int32 w, h;
			windowResolution(w, h);
			_mousePosX = static_cast<float32>(mw) / static_cast<float32>(w);
			_mousePosY = 1.0f - static_cast<float32>(mh) / static_cast<float32>(h);
		};

		float64 elapsedTime() const{ return m_inputHandler->elapsedTime(); }

		InputHandler* inputHandler(){ return m_inputHandler; }
		const InputHandler* inputHandler() const { return m_inputHandler; }

	private:
		InputHandler* m_inputHandler;

		HDC m_hdc;
		HWND m_hwnd;
		WINDOWPLACEMENT m_windowPlacement;

		bool32 m_initialized;
		bool32 m_active;

		bool32 createWindow(TCHAR* _caption, TCHAR* _classname, ULONG _style, int32 _x, int32 _y, int32 _width, int32 _height);

		bool32 createOpenGLWindow(TCHAR* _name, int32 _width, int32 _height, int32 _xpos, int32 _ypos, uint32 _options);
	};
	



#endif WINDOW_H
