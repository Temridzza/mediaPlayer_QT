#поддиректория src
set(SRC_FILES
        mainWindow.cpp
        audioPlayer.cpp
        converterJSON.cpp
)

set(INCLUDE_FILES
        ${INCLUDE}/audioPlayer.h
        ${INCLUDE}/mainWindow.h
        ${INCLUDE}/converterJSON.h
)

add_executable(mediaPlayer
        main.cpp
        mainwindow.ui
        ${SRC_FILES}
        ${INCLUDE_FILES}
)

target_include_directories(mediaPlayer PUBLIC ${INCLUDE})

target_link_libraries(mediaPlayer PRIVATE
        Qt5::Widgets
        Qt5::Multimedia
        Qt5::MultimediaWidgets
        Qt5::Bluetooth
        Qt5::Core
        nlohmann_json::nlohmann_json
)