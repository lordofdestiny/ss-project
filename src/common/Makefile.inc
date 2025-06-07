include $(MakefileBuildRoot)/Makefile.global_vars

MODULE_TYPE := lib
MODULE_NAME := common

include $(MakefileBuildRoot)/Makefile.module_vars

CMN_SRC = $(shell find ./$(common-src-path) -name "*.cpp" -printf "%P\n")
CMN_OBJ = $(addprefix $(common-obj-path)/,$(CMN_SRC:.cpp=.o))
vpath $(CMN_SRC)/%.cpp $(sort $(dir $(CMN_SRC)))

$(common-lib-path): $(CMN_OBJ)
	@mkdir -p $(dir $@)
	$(SILENT)ar rcs $@ $^
	@echo "Common built."

$(common-obj-path)/%.o: $(common-src-path)/%.cpp
	@mkdir -p $(dir $@)
	$(SILENT)$(CXX) $(CXXFLAGS) -c -I$(INC_DIR) -o $@ $<

-include $(CMN_OBJ:.o=.d)