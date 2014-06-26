
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

#include "fbx_render_test.h"

#include "Font/FreeTypeUtil.h"

//Globals

void display()
{
    //Clear all the buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);

    SceneManager::getInstance().renderScene();

    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 300, 400, 400);
}

void timer( int dt )
{
    updateMatrix(false);

    glutPostRedisplay();
    glutTimerFunc( dt, timer, dt );
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
    glutInitWindowSize(400, 700);
    glutCreateWindow("GL 3.3 Test");

    glutTimerFunc( 1000/60, timer, 1000/60 );

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

    TextureManager::getInstance().setImageLoader(new ImageLoader());

    initResource();

    camera.setViewport(400, 400);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;
}


void TestFont()
{
    auto fontFile = resDir + "Fonts/entsani.ttf";

    auto font = std::make_shared<Font>(330, fontFile.c_str());
    auto atlas = TextureAtlas::create();
    atlas->init(512, 512, 4);

    FreeTypeUtil::LoadGlyphs(atlas, font, L"Hello People我");

    atlas->upload();

    MeshPtr mesh = createBox(50, atlas->getTexture());
    InitMeshInHardward(mesh);

    box = std::make_shared<SceneNode>();
    AddMeshToNode(box, mesh);

    box->setLocalTranslation(Vec3(0, 0, 52));
    SceneManager::getInstance().addRootNode(box);
}
