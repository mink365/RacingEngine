TEMPLATE = subdirs

#CONFIG += ordered

SUBDIRS += \
	Examples \
	Source \

OTHER_FILES += \
	common.pri

# dependencies
Examples.depends = Source
