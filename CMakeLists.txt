cmake_minimum_required(VERSION 3.27)

#Load Project Configuration from project.conf
set(conf_file "${CMAKE_CURRENT_SOURCE_DIR}/project.conf")

if(NOT EXISTS ${conf_file})
    message(FATAL_ERROR "Can't load project configurations, project.conf file missing!")
endif()

file(READ ${conf_file} conf_data)

string(REGEX REPLACE "[,\ \\\n]" ";" conf_data ${conf_data})

message(STATUS ${conf_data})
message(STATUS "-------------------------Configurations Debug: -------------------------")
foreach(conf ${conf_data})
    string(REGEX REPLACE "\"*\"" "" conf ${conf})    

    string(FIND ${conf} "=" equal_seperator_pos)
    string(LENGTH ${conf} string_length)

    math(EXPR value_start "${equal_seperator_pos} + 1")
    math(EXPR value_len "${string_length} - ${value_start}")

    set(key_start 0)
    set(key_len ${equal_seperator_pos})

    string(SUBSTRING ${conf} ${key_start} ${key_len} key)
    string(SUBSTRING ${conf} ${value_start} ${value_len} value)

    set(${key} ${value})
    message(STATUS "Key: ${key} got set to Value: ${value}, key_start: ${key_start}, key_len: ${key_len}, 
            value_start: ${value_start}, value_len: ${value_len}")
endforeach()

message(STATUS "-----------------------Configurations Debug End: -----------------------")
message(STATUS "")

######################### Project, target etc. stuff #########################

project(${PROJECT_NAME} VERSION ${PROJECT_VERSION})

set(TARGET ${PROJECT_NAME}_${PROJECT_VERSION})

#Project files logic
function(get_files dir)
    file(GLOB found_files "${dir}/*")
    foreach(_file ${found_files})
        message(STATUS "${_file} in ${found_files}")
        if(IS_DIRECTORY ${_file})
            get_files(${_file})
        else()
            #Path normalization
            #replace / or \ with \ only of the working dir string
            string(REGEX REPLACE "[/\\]" "/" WD ${CMAKE_CURRENT_SOURCE_DIR})

            #replace / or \ with \ only of the file name string
            string(REGEX REPLACE "[/\\]" "/" FN ${_file})

            #Final string SUB_FILE_NAME to be concated
            string(REPLACE ${WD} "" SUB_FILE_NAME ${FN})

            string(SUBSTRING ${SUB_FILE_NAME} 0 1 FIRST_CHAR)

            if(FIRST_CHAR STREQUAL "/")
                string(LENGTH ${SUB_FILE_NAME} STR_LENGTH)

                math(EXPR subbed "${STR_LENGTH}-1" OUTPUT_FORMAT DECIMAL)

                string(SUBSTRING ${SUB_FILE_NAME} 1 ${subbed} SUB_FILE_NAME)

                set(SUB_FILE_NAME ${SUB_FILE_NAME} PARENT_SCOPE)
            endif()

            if("${SF}" STREQUAL "")
                string(CONCAT SF "${SUB_FILE_NAME}")
            else()
                string(CONCAT SF "${SF}" ";" "${SUB_FILE_NAME}")
            endif()
        endif()

    endforeach()
    set(SOURCE_FILES ${SF} PARENT_SCOPE)
endfunction()

message(STATUS "--------------------------------------------------Source Files Debug: --------------------------------------------------")
get_files(src)
message(STATUS "-------------------------------------------------Source Files Debug End-------------------------------------------------")
message(STATUS "")
message(STATUS "---------------CURRENT PROJECT FILES INCLUDED: ---------------")
message(STATUS "${SOURCE_FILES}")
message(STATUS "--------------------------------------------------------------")

# add_executable("${TARGET}" "${SOURCE_FILES}")
add_library("${TARGET}" "${SOURCE_FILES}")