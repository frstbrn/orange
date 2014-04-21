
MODULES = common
MODULES += infrastructure

ALL_TARGET=$(MODULES)

#what's this again?
ALL_TARGET_REV=`echo $(ALL_TARGET) | sed 's, ,\n,g' | tac`

################################################################################

all:
	for module in $(ALL_TARGET); do cd $$module|| exit $?; make $@|| exit $?; cd -;done

test:
	for module in $(ALL_TARGET); do cd $$module|| exit $?; make $@|| exit $?; cd -;done

clean:
	for module in $(ALL_TARGET_REV); do cd $$module|| exit $?; make $@|| exit $?; cd -;done

love: clean all
