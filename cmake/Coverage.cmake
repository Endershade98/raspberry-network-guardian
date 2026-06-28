function(enable_coverage target)

    if(NOT ENABLE_COVERAGE)
        return()
    endif()

    message(STATUS "Coverage enabled for ${target}")

    target_compile_options(
        ${target}
        PRIVATE
            -O0
            -g
            --coverage
    )

    target_link_options(
        ${target}
        PRIVATE
            --coverage
    )

endfunction()