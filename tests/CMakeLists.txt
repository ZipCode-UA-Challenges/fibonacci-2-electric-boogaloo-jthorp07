set(Tests
        # Sources
        "test.cpp"
)

set(Dirs
        "../include"
)

add_library(tests ${Tests})
target_include_directories(tests PUBLIC ${Dirs})