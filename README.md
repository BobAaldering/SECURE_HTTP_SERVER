# SECURE HTTP SERVER

This project contains the implementation of an HTTP server, to which information should be sent securely. SSL is used, further deepening RSA.
You can obtain information by means of a GET or POST request to the server. When you make a GET request you get information about the application of the server. With a POST request you can send a string to the server, which returns it in capital letters.
All communication is secured, and no one else can obtain data from it, as various security requirements have been applied to it.

## How to use example

Chances are, the project won't compile right away at first. That's because the default HTTP library within `ESP-IDF` must be overridden by its HTTPS representation. You can solve this in the following ways:

1. Within the `ESP-IDF` terminal, execute the following command: `idf.py menuconfig`. 
2. Now the menu of `ESP-IDF` should be available in the command line.
3. Now navigate to `Component config --->`. Press enter.
4. Navigate to `ESP HTTPS server --->`. Press enter again.
5. Enable now the specific HTTPS component. Press enter. After hitting this button, it must look like `[*] Enable ESP_HTTPS_SERVER component`. 
6. Press `Q` for quitting the menu.

After these steps, it is possible to compile and flash the program.

## Certificates

You will need to approve a security exception in your browser. This is because of a self signed
certificate; this will be always the case, unless you preload the CA root into your browser/system
as trusted.

You can generate a new certificate using the OpenSSL command line tool:

```
openssl req -newkey rsa:2048 -nodes -keyout prvtkey.pem -x509 -days 3650 -out cacert.pem -subj "/CN=ESP32 HTTPS server example"
```

Expiry time and metadata fields can be adjusted in the invocation.

## Accessing the server

You can access the server in a number of ways. It has been tested using 'curl'. Normally your certificates must be in order. Based on this you can now start using the server.

To get the instructions, run the following command (note that this must be the IP address assigned to the ESP32 at that time):

```
curl https://192.168.1.34
```

To send data to the server, translating the string to uppercase (note that this must be the IP address assigned to the ESP32 at that time):

```
curl -d "text comes here" -X POST 192.168.1.34
```

If you still have problems with the SSL/RSA certifications, you can try the following:

```
curl -k https://192.168.1.34
curl -d "text comes here" -k -X POST 192.168.1.34
```

## Wi-Fi settings

Please set the Wi-Fi settings in the `wifi_pass.h` header file.

## Test functionality

The program has been tested with various functionalities that `ESP-IDF` has for this. In the file `test_https_server.py` a simple test function is written that queries data, and compares this with the data that the program contains by default. Note that `ESP-IDF` also has a testing framework.

## Disclaimer

There is NO guarantee to compile/flash the program on a Windows machine. It is also NOT recommended to use Windows for testing the software. It has been tested on a UNIX-like operating system.

THIS EXAMPLE SHALL NOT BE APPLIED IN PROFESSIONAL USE. The certificates are only examples, if it is used within a professional environment it is necessary to generate the certificates yourself, given in the `main/certs` folder. 
