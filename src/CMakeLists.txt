add_executable(Charon)

target_include_directories(Charon PRIVATE
	./Charon
	./vendor/asio/asio/include
	${NSCLDAQ_DIR}/include
)

target_sources(Charon PRIVATE
	Charon/ThreadSafeQueue.h
	Charon/TCPServer.h
	Charon/TCPServer.cpp
	Charon/StygianMessage.h
)

set(THREADS_PREFER_PTHREAD_FLAG On)
find_package(Threads REQUIRED)
target_link_libraries(Charon Threads::Threads)

set_target_properties(Charon PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CHARON_BINARY_DIR})