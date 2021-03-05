#include <iostream>
#include <thread>
#include <future>
#include <vector>
#include "stopwatch.h"

int n = 10000000;

double rand_sum(int count) {
	double res = 0;
	
	for (int i = 0; i < count; i++) 
		res += rand() / (double)RAND_MAX;
	std::cout << "rand_sum of " << count << " = " << res << std::endl;

	return res;
} 

void output_times(const Stopwatch& sw) {
	auto times = sw.times();
	std::cout << "  t_proc = " << times.proc() << "s" << std::endl;
	std::cout << "  t_wall = " << times.wall() << "s" << std::endl;
	std::cout << "  t_proc/t_wall = " << times.proc() / times.wall() << std::endl;
}

void check_simple() {	
	Stopwatch sw;
	std::cout << "Start stopwatch, checking separate functions time_wallclock, time_processor..." << std::endl;
	sw.start();
	rand_sum(n);
	output_times(sw);

	std::cout << std::endl;
	std::cout << "Stop & resume & wait 1s & stop the same watch... " << std::endl;
	sw.stop(); 
	sw.resume();
	std::this_thread::sleep_for(std::chrono::seconds(1));
	output_times(sw);
}

void check_measure_func() {
	Stopwatch sw;
	std::cout << "Start watch, checking function measure..." << std::endl;
	double s = sw.measure(&rand_sum, n);
	output_times(sw);
}

void check_async_run(std::size_t ntimes) {
	Stopwatch sw;
	std::cout << "Doing useful work " << ntimes << " times in separate threads... " << std::endl;
	sw.start();
	auto futures = std::vector<std::future<double>>();
	for (std::size_t i = 0; i < ntimes; ++i)
		futures.push_back(std::async(std::launch::async, &rand_sum, n));
	for (auto& i: futures) i.get();
	output_times(sw);
}


int main(int argc, char const *argv[]) {
	check_simple();
	std::cout << "------------- " << std::endl;
	check_measure_func();
	std::cout << "------------- " << std::endl;
	check_async_run(4);

	return 0;
}

