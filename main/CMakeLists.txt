set(COMPONENT_SRCS "spiffs.c")
set(COMPONENT_ADD_INCLUDEDIRS "")

idf_component_register(
    SRCS "${COMPONENT_SRCS}"
)
