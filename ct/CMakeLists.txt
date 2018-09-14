project(ct_meta)

#Make sure metapackage does not fail when building documentation
add_custom_target(doc
        COMMAND ${CMAKE_COMMAND} -E echo_append "Building CT Documentation..."
        VERBATIM)
        
add_custom_target(clang-format
        COMMAND ${CMAKE_COMMAND} -E echo_append "Running clang-format on CT..."
        VERBATIM)

add_custom_target(clang-tidy
        COMMAND ${CMAKE_COMMAND} -E echo_append "Running clang-tidy on CT..."
        VERBATIM)    
