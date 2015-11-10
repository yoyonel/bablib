#include "Chrono.h"
BABLIB_NAMESPACE_USE

Chrono::Chrono() {
    time.start();
    }

void Chrono::start() {
    time.start();
    }

int Chrono::restart() {
    return time.restart();
    }

int Chrono::elapsed() const {
    return time.elapsed();
    }

float Chrono::elapsedSecs() const {
    return float(elapsed()) / 1000.0f;
    }

