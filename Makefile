build:
	clang++ -std=c++11 main2.cpp timer_info_container.cpp callback_queue.cpp timer_scheduler.cpp process_callback_worker.cpp -o timers
