set(PREFIX QtEventSystem)
add_single_file_targets(PREFIX ${PREFIX} DIR ${CMAKE_CURRENT_SOURCE_DIR} LIBS Qt5::Core Qt5::CorePrivate Qt5::Widgets Qt5::Network Qt5::Quick Qt5::Concurrent Qt5::QuickControls2
                        INCLUDES ${Qt5Gui_PRIVATE_INCLUDE_DIRS})
