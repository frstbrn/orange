
MAT_HOMEDIR=../

## WRITE YOUR MODULE NAME
MOD_NAME=common

# INCLUDE
include ../make.inc

## ADD DEPENDENT MODULES HERE
#SUB_MODULES = . mutil log mapp mbus mconn mtimer mrpc
SUB_MODULES =

## ADD DEPENDENT LIBARYS HERE
#EXT_LIBARYS += $(MAT_HOMEDIR)/src/extlib/libemd5.a
EXT_LIBARYS +=
SUB_LIBARYS =

## CAREFULLY MODIFY FOLLOWING
ALL_INC +=
ALL_SRC = $(foreach dir, $(SUB_MODULES), $(wildcard $(dir)/*.cpp))
ALL_OBJ = $(ALL_SRC:.cpp=.o)
ALL_DEP = $(ALL_SRC:.cpp=.d)

DEL_OBJ = $(foreach dir, $(SUB_MODULES), $(wildcard $(dir)/*.o))
DEL_DEP = $(foreach dir, $(SUB_MODULES), $(wildcard $(dir)/*.d))

.PHONY: all clean

all: $(ALL_DEP) $(APP_LIB)

$(APP_LIB): $(ALL_OBJ)
	@$(ARCHIVE_LIB_WRAPPER)

test:

clean:
	@rm -rfv *.o $(APP_LIB) $(DEL_OBJ) $(DEL_DEP)

love: clean all

ifneq "$(MAKECMDGOALS)" "clean"
  -include $(ALL_DEP)
endif

