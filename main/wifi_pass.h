/* This is a preprocessor directive. It is a way to tell the compiler to only execute the code if the
variable WIFI_SSID is not defined. If the variable is defined, then the code is ignored. */
#ifndef WIFI_SSID
    #define WIFI_SSID ("SSID")
#endif

/* This is a preprocessor directive. It is a way to tell the compiler to only execute the code if the
variable WIFI_PASS is not defined. If the variable is defined, then the code is ignored. */
#ifndef WIFI_PASS
    #define WIFI_PASS ("PASS")
#endif
