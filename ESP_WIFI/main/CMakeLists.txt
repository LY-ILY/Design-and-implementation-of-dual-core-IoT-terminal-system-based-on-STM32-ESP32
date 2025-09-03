idf_build_get_property(target IDF_TARGET)

if(${target} STREQUAL "linux")
    set(requires mqtt esp_driver_gpio esp_driver_uart esp_wifi esp_event esp_stubs protocol_examples_common nvs_flash)
endif()

if("${CONFIG_EXAMPLE_IPV4}" STREQUAL y)
    set(tcp_client_ip tcp_client_v4.c)
else()
    set(tcp_client_ip tcp_client_v6.c)
endif()

idf_component_register(SRCS "my_command_manager.c" "my_wifi.c" "my_mqtt.c" "my_uart.c" "my_task.c" "my_semaphore.c" "my_topic.c" "my_frame.c" "my_crc.c" "main.c" "tcp_client_v6.c" "tcp_client_v4.c" "${tcp_client_ip}"
                                INCLUDE_DIRS "."
                                REQUIRES ${requires})

