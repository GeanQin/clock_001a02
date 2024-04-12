#include "esp_littlefs.h"
#include "littlefs_util.h"

static const esp_vfs_littlefs_conf_t conf = {
    .base_path = "/littlefs",
    .partition_label = "storage",
    .format_if_mount_failed = true,
    .dont_mount = false,
};

int littlefs_util_init()
{
    printf("Initializing LittleFS\n");

    esp_err_t ret = esp_vfs_littlefs_register(&conf);

    if (ret != ESP_OK)
    {
        if (ret == ESP_FAIL)
        {
            printf("Failed to mount or format filesystem\n");
        }
        else if (ret == ESP_ERR_NOT_FOUND)
        {
            printf("Failed to find LittleFS partition\n");
        }
        else
        {
            printf("Failed to initialize LittleFS (%s)\n", esp_err_to_name(ret));
        }
        return -1;
    }

    size_t total = 0, used = 0;
    ret = esp_littlefs_info(conf.partition_label, &total, &used);
    if (ret != ESP_OK)
    {
        printf("Failed to get LittleFS partition information (%s)\n", esp_err_to_name(ret));
        esp_littlefs_format(conf.partition_label);
    }
    else
    {
        printf("Partition size: total: %d, used: %d\n", total, used);
    }
    
    return 0;
}

void littlefs_util_format()
{
    esp_littlefs_format(conf.partition_label);
}

void littlefs_util_deinit()
{
    esp_vfs_littlefs_unregister(conf.partition_label);
    printf("LittleFS unmounted\n");
}