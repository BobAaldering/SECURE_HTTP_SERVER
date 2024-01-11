#ifndef HTTPS_WIFI_H
#define HTTPS_WIFI_H

#include <stdio.h> // Include the facilities provided in the 'stdio' header file.
#include <string.h> // Include the facilities provided in the 'string' header file.

#include <esp_wifi.h> // Include the facilities provided in the 'esp_wifi' header file.
#include <esp_event.h> // Include the facilities provided in the 'esp_event' header file.
#include <esp_log.h> // Include the facilities provided in the 'esp_log' header file.
#include <esp_system.h> // Include the facilities provided in the 'esp_system' header file.
#include <sys/param.h> // Include the facilities provided in the 'sys/param' header file.

#include "esp_netif.h" // Include the facilities provided in the 'esp_netif' header file.
#include "esp_eth.h" // Include the facilities provided in the 'esp_eth' header file.
#include "esp_https_server.h" // Include the facilities provided in the 'esp_https_server' header file.

#include "wifi_pass.h" // Include the facilities provided in the 'wifi_pass' header file.

/* This is a simple way to define a constant. */
#define MAX_BUFFER_POST_LENGTH (1024)

/* This is a simple way to enable or disable a feature. If the
`CONFIG_EXAMPLE_ENABLE_HTTPS_USER_CALLBACK` is defined, then the `https_server_user_callback`
function will be defined. If the `CONFIG_EXAMPLE_ENABLE_HTTPS_USER_CALLBACK` is not defined, then
the `https_server_user_callback` function will not be defined. */
#if CONFIG_EXAMPLE_ENABLE_HTTPS_USER_CALLBACK

    /* This is a function prototype for the `https_server_user_callback` function. */
    extern void https_server_user_callback(esp_https_server_user_cb_arg_t *user_cb);

#endif

/* This is a function prototype for the `wifi_event_handler` function. */
extern void wifi_event_handler(void* event_handler_arg, esp_event_base_t event_base, int32_t event_id, void* event_data);

/* This is a function prototype for the `wifi_event_handler` function. */
extern void wifi_connection(void);

/* These are function prototypes for the `get_handler` and `post_handler` functions. */
extern esp_err_t get_handler(httpd_req_t* req);

/* This is a function prototype for the `post_handler` function. */
extern esp_err_t post_handler(httpd_req_t* req);

/* This is a function prototype for the `start_webserver` function. */
extern httpd_handle_t start_webserver(void);

/* This is a function prototype for the `stop_webserver` function. */
extern void stop_webserver(httpd_handle_t server);

#endif
