add_executable(Charon)

target_include_directories(Charon PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/Charon
	${CMAKE_CURRENT_SOURCE_DIR}/vendor/asio/asio/include
	${NSCLDAQ_DIR}/utilities/format/
	${NSCLDAQ_DIR}/daq/current/include/
	${NSCLDAQ_DIR}/libtcl/include/exception/
)

target_sources(Charon PRIVATE
	Charon/ThreadSafeQueue.h
	Charon/TCPServer.h
	Charon/TCPServer.cpp
	Charon/StygianMessage.h
	Charon/Application.h
	Charon/Application.cpp
	Charon/main.cpp
	Charon/NscldaqSource.h
	Charon/NscldaqSource.cpp
)

set(THREADS_PREFER_PTHREAD_FLAG On)
find_package(Threads REQUIRED)
target_link_libraries(Charon Threads::Threads)

set_target_properties(Charon PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CHARON_BINARY_DIR})
