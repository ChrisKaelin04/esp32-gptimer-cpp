#pragma once
#include "gptimer.h"
#include <optional>

class Gptimer {
    gptimer_handle_t handle_;

    explicit Gptimer(gptimer_handle_t passed_handle_) noexcept: handle_(passed_handle_) {}
    public:
        ~Gptimer() noexcept {
            if (handle_) gptimer_del_timer(handle_);
        }

        Gptimer(const Gptimer& timer) = delete;
        Gptimer& operator=(const Gptimer& timer) = delete;

        Gptimer(Gptimer&& other) noexcept : handle_(other.handle_) { other.handle_ = nullptr; }
        Gptimer& operator=(Gptimer&& timer) noexcept {
            if (&timer == this) return *this;
            if (handle_) gptimer_del_timer(handle_);
            this->handle_ = timer.handle_;
            timer.handle_ = nullptr;
            return *this;
        }

        static std::optional<Gptimer> create(const gptimer_config_t& cfg) {
            gptimer_handle_t handle = nullptr;
            esp_err_t out = gptimer_new_timer(&cfg, &handle);
            if (out == ESP_OK) return Gptimer(handle);
            return std::nullopt;
        }

        gptimer_handle_t get() const noexcept {
            return handle_;
        }
};
