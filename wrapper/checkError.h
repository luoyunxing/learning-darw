#pragma once

//本checktrror类
//仅提供OpenGL函数的错误检查

#define GL_CALL(func)  func ; checkError();


void checkError();