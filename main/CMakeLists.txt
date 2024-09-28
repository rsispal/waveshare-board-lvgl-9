idf_component_register(SRCS "main.cpp"
                    INCLUDE_DIRS "." ${LV_DEMO_DIR})

lvgl_port_create_c_image("images/esp_logo.png" "images/" "ARGB8888" "NONE")
lvgl_port_add_images(${COMPONENT_LIB} "images/")

lvgl_port_create_c_image("images/icons/BC2020.png" "images/icons/" "ARGB8888" "NONE")
lvgl_port_create_c_image("images/icons/BC2424.png" "images/icons/" "ARGB8888" "NONE")
lvgl_port_create_c_image("images/icons/BS5050.png" "images/icons/" "ARGB8888" "NONE")
lvgl_port_create_c_image("images/icons/RM5050.png" "images/icons/" "ARGB8888" "NONE")
lvgl_port_create_c_image("images/icons/RST5050.png" "images/icons/" "ARGB8888" "NONE")
lvgl_port_create_c_image("images/icons/VM5050.png" "images/icons/" "ARGB8888" "NONE")
lvgl_port_add_images(${COMPONENT_LIB} "images/icons/")

set_source_files_properties(
    PROPERTIES COMPILE_OPTIONS
    "-DLV_LVGL_H_INCLUDE_SIMPLE;-Wno-format;"
    )
