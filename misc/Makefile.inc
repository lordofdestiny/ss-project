include $(MakefileBuildRoot)/Makefile.global_vars

MODULE_NAME := misc

include $(MakefileBuildRoot)/Makefile.module_vars

MISC_SRC_DIR=$(MODULE_NAME)

LEXER_EXT=ll
PARSER_EXT=yy

MISC_SRC := $(shell find ./$(MISC_SRC_DIR) -regex ".*\.\($(LEXER_EXT)\|$(PARSER_EXT)\)" -printf "%P\n")

LEXER_SRC = $(filter %.$(LEXER_EXT), $(MISC_SRC))
PARSER_SRC = $(filter %.$(PARSER_EXT), $(MISC_SRC))

LEXER_CC = $(misc-build-src-path)/$(LEXER_SRC:.$(LEXER_EXT)=.cc)
PARSER_CC = $(misc-build-src-path)/$(PARSER_SRC:.$(PARSER_EXT)=.cc)

PARSER_HH = $(misc-build-include-path)/$(PARSER_SRC:.$(PARSER_EXT)=.hh)
LOCATION_HH = $(misc-build-include-path)/location.hh

LEXER_OBJ = $(misc-obj-path)/$(LEXER_SRC:.$(LEXER_EXT)=.o)
PARSER_OBJ = $(misc-obj-path)/$(PARSER_SRC:.$(PARSER_EXT)=.o)

$(misc-obj): $(LEXER_OBJ) $(PARSER_OBJ)
	@mkdir -p $(dir $@)
	$(SILENT)$(CXX) -r -o $@ $^
	@echo "Misc built."

CXXFLAGS_EXTRA = -Wno-unused-function

INC_PATH = $(addprefix -I,$(assembler-include-path)) -I$(misc-build-include-path)

$(PARSER_OBJ): $(PARSER_CC) $(common-lib-path)
	@mkdir -p $(dir $@)
	$(SILENT)$(CXX) -c $(CXXFLAGS) $(CXXFLAGS_EXTRA) $(INC_PATH) -o $@  $<

$(LEXER_OBJ): $(LEXER_CC) $(PARSER_CC) $(common-lib-path)
	@mkdir -p $(dir $@)
	$(SILENT)$(CXX) -c $(CXXFLAGS) $(CXXFLAGS_EXTRA) $(INC_PATH) -o $@  $<

$(PARSER_CC) $(PARSER_HH) $(LOCATION_HH)&: $(MISC_SRC_DIR)/$(PARSER_SRC) 
	@mkdir -p $(dir $(PARSER_CC)) $(dir $(PARSER_HH))
	$(SILENT)bison -d $< -o $(PARSER_CC) --defines=$(PARSER_HH)

$(LEXER_CC): $(MISC_SRC_DIR)/$(LEXER_SRC) $(PARSER_CC)
	@mkdir -p $(dir $@)
	$(SILENT)flex -d -o $@ $<
