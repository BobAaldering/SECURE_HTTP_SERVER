#include "https_wifi.h" // Include the facilities provided in the 'https_wifi' header file.

/* This is a global variable that is used to identify the log messages. */
static const char* TAG = "HTTPS";

/* This is a variable that is used to identify the URI. The URI is the address of the website. The
method is the type of request that is sent to the server. The handler is the function that is called
when the request is received. */
static const httpd_uri_t uri_get = {
    .uri = "/",
    .method = HTTP_GET,
    .handler = get_handler
};

/* This is a variable that is used to identify the URI. The URI is the address of the website. The
method is the type of request that is sent to the server. The handler is the function that is called
when the request is received. */
static const httpd_uri_t uri_post = {
    .uri = "/",
    .method = HTTP_POST,
    .handler = post_handler
};

/**
 * It prints out the certificate information of the client.
 * 
 * @param user_cb The user callback argument.
 * 
 * @return The callback function is called when a new HTTPS session is created.
 */
void https_server_user_callback(esp_https_server_user_cb_arg_t *user_cb)
{
    ESP_LOGI(TAG, "Session Created!");
    const mbedtls_x509_crt *cert;

    const size_t buf_size = 1024;
    char *buf = calloc(buf_size, sizeof(char));

    /* Checking if the buffer is NULL. If it is NULL, then it will print out an error message. */
    if (buf == NULL) 
    {
        ESP_LOGE(TAG, "Out of memory - Callback execution failed!");
        return;
    }

    /* Getting the certificate of the client. */
    cert = mbedtls_ssl_get_peer_cert(&user_cb->tls->ssl);

    /* Checking if the certificate is not NULL. If it is not NULL, then it will print out the
    certificate information. */
    if (cert != NULL) 
    {
        mbedtls_x509_crt_info((char *) buf, buf_size - 1, "      ", cert);
        ESP_LOGI(TAG, "Peer certificate info:\n%s", buf);
    } 
    else 
    {
        ESP_LOGW(TAG, "Could not obtain the peer certificate!");
    }

    free(buf);
}

/**
 * It prints out the event that is happening.
 * 
 * @param event_handler_arg The argument that you want to pass to the event handler.
 * @param event_base The base of the event.
 * @param event_id The ID of the event that occurred.
 * @param event_data The data that is sent with the event.
 */
void wifi_event_handler(void* event_handler_arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    // Go through the number of events that can occur.
    switch(event_id) 
    {
        case WIFI_EVENT_STA_START:
            printf("Connecting to your Wi-Fi network...\n"); // You are starting a connection.
            break;
        case WIFI_EVENT_STA_CONNECTED:
            printf("Connected to your Wi-Fi network!\n"); // You are connected to the Wi-Fi network.
            break;
        case WIFI_EVENT_STA_DISCONNECTED:
            printf("Lost you Wi-Fi connection!\n"); // You lost the Wi-Fi connection.
            break;
        case IP_EVENT_STA_GOT_IP:
            printf("We got an IP-adress!\n"); // We got an IP-address.
            break;
        default:
            printf("No ID of the event found! Please try again...\n"); // No event is found, show this default message.
            break;
    }
}

/**
 * It connects to the wifi network.
 */
void wifi_connection(void) 
{
    /* It initializes the network interface. */
    esp_netif_init();

    /* Creating a default event loop. */
    esp_event_loop_create_default();

    /* A function that creates a default network interface. */
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t wifi_initiation = WIFI_INIT_CONFIG_DEFAULT();

    esp_wifi_init(&wifi_initiation);

    
    /* Registering the event handler for the WIFI_EVENT. The ESP_EVENT_ANY_ID is the ID of the event
    that is triggered when the WIFI_EVENT occurs. The wifi_event_handler is the function that is
    called when the WIFI_EVENT occurs. The NULL is the argument that is passed to the
    wifi_event_handler. */
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler, NULL);


    /* Registering the event handler for the IP_EVENT. The IP_EVENT is the event that is triggered when
    the IP address is obtained. The IP_EVENT_STA_GOT_IP is the event that is triggered when the IP
    address is obtained. The wifi_event_handler is the function that is called when the IP address
    is obtained. The NULL is the argument that is passed to the wifi_event_handler. */
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_event_handler, NULL);

    /* This is the configuration of the Wi-Fi network. The `.sta` is the Wi-Fi station configuration.
    The `.ssid` is the name of the Wi-Fi network. The `.password` is the password of the Wi-Fi
    network. */
    wifi_config_t wifi_configuration = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS
        }
    };

    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_configuration);
    esp_wifi_start();
    esp_wifi_connect();
}

/**
 * The function returns a string that is formatted as a HTML page
 * 
 * @param req The request object.
 * 
 * @return The response within ESP_ERROR format.
 */
esp_err_t get_handler(httpd_req_t* req)
{
    const char resp[] = "Welcome to the secure PintApple server!\n\n"
                        "You can send data to this server. This must be in text format. Then the data is returned in capital letters.\n\n"
                        "The certificates given in the project are an example. These may NOT be used in professional use!\n"
                        "Copyright (C) 2022 PintApple, Inc. "
                        "This is free software; see the source for copying conditions.\n"
                        "There is NO warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n";

    httpd_resp_set_type(req, "text/html");

    /* It sends the response to the client. */
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    
    return ESP_OK;
}

/**
 * Receive the POST request and store the content in a string. 
 * Capitalize the string and send it back to the client.
 * 
 * @param req The request object.
 * 
 * @return The response within ESP_ERROR format.
 */
esp_err_t post_handler(httpd_req_t* req)
{
    char content[MAX_BUFFER_POST_LENGTH];
    char content_changed[MAX_BUFFER_POST_LENGTH];

    size_t recv_size = MIN(req->content_len, sizeof(content));
    int ret = httpd_req_recv(req, content, recv_size);

    printf("POST CONTENT: \'%.*s\'. SIZE STRING: %u\n", recv_size, content, recv_size);

    /* Checking if the request is successful. If the request is not successful, then it will return an error. */
    if (ret <= 0)
    {
        if (ret == HTTPD_SOCK_ERR_TIMEOUT)
        {
            httpd_resp_send_408(req);
        }

        return ESP_FAIL;
    }

    /* Converting the string to uppercase. */
    for (size_t i = 0; i < recv_size; i++)
    {
        content[i] = toupper(content[i]);
    }

    strncpy(content_changed, content, recv_size);

    printf("STRING AFTER CAPITALIZING: %s\n", content_changed);

    /* Sending the response to the client. */
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, content_changed, HTTPD_RESP_USE_STRLEN);

    return ESP_OK;
}

/**
 * Start the HTTPS server and register the URI handlers
 * 
 * @return The server handle.
 */
httpd_handle_t start_webserver(void)
{
    ESP_LOGI(TAG, "Starting the server...");

    /* Initializing the HTTPS server. */
    httpd_ssl_config_t config = HTTPD_SSL_CONFIG_DEFAULT();
    httpd_handle_t server = NULL;

    /* This is a way to define a variable that is located in another file. */
    extern const unsigned char cacert_pem_start[] asm("_binary_cacert_pem_start");
    extern const unsigned char cacert_pem_end[] asm("_binary_cacert_pem_end");

    config.cacert_pem = cacert_pem_start;
    config.cacert_len = cacert_pem_end - cacert_pem_start;

    /* This is a way to define a variable that is located in another file. */
    extern const unsigned char prvtkey_pem_start[] asm("_binary_prvtkey_pem_start");
    extern const unsigned char prvtkey_pem_end[] asm("_binary_prvtkey_pem_end");

    config.prvtkey_pem = prvtkey_pem_start;
    config.prvtkey_len = prvtkey_pem_end - prvtkey_pem_start;

    /* Starting the HTTPS server. */
    esp_err_t ret = httpd_ssl_start(&server, &config);

    /* This is a way to define a variable that is located in another file. */
    #if CONFIG_EXAMPLE_ENABLE_HTTPS_USER_CALLBACK
        conf.user_cb = https_server_user_callback; // Use this callback function.
    #endif

    /* Checking if the HTTPS server is started successfully. If it is not, then it will print out an error message. */
    if (ESP_OK != ret) 
    {
        ESP_LOGI(TAG, "Error starting the server!");
        return NULL;
    }

    ESP_LOGI(TAG, "Registering the URI handlers...");

    httpd_register_uri_handler(server, &uri_get);
    httpd_register_uri_handler(server, &uri_post);

    ESP_LOGI(TAG, "Registered to the URI handlers! Listening for incoming requests...");

    return server;
}

/**
 * Stops the webserver
 * 
 * @param server The handle to the server.
 */
void stop_webserver(httpd_handle_t server)
{
    /* Stopping the HTTPS server. */
    httpd_ssl_stop(server);
}
