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


/* ---- callback API (mirrors the real IDF signatures) ---- */

struct gptimer_alarm_event_data_t {
    unsigned long long count_value;
    unsigned long long alarm_value;
};

typedef bool (*gptimer_alarm_cb_t)(gptimer_handle_t timer,
                                   const gptimer_alarm_event_data_t* edata,
                                   void* user_ctx);

struct gptimer_event_callbacks_t {
    gptimer_alarm_cb_t on_alarm;
};

/* the driver stores exactly these two things and nothing else */
inline gptimer_alarm_cb_t g_registered_cb = nullptr;
inline void*              g_registered_ctx = nullptr;
inline gptimer_handle_t   g_registered_timer = nullptr;
inline int                g_registerCt = 0;

inline esp_err_t gptimer_register_event_callbacks(gptimer_handle_t timer,
                                                  const gptimer_event_callbacks_t* cbs,
                                                  void* user_data) {
    g_registered_timer = timer;
    g_registered_cb = cbs->on_alarm;
    g_registered_ctx = user_data;
    g_registerCt++;
    std::printf("  [C] register_cb timer=%p fn=%p ctx=%p\n",
                (void*)timer, (void*)(void(*)())cbs->on_alarm, user_data);
    return ESP_OK;
}

/* test-only: pretend the hardware alarm fired */
inline bool fakeFireAlarm(unsigned long long count, unsigned long long alarm) {
    if (!g_registered_cb) { std::puts("  [C] alarm fired, no callback registered"); return false; }
    gptimer_alarm_event_data_t ed{count, alarm};
    return g_registered_cb(g_registered_timer, &ed, g_registered_ctx);
}

inline void fakeReset() {
    timerCreatedCt = 0;
    timerDelCt = 0;
    g_registered_cb = nullptr;
    g_registered_ctx = nullptr;
    g_registered_timer = nullptr;
    g_registerCt = 0;
}
