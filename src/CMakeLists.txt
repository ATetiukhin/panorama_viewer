cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

set(APP_NAME panorama_viewer)

set(ALL_FILES
  main.cpp
)

if (WIN32)
  set(GUI_TYPE WIN32)
elseif (APPLE)
  set(GUI_TYPE MACOSX_BUNDLE)
endif ()

add_executable(${APP_NAME} ${GUI_TYPE} ${ALL_FILES})
target_link_libraries(${APP_NAME}
  PRIVATE
    Qt5::Core
    Qt5::Qml
    Qt5::Quick
)

if (WIN32)
  get_target_property(QT5_QMAKE_EXECUTABLE Qt5::qmake IMPORTED_LOCATION)
  get_filename_component(QT5_WINDEPLOYQT_EXECUTABLE ${QT5_QMAKE_EXECUTABLE} PATH)
  set(QT5_WINDEPLOYQT_EXECUTABLE "${QT5_WINDEPLOYQT_EXECUTABLE}/windeployqt.exe")

  add_custom_command(TARGET ${APP_NAME} POST_BUILD
                     COMMAND ${QT5_WINDEPLOYQT_EXECUTABLE} --qmldir ${CMAKE_SOURCE_DIR} $<TARGET_FILE_DIR:${APP_NAME}>)
  source_group("${APP_NAME}" FILES ${ALL_FILES})
endif (WIN32)

set_target_properties(${APP_NAME} PROPERTIES FOLDER ${APP_NAME})
