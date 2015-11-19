#include <iostream>
#include <functional>
#include <string>
#include <chrono>

template <typename F>
float calc1(F f) { return -1.0f * f(3.3f) + 666.0f; }

float calc2(std::function<float(float)> f) { return -1.0f * f(3.3f) + 666.0f; }

int main() {
    using namespace std::chrono;

	{
		const auto tp1 = system_clock::now();
		for (int i = 0; i < 1e8; ++i) {
			calc1([](float arg){ return arg * 0.5f; });
		}
		const auto tp2 = high_resolution_clock::now();

		const auto d = duration_cast<milliseconds>(tp2 - tp1);  
		std::cout << d.count() << std::endl;
	}

	{
		const auto tp1 = system_clock::now();
		for (int i = 0; i < 1e8; ++i) {
			calc2([](float arg){ return arg * 0.5f; });
		}
		const auto tp2 = high_resolution_clock::now();

		const auto d = duration_cast<milliseconds>(tp2 - tp1);  
		std::cout << d.count() << std::endl;
	}
    return 0;
}
