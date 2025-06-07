MODULE_TYPE := bin
MODULE_NAME := assembler

include $(MakefileBuildRoot)/Makefile.module_vars

ASM_SRC = $(shell find ./$(assembler-src-path) -name "*.cpp" -printf "%P\n")
ASM_OBJ = $(addprefix $(assembler-obj-path)/, $(ASM_SRC:.cpp=.o))
vpath $(ASM_SRC)/%.cpp $(sort $(dir $(ASM_SRC)))

assembler: $(assembler-bin-path)

$(assembler-bin-path): $(ASM_OBJ) $(misc-obj) $(common-lib-path)
	@mkdir -p $(dir $@)
	$(SILENT)$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "Assembler built."

$(assembler-obj-path)/%.o: $(assembler-src-path)/%.cpp $(misc-obj)
	@mkdir -p $(dir $@)
	$(SILENT)$(CXX) -c $(CXXFLAGS) $(addprefix -I, $(assembler-include-path)) -o $@ $<

-include $(ASM_OBJ:.o=.d)
