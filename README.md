# simple-stopwatch-cpp11

Simple library for in-code measuring of program computational efficiency. It measures two types of time simultaniously: processor clock time (time which is spent for processor calculations) and wall-clock (system clock time). 

## Usage
Include `stopwatch.h` into your project and use the `Stopwatch` class like:
```
Stopwatch sw;
sw.start();
... *your code here*
sw.stop();
```
or, for a function:
```
Stopwatch sw;
auto a = sw.measure(&func, arg1, arg2);
// or
sw.measure(&func, arg1, arg2);
```
Then get measured times by calling `sw.times().proc()` or `sw.times().wall()`, which are of type `float` and return seconds by default:
```
sw.times.proc(); // get processor time in seconds
sw.times.wall<std::chrono::minutes>(); // get wall-clock time in minutes
sw.times.proc<std::chrono::milliseconds>(); // get processor time in milliseconds
```

Stopwatch can be resumed after stop (`sw.resume()`). To start it all over, one should call `sw.start()` at any time.

## How to build
Clone the repository anywhere, go into the folder with `test.cpp` and type in your console:
```
c++ -pthread test.cpp -o out
```
The line can vary depending on your compiler and OS. Main ideas: call the compiler (`c++`) with pthread (`-pthread`) support for `test.cpp` file to get `out` executable file. Pthread support is **NOT** needed for `Stopwatch` class functionality to work, it is included only for test demonstration.

## Test & some explanations
The repo provides test `test.cpp` file to demonstrate the functionality. Sample output with author's comments:
```
Start stopwatch, checking times calling a "heavy" function...
rand_sum of 10000000 = 5.00034e+06
  t_proc = 0.07952s
  t_wall = 0.0795423s
  t_proc/t_wall = 0.999719 // processor spent quite all time for work

Stop & resume & wait 1s & stop the same watch... 
  t_proc = 0.079543s
  t_wall = 1.08035s
  t_proc/t_wall = 0.0736272 // processor spent a bit of additional time 
  	// for calling sleeping routine (std::this_thread::sleep_for(...)), 
  	// but 1 seconds of waiting contributed here to get overall 
  	// ratio = 0.07 efficiency: 7% of previous calculations and 
  	// 93% of waiting.
------------- 
Start watch, checking function measure...
rand_sum of 10000000 = 5.00045e+06
  t_proc = 0.066823s
  t_wall = 0.0669298s
  t_proc/t_wall = 0.998404 // processor spent quite all time for work, 
  	// but now the result was obtained by measure() method
------------- 
Doing useful work 4 times in separate threads... 
rand_sum of 10000000 = 5.00197e+06
rand_sum of 10000000 = 5.00069e+06
rand_sum of 10000000 = 4.99942e+06
rand_sum of 10000000 = 4.99831e+06
  t_proc = 15.9002s
  t_wall = 4.69266s
  t_proc/t_wall = 3.38831 // the program created 4 threads and 
  	// they together spent 3.39 x time more than system time that passed
```
Ratio `t_proc/t_wall` indicates efficiency of programs' processor core usage.