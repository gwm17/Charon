add_executable(Anubis)

target_include_directories(Anubis PRIVATE
	./Anubis
	./vendor
	${NSCLDAQ_DIR}/include
)

target_sources(Anubis PRIVATE
)

set_target_properties(Anubis PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${ANUBIS_BINARY_DIR})