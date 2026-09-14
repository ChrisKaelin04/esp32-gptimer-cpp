#include "gptimer.hpp"
#include <iostream>
#include <utility>
#include <cassert>

int main(){
    int failures = 0;

    //Basic 
    fakeReset();
    {
        auto gtimer = Gptimer::create({1, 3, 1});
        if (!gtimer) return -1;
    }
    
    if (timerCreatedCt != 1) {
        std::puts("Creation failure");
        failures++;
    }
    if (timerDelCt != 1) {
        std::puts("Deletion failure");
        failures++;
    }

    //Basic with release
    gptimer_handle_t raw = nullptr;
    fakeReset();
    {
        auto gtimer = Gptimer::create({1, 3, 1});
        if (!gtimer) return -1;
        raw = gtimer->release();
    }
    
    if (timerCreatedCt != 1) {
        std::puts("Creation failure");
        failures++;
    }
    if (timerDelCt != 0) {
        std::puts("Deletion failure");
        failures++;
    }
    gptimer_del_timer(raw);
    if (timerDelCt != 1) {
        std::puts("Deletion failure");
        failures++;
    }

    //Move Construct
    fakeReset();
    {
        auto gtimer = Gptimer::create({1, 3, 1});
        if (!gtimer) return -1;
        auto gtimer2 = std::move(*gtimer);
    }

    if (timerCreatedCt != 1) {
        std::puts("Creation failure");
        failures++;
    }

    if (timerDelCt != 1) {
        std::puts("Deletion failure");
        failures++;
    }

    //Move Overtop
    fakeReset();
    {
        auto gtimer = Gptimer::create({1, 3, 1});
        if (!gtimer) return -1;
        auto gtimer2 = Gptimer::create({1, 2, 1});
        gtimer2 = std::move(*gtimer);
        if (timerDelCt > 1) {
            std::puts("Deletion failure");
            failures++;
        }
    }

    if (timerCreatedCt != 2) {
        std::puts("Creation failure");
        failures++;
    }

    if (timerDelCt != 2) {
        std::puts("Deletion failure");
        failures++;
    }

    //Self Move
    fakeReset();
    {
        auto gtimer = Gptimer::create({1, 3, 1});
        gtimer = std::move(*gtimer);
        if (timerDelCt != 0) {
            std::puts("Deletion failure");
            failures++;
        }
    }

    if (timerCreatedCt != 1) {
        std::puts("Creation failure");
        failures++;
    }

    if (timerDelCt != 1) {
        std::puts("Deletion failure");
        failures++;
    }

    

    assert(failures == 0);


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
