#include"shader.h"


Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	//声明装入shader代码的string
	std::string vertexCode;
	std::string fragmentCode;
	//声明用于读取vs和fs文件的inFileStream
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	//检查文件读取
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		//打开文件
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		//将文件输入流输入到stringStream里面
		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		//关闭文件
		vShaderFile.close();
		fShaderFile.close();

		//把stringStream中的读取出来，转化到vertexCode中
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure& e)
	{
		std::cout << "Shader File Error:" << e.what() << std::endl;
	}

	const char* vertexShaderSource = vertexCode.c_str();
	const char* fragmentShaderSource = fragmentCode.c_str();

	//创建shader程序
	GLuint vertex, fragment;
	vertex = glCreateShader(GL_VERTEX_SHADER);
	fragment = glCreateShader(GL_FRAGMENT_SHADER);

	//将源代码输入shader程序
	glShaderSource(vertex, 1, &vertexShaderSource, NULL);
	glShaderSource(fragment, 1, &fragmentShaderSource, NULL);

	//执行shader代码编译
	glCompileShader(vertex);
	//检查编译是否正确
	checkShaderError(vertex, "COMPILE");

	glCompileShader(fragment);
	//检查编译是否正确
	checkShaderError(fragment, "COMPILE");

	//--------将两个Shader链接为一个program--------//

	//创建program外壳
	m_program = glCreateProgram();

	//将vertex和pragment放入program
	glAttachShader(m_program, vertex);
	glAttachShader(m_program, fragment);

	//执行program链接操作，形成最终可执行的shader程序
	glLinkProgram(m_program);
	//检查错误
	checkShaderError(m_program, "LINK");

	//清理
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

Shader::~Shader()
{

}

void Shader::begin()
{
	GL_CALL(glUseProgram(m_program));
}

void Shader::end()
{
	GL_CALL(glUseProgram(0));
}

void Shader::setFloat(const std::string& name, float value)
{
	//通过名称拿到uniform变量的位置location
	GLint location =GL_CALL(glGetUniformLocation(m_program, name.c_str()));

	//通过location更新uniform变量的值
	GL_CALL(glUniform1f(location, value));
}

void Shader::setColor(const std::string& name, float r, float g, float b)
{
	//通过名称拿到uniform变量的位置location
	GLint location = GL_CALL(glGetUniformLocation(m_program, name.c_str()));

	//通过location更新uniform变量的值
	GL_CALL(glUniform3f(location, r, g, b));
}

void Shader::setColorVector(const std::string& name, const float* values)
{
	//通过名称拿到uniform变量的位置location
	GLint location = GL_CALL(glGetUniformLocation(m_program, name.c_str()));

	//通过location更新uniform变量的值
	//第二个参数： 数组里面包括多少个向量vec3
	GL_CALL(glUniform3fv(location, 1, values));
}

void Shader::setInt(const std::string& name, int value)
{
	//通过名称拿到uniform变量的位置location
	GLint location = GL_CALL(glGetUniformLocation(m_program, name.c_str()));

	//通过location更新uniform变量的值
	GL_CALL(glUniform1i(location, value));
}

void Shader::checkShaderError(GLuint target, std::string type)
{

	int success = 0;   //检查变量
	char infoLog[1024];

	if (type == "COMPILE")
	{
		glGetShaderiv(target, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(target, 1024, NULL, infoLog);
			std::cout << "Error shader compile" << "\n" << infoLog << std::endl;
		}
	}
	else if (type == "LINK")
	{
		glGetProgramiv(target, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(target, 1024, NULL, infoLog);
			std::cout << "Error program link" << "\n" << infoLog << std::endl;
		}
	}
}