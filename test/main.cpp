#include "gptimer.hpp"
#include <iostream>
#include <utility>

int main(){
    gptimer_config_t cfg = {1, 1, 1};
    auto gtimer = Gptimer::create(cfg);
    if (!gtimer) return -1;
    auto handle = gtimer->get();
    std::cout<< "handle " << handle << std::endl;

    std::puts("-- move construct --");
    {
        auto a = Gptimer::create(cfg);
        Gptimer b = std::move(*a);
        std::printf("  src=%p dst=%p\n", (void*)a->get(), (void*)b.get());
    }

    std::puts("-- move assign --");
    {
        auto a = Gptimer::create(cfg);
        auto b = Gptimer::create(cfg);
        *a = std::move(*b);      // expect a del_timer RIGHT HERE (a's original)
        std::puts("  assigned");
    }

    return 0;
}
