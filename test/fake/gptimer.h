#pragma once
#include <cstdio>
typedef struct gptimer_t* gptimer_handle_t;
typedef int esp_err_t;
#define ESP_OK 0
#define ESP_ERR_NO_MEM 0x101
struct gptimer_config_t { int clk_src; int direction; unsigned resolution_hz; };

inline int timerCreatedCt = 0;
inline int timerDelCt = 0;

inline esp_err_t gptimer_new_timer(const gptimer_config_t*, gptimer_handle_t* out) {
    *out = (gptimer_handle_t)new int(1);
    std::printf("  [C] new_timer -> %p\n", (void*)*out);
    timerCreatedCt++;
    return ESP_OK;
}
inline esp_err_t gptimer_del_timer(gptimer_handle_t h) {
    std::printf("  [C] del_timer  <- %p\n", (void*)h);
    timerDelCt++;
    delete (int*)h;
    return ESP_OK;
}

inline void fakeReset() {
    timerCreatedCt = 0;
    timerDelCt = 0;
}
