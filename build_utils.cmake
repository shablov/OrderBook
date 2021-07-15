function(set_common_project_properties TARGET)
    target_compile_options(${TARGET}
        PRIVATE
            -fPIC

            -Werror

            -Wall
            -Wextra
            -Wpedantic

            -Wcast-qual
            -Wcast-align
            -Wsign-promo
            -Wconversion
            -Wfloat-equal
            -Wenum-compare
            -Wold-style-cast
            -Wredundant-decls
            -Wsign-conversion
            -Wctor-dtor-privacy
            -Woverloaded-virtual
    )

    set_target_properties(${TARGET} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
    )
endfunction()
