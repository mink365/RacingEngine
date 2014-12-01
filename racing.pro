TEMPLATE = subdirs

CONFIG += ordered

examples.file = Examples/Examples.pro
engine.file = Source/Source.pro

SUBDIRS += \
        engine \
        examples \

OTHER_FILES += \
	common.pri

# dependencies
Examples.depends = Source
