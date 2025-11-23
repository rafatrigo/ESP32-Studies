#include <string.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"

// LwIP (Lightweight IP) Headers for Sockets
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"

// custom component
#include "wifi_connect.h"

// Servidor público de horário (Protocolo Daytime)
#define HOST_IP_ADDR "httpbin.org"
#define PORT_STR "80"

static const char *TAG = "tcp_client";

// simple HTTP GET request
static const char *request =
    "GET /get HTTP/1.1\r\n"
    "Host:" HOST_IP_ADDR "\r\n"
    "User-Agent: esp32\r\n"
    "Connection: close\r\n"
    "\r\n";

static void tcp_client_task(void *pvParameters)
{
    char rx_buffer[128];
    char host_ip[] = HOST_IP_ADDR;

    struct addrinfo hints = {
        .ai_family = AF_INET,       // IPv4
        .ai_socktype = SOCK_STREAM, // TCP
    };

    struct addrinfo *res;

    while(1) {

        // resolve DNS
        int err = lwip_getaddrinfo(HOST_IP_ADDR, PORT_STR, &hints, &res);
        if (err != 0 || res == NULL) {
            ESP_LOGE(TAG, "DNS lookup failed err=%d res=%p", err, res);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            continue;
        }

        // print resolved IP address
        struct sockaddr_in *addr = (struct sockaddr_in *)res->ai_addr;
        ESP_LOGI(TAG, "DNS lookup succeeded. IP=%s", inet_ntoa(*addr));

        // create socket
        int sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (sock < 0) {
            ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
            lwip_freeaddrinfo(res);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            continue;
        }

        // connect to server
        ESP_LOGI(TAG, "Connecting to %s:%s", host_ip, PORT_STR);
        if (lwip_connect(sock, res->ai_addr, res->ai_addrlen) != 0) {
            ESP_LOGE(TAG, "Socket unable to connect: errno %d", errno);
            lwip_close(sock);
            lwip_freeaddrinfo(res);
            vTaskDelay(2000 / portTICK_PERIOD_MS);
            continue;
        }

        ESP_LOGI(TAG, "Successfully connected");
        lwip_freeaddrinfo(res); // memory cleanup - no longer needed

        // send HTTP GET request
        int err_send = lwip_send(sock, request, strlen(request), 0);
        if (err_send < 0) {
            ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
            continue;
        }

        ESP_LOGI(TAG, "HTTP request sent");

        // receive response
        ESP_LOGI(TAG, "Reading response...");

        int len;
        do {
            len = lwip_recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0);
            if (len < 0) {
                ESP_LOGE(TAG, "Error occurred during receiving: errno %d", errno);
                break;
            } else if (len == 0) {
                ESP_LOGI(TAG, "Connection closed by server.");
                break;
            } else {
                rx_buffer[len] =  0; // null-terminate whatever is received
                ESP_LOGI(TAG, "Received chunk of %d bytes:\n%s", len, rx_buffer);
            }
        } while (len > 0);

        // cleanup and close socket
        if (sock != -1) {
            ESP_LOGI(TAG, "Shutting down socket and restarting...");
            lwip_shutdown(sock, 0);
            lwip_close(sock);
        }

        // wait 10 seconds before next iteration
        vTaskDelay(10000 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

void app_main(void)
{
    // start the wifi connection
    wifi_connect_main();

    // start the tcp client task
    xTaskCreate(tcp_client_task, "tcp_client", 4096, NULL, 5, NULL);
}
