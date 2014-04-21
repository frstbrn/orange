## ATTENTION, BEFORE USING THIS INCLUDE, MAKE SURE THAT YOU HAD DECLARED "MAT_HOMEDIR"

## BASIC ENVIRONMENT VARIABLES
MOD_NAME_UC=`echo $(MOD_NAME) | tr '[a-z]' '[A-Z]'`

# EXTLIB DECLARATION
EXTLIB_DIR=$(MAT_HOMEDIR)/src/extlib

## COMPILATION CONFIGURATIONS
TDR=$(TSF4G_BINDIR)/tdr
CPP=g++
CC=cc
AR=ar

## CFLAGS
CFLAGS=-g -Werror -Wall $(STATIC) $(NDEBUG) -fPIC
CPPFLAGS=$(CFLAGS) -Wno-invalid-offsetof
ALL_INC += -I ~/orange/
LDFLAGS+=

################################################################################

## COMPILING WRAPPERS
COMPILE_CPP = $(CPP) $(ALL_INC) -c $(CPPFLAGS)  $< -o $@
COMPILE_CC = $(CC) $(ALL_INC) -c $(CFLAGS)  $< -o $@
COMPILE_DEP_CPP = $(CPP) $(ALL_INC) -MM $(CPPFLAGS) $< > $@ && sed -i 's,$(notdir $(basename $@))\.o[ :]*,$(basename $@).o $(basename $@).d : ,g' $@
COMPILE_DEP_CC = $(CC) $(ALL_INC) -MM $(CFLAGS) $< > $@ && sed -i 's,$(notdir $(basename $@))\.o[ :]*,$(basename $@).o $(basename $@).d : ,g' $@
CREATE_BINDIR = if [ ! -d $(APP_BINDIR) ]; then echo "Creating directory [$(APP_BINDIR)]"; mkdir -p $(APP_BINDIR); fi

COMPILE_CPP_WRAPPER = @echo "Compiling [$<]" && $(COMPILE_CPP) && exit 0; \
					  echo "The following command failed:" 1>&2;\
					  echo "\t$(COMPILE_CPP)" 1>&2;\
					  $(COMPILE_CPP) >/dev/null 2>&1

COMPILE_CC_WRAPPER = @echo "Compiling [$<]" && $(COMPILE_CC) && exit 0; \
					  echo "The following command failed:" 1>&2;\
					  echo "\t$(COMPILE_CC)" 1>&2;\
					  $(COMPILE_CC) >/dev/null 2>&1

COMPILE_DEP_WRAPPER_CPP = @echo "Generating dependencies of [$<]" && $(COMPILE_DEP_CPP) && exit 0; \
					  echo "The following command failed:" 1>&2;\
					  echo "\t$(COMPILE_DEP_CPP)" 1>&2;\
					  $(COMPILE_DEP_CPP) >/dev/null 2>&1

COMPILE_DEP_WRAPPER_CC = @echo "Generating dependencies of [$<]" && $(COMPILE_DEP_CC) && exit 0; \
					  echo "The following command failed:" 1>&2;\
					  echo "\t$(COMPILE_DEP_CC)" 1>&2;\
					  $(COMPILE_DEP_CC) >/dev/null 2>&1

################################################################################

ARCHIVE_LIB = $(AR) r $@ $^

ARCHIVE_LIB_WRAPPER = @echo "Archiving [$@]" && $(ARCHIVE_LIB) 2>/dev/null && exit 0; \
					  echo "The following command failed:" 1>&2;\
					  echo "\t$(ARCHIVE_LIB)" 1>&2;\
					  $(ARCHIVE_LIB) >/dev/null 2>&1

BUILD_TEST = $(CPP) -o $@ $^ $(LDFLAGS) #linked with cpp

BUILD_TEST_WRAPPER = @echo "Building [$@]" && $(BUILD_TEST) && exit 0; \
					  echo "Failed:" 1>&2;\
					  echo "\t$(BUILD_TEST)" 1>&2;\
					  $(BUILD_TEST) >/dev/null 2>&1

################################################################################

## DEPENDENCIES
%.o: %.c
	@$(COMPILE_CC_WRAPPER)

%.o: %.cpp
	@$(COMPILE_CPP_WRAPPER) 
%.d: %.c
	@set -e; rm -f $@;
	@$(COMPILE_DEP_WRAPPER_CC)

%.d: %.cpp
	@set -e; rm -f $@;
	@$(COMPILE_DEP_WRAPPER_CPP)

