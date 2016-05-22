

set(PRE_HEADER_FILES
        ${RE_EXAMPLES_SOURCE_DIR}/BaseTest.cpp
        ${RE_EXAMPLES_SOURCE_DIR}/BaseTest.h
        ${RE_EXAMPLES_SOURCE_DIR}/FeatureTestsApp.cpp
        ${RE_EXAMPLES_SOURCE_DIR}/FeatureTestsApp.h
        ${RE_EXAMPLES_SOURCE_DIR}/ShapeGenerater.cpp
        ${RE_EXAMPLES_SOURCE_DIR}/ShapeGenerater.h
        )

set(EXAMPLES_SOURCE_FILES 
        ${RE_EXAMPLES_SOURCE_DIR}/Audio/AudioTest.cpp
        ${RE_EXAMPLES_SOURCE_DIR}/BaseTest.cpp
        ${RE_EXAMPLES_SOURCE_DIR}/Bullet/BulletTest.cpp
        ${RE_EXAMPLES_SOURCE_DIR}/Bumpmap/Bumpmap.cpp
        ${RE_EXAMPLES_SOURCE_DIR}/FBX/FBXTest.cpp
        ${RE_EXAMPLES_SOURCE_DIR}/FeatureTestsApp.cpp
        ${RE_EXAMPLES_SOURCE_DIR}/Font/FontTest.cpp
        ${RE_EXAMPLES_SOURCE_DIR}/Light/LightTest.cpp
        ${RE_EXAMPLES_SOURCE_DIR}/Lightmap/Lightmap.cpp
        ${RE_EXAMPLES_SOURCE_DIR}/MotoScene/SceneMaterialTest.cpp
        ${RE_EXAMPLES_SOURCE_DIR}/Reflection/Reflection.cpp
        ${RE_EXAMPLES_SOURCE_DIR}/Refraction/Refraction.cpp
        ${RE_EXAMPLES_SOURCE_DIR}/RenderToTexture/RenderToTextureTest.cpp
        ${RE_EXAMPLES_SOURCE_DIR}/Serialization/SerializationTest.cpp
        ${RE_EXAMPLES_SOURCE_DIR}/Shadow/ShadowTest.cpp
        ${RE_EXAMPLES_SOURCE_DIR}/ShapeGenerater.cpp
        ${RE_EXAMPLES_SOURCE_DIR}/Skybox/Skybox.cpp
        ${RE_EXAMPLES_SOURCE_DIR}/Tween/TweenTest.cpp
        ${RE_EXAMPLES_SOURCE_DIR}/UI/LayoutTest.cpp
        ${RE_EXAMPLES_SOURCE_DIR}/UI/MaskTest.cpp
        ${RE_EXAMPLES_SOURCE_DIR}/UI/ScrollViewTest.cpp
        ${RE_EXAMPLES_SOURCE_DIR}/UI/UITest.cpp
        )

set(EXAMPLES_HEADER_FILES 
        ${RE_EXAMPLES_SOURCE_DIR}/Audio/AudioTest.h 
        ${RE_EXAMPLES_SOURCE_DIR}/BaseTest.h 
        ${RE_EXAMPLES_SOURCE_DIR}/Bullet/BulletTest.h 
        ${RE_EXAMPLES_SOURCE_DIR}/Bumpmap/Bumpmap.h 
        ${RE_EXAMPLES_SOURCE_DIR}/FBX/FBXTest.h 
        ${RE_EXAMPLES_SOURCE_DIR}/FeatureTestsApp.h 
        ${RE_EXAMPLES_SOURCE_DIR}/Font/FontTest.h 
        ${RE_EXAMPLES_SOURCE_DIR}/Light/LightTest.h 
        ${RE_EXAMPLES_SOURCE_DIR}/Lightmap/Lightmap.h 
        ${RE_EXAMPLES_SOURCE_DIR}/MotoScene/SceneMaterialTest.h 
        ${RE_EXAMPLES_SOURCE_DIR}/Reflection/Reflection.h 
        ${RE_EXAMPLES_SOURCE_DIR}/Refraction/Refraction.h 
        ${RE_EXAMPLES_SOURCE_DIR}/RenderToTexture/RenderToTextureTest.h 
        ${RE_EXAMPLES_SOURCE_DIR}/Serialization/SerializationTest.h 
        ${RE_EXAMPLES_SOURCE_DIR}/Shadow/ShadowTest.h 
        ${RE_EXAMPLES_SOURCE_DIR}/ShapeGenerater.h 
        ${RE_EXAMPLES_SOURCE_DIR}/Skybox/Skybox.h 
        ${RE_EXAMPLES_SOURCE_DIR}/Tween/TweenTest.h 
        ${RE_EXAMPLES_SOURCE_DIR}/UI/LayoutTest.h 
        ${RE_EXAMPLES_SOURCE_DIR}/UI/MaskTest.h 
        ${RE_EXAMPLES_SOURCE_DIR}/UI/ScrollViewTest.h 
        ${RE_EXAMPLES_SOURCE_DIR}/UI/UITest.h 
        )

SOURCE_GROUP("Examples\\Source Files" FILES ${EXAMPLES_SOURCE_FILES})
SOURCE_GROUP("Examples\\Header Files" FILES ${EXAMPLES_HEADER_FILES})

set(RE_EXAMPLES_BUILD_DIR "${RE_PROJECT_DIR}/Build/Examples")

if(${RE_PLATFORM_MAC})
        set(EXAMPLE_MAIN_SOURCE_FILES
                ${RE_EXAMPLES_BUILD_DIR}/Desktop/main.cpp
                )
elseif(${RE_PLATFORM_ANDROID})
        set(EXAMPLE_MAIN_SOURCE_FILES
                ${RE_EXAMPLES_BUILD_DIR}/Android/jni/main.cpp
                )
elseif(${RE_PLATFORM_IOS})
        enable_language( Swift )

        set(EXAMPLE_MAIN_SOURCE_FILES
                ${RE_PROJECT_DIR}/Source/Platform/iOS/main.swift
                ${RE_PROJECT_DIR}/Source/Platform/iOS/AppDelegate.swift
                ${RE_PROJECT_DIR}/Source/Platform/iOS/GameLoop.swift
                ${RE_PROJECT_DIR}/Source/Platform/iOS/GameView.swift
                ${RE_PROJECT_DIR}/Source/Platform/iOS/GameViewController.swift
                )

        file(GLOB RESOURCES ${RE_EXAMPLES_DIR}/Resources/*)
        set_source_files_properties(${RESOURCES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
endif()

include_directories(
        ${RE_EXAMPLES_SOURCE_DIR}
        )

set(RE_EXAMPLES_SOURCE_DIRS
        ${PRE_HEADER_FILES}
        ${EXAMPLES_SOURCE_FILES}
        ${EXAMPLES_HEADER_FILES}
        ${EXAMPLE_MAIN_SOURCE_FILES}
        )

if(${RE_PLATFORM_ANDROID})
        add_library(Examples SHARED
                ${RE_EXAMPLES_SOURCE_DIRS}
                )
else(${RE_PLATFORM_IOS})
        add_executable(Examples MACOSX_BUNDLE
                ${RE_EXAMPLES_SOURCE_DIRS}

                ${RESOURCES}
                )

        set_target_properties(Examples PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${RE_EXAMPLES_BUILD_DIR}/iOS/Info.plist.in)
        set(MACOSX_BUNDLE_GUI_IDENTIFIER "org.racing.examples")

        #
        set_xcode_property(Examples GCC_GENERATE_DEBUGGING_SYMBOLS YES)
        set_xcode_property(Examples SWIFT_OBJC_BRIDGING_HEADER ${RE_PROJECT_DIR}/Source/Platform/iOS/Engine-Bridging-Header.h)
        set_xcode_property(Examples LD_RUNPATH_SEARCH_PATHS @executable_path/Frameworks)
else()
        add_executable(Examples
        ${RE_EXAMPLES_SOURCE_DIRS}
        )
endif()

target_link_libraries(Examples
        Engine
        Tween
        yaml-cpp
        ${RE_LINK_LIBRARIES}
        )

if(${RE_PLATFORM_ANDROID})
        set_target_properties(Examples PROPERTIES COMPILE_DEFINITIONS "ANDROID")

        set(ANDROID_APK_API_LEVEL "21")

        android_create_apk(Examples "org.racing.examples" 
        "${CMAKE_BINARY_DIR}/apk" 
        "${RE_EXAMPLES_BUILD_DIR}/Android/libs" 
        "${RE_EXAMPLES_BUILD_DIR}/Android" 
        "${RE_EXAMPLES_BUILD_DIR}/Android/assets")
endif()

