SET(TARGET utils)

AUX_SOURCE_DIRECTORY(./ SRC)

ADD_LIBRARY(${TARGET} ${SRC})

TARGET_LINK_LIBRARIES(${TARGET}
    ${LINK_PATH}/libcurl.a
    ${LINK_PATH}/libboost_unit_test_framework.a
    rt)

