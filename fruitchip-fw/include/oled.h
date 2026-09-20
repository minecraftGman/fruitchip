#pragma once

// Initializes I2C0 on OLED_SDA_PIN/OLED_SCL_PIN and the SSD1306 panel.
// Must be called from core1, after reset_init_irq() and after
// multicore_lockout_victim_init(), since it touches flash-adjacent state
// (settings) indirectly via oled_task().
void oled_init(void);

// Shows the boot splash for splash_ms milliseconds, then clears. Call once,
// right after oled_init(), before entering the status loop.
void oled_show_splash(uint32_t splash_ms);

// Redraws the display with current status/settings. Cheap enough to call
// in a loop from core1; does not touch anything shared with core0's
// time-critical boot ROM sniffer loop.
void oled_task(void);
