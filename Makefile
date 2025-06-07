export MakefileBuildRoot=$(abspath .)

include Makefile.global_vars

ifeq ($(MAKECMDGOALS),) 
    $(error No target specified.)
endif

include ./misc/Makefile.inc
include ./src/common/Makefile.inc
include ./src/assembler/Makefile.inc
include ./src/linker/Makefile.inc
include ./src/emulator/Makefile.inc

binaries := assembler linker emulator

all: $(binaries)
	@echo Done.

clean: clean_tests
	rm -rf $(BUILD_DIR) $(OUT_DIR)

clean_tests:
	@cd ./tests/nivo-a; rm -f *.o *.hex $(binaries)
	@cd ./tests/nivo-b; rm -f *.o *.hex $(binaries)

bin_paths = $(assembler-bin-path) $(linker-bin-path) $(emulator-bin-path)

test_a: ./tests/nivo-a
	@cd $<; rm -f *.o *.hex $(binaries)
	@cp $(bin_paths) $<
	@cd $<; bash ./start.sh

test_b: ./tests/nivo-b
	@cd $<; rm -f *.o *.hex $(binaries)
	@cp $(bin_paths) $<
	@cd $<; bash ./start.sh

.PHONY: all assembler linker emulator clean clean_tests test_a test_b
