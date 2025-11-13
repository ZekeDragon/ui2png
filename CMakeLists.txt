cmake_minimum_required(VERSION 3.16)

project(ui2png LANGUAGES CXX VERSION 0.2.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets UiTools LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets UiTools LinguistTools)

qt_standard_project_setup()

set(TS_FILES ui2png_en_US.ts)

qt_add_executable(ui2png
    main.cpp
    Ui2Png.cpp
    Ui2Png.hpp
    ${TS_FILES}
)
target_link_libraries(ui2png PUBLIC
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::UiTools
)
target_compile_definitions(ui2png PRIVATE
    UI2PNG_VERSION_STRING="${PROJECT_VERSION}"
)

if(COMMAND qt_create_translation)
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

install(TARGETS ui2png
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_app_script(
    TARGET ui2png
    OUTPUT_SCRIPT UI2PNG_DEPLOY_SCRIPT
)
install(SCRIPT ${UI2PNG_DEPLOY_SCRIPT})

qt_finalize_project()
