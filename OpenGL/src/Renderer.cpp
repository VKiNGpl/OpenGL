#include "Renderer.h"

#include <iostream>

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, const unsigned int line)
{
	if (const GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << "): " << function <<
			" " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}

void Renderer::Clear()
{
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const GLProgram& program)
{
	program.Bind();
	va.Bind();
	ib.Bind();

	GL_CALL(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}
