
//In this example, we will use a few IBO/VBO and a few VAO.
//We will use glDrawRangeElements to render some shapes.
//We will use a couple of simple shaders. GL 3.3 shaders.
//We will create a 3.3 forward context with freeGLUT.
//
//We are using GLEW to get function pointers.
//
//As for the VBO, we will use an interleaved vertex format.
//Vertex, texcoords and normals.
//Vertex and color.
//
//As for the IBO, we will use 16 bit unsigned integers.
//
//http://freeglut.sourceforge.net
//http://glew.sourceforge.net


#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "shader/ShaderUtil.h"

using namespace std;




#define BUFFER_OFFSET(i) ((void*)(i))


#define TRUE true
#define FALSE false


//Vertex, tex0
//
//SIZE : 4+4+4 +4+4 = 4*6 = 20 bytes
//It's better to make it multiple of 32
//32-20 = 12 bytes (of garbage should be added)
//12/4 = 3 floats should be added
struct TVertex_VT
{
        float   x, y, z;
        float   s0, t0;
        float   padding[3];
};

//Vertex, normal, tex0
//
//SIZE : 4+4+4 +4+4+4 +4+4 = 4*8 = 32 bytes
struct TVertex_VNT
{
        float   x, y, z;
        float   nx, ny, nz;
        float   s0, t0;
};

//Vertex, color
//
//SIZE : 4+4+4 +4 = 4*4 = 16 bytes
//It's better to make it multiple of 32
//32-16 = 16 bytes (of garbage should be added)
//16/4 = 4 floats should be added
struct TVertex_VC
{
        float   x, y, z;
        unsigned int    color;
        float   padding[4];
};




//Globals

//A quad
GLushort        pindex_quad[6];
TVertex_VC      pvertex_quad[4];

//A triangle
GLushort                pindex_triangle[3];
TVertex_VNT             pvertex_triangle[3];

//1 VAO for the quad
//1 VAO for the triangle
GLuint VAOID[2];
//1 IBO for the quad (Index Buffer Object)
//1 IBO for the triangle
GLuint IBOID[2];
//1 IBO for the quad (Vertex Buffer Object)
//1 IBO for the triangle
GLuint VBOID[2];

int ProjectionModelviewMatrix_Loc[2];           //The location of ProjectionModelviewMatrix in the shaders

ShaderUtil shaderUtil;
Shader shader;


// loadFile - loads text file into char* fname
// allocates memory - so need to delete after use
// size of file returned in fSize
std::string loadFile(const char *fname)
{
        std::ifstream file(fname);
        if(!file.is_open())
        {
                cout << "Unable to open file " << fname << endl;
                exit(1);
        }

        std::stringstream fileData;
        fileData << file.rdbuf();
        file.close();

        return fileData.str();
}


// printShaderInfoLog
// From OpenGL Shading Language 3rd Edition, p215-216
// Display (hopefully) useful error messages if shader fails to compile
void printShaderInfoLog(GLint shader)
{
        int infoLogLen = 0;
        int charsWritten = 0;
        GLchar *infoLog;

        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);

        if (infoLogLen > 0)
        {
                infoLog = new GLchar[infoLogLen];
                // error check for fail to allocate memory omitted
                glGetShaderInfoLog(shader, infoLogLen, &charsWritten, infoLog);
                cout << "InfoLog : " << endl << infoLog << endl;
                delete [] infoLog;
        }
}


void InitGLStates()
{
        glShadeModel(GL_SMOOTH);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glReadBuffer(GL_BACK);
        glDrawBuffer(GL_BACK);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glDepthMask(TRUE);
        glDisable(GL_STENCIL_TEST);
        glStencilMask(0xFFFFFFFF);
        glStencilFunc(GL_EQUAL, 0x00000000, 0x00000001);
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);
        glEnable(GL_CULL_FACE);
        glClearColor(1.0, 0.0, 0.0, 0.0);
        glClearDepth(1.0);
        glClearStencil(0);
        glDisable(GL_BLEND);
        glDisable(GL_ALPHA_TEST);
        glDisable(GL_DITHER);
        glActiveTexture(GL_TEXTURE0);
}



int LoadShader(const char *pfilePath_vs, const char *pfilePath_fs, bool bindTexCoord0, bool bindNormal, bool bindColor)
{
        // load shaders & get length of each
        int vlen;
        int flen;
        std::string vertexShaderString = loadFile(pfilePath_vs);
        std::string fragmentShaderString = loadFile(pfilePath_fs);
        vlen = vertexShaderString.length();
        flen = fragmentShaderString.length();

        shader.setVertexSource(vertexShaderString);
        shader.setFragmentSource(fragmentShaderString);

        shaderUtil.compileShader(&shader);

        GLint IsLinked;
        glGetProgramiv(shader.getProgram(), GL_LINK_STATUS, (GLint *)&IsLinked);
        if(IsLinked==FALSE)
        {
                cout << "Failed to link shader." << endl;

                GLint maxLength;
                glGetProgramiv(shader.getProgram(), GL_INFO_LOG_LENGTH, &maxLength);
                if(maxLength>0)
                {
                        char *pLinkInfoLog = new char[maxLength];
                        glGetProgramInfoLog(shader.getProgram(), maxLength, &maxLength, pLinkInfoLog);
                        cout << pLinkInfoLog << endl;
                        delete [] pLinkInfoLog;
                }

                return -1;
        }

        // set vertex attribute
        Attribute *vertAttr = shader.getAttribute("InVertex");
        vertAttr->setType(ATTR_FORMAT_FLOAT);
        vertAttr->setSize(3);
        vertAttr->setStride((3 + 1 + 4) * 4);
        vertAttr->setOffset(0);

        Attribute *colorAttr = shader.getAttribute("InColor");
        colorAttr->setType(ATTR_FORMAT_UBYTE);
        colorAttr->setSize(4);
        colorAttr->setStride((3 + 1 + 4) * 4);
        colorAttr->setOffset((3) * 4);
        colorAttr->setNormalized(true);

        return 1;               //Success
}

void CreateGeometry()
{
        //A quad
        pvertex_quad[0].x=-0.8f;
        pvertex_quad[0].y=-0.5f;
        pvertex_quad[0].z=-0.9f;
        pvertex_quad[0].color=0xFFFFFFFF;

        pvertex_quad[1].x=0.0f;
        pvertex_quad[1].y=-0.5f;
        pvertex_quad[1].z=-0.9f;
        pvertex_quad[1].color=0xFFFF0000;

        pvertex_quad[2].x=-0.8f;
        pvertex_quad[2].y=0.5f;
        pvertex_quad[2].z=-0.9f;
        pvertex_quad[2].color=0xFF00FF00;

        pvertex_quad[3].x=0.0f;
        pvertex_quad[3].y=0.5f;
        pvertex_quad[3].z=-0.9f;
        pvertex_quad[3].color=0xFF0000FF;

        pindex_quad[0]=0;
        pindex_quad[1]=1;
        pindex_quad[2]=2;
        pindex_quad[3]=2;
        pindex_quad[4]=1;
        pindex_quad[5]=3;


        //Create the IBO for the quad
        //16 bit indices
        glGenBuffers(1, &IBOID[0]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOID[0]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(GLushort), pindex_quad, GL_STATIC_DRAW);

        GLenum error=glGetError();

        //Create the IBO for the triangle
        //16 bit indices
        //We could have actually made one big IBO for both the quad and triangle.
        glGenBuffers(1, &IBOID[1]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOID[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3*sizeof(GLushort), pindex_triangle, GL_STATIC_DRAW);

        error=glGetError();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        error=glGetError();

        //Create VBO for the quad
        glGenBuffers(1, &VBOID[0]);
        glBindBuffer(GL_ARRAY_BUFFER, VBOID[0]);
        glBufferData(GL_ARRAY_BUFFER, 4*sizeof(TVertex_VC), pvertex_quad, GL_STATIC_DRAW);

        error=glGetError();

        //Just testing
        glBindBuffer(GL_ARRAY_BUFFER, 0);

//        //VAO for the quad *********************
//        glGenVertexArrays(1, &VAOID[0]);
//        glBindVertexArray(VAOID[0]);

//        //Bind the VBO and setup pointers for the VAO
//        glBindBuffer(GL_ARRAY_BUFFER, VBOID[0]);
//        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TVertex_VC), BUFFER_OFFSET(0));
//        glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(TVertex_VC), BUFFER_OFFSET(sizeof(float)*3));
//        glEnableVertexAttribArray(0);
//        glDisableVertexAttribArray(1);
//        glDisableVertexAttribArray(2);
//        glEnableVertexAttribArray(3);

//        //Bind the IBO for the VAO
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOID[0]);

        //Just testing
        glBindVertexArray(0);
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        glDisableVertexAttribArray(3);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void display()
{
        float projectionModelviewMatrix[16];

        //Just set it to identity matrix
        memset(projectionModelviewMatrix, 0, sizeof(float)*16);
        projectionModelviewMatrix[0]=1.0;
        projectionModelviewMatrix[5]=1.0;
        projectionModelviewMatrix[10]=1.0;
        projectionModelviewMatrix[15]=1.0;

        //Clear all the buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        //Bind the shader that we want to use
        glUseProgram(shader.getProgram());
        //Setup all uniforms for your shader
        ProjectionModelviewMatrix_Loc[0]=glGetUniformLocation(shader.getProgram(), "ProjectionModelviewMatrix");
        glUniformMatrix4fv(ProjectionModelviewMatrix_Loc[0], 1, FALSE, projectionModelviewMatrix);

        int vertexLocation = shader.getAttribute("InVertex")->getLocation();
        int colorLocation = shader.getAttribute("InColor")->getLocation();

        //Bind the VBO and setup pointers for the VAO
        glBindBuffer(GL_ARRAY_BUFFER, VBOID[0]);

//        glEnableVertexAttribArray(vertexLocation);
//        glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(TVertex_VC), BUFFER_OFFSET(0));

//        glEnableVertexAttribArray(colorLocation);
//        glVertexAttribPointer(colorLocation, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(TVertex_VC), BUFFER_OFFSET(sizeof(float)*3));

        shaderUtil.bindShader(&shader);

        //Bind the IBO for the VAO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOID[0]);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);

        glutSwapBuffers();
}

void reshape(int w, int h)
{
        glViewport(0, 0, w, h);
}

int main (int argc, char* argv[])
{
        int i;
        int NumberOfExtensions;
        int OpenGLVersion[2];

        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
        //We want to make a GL 3.3 context
        //glutInitContextVersion(3, 3);
        glutInitContextFlags(GLUT_CORE_PROFILE);
        glutInitWindowPosition(100, 50);
        glutInitWindowSize(600, 600);
        glutCreateWindow("GL 3.3 Test");

        //Currently, GLEW uses glGetString(GL_EXTENSIONS) which is not legal code
        //in GL 3.3, therefore GLEW would fail if we don't set this to TRUE.
        //GLEW will avoid looking for extensions and will just get function pointers for all GL functions.
        glewExperimental=TRUE;
        GLenum err=glewInit();
        if(err!=GLEW_OK)
        {
                //Problem: glewInit failed, something is seriously wrong.
                cout<<"glewInit failed, aborting."<<endl;
                exit(1);
        }

        //The old way of getting the GL version
        //This will give you something like 3.3.2895 WinXP SSE
        //where the 3.3 would be the version number and the rest is vendor
        //dependent information.
        //In this case, 2895 is a build number.
        //Then the OS : WinXP
        //Then CPU features such as SSE
        cout<<"OpenGL version = "<<glGetString(GL_VERSION)<<endl;

        //This is the new way for getting the GL version.
        //It returns integers. Much better than the old glGetString(GL_VERSION).
        glGetIntegerv(GL_MAJOR_VERSION, &OpenGLVersion[0]);
        glGetIntegerv(GL_MINOR_VERSION, &OpenGLVersion[1]);
        cout<<"OpenGL major version = "<<OpenGLVersion[0]<<endl;
        cout<<"OpenGL minor version = "<<OpenGLVersion[1]<<endl<<endl;

        //The old method to get the extension list is obsolete.
        //You must use glGetIntegerv and glGetStringi
        glGetIntegerv(GL_NUM_EXTENSIONS, &NumberOfExtensions);

        //We don't need any extensions. Useless code.
        for(i=0; i<NumberOfExtensions; i++)
        {
                const GLubyte *ccc=glGetStringi(GL_EXTENSIONS, i);
        }

        InitGLStates();

        if(LoadShader("../RacingEngineC++/examples/shaders/Shader1.vert", "../RacingEngineC++/examples/shaders/Shader1.frag", false, false, true)==-1)
        {
                exit(1);
        }
        else
        {

        }



        CreateGeometry();

        glutDisplayFunc(display);
        glutReshapeFunc(reshape);

        glutMainLoop();
        return 0;
}
