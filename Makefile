SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := .

EXE := ./mips2c

SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CPPFLAGS := -Iinclude -MMD -MP
CFLAGS := -pedantic -Wall -Wextra -Wfloat-equal -std=gnu99 -g
LDLIBS := -lncurses

.PHONY: all
all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $^ -o $@ $(LDLIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

.PHONY: clean
clean:
	@$(RM) -rv $(OBJ_DIR) $(EXE)

.PHONY: check
check:
	@cd tests; bash run_tests.sh

-include $(OBJ:.o=.d)
