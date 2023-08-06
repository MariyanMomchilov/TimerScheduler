build:
	clang++ -std=c++11 main2.cpp timer_info_container.cpp callback_queue.cpp timer_scheduler.cpp -o timers

execution_schedule:
	clang++ -std=c++11 ./tests/execution_schedule_time.cpp timer_info_container.cpp callback_queue.cpp timer_scheduler.cpp -o execution_schedule

execution_order:
	clang++ -std=c++11 ./tests/execution_order.cpp timer_info_container.cpp callback_queue.cpp timer_scheduler.cpp -o execution_order
