# - Try to find Bullet
# Once done this will define
# BULLET_FOUND - Was bullet found
# BULLET_INCLUDE_DIRS - the Bullet include directories
# BULLET_LIBRARIES - Link to this, by default it includes
#                   all bullet components (Dynamics,
#                   Collision, LinearMath, & SoftBody)

if(ANDROID)
    find_path(BULLET_INCLUDE_DIRS btBulletCollisionCommon.h
            PATHS
            "${RE_EXTERNAL_DIR}/bullet/include"
            NO_DEFAULT_PATH
            )

    find_library(BULLET_LIBRARIES
            NAMES bullet libbullet
            PATHS
            "${RE_EXTERNAL_DIR}/bullet/lib/android/arm"
            NO_DEFAULT_PATH
            )
elseif(IOS)
    FIND_PATH(BULLET_INCLUDE_DIRS btBulletCollisionCommon.h
            PATHS
            "${RE_EXTERNAL_DIR}/bullet/include"
            NO_DEFAULT_PATH
            )

    find_library(BULLET_LIBRARIES
            NAMES bullet libbullet
            PATHS
            "${RE_EXTERNAL_DIR}/bullet/lib/ios/${IOS_ARCH}"
            NO_DEFAULT_PATH
            )
else()
    find_path(BULLET_INCLUDE_DIRS bullet/btBulletCollisionCommon.h
            )

    find_library(BULLET_COLLISION_LIBRARIE
            NAMES BulletCollision
            )

    find_library(BULLET_DYNAMICS_LIBRARIE
            NAMES BulletDynamics
            )

    find_library(BULLET_LINEAR_MATH_LIBRARIE
            NAMES LinearMath
            )

    set(BULLET_LIBRARIES
            ${BULLET_COLLISION_LIBRARIE}
            ${BULLET_DYNAMICS_LIBRARIE}
            ${BULLET_LINEAR_MATH_LIBRARIE}
            )

endif()

if(BULLET_INCLUDE_DIRS)
    set(BULLET_FOUND TRUE)
    MESSAGE(STATUS "Found Bullet's include: ${BULLET_INCLUDE_DIRS} lib: ${BULLET_LIBRARIES}")
else()
    MESSAGE(STATUS "Could NOT find Bullet.")
endif()