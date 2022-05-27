set(srcs "main.c"
    "components/LED.c"
    "components/indicator_code.c")

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS  ".")
