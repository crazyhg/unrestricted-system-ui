TEMPLATE = subdirs
SUBDIRS = $$system(ls -1d ut_*/*.pro ft_*/*.pro 2>/dev/null | grep -v ut_template | sed 's!/.*!!')

QMAKE_STRIP = echo

check.target = check
check.CONFIG = recursive
QMAKE_EXTRA_TARGETS += check

check-xml.target = check-xml
check-xml.CONFIG = recursive
QMAKE_EXTRA_TARGETS += check-xml

QMAKE_CLEAN += **/*.log.xml ./coverage.log.xml

support_files.commands += $$PWD/gen-tests-xml.sh > $$OUT_PWD/tests.xml
support_files.target = support_files
support_files.files += $$OUT_PWD/tests.xml
support_files.path = /usr/share/system-ui-tests
support_files.CONFIG += no_check_exist

INSTALLS += support_files
