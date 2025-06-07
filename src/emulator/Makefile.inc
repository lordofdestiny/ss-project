include $(MakefileBuildRoot)/Makefile.global_vars

MODULE_TYPE := bin
MODULE_NAME := emulator

include $(MakefileBuildRoot)/Makefile.module_vars

EMU_SRC = $(shell find ./$(emulator-src-path) -name "*.cpp" -printf "%P\n")
EMU_OBJ = $(addprefix $(emulator-obj-path)/, $(EMU_SRC:.cpp=.o))
vpath $(EMU_SRC)/%.cpp $(sort $(dir $(EMU_SRC)))

emulator: $(emulator-bin-path)

$(emulator-bin-path): $(EMU_OBJ) $(common-lib-path)
	@mkdir -p $(dir $@)
	$(SILENT)$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "Emulator built."

$(emulator-obj-path)/%.o: $(emulator-src-path)/%.cpp
	@mkdir -p $(dir $@)
	$(SILENT)$(CXX) -c $(CXXFLAGS) $(addprefix -I, $(emulator-include-path)) -o $@ $<

-include $(EMU_OBJ:.o=.d)
