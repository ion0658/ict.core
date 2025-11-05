find_package(Doxygen)
if(DOXYGEN_FOUND)
    doxygen_add_docs(
        doc
        ${CMAKE_SOURCE_DIR}/src/include
        COMMENT "Generating API documentation with Doxygen"
    )
else (DOXYGEN_FOUND)
    message(WARNING "Doxygen not found, API documentation will not be generated")
endif (DOXYGEN_FOUND)

