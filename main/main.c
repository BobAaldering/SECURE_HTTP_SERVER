#include <nvs_flash.h> // Include the facilities provided in the 'nvs_flash' header file.

#include "https_wifi.h" // Include the facilities provided in the 'https_wifi' header file.

// This is the main function for the ESP-IDF framework.
void app_main(void)
{
    /* This is a macro that checks if the function `nvs_flash_init()` returns an error. If it does, it
    prints the error and exits the program. */
    ESP_ERROR_CHECK(nvs_flash_init());

    /* Connecting to the wifi network. */
    wifi_connection();

    /* Starting the webserver. */
    start_webserver();
}
