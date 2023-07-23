build:
	clang++ -std=c++11 main.cpp timer_info_container.cpp callback_queue.cpp timer_scheduler.cpp -o timers
