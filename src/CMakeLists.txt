set(Sources
    fibonacci.cpp
    main.cpp
    choose_timer_unit.cpp
)

set(Dirs
    "../include"
)

add_library(src ${Sources})
target_include_directories(src PUBLIC ${Dirs})
