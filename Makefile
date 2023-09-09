main2:
	clang++ -std=c++11 main2.cpp timer_info_container.cpp callback_queue.cpp timer_scheduler.cpp process_callback_worker.cpp -o timers
main:
	clang++ -std=c++11 main.cpp timer_info_container.cpp callback_queue.cpp timer_scheduler.cpp process_callback_worker.cpp -o timers
order:
	clang++ -std=c++11 ./tests/system/execution_order.cpp timer_info_container.cpp callback_queue.cpp timer_scheduler.cpp process_callback_worker.cpp -o timers
time:
	clang++ -std=c++11 ./tests/system/execution_schedule_time.cpp timer_info_container.cpp callback_queue.cpp timer_scheduler.cpp process_callback_worker.cpp -o timers
