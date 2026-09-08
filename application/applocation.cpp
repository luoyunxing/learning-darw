#include"application.h"

//创建单例类唯一变量
Application* Application::mInstance = nullptr;
Application* Application::getInstance()
{
	if (mInstance == nullptr)
	{
		mInstance = new Application();
	}
	return mInstance;
}


//构造函数
Application::Application()
{

}
//析构函数
Application::~Application()
{

}
//初始化
bool Application::init(const int & width , const int & height)
{
	m_Width = width;
	m_Height = height;

	//初始化GLFW基本环境
	glfwInit();
	//设置OpenGL主次版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	//设置OpenGL启用核心模式
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//创建窗体对象
	m_window = glfwCreateWindow(m_Width, m_Height, "win_1", NULL, NULL);
	if (m_window == nullptr)
	{
		return false;
	}
	//使用当前创建的窗体对象作为画框
	glfwMakeContextCurrent(m_window);

	//加载当前版本所有OpenGL函数
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	//设置窗体监听
	glfwSetFramebufferSizeCallback(m_window, frameBufferSizeCallBack);
	//设置键盘监听
	glfwSetKeyCallback(m_window, keyCallBack);

	return true;
}
//数据更新
bool Application::update()
{

	if (glfwWindowShouldClose(m_window))
	{
		return false;
	}

	//检查消息队列
	glfwPollEvents();

	//切换双缓存
	glfwSwapBuffers(m_window);

	return true;
}
//销毁
void Application::destroy()
{
	//结束清理
	glfwTerminate();
}

//窗体大小回调函数
void  Application::frameBufferSizeCallBack(GLFWwindow* window, int width, int  height)
{
	std::cout << width << height << std::endl;
	//调用外部函数
	if (Application::getInstance()->m_ResizeCallback != nullptr)
	{
		Application::getInstance()->m_ResizeCallback(width, height);
	}
}

//键盘响应回调函数
void Application::keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (Application::getInstance()->keyCallBack != nullptr)
	{
		Application::getInstance()->m_KeyBoradCallback(key, action, mods);
	}
}