#pragma once
#include "gptimer.h"
#include <cassert>
#include <optional>
#include <memory>

class Gptimer {
    struct Ctl {
        gptimer_handle_t handle = nullptr;
        void* user_ctx = nullptr;
        gptimer_alarm_cb_t cb = nullptr;
    };
    std::unique_ptr<Ctl> ctl_;

    explicit Gptimer(gptimer_handle_t passed_handle_) noexcept : ctl_(std::make_unique<Ctl>()) {
        //Make unique could fail but if it does then rebooting is alright
        //Means something terrible went wrong and we should stop anyways
        ctl_->handle = passed_handle_;
    }
    public:
        ~Gptimer() noexcept {
            if (ctl_ && ctl_->handle) {
                gptimer_del_timer(ctl_->handle);
            }
        }

        Gptimer(const Gptimer& timer) = delete;
        Gptimer& operator=(const Gptimer& timer) = delete;

        Gptimer(Gptimer&& other) noexcept : ctl_(std::move(other.ctl_)) {}

        Gptimer& operator=(Gptimer&& timer) noexcept {
            if (&timer == this) return *this;
            if (ctl_ && ctl_->handle) gptimer_del_timer(ctl_->handle);
            ctl_ = std::move(timer.ctl_);
            return *this;
        }

        static std::optional<Gptimer> create(const gptimer_config_t& cfg) {
            gptimer_handle_t handle = nullptr;
            esp_err_t out = gptimer_new_timer(&cfg, &handle);
            if (out == ESP_OK) return Gptimer(handle);
            return std::nullopt;
        }

        gptimer_handle_t get() const noexcept { return ctl_ ? ctl_->handle : nullptr; }

        /*
        Removed release(), no gptimer-related function (there are only 4) actually require
        ownership transfer. They either delete or borrow, so release is unnecessary.
        */

        void on_alarm(gptimer_alarm_cb_t cb, void* user_ctx) {
            ctl_->cb = cb;
            ctl_->user_ctx = user_ctx;
            gptimer_event_callbacks_t eCB{&Gptimer::trampoline};
            gptimer_register_event_callbacks(ctl_->handle, &eCB, ctl_.get());
        }

        static bool trampoline(gptimer_handle_t handle, const gptimer_alarm_event_data_t* eData, void* ctx) {
            auto* c = static_cast<Ctl*>(ctx);
            assert(c->handle == handle);
            if (c->cb) c->cb(handle, eData, c->user_ctx);
            return false;
        }
};
