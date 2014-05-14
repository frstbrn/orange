
MAT_HOMEDIR=../

## WRITE YOUR MODULE NAME
MOD_NAME=communication

# INCLUDE
include ../../make.inc

## ADD DEPENDENT MODULES HERE
SUB_MODULES = bus

## ADD DEPENDENT LIBARYS HERE
#EXT_LIBARYS += $(MAT_HOMEDIR)/src/extlib/libemd5.a
EXT_LIBARYS +=
SUB_LIBARYS =

## CAREFULLY MODIFY FOLLOWING
ALL_INC +=
ALL_LIB = libcommu.a
#ALL_SRC = $(foreach dir, $(SUB_MODULES), $(wildcard $(dir)/*.cpp))
ALL_SRC = bus/mem_q.cpp bus/shm_pool.cpp bus/shm_q.cpp
ALL_OBJ = $(ALL_SRC:.cpp=.o)
ALL_DEP = $(ALL_SRC:.cpp=.d)

TEST_BIN = bus_ut
TEST_OBJ = bus/bus_ut.o

DEL_OBJ = $(foreach dir, $(SUB_MODULES), $(wildcard $(dir)/*.o))
DEL_DEP = $(foreach dir, $(SUB_MODULES), $(wildcard $(dir)/*.d))

.PHONY: all clean

all: $(ALL_DEP) $(ALL_LIB)

$(ALL_LIB): $(ALL_OBJ)
	@$(ARCHIVE_LIB_WRAPPER)

test: $(TEST_BIN)

clean:
	@rm -rfv *.o $(ALL_LIB) $(DEL_OBJ) $(DEL_DEP) $(TEST_BIN) $(TEST_OBJ)

love: clean all

bus_ut: $(TEST_OBJ) $(ALL_LIB)
	$(CPP) -o $@ $^ $(LDFLAGS)

ifneq "$(MAKECMDGOALS)" "clean"
  -include $(ALL_DEP)
endif

