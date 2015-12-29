
aux_source_directory(${RE_SOURCE_DIR}/External/universal-tween-engine-cpp TWEEN_SRC_FILES)

aux_source_directory(${RE_SOURCE_DIR}/External/universal-tween-engine-cpp/paths TWEEN_SRC_PATH_FILES)

aux_source_directory(${RE_SOURCE_DIR}/External/universal-tween-engine-cpp/equations TWEEN_SRC_EQUATIONS_FILES)

add_library(Tween
        ${TWEEN_SRC_FILES}
        ${TWEEN_SRC_PATH_FILES}
        ${TWEEN_SRC_EQUATIONS_FILES}
        )