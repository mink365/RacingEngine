
set(TWEEN_SOURCE_DIR
        ${RE_SOURCE_DIR}/External/universal-tween-engine-cpp
        )

file(GLOB_RECURSE TWEEN_SOURCE_FILES "${TWEEN_SOURCE_DIR}/[a-zA-Z]*.cpp")
file(GLOB_RECURSE TWEEN_HEADER_FILES "${TWEEN_SOURCE_DIR}/[a-zA-Z]*.h")

add_library(Tween
        ${TWEEN_SOURCE_FILES}
        ${TWEEN_HEADER_FILES}
        )
