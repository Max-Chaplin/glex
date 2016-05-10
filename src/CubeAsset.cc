#include "CubeAsset.h"

CubeAsset::CubeAsset(GLfloat x, GLfloat y, GLfloat z): model_matrix(glm::mat4(1.0)){
  // model coordinates, origin at centre.
  GLfloat vertex_buffer [] {
      -0.5f+x, -0.5f+y, -0.5f+z  //0
    , -0.5f+x,  0.5f+y, -0.5f+z  //1
    ,  0.5f+x, -0.5f+y, -0.5f+z  //2
    ,  0.5f+x,  0.5f+y, -0.5f+z  //3
	
    , -0.5f+x, -0.5f+y,  0.5f+z  //4
    , -0.5f+x,  0.5f+y,  0.5f+z  //5
    ,  0.5f+x, -0.5f+y,  0.5f+z  //6
    ,  0.5f+x,  0.5f+y,  0.5f+z  //7
  };
  GLfloat vertex_buffer_length = sizeof(vertex_buffer);
  

  GLuint element_buffer []  {
    0, 1, 2
      , 1, 3, 2//front
      
      , 1, 5, 7
      , 1, 7, 3//top

      , 3, 6, 2
      , 3, 7, 6//right

      , 7, 4, 6
      , 7, 5, 4//back
                  
      , 5, 1, 0
      , 5, 4, 0//left

      , 6, 4, 0
      , 6, 2, 0//bottom
  };
  element_buffer_length = sizeof(element_buffer);

  // Transfer buffers to the GPU
  //

  // create buffer
  glGenBuffers(1, &vertex_buffer_token);

  // immediately bind the buffer and transfer the data
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_token);
  glBufferData(GL_ARRAY_BUFFER, vertex_buffer_length, vertex_buffer, GL_STATIC_DRAW);

  glGenBuffers(1, &element_buffer_token);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_token);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, element_buffer_length, element_buffer, GL_STATIC_DRAW);
}

CubeAsset::~CubeAsset() {
}

#ifdef DEBUG
#define checkGLError() checkError(__FILE__, __LINE__)
#else
// define symbol to be nothing
#define checkGLError()
#endif

void checkError(std::string file, int line) {
  GLenum gl_error = glGetError();
  if(GL_NO_ERROR != gl_error) {
    std::cerr << "GL error in " << file << " at line " << line << " error: " << gl_error << std::endl;
    exit(-1);
  }
}

void CubeAsset::Draw(GLuint program_token) {
  if(!glIsProgram(program_token)) {
    std::cerr << "Drawing Cube with invalid program" << std::endl;
    return;
  }
  GLint validation_ok;
  glValidateProgram(program_token);
  glGetProgramiv(program_token, GL_VALIDATE_STATUS, &validation_ok);
  if(!validation_ok) {
    GLint maxLength = 0;
    glGetProgramiv(program_token, GL_INFO_LOG_LENGTH, &maxLength);

    //The maxLength includes the NULL character
    std::vector<char> errorLog(maxLength);
    glGetProgramInfoLog(program_token, maxLength, &maxLength, &errorLog[0]);

    std::cerr << "Invalid program " << program_token << " with error code " << validation_ok << std::endl;
    for(auto c: errorLog) {
      std::cerr << c;
    }
    exit(-1);
  }

  GLuint position_attrib = glGetAttribLocation(program_token, "position");
  GLuint model_uniform = glGetUniformLocation(program_token, "model");
  checkGLError();

  glUseProgram(program_token);
  checkGLError();

  glUniformMatrix4fv(model_uniform, 1, false, glm::value_ptr(model_matrix));
  rotate_x(45.0f);
  
  // use the previously transferred buffer as the vertex array.  This way
  // we transfer the buffer once -- at construction -- not on every frame.
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_token);
  glVertexAttribPointer(
                        position_attrib,               /* attribute */
                        3,                             /* size */
                        GL_FLOAT,                      /* type */
                        GL_FALSE,                      /* normalized? */
                        0,                             /* stride */
                        (void*)0                       /* array buffer offset */
                        );
  glEnableVertexAttribArray(position_attrib);

  checkGLError();

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_token);
  glDrawElements(
                 GL_TRIANGLES,
                 element_buffer_length,
                 GL_UNSIGNED_INT,
                 (GLvoid*) 0
                 );

  checkGLError();

  glDisableVertexAttribArray(position_attrib);
}

void CubeAsset::rotate_x(float angle) {

    glm::vec3 unit_x_axis(1.0,0.0,0.0);
    glm::vec3 unit_y_axis(0.0,1.0,0.0);
    glm::mat4 id(model_matrix);
    model_matrix = glm::rotate(id, 0.1f, unit_x_axis);
  }
