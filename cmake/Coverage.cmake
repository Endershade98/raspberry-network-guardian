function(enable_coverage target)


    if(NOT CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")

        message(
            WARNING
            "Coverage only supported on GCC/Clang"
        )

        return()

    endif()


    target_compile_options(${target}
        PRIVATE

        -O0
        -g
        --coverage
    )


    target_link_options(${target}
        PRIVATE

        --coverage
    )


endfunction()