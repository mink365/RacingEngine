



	
set(EXAMPLES_SOURCE_FILES 
        ${RE_EXAMPLES_DIR}/Audio/AudioTest.cpp
        ${RE_EXAMPLES_DIR}/BaseTest.cpp
        ${RE_EXAMPLES_DIR}/Bullet/BulletTest.cpp
        ${RE_EXAMPLES_DIR}/Bumpmap/Bumpmap.cpp
        ${RE_EXAMPLES_DIR}/FBX/FBXTest.cpp
        ${RE_EXAMPLES_DIR}/FeatureTestsApp.cpp
        ${RE_EXAMPLES_DIR}/Font/FontTest.cpp
        ${RE_EXAMPLES_DIR}/Light/LightTest.cpp
        ${RE_EXAMPLES_DIR}/Lightmap/Lightmap.cpp
        ${RE_EXAMPLES_DIR}/MotoScene/SceneMaterialTest.cpp
        ${RE_EXAMPLES_DIR}/Reflection/Reflection.cpp
        ${RE_EXAMPLES_DIR}/Refraction/Refraction.cpp
        ${RE_EXAMPLES_DIR}/RenderToTexture/RenderToTextureTest.cpp
        ${RE_EXAMPLES_DIR}/Serialization/SerializationTest.cpp
        ${RE_EXAMPLES_DIR}/Shadow/ShadowTest.cpp
        ${RE_EXAMPLES_DIR}/ShapeGenerater.cpp
        ${RE_EXAMPLES_DIR}/Skybox/Skybox.cpp
        ${RE_EXAMPLES_DIR}/Tween/TweenTest.cpp
        ${RE_EXAMPLES_DIR}/UI/LayoutTest.cpp
        ${RE_EXAMPLES_DIR}/UI/MaskTest.cpp
        ${RE_EXAMPLES_DIR}/UI/ScrollViewTest.cpp
        ${RE_EXAMPLES_DIR}/UI/UITest.cpp
        )

set(EXAMPLES_HEADER_FILES 
        ${RE_EXAMPLES_DIR}/Audio/AudioTest.h 
        ${RE_EXAMPLES_DIR}/BaseTest.h 
        ${RE_EXAMPLES_DIR}/Bullet/BulletTest.h 
        ${RE_EXAMPLES_DIR}/Bumpmap/Bumpmap.h 
        ${RE_EXAMPLES_DIR}/FBX/FBXTest.h 
        ${RE_EXAMPLES_DIR}/FeatureTestsApp.h 
        ${RE_EXAMPLES_DIR}/Font/FontTest.h 
        ${RE_EXAMPLES_DIR}/Light/LightTest.h 
        ${RE_EXAMPLES_DIR}/Lightmap/Lightmap.h 
        ${RE_EXAMPLES_DIR}/MotoScene/SceneMaterialTest.h 
        ${RE_EXAMPLES_DIR}/Reflection/Reflection.h 
        ${RE_EXAMPLES_DIR}/Refraction/Refraction.h 
        ${RE_EXAMPLES_DIR}/RenderToTexture/RenderToTextureTest.h 
        ${RE_EXAMPLES_DIR}/Serialization/SerializationTest.h 
        ${RE_EXAMPLES_DIR}/Shadow/ShadowTest.h 
        ${RE_EXAMPLES_DIR}/ShapeGenerater.h 
        ${RE_EXAMPLES_DIR}/Skybox/Skybox.h 
        ${RE_EXAMPLES_DIR}/Tween/TweenTest.h 
        ${RE_EXAMPLES_DIR}/UI/LayoutTest.h 
        ${RE_EXAMPLES_DIR}/UI/MaskTest.h 
        ${RE_EXAMPLES_DIR}/UI/ScrollViewTest.h 
        ${RE_EXAMPLES_DIR}/UI/UITest.h 
        )

SOURCE_GROUP("Examples\\Source Files" FILES ${EXAMPLES_SOURCE_FILES})
SOURCE_GROUP("Examples\\Header Files" FILES ${EXAMPLES_HEADER_FILES})
 

add_executable(Examples 
        ${EXAMPLES_SOURCE_FILES}
	    ${EXAMPLES_HEADER_FILES} 
        )