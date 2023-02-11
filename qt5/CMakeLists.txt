set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
#find_package(PkgConfig)
set(QT_VERSION_MAJOR 5)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Widgets Network Quick Concurrent QuickControls2 REQUIRED)

set(PREFIX qt)
#add_subdirectory(EventSystem)
#add_subdirectory(ShapeMakerDemo)
#add_subdirectory(QTableWidget)
#add_subdirectory(QTreeWidget)
#add_subdirectory(FileSystemModel)
#add_subdirectory(StringListModel)
add_subdirectory(CustomModelsReadOnlyDemo)
add_subdirectory(CustomStartDelegateDemo)
#add_subdirectory(benchmark)

add_single_file_targets(PREFIX ${PREFIX} DIR ${CMAKE_CURRENT_SOURCE_DIR} LIBS Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::CorePrivate Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::Concurrent Qt${QT_VERSION_MAJOR}::QuickControls2
                        INCLUDES ${Qt${QT_VERSION_MAJOR}Gui_PRIVATE_INCLUDE_DIRS})
#add_dir_target(PREFIX ${PREFIX} DIR ${CMAKE_CURRENT_SOURCE_DIR} LIBS Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::CorePrivate Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::Concurrent Qt${QT_VERSION_MAJOR}::QuickControls2)

