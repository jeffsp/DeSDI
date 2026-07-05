default: help

.PHONY: build_parser # Build the C++ ingestion parser using CMake
build_parser:
	@cd parser && cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -Wno-dev && cmake --build build

.PHONY: test_parser # Run the Catch2 unit tests for the parser
test_parser:
	@cd parser/build && ctest --output-on-failure

.PHONY: clean # Remove the parser build directory
clean:
	@rm -rf ./parser/build

.PHONY: help # Generate list of targets with descriptions
help:
	@grep '^.PHONY: .* #' Makefile | sed 's/\.PHONY: \(.*\) # \(.*\)/\1	\2/' | expand -t20
