 
using wall_clock = std::chrono::system_clock; // System clock.
using fseconds = std::chrono::duration<float>; // Seconds.

using wall_t = std::chrono::time_point<wall_clock>; // Point in time, for system clock.
using proc_t = std::clock_t; // Point in time, for processor clock ticks time.

using wall_t_elapsed = std::chrono::duration<float>; // System time is measured by float variable.	
using proc_t_elapsed = std::clock_t; // Process running time is measured by std::clock_t.

// System time now.
static wall_t now(wall_t&) { return wall_clock::now(); } 
// Processor time now.
static proc_t now(proc_t&) { return std::clock(); } 


struct time_point_t { proc_t p; wall_t w; };
	// Time point: processor and system time.


// Find out time now; first \--- processor time, 
//                   second \--- system time.
static time_point_t now_wall_and_proc() { time_point_t a; a.w = now(a.w); a.p = now(a.p); return a; }

// Find out time now; first \--- system time, 
//                   second \--- processor time.
static time_point_t now_proc_and_wall() { time_point_t a; a.p = now(a.p); a.w = now(a.w); return a; }

// Find out time now, by default.
static time_point_t now() { return now_wall_and_proc(); }


// Class to measure elapsed times (time intervals).
class elapsed_time_t {
public:
	// Class constructor for zero elapsed time.
	elapsed_time_t() : p(0), 
					   w(std::chrono::duration<float>::zero()) {}
	
	// Class constructor for non-zero elapsed time.
	// p --- process time. w --- system time.
	elapsed_time_t(proc_t_elapsed p, wall_t_elapsed w) : p(p), w(w) {}

	// Get processor time.
	template<class T = fseconds> 
	float proc() {
		return p / (double)CLOCKS_PER_SEC * (double)(typename T::period().den) / (double)(typename T::period().num);
	} 

	// Get system time.	
	template<class T = fseconds> 
	float wall() {
		using dur = std::chrono::duration<float, typename T::period>;
		return std::chrono::duration_cast<dur>(w).count();
	} 

	// Add time interval t.
	elapsed_time_t& operator += (const elapsed_time_t& t) {
		p += t.p;
		w += t.w;
		return *this;
	}
	
	// Sum with time interval t.
	elapsed_time_t operator + (const elapsed_time_t& t) const {
		elapsed_time_t res;
		res.p = p + t.p;
		res.w = w + t.w;
		return res;
	}

private:
	proc_t_elapsed p;
		// Elapsed processor time. 
	wall_t_elapsed w;
		// Elapsed system time.  
};

//	Measure time between two time points t1 and t2.
static elapsed_time_t operator - (const time_point_t& t1, const time_point_t& t2) {
	return elapsed_time_t(t1.p - t2.p, t1.w - t2.w);
}