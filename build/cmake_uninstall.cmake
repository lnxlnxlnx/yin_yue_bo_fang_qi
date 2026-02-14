# cmake_uninstall.cmake.in
if(NOT EXISTS "/home/lnx/program_study/little_project/yin_yue_bo_fang_qi/build/install_manifest.txt")
    message(FATAL_ERROR "Install manifest not found! Please install the project first.")
endif()

file(READ "/home/lnx/program_study/little_project/yin_yue_bo_fang_qi/build/install_manifest.txt" files)
string(REGEX REPLACE "\n" ";" files "${files}")
foreach(file ${files})
    message(STATUS "Uninstalling: ${file}")
    if(EXISTS ${file} OR IS_SYMLINK ${file})
        execute_process(
            COMMAND sudo rm -f ${file}
            RESULT_VARIABLE rm_result
        )
        if(NOT ${rm_result} EQUAL 0)
            message(FATAL_ERROR "Failed to uninstall ${file}")
        endif()
    else()
        message(STATUS "File ${file} does not exist, skipping.")
    endif()
endforeach()
