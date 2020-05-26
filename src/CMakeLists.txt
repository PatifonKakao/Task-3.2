# задаем набор исходников
set(SOURCES
vitdec.h
vitdec.cpp
)

# создаем статическую библиотеку
add_library(project_lib ${SOURCES})

# добавлем путь к исходникам для возожности подключения библиотеки
target_include_directories(project_lib PUBLIC ${SRC_ROOT_DIR})

# группируем исходники по типу по категориям
source_group(TREE ${CMAKE_CURRENT_LIST_DIR} FILES ${SOURCES})

# В solution эта библиотека лежит в simple_project
set_property(TARGET project_lib PROPERTY FOLDER "project_lib")

# включаем в сборку проекnа подпроект с тестами
add_subdirectory(tests)