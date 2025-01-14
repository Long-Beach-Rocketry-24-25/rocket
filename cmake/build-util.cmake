
function(add_executable_for DEVICE EXECUTABLE LINKER_SCRIPT)
    if (${DEVICE})
        message("Adding executable: ${EXECUTABLE}")
        message("Target: ${DEVICE}")
        add_executable(${EXECUTABLE} ${ARGN})

        if (NOT "${DEVICE}" MATCHES "NATIVE")
            message("Selecting linker opts for ARM MCU")
            target_link_options(${EXECUTABLE} PRIVATE
                -T${LINKER_SCRIPT}
                -specs=nosys.specs
                -lc
                -lm
                -lnosys
                -Wl,-Map=${EXECUTABLE}.map,--cref
                -Wl,--gc-sections
                # -Wl, --print-memory-usage
            )

            add_custom_command(TARGET ${EXECUTABLE}
                POST_BUILD
                COMMAND arm-none-eabi-objcopy -O ihex ${EXECUTABLE} ${EXECUTABLE}.hex
                COMMAND arm-none-eabi-objcopy -O binary ${EXECUTABLE} ${EXECUTABLE}.bin
                COMMAND arm-none-eabi-objcopy -O srec ${EXECUTABLE} ${EXECUTABLE}.elf
                COMMAND arm-none-eabi-size --format=berkeley ${EXECUTABLE}.elf
            )
        endif()
    endif()
endfunction()

function(add_library_for DEVICE TARGET)
    if (${DEVICE})
        add_library(${TARGET} ${ARGN})
    endif()
endfunction()

function(add_subdirectory_for DEVICE TARGET)
    if (${DEVICE})
        add_subdirectory(${TARGET})
    endif()
endfunction()

function(target_include_directories_for DEVICE TARGET)
    if (${DEVICE})
        target_include_directories(${TARGET} ${ARGN})
    endif()
endfunction()

function(target_link_libraries_for DEVICE TARGET)
    if (${DEVICE})
        target_link_libraries(${TARGET} ${ARGN})
    endif()
endfunction()

function(target_link_options_for DEVICE TARGET)
    if(${DEVICE})
        target_link_options(${TARGET} ${ARGN})
    endif()
endfunction()

function(target_compile_definitions_for DEVICE TARGET)
    if (${DEVICE})
        target_compile_definitions(${TARGET} ${ARGN})
    endif()
endfunction()

function(target_preprocess_for DEVICE EXECUTABLE SRC_FILE OUT_FILE)
    if (${DEVICE})
        message("Preprocessing: ${SRC_FILE} -> ${OUT_FILE}")
        add_custom_command(TARGET ${EXECUTABLE}
            PRE_BUILD
            COMMAND arm-none-eabi-gcc -E -P -x c ${SRC_FILE} -o ${OUT_FILE} ${ARGN}
            DEPENDS ${SRC_FILE}
        )
    endif()
endfunction()

function(get_directories VAR PATH)
    set(TEMP "")
    file(GLOB_RECURSE DIRECTORIES LIST_DIRECTORIES true "${PATH}/*")
    foreach(DIR ${DIRECTORIES})
        if (IS_DIRECTORY ${DIR})
            list(APPEND TEMP ${DIR})
        endif()
    endforeach()
    set(${VAR} ${TEMP} PARENT_SCOPE)
endfunction()
