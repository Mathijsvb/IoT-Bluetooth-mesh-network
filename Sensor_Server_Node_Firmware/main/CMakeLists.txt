set(srcs "main.c"
         "components/LED.c"
         "components/commands.c"
         "components/communication.c")

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS  ".")
