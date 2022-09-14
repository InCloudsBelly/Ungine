#pragma once

#include "RenderCommandQueue.h"
#include "RendererAPI.h"

namespace U {

	class ShaderLibrary;

	class Renderer
	{
	public:
		typedef void(*RenderCommandFn)(void*);

		// Commands
		static void Clear();
		static void Clear(float r, float g, float b, float a = 1.0f);
		static void SetClearColor(float r, float g, float b, float a);

		static void DrawIndexed(uint32_t count, bool depthTest = true);

		static void ClearMagenta();

		static void Init();

		static const Scope<ShaderLibrary>& GetShaderLibrary() { return Get().m_ShaderLibrary; }

		static void* Submit(RenderCommandFn fn, unsigned int size)
		{
			return s_Instance->m_CommandQueue.Allocate(fn, size);
		}

		void WaitAndRender();

		inline static Renderer& Get() { return *s_Instance; }
	private:
		static Renderer* s_Instance;

		RenderCommandQueue m_CommandQueue;
		Scope<ShaderLibrary> m_ShaderLibrary;
	};

}

#define U_RENDER_PASTE2(a, b) a ## b
#define U_RENDER_PASTE(a, b) U_RENDER_PASTE2(a, b)
#define U_RENDER_UNIQUE(x) U_RENDER_PASTE(x, __LINE__)

#define U_RENDER(code) \
    struct U_RENDER_UNIQUE(HZRenderCommand) \
    {\
        static void Execute(void*)\
        {\
            code\
        }\
    };\
	{\
		auto mem = ::U::Renderer::Submit(U_RENDER_UNIQUE(HZRenderCommand)::Execute, sizeof(U_RENDER_UNIQUE(HZRenderCommand)));\
		new (mem) U_RENDER_UNIQUE(HZRenderCommand)();\
	}\

#define U_RENDER_1(arg0, code) \
	do {\
    struct U_RENDER_UNIQUE(HZRenderCommand) \
    {\
		U_RENDER_UNIQUE(HZRenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0) \
		: arg0(arg0) {}\
		\
        static void Execute(void* argBuffer)\
        {\
			auto& arg0 = ((U_RENDER_UNIQUE(HZRenderCommand)*)argBuffer)->arg0;\
            code\
        }\
		\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0;\
    };\
	{\
		auto mem = ::U::Renderer::Submit(U_RENDER_UNIQUE(HZRenderCommand)::Execute, sizeof(U_RENDER_UNIQUE(HZRenderCommand)));\
		new (mem) U_RENDER_UNIQUE(HZRenderCommand)(arg0);\
	} } while(0)

#define U_RENDER_2(arg0, arg1, code) \
    struct U_RENDER_UNIQUE(HZRenderCommand) \
    {\
		U_RENDER_UNIQUE(HZRenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1) \
		: arg0(arg0), arg1(arg1) {}\
		\
        static void Execute(void* argBuffer)\
        {\
			auto& arg0 = ((U_RENDER_UNIQUE(HZRenderCommand)*)argBuffer)->arg0;\
			auto& arg1 = ((U_RENDER_UNIQUE(HZRenderCommand)*)argBuffer)->arg1;\
            code\
        }\
		\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1;\
    };\
	{\
		auto mem = ::U::Renderer::Submit(U_RENDER_UNIQUE(HZRenderCommand)::Execute, sizeof(U_RENDER_UNIQUE(HZRenderCommand)));\
		new (mem) U_RENDER_UNIQUE(HZRenderCommand)(arg0, arg1);\
	}\

#define U_RENDER_3(arg0, arg1, arg2, code) \
    struct U_RENDER_UNIQUE(HZRenderCommand) \
    {\
		U_RENDER_UNIQUE(HZRenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2) \
		: arg0(arg0), arg1(arg1), arg2(arg2) {}\
		\
        static void Execute(void* argBuffer)\
        {\
			auto& arg0 = ((U_RENDER_UNIQUE(HZRenderCommand)*)argBuffer)->arg0;\
			auto& arg1 = ((U_RENDER_UNIQUE(HZRenderCommand)*)argBuffer)->arg1;\
			auto& arg2 = ((U_RENDER_UNIQUE(HZRenderCommand)*)argBuffer)->arg2;\
            code\
        }\
		\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2;\
    };\
	{\
		auto mem = ::U::Renderer::Submit(U_RENDER_UNIQUE(HZRenderCommand)::Execute, sizeof(U_RENDER_UNIQUE(HZRenderCommand)));\
		new (mem) U_RENDER_UNIQUE(HZRenderCommand)(arg0, arg1, arg2);\
	}\

#define U_RENDER_4(arg0, arg1, arg2, arg3, code) \
    struct U_RENDER_UNIQUE(HZRenderCommand) \
    {\
		U_RENDER_UNIQUE(HZRenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg3)>::type>::type arg3)\
		: arg0(arg0), arg1(arg1), arg2(arg2), arg3(arg3) {}\
		\
        static void Execute(void* argBuffer)\
        {\
			auto& arg0 = ((U_RENDER_UNIQUE(HZRenderCommand)*)argBuffer)->arg0;\
			auto& arg1 = ((U_RENDER_UNIQUE(HZRenderCommand)*)argBuffer)->arg1;\
			auto& arg2 = ((U_RENDER_UNIQUE(HZRenderCommand)*)argBuffer)->arg2;\
			auto& arg3 = ((U_RENDER_UNIQUE(HZRenderCommand)*)argBuffer)->arg3;\
            code\
        }\
		\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg3)>::type>::type arg3;\
    };\
	{\
		auto mem = Renderer::Submit(U_RENDER_UNIQUE(HZRenderCommand)::Execute, sizeof(U_RENDER_UNIQUE(HZRenderCommand)));\
		new (mem) U_RENDER_UNIQUE(HZRenderCommand)(arg0, arg1, arg2, arg3);\
	}

#define U_RENDER_S(code) auto self = this;\
	U_RENDER_1(self, code)

#define U_RENDER_S1(arg0, code) auto self = this;\
	U_RENDER_2(self, arg0, code)

#define U_RENDER_S2(arg0, arg1, code) auto self = this;\
	U_RENDER_3(self, arg0, arg1, code)

#define U_RENDER_S3(arg0, arg1, arg2, code) auto self = this;\
	U_RENDER_4(self, arg0, arg1, arg2, code)