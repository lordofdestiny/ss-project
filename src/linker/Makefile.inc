include $(MakefileBuildRoot)/Makefile.global_vars

MODULE_TYPE := bin
MODULE_NAME := linker

include $(MakefileBuildRoot)/Makefile.module_vars

LNK_SRC = $(shell find ./$(linker-src-path) -name "*.cpp" -printf "%P\n")
LNK_OBJ = $(addprefix $(linker-obj-path)/, $(LNK_SRC:.cpp=.o))
vpath $(LNK_SRC)/%.cpp $(sort $(dir $(LNK_SRC)))

linker: $(linker-bin-path)

$(linker-bin-path): $(LNK_OBJ) $(common-lib-path)
	@mkdir -p $(dir $@)
	$(SILENT)$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "Linker built."

$(linker-obj-path)/%.o: $(linker-src-path)/%.cpp
	@mkdir -p $(dir $@)
	$(SILENT)$(CXX) -c $(CXXFLAGS) $(addprefix -I, $(linker-include-path)) -o $@ $<

-include $(LNK_OBJ:.o=.d)
