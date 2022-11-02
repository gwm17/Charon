add_executable(Charon)

target_include_directories(Charon PRIVATE
	./Charon
	./vendor/asio/include/asio
	${NSCLDAQ_DIR}/include
)

target_sources(Charon PRIVATE
	Charon/ThreadSafeQueue.h
)

set_target_properties(Charon PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CHARON_BINARY_DIR})