idf_component_register(
                    SRCS "main.c" "https_wifi.c"
                    INCLUDE_DIRS "."
                    EMBED_TXTFILES "certs/cacert.pem"
                                   "certs/prvtkey.pem")
