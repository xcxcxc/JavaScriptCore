# Note: I think the BlackBerry counterpart is using the wrong variable names for ICU
# because these are the ones defined by FindICU.cmake.
list(APPEND JSC_LIBRARIES
    ${ICU_I18N_LIBRARIES}
    ${ICU_LIBRARIES}
    ${ICU_DATA_LIBRARIES}
)

