
#add_executable(demo_map demo_map.cc)
#add_executable(demo_print demo_print.cc)
#add_executable(copy_elision_non_moveable copy_elision_non_moveable.cc)
#add_executable(func_except_type func_except_type.cc)
#add_executable(custom_structured_binding custom_structured_binding.cc)
#add_executable(attribute attribute.cc)
#add_executable(optional_simple optional_simple.cc)
#
#add_executable(optonal_return Optional/optional_return.cc)

file(GLOB_RECURSE SRCS *.cc)
foreach (SRC_FILE IN LISTS SRCS)
    message(${SRC_FILE})
    get_filename_component(TARGET_NAME ${SRC_FILE} NAME_WLE)
    message(${TARGET_NAME})
    add_executable(${TARGET_NAME} ${SRC_FILE})
endforeach ()
