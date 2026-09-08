# gptimer-cpp

A zero-overhead C++ RAII wrapper over the ESP-IDF `gptimer` C API.

## Design contract
- Owns exactly one `gptimer_handle_t`. `gptimer_del_timer()` releases every
  sub-resource (pm_lock, intr, group), so the class owns one thing, not four.
- Copy is deleted: two owners means a double `gptimer_del_timer()`.
- Move transfers the handle and leaves the source empty (`handle_ == nullptr`).
  The destructor must survive a moved-from object.
- Construction can fail (`esp_err_t`). Exceptions are unavailable under
  `-fno-exceptions`, so construction goes through a static factory returning
  `std::optional<Gptimer>`; a constructed object is always valid.
