
BUILD_DIR := build
COMMON_OBJ_DIR := $(BUILD_DIR)/obj/common
ASM_OBJ_DIR := $(BUILD_DIR)/obj/assembler
LNK_OBJ_DIR := $(BUILD_DIR)/obj/linker
EMU_OBJ_DIR := $(BUILD_DIR)/obj/emulator

DIR_LIBS := lib

DIR_INC := include
COMMON_INC := $(DIR_INC)
ASM_INC := $(DIR_INC)/assembler
LNK_INC := $(DIR_INC)/linker
EMU_INC := $(DIR_INC)/emulator

DEBUG_FLAG = -D DEBUG_PRINT=0

#executables
ASSEMBLER := $(BUILD_DIR)/assembler
LINKER := $(BUILD_DIR)/linker
EMULATOR := $(BUILD_DIR)/emulator

#misc dir
MISC_DIR := misc
MISC_SRC = $(filter-out parser2.yy, $(shell find ./$(MISC_DIR) -type f -printf "%P\n"))
MISC_OBJ_BASE = $(addsuffix .o, $(basename $(MISC_SRC)))
MISC_OBJ = $(addprefix $(ASM_OBJ_DIR)/, $(MISC_OBJ_BASE))

LEXER_SRC := lexer.ll
PARSER_SRC := parser.yy

LEXER_CC := $(addsuffix .cc, $(basename $(LEXER_SRC)))
PARSER_CC := $(addsuffix .cc, $(basename $(PARSER_SRC)))


#source dirs
SRC_DIR := src
COMMON_DIR ::= $(SRC_DIR)/common
ASM_DIR := $(SRC_DIR)/assembler
LNK_DIR := $(SRC_DIR)/linker
EMU_DIR := $(SRC_DIR)/emulator

#source files
COMMON_SRC = $(shell find ./$(COMMON_DIR) -name "*.cpp" -printf "%P\n")
ASM_SRC = $(shell find ./$(ASM_DIR) -name "*.cpp" -printf "%P\n")
LNK_SRC = $(shell find ./$(LNK_DIR) -name "*.cpp" -printf "%P\n")
EMU_SRC = $(shell find ./$(EMU_DIR) -name "*.cpp" -printf "%P\n")

#default flags
CXXFLAGS = -g -Wall -Werror -Wextra -Wpedantic -std=c++17 -pedantic-errors -Wno-attributes -Wno-ignored-attributes
CXXFLAGS += ${DEBUG_FLAG}
CXXFLAGS += -MMD -MP -MF"${@:%.o=%.d}"

LDLIBS := --library-path . $(patsubst %,--library=:%,${LIBS})

COMMON_OBJ = $(addprefix $(COMMON_OBJ_DIR)/, $(COMMON_SRC:.cpp=.o))
vpath $(COMMON_SRC)/%.cpp $(sort $(dir $(COMMON_SRC)))

ASM_OBJ = $(addprefix $(ASM_OBJ_DIR)/, $(ASM_SRC:.cpp=.o))
vpath $(ASM_SRC)/%.cpp $(sort $(dir $(ASM_SRC)))

LNK_OBJ = $(addprefix $(LNK_OBJ_DIR)/, $(LNK_SRC:.cpp=.o))
vpath $(LNK_SRC)/%.cpp $(sort $(dir $(LNK_SRC)))

EMU_OBJ = $(addprefix $(EMU_OBJ_DIR)/, $(EMU_SRC:.cpp=.o))
vpath $(EMU_SRC)/%.cpp $(sort $(dir $(EMU_SRC)))


$(COMMON_OBJ_DIR)/%.o: $(COMMON_DIR)/%.cpp Makefile
	@mkdir -p $(dir $@)
	${CXX} -c ${CXXFLAGS} -I $(COMMON_INC) -o $@ $<


$(ASM_DIR)/$(PARSER_CC): $(MISC_DIR)/$(PARSER_SRC)
	@mkdir -p $(dir $@)
	bison -Wcounterexamples -o $@ $<
	for file in $(ASM_DIR)/*.hh; do mv $$file $(ASM_INC); done


$(ASM_DIR)/$(LEXER_CC): $(MISC_DIR)/$(LEXER_SRC) | $(MISC_DIR)/$(PARSER_SRC)
	@mkdir -p $(dir $@)
	flex -d -o $@ $<

$(ASM_OBJ_DIR)/parser.o: $(ASM_DIR)/$(PARSER_CC) Makefile
	@mkdir -p $(dir $@)
	g++ -c ${CXXFLAGS} -Wno-unused-function -I $(ASM_INC) -o $@  $<

$(ASM_OBJ_DIR)/lexer.o: $(ASM_DIR)/$(LEXER_CC) Makefile | $(ASM_DIR)/$(PARSER_CC)
	@mkdir -p $(dir $@)
	g++ -c ${CXXFLAGS} -Wno-unused-function -I $(ASM_INC) -o $@  $<

$(ASM_OBJ_DIR)/%.o: $(ASM_DIR)/%.cpp $(COMMON_OBJ) Makefile
	@mkdir -p $(dir $@)
	${CXX} -c ${CXXFLAGS} -I $(ASM_INC) -I $(COMMON_INC) -o $@ $<

$(LNK_OBJ_DIR)/%.o: $(LNK_DIR)/%.cpp $(COMMON_OBJ) Makefile
	@mkdir -p $(dir $@)
	g++ -c $(CXXFLAGS) -I $(LNK_INC) -I $(COMMON_INC) -o $@ $<

$(EMU_OBJ_DIR)/%.o: $(EMU_DIR)/%.cpp $(COMMON_OBJ) Makefile
	@mkdir -p $(dir $@)
	g++ -c $(CXXFLAGS) -I $(EMU_INC) -I $(COMMON_INC) -o $@ $<

$(ASSEMBLER): $(MISC_OBJ) $(ASM_OBJ) $(COMMON_OBJ)
	@mkdir -p $(dir $@)
	g++ $(CXXFLAGS) -I $(ASM_INC) -o $@ $^

$(LINKER): $(LNK_OBJ) $(COMMON_OBJ)
	@mkdir -p $(dir $@)
	g++ $(CXXFLAGS) -I $(LNK_INC) -o $@ $^

$(EMULATOR): $(EMU_OBJ) $(COMMON_OBJ)
	@mkdir -p $(dir $@)
	g++ $(CXXFLAGS) -I $(EMU_INC) -o $@ $^


all: assembler linker emulator

assembler: $(ASSEMBLER)
linker: $(LINKER)
emulator: $(EMULATOR)

clean:
	rm -rf $(BUILD_DIR)
	find . \( -name "*.cc" -o -name "*.hh" \) -type f -delete

.PHONY: all assembler linker emulator clean
.PRECIOUS: %.o

-include $(COMMON_OBJ:.o=.d)
-include $(ASM_OBJ:.o=.d)
-include $(LNK_OBJ:.o=.d)
-include $(EMU_OBJ:.o=.d)
