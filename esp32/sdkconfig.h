#define CONFIG_TRACEMEM_RESERVE_DRAM 0x0
#define CONFIG_BT_RESERVE_DRAM 0x0
#define CONFIG_ULP_COPROC_RESERVE_MEM 0
#define CONFIG_PHY_DATA_OFFSET 0xf000
#define CONFIG_APP_OFFSET 0x10000

#define CONFIG_SPI_FLASH_ROM_DRIVER_PATCH 1
#define CONFIG_FOUR_UNIVERSAL_MAC_ADDRESS 1
#define CONFIG_NUMBER_OF_UNIVERSAL_MAC_ADDRESS 4

#define CONFIG_TCPIP_TASK_STACK_SIZE 2560

#define CONFIG_ESP32_APPTRACE_DEST_NONE 1
#define CONFIG_ESP32_PHY_AUTO_INIT 1
#define CONFIG_ESP32_PHY_MAX_TX_POWER 20
#define CONFIG_ESP32_PANIC_PRINT_REBOOT 1
#define CONFIG_ESP32_RTC_CLOCK_SOURCE_INTERNAL_RC 1
#define CONFIG_ESP32_TIME_SYSCALL_USE_RTC_FRC1 1
#define CONFIG_ESP32_DEFAULT_CPU_FREQ_MHZ 240
#define CONFIG_ESP32_DEFAULT_CPU_FREQ_240 1
#define CONFIG_ESP32_DEBUG_OCDAWARE 1
#define CONFIG_ESP32_DEEP_SLEEP_WAKEUP_DELAY 0
#define CONFIG_ESP32_ENABLE_COREDUMP_TO_NONE 1
#define CONFIG_ESP32_PHY_CALIBRATION_AND_DATA_STORAGE 1
#define CONFIG_ESP32_WIFI_AMPDU_ENABLED 1
#define CONFIG_ESP32_WIFI_NVS_ENABLED 1
#define CONFIG_ESP32_WIFI_STATIC_RX_BUFFER_NUM 10
#define CONFIG_ESP32_WIFI_DYNAMIC_RX_BUFFER_NUM 0
#define CONFIG_ESP32_WIFI_TX_BUFFER_TYPE 1
#define CONFIG_ESP32_WIFI_DYNAMIC_TX_BUFFER 1
#define CONFIG_ESP32_WIFI_DYNAMIC_TX_BUFFER_NUM 32
#define CONFIG_ESP32_XTAL_FREQ_AUTO 1
#define CONFIG_ESP32_XTAL_FREQ 0
#define CONFIG_ESP32_RTC_CLK_CAL_CYCLES 1024

#define CONFIG_FOUR_MAC_ADDRESS_FROM_EFUSE 1
#define CONFIG_NUMBER_OF_MAC_ADDRESS_GENERATED_FROM_EFUSE 4

#define CONFIG_INT_WDT 1
#define CONFIG_INT_WDT_TIMEOUT_MS 300
#define CONFIG_INT_WDT_CHECK_CPU1 0
#define CONFIG_TASK_WDT 1
#define CONFIG_TASK_WDT_TIMEOUT_S 5
#define CONFIG_TASK_WDT_CHECK_IDLE_TASK 0
#define CONFIG_TASK_WDT_CHECK_IDLE_TASK_CPU1 0

#define CONFIG_FREERTOS_UNICORE 1
#define CONFIG_FREERTOS_CORETIMER_0 1
#define CONFIG_FREERTOS_HZ 100
#define CONFIG_FREERTOS_ASSERT_FAIL_ABORT 1
#define CONFIG_FREERTOS_ASSERT_ON_UNTESTED_FUNCTION 1
#define CONFIG_FREERTOS_CHECK_STACKOVERFLOW_NONE 1
#define CONFIG_FREERTOS_THREAD_LOCAL_STORAGE_POINTERS 2
#define CONFIG_FREERTOS_ISR_STACKSIZE 1536
#define CONFIG_FREERTOS_BREAK_ON_SCHEDULER_START_JTAG 1
#define CONFIG_FREERTOS_MAX_TASK_NAME_LEN 16
#define CONFIG_SUPPORT_STATIC_ALLOCATION 1
#define CONFIG_ENABLE_STATIC_TASK_CLEAN_UP_HOOK 1

#define CONFIG_MAIN_TASK_STACK_SIZE 4096
#define CONFIG_BTC_TASK_STACK_SIZE 3072
#define CONFIG_SYSTEM_EVENT_TASK_STACK_SIZE 4096
#define CONFIG_SYSTEM_EVENT_QUEUE_SIZE 32
#define CONFIG_TIMER_TASK_PRIORITY 1
#define CONFIG_TIMER_TASK_STACK_DEPTH 2048
#define CONFIG_TIMER_QUEUE_LENGTH 10

#define CONFIG_NEWLIB_STDOUT_ADDCR 1
#define CONFIG_PHY_ENABLED 1
#define CONFIG_WIFI_ENABLED 1
#define CONFIG_OPTIMIZATION_LEVEL_DEBUG 1
#define CONFIG_MEMMAP_SMP 1

#define CONFIG_PARTITION_TABLE_SINGLE_APP 1
#define CONFIG_PARTITION_TABLE_FILENAME "partitions_singleapp.csv"
#define CONFIG_PARTITION_TABLE_CUSTOM_APP_BIN_OFFSET 0x10000
#define CONFIG_PARTITION_TABLE_CUSTOM_FILENAME "partitions.csv"

#define CONFIG_CONSOLE_UART_BAUDRATE 115200
#define CONFIG_CONSOLE_UART_NUM 0
#define CONFIG_CONSOLE_UART_DEFAULT 1

#define CONFIG_LOG_DEFAULT_LEVEL_INFO 1
#define CONFIG_LOG_BOOTLOADER_LEVEL_WARN 1
#define CONFIG_LOG_DEFAULT_LEVEL 3
#define CONFIG_LOG_COLORS 1
#define CONFIG_LOG_BOOTLOADER_LEVEL 2

#define CONFIG_LWIP_THREAD_LOCAL_STORAGE_INDEX 0
#define CONFIG_LWIP_DHCP_DOES_ARP_CHECK 1
#define CONFIG_LWIP_DHCP_MAX_NTP_SERVERS 1
#define CONFIG_LWIP_MAX_SOCKETS 8
#define CONFIG_LWIP_SO_REUSE 1
#define CONFIG_TCP_MAXRTX 12
#define CONFIG_TCP_SYNMAXRTX 6

#define CONFIG_MBEDTLS_HARDWARE_AES 1
#define CONFIG_MBEDTLS_HARDWARE_MPI 1
#define CONFIG_MBEDTLS_HARDWARE_SHA 1
#define CONFIG_MBEDTLS_SSL_MAX_CONTENT_LEN 16384
#define CONFIG_MBEDTLS_MPI_USE_INTERRUPT 1
#define CONFIG_MBEDTLS_HAVE_TIME 1

#define CONFIG_TOOLPREFIX "xtensa-esp32-elf-"
#define CONFIG_PYTHON "python2"
