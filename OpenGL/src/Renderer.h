#pragma once

#include <GL/glew.h>

/**
 * \brief Defines the number of vertices per triangle
 */
#define TRIANGLE_VERTS 3

 /**
  * \brief Defines the size of each vertex
  */
#define VERT_SIZE 2

  /**
   * \brief Calls __debugbreak() when function return is "false"\n
   * \param x function call being asserted
   */
#define ASSERT(x) if (!(x)) __debugbreak()

   /**
	* \brief Clears GL Error flags before calling function "x" from the GL library.\n
	* If function "x" sets a new error flag a "__debugbreak()" call will be executed.\n
	* \param x GL function call being checked for error conditions
	*/
#define GL_CALL(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, const unsigned int line);