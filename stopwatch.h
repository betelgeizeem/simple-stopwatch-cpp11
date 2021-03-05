#pragma once
#include "times.h"

// Stopwatch class. Measures time.
class Stopwatch {
public:
	// Class constructor creates a Stopwatch object
	// but does not turn it on. 
	Stopwatch() : start_time(),
				  elapsed(),
				  up(false) {}

	// Measure time spent on function execution for a function f(args)
	// returning void.
	template<class F, class... Args>
	typename std::enable_if<std::is_void<typename std::result_of<F(Args...)>::type>::value>::type
	measure(F&& f, Args&&... args) {
    	start(); 
		std::forward<F>(f)(std::forward<Args>(args)...);
		stop();
	}

	// Measure time spent on function execution for a function f(args)
	// returning non-void.
	template<class F, class... Args>
	typename std::enable_if<!std::is_void<typename std::result_of<F(Args...)>::type>::value, typename std::result_of<F(Args...)>::type>::type
	measure(F&& f, Args&&... args) {
		start(); 
		auto res = std::forward<F>(f)(std::forward<Args>(args)...);
		stop();
		return res;
	}

    // Start measuring time (turn stopwatch on).
	void start() { 
    	up = true;
    	elapsed = elapsed_time_t();
    	start_time = now();
    }

	// Stop measuring time (turn stopwatch off).
	void stop() {
    	if (up) {
	    	elapsed += now_proc_and_wall() - start_time;
	    	up = false;
	    }
    }

	// Resume measuring time (turn stopwatch on without starting over).
    void resume () {
    	if (!up) {
		    start_time = now();
	    	up = true;
	    }
    }

	// Check if a stopwatch is on, i.e. measures time at the moment.
    bool is_up() const { return up; }

	// Get times measured by a stopwatch.
	template<class T = fseconds>
    elapsed_time_t times() const { 
    	if (up) 
    		return elapsed + (now_proc_and_wall() - start_time);
    	else    
    		return elapsed;
    }

private:
	elapsed_time_t elapsed; // Elapsed time.
	time_point_t start_time; // Start time.

    bool up; // Variable that indicates if a stopwatch is on.
};
