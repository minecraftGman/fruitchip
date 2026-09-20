#include <stdio.h>

#include <hardware/i2c.h>
#include <hardware/gpio.h>

#include <ssd1306.h>

#include <git_version.h>
#include <settings.h>

#include <oled.h>
#include <oled_splash.h>

static ssd1306_t disp;
static bool disp_ok = false;

void oled_init(void)
{
    i2c_init(i2c0, 400 * 1000);
    gpio_set_function(OLED_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(OLED_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(OLED_SDA_PIN);
    gpio_pull_up(OLED_SCL_PIN);

    disp.external_vcc = false;

    // most of these cheap 0.96" modules are 0x3C; a few strap to 0x3D
    disp_ok = ssd1306_init(&disp, 128, 64, 0x3C, i2c0);

    if (!disp_ok)
        printf("oled: init failed (check wiring / i2c address)\n");
}

void oled_show_splash(uint32_t splash_ms)
{
    if (!disp_ok)
        return;

    ssd1306_clear(&disp);

#if OLED_SPLASH_USE_BITMAP
    // Full-screen 128x64 1bpp bitmap, defined in oled_splash.h.
    ssd1306_bmp_show_image(&disp, oled_splash_bmp, sizeof(oled_splash_bmp));
#endif

    ssd1306_show(&disp);
    sleep_ms(splash_ms);
}

void oled_task(void)
{
    if (!disp_ok)
        return;

    ssd1306_clear(&disp);

    ssd1306_draw_string(&disp, 0, 0, 1, "fruitchip");
    ssd1306_draw_string(&disp, 0, 12, 1, GIT_REV);

    ssd1306_draw_string(&disp, 0, 28, 1,
        settings_get_menu_autoboot() ? "autoboot: on" : "autoboot: off");

    char line[32];
    snprintf(line, sizeof(line), "delay: %us", settings_get_menu_autoboot_delay_sec());
    ssd1306_draw_string(&disp, 0, 40, 1, line);

    snprintf(line, sizeof(line), "item: %u", settings_get_menu_autoboot_item_idx());
    ssd1306_draw_string(&disp, 0, 52, 1, line);

    ssd1306_show(&disp);
}
