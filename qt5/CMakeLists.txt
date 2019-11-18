cmake_minimum_required(VERSION 3.7)
project(qt5)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
list(APPEND CMAKE_PREFIX_PATH ${QT_CMAKE})
find_package(PkgConfig)
#pkg_check_modules(cryptopp REQUIRED libcryptopp)
find_package(Qt5 REQUIRED Core Widgets Network Quick Concurrent)

add_executable(SizePolicy SizePolicy/main.cc)
qt5_use_modules(SizePolicy LINK_PRIVATE Widgets)

add_executable(Pimp Pimp/main.cc Pimp/main.cc Pimp/MyFoo.cc Pimp/MyFoo.h Pimp/MyFoo_p.h Pimp/MyFoo_p.h)
qt5_use_modules(Pimp LINK_PRIVATE Widgets)

qt5_add_resources(AbsolutePositioningSource resources/resource.qrc)
add_executable(AbsolutePositioning AbsolutePositioning/main.cc ${AbsolutePositioningSource})
qt5_use_modules(AbsolutePositioning LINK_PRIVATE Widgets)

add_executable(DraggableButton DraggableButton/main.cc DraggableButton/DraggableButton.cc)
qt5_use_modules(DraggableButton LINK_PRIVATE Widgets)

add_executable(HttpClient HttpClient/main.cc HttpClient/HttpClientWindow.cc HttpClient/HttpClientWindow.h)
qt5_use_modules(HttpClient LINK_PRIVATE Core Widgets Network)

add_executable(Crypto Crypto/main.cc)
target_include_directories(Crypto PRIVATE ${cryptopp_INCLUDE_DIRS})
target_link_libraries(Crypto PRIVATE ${cryptopp_LINK_LIBRARIES} Qt5::Core)

add_executable(qml_bloc qml_bloc/main.cc qml_bloc/async.qml qml_bloc/foo.cc qml_bloc/foo.h qml_bloc/http.cc qml_bloc/http.h)
qt5_use_modules(qml_bloc LINK_PRIVATE Core Widgets Network Quick)

add_executable(json json/main.cc)
qt5_use_modules(json LINK_PRIVATE Core)

add_executable(http_thread Thread/network_manager_in_seperated_thread.cc)
qt5_use_modules(http_thread LINK_PRIVATE Core Network)

add_executable(dialog_future_watcher DialogFutureWatcher/main.cc)
qt5_use_modules(dialog_future_watcher LINK_PRIVATE Core Widgets Concurrent)

add_subdirectory(widgets)
add_subdirectory(QtQuickPlayground)
add_subdirectory(core)
add_subdirectory(attached_property)
add_subdirectory(custom_qquickitem)
add_subdirectory(extendqml)
add_subdirectory(qml_javascript_cpp)
add_subdirectory(dynamic_component_with_repeater)
add_subdirectory(property_value_source)
