set(srcs "main.c"
        "components/BLE_Mesh.c"
        "components/LED.c")

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS  ".")
