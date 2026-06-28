function(enable_sanitizers target)

    if(NOT CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        message(
            WARNING
            "Sanitizers only supported on GCC/Clang"
        )

        return()
    endif()


    target_compile_options(${target}
        PRIVATE

        -fsanitize=address
        -fsanitize=undefined

        -fno-omit-frame-pointer
    )


    target_link_options(${target}
        PRIVATE

        -fsanitize=address
        -fsanitize=undefined
    )

endfunction()