
#include "platform.h"

#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>

#include <iostream>

#include "shader/Shader.h"
#include "shader/ShaderUtil.h"
#include "math/Matrix.h"

#include "examples/mat4.h"

ShaderUtil shaderUtil;

reMat4 modelM, viewM, projM;
GLuint vertexHandle, indexHandle;
Shader shader;

mat4 model, view, projection;

char *shader_read( const char *filename )
{
    FILE * file;
    char * buffer;
    size_t size;

    file = fopen( filename, "rb" );
    if( !file )
    {
        fprintf( stderr, "Unable to open file \"%s\".\n", filename );
        return 0;
    }
    fseek( file, 0, SEEK_END );
    size = ftell( file );
    fseek(file, 0, SEEK_SET );
    buffer = (char *) malloc( (size+1) * sizeof( char *) );
    fread( buffer, sizeof(char), size, file );
    buffer[size] = 0;
    fclose( file );
    return buffer;
}

// ---------------------------------------------------------------- display ---
void display( void )
{
    static float theta=0, phi=0;
    static GLuint Color = 0;

    if( !Color )
    {
        Color = glGetUniformLocation( shader.getProgram(), "Color" );
    }

    theta += .5; phi += .5;

    int viewport[4];
    glGetIntegerv( GL_VIEWPORT, viewport );
    glClearColor(1,1,1,1);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glEnable( GL_POINT_SMOOTH );
    glEnable( GL_DEPTH_TEST );

    glutSolidCube(20);

    glUseProgram(shader.getProgram());

    {
        glUniform4f( Color, 0, 0, 0, 0.5 );

        glUniformMatrix4fv( glGetUniformLocation( shader.getProgram(), "model" ),
                            1, 0, model.data);
        glUniformMatrix4fv( glGetUniformLocation( shader.getProgram(), "view" ),
                            1, 0, view.data);
        glUniformMatrix4fv( glGetUniformLocation( shader.getProgram(), "projection" ),
                            1, 0, projection.data);
    }

    glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);

    glBindBuffer(GL_ARRAY_BUFFER, vertexHandle);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexHandle);

    shaderUtil.bindShader(&shader);

    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_INT, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glPopClientAttrib();

    glUseProgram( 0 );

    glutSwapBuffers( );
}


// ---------------------------------------------------------------- reshape ---
void reshape( int width, int height )
{
    glViewport(0, 0, width, height);

//    mat4_set_perspective(&projection, 120.0, 1.0, 2, 10);
//    mat4_set_translation(&view, 0, 0, 4);
    mat4_set_orthographic(&projection, -2, 2, -2, 2, -2, 3);
}


// --------------------------------------------------------------- keyboard ---
void keyboard( unsigned char key, int x, int y )
{
    if ( key == 27 )
    {
        exit( EXIT_SUCCESS );
    }
}


// ------------------------------------------------------------------ timer ---
void timer( int dt )
{
    glutPostRedisplay();
    glutTimerFunc( dt, timer, dt );
}

// ------------------------------------------------------------------- main ---
int main( int argc, char **argv )
{
    glutInit( &argc, argv );
    glutInitWindowSize( 400, 400 );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    glutCreateWindow( argv[0] );
    glutReshapeFunc( reshape );
    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
    glutTimerFunc( 1000/60, timer, 1000/60 );

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf( stderr, "Error: %s\n", glewGetErrorString(err) );
        exit( EXIT_FAILURE );
    }
    fprintf( stderr, "Using GLEW %s\n", glewGetString(GLEW_VERSION) );


    typedef struct { float x,y,z;} xyz;
    typedef struct { float r,g,b,a;} rgba;
    typedef struct { xyz position, normal; rgba color;} vertex;
    xyz v[] = { { 1, 1, 1},  {-1, 1, 1},  {-1,-1, 1}, { 1,-1, 1},
                { 1,-1,-1},  { 1, 1,-1},  {-1, 1,-1}, {-1,-1,-1} };
    xyz n[] = { { 0, 0, 1},  { 1, 0, 0},  { 0, 1, 0} ,
                {-1, 0, 1},  { 0,-1, 0},  { 0, 0,-1} };
    rgba c[] = { {1, 1, 1, 1},  {1, 1, 0, 1},  {1, 0, 1, 1},  {0, 1, 1, 1},
                 {1, 0, 0, 1},  {0, 0, 1, 1},  {0, 1, 0, 1},  {0, 0, 0, 1} };

    vertex vertices[24] =  {
      {v[0],n[0],c[0]}, {v[1],n[0],c[1]}, {v[2],n[0],c[2]}, {v[3],n[0],c[3]},
      {v[0],n[1],c[0]}, {v[3],n[1],c[3]}, {v[4],n[1],c[4]}, {v[5],n[1],c[5]},
      {v[0],n[2],c[0]}, {v[5],n[2],c[5]}, {v[6],n[2],c[6]}, {v[1],n[2],c[1]},
      {v[1],n[3],c[1]}, {v[6],n[3],c[6]}, {v[7],n[3],c[7]}, {v[2],n[3],c[2]},
      {v[7],n[4],c[7]}, {v[4],n[4],c[4]}, {v[3],n[4],c[3]}, {v[2],n[4],c[2]},
      {v[4],n[5],c[4]}, {v[7],n[5],c[7]}, {v[6],n[5],c[6]}, {v[5],n[5],c[5]} };

    GLuint indices[24] = { 0, 1, 2, 3,    4, 5, 6, 7,   8, 9,10,11,
                           12,13,14,15,  16,17,18,19,  20,21,22,23 };

//    cube = vertex_buffer_new( "vertex:3f,normal:3f,color:4f" );
//    vertex_buffer_push_back( cube, vertices, 24, indices, 24 );

    // vertex buffer
    glGenBuffers(1, &vertexHandle);

    glBindBuffer(GL_ARRAY_BUFFER, vertexHandle);

    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(vertex), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // index buffer
    glGenBuffers(1, &indexHandle);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexHandle);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 24 * 4, indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // upload shader
    std::string shader_dir = "../RacingEngineC++/examples/shaders/";

    char *vertShader = shader_read((shader_dir + "cube.vert").c_str());
    char *fragShader = shader_read((shader_dir + "cube.frag").c_str());

    std::cout << "vert : " << vertShader << "frag : " << fragShader << std::endl;

    shader.setVertexSource(vertShader);
    shader.setFragmentSource(fragShader);

    shaderUtil.compileShader(&shader);

    // set vertex attribute
    Attribute *vertAttr = shader.getAttribute("vertex");
    vertAttr->setType(ATTR_FORMAT_FLOAT);
    vertAttr->setSize(3);
    vertAttr->setStride((3 + 3 + 4) * 4);
    vertAttr->setOffset(0);

    Attribute *colorAttr = shader.getAttribute("color");
    colorAttr->setType(ATTR_FORMAT_FLOAT);
    colorAttr->setSize(4);
    colorAttr->setStride((3 + 3 + 4) * 4);
    colorAttr->setOffset((3 + 3) * 4);

    // set uniform value
    shader.getUniform("model")->setData((ubyte *)&modelM);
    shader.getUniform("view")->setData((ubyte *)&viewM);
    shader.getUniform("projection")->setData((ubyte *)&projM);

    std::cout << " vertID: " << shader.getVertexShader() << " fragID: " << shader.getFragmentShader() << std::endl;

//    std::cout << " vertAtt: " << shader.getAttributes() << " uniforms: " << shader.getUniforms() << std::endl;


    mat4_set_identity(&projection);
    mat4_set_identity(&model);
    mat4_set_identity(&view);

    mat4_set_rotation(&model, 45.0, 0, 0 , 1);

    glutMainLoop( );
    return EXIT_SUCCESS;
}
