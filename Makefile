LOCAL_MAKE_INCLUDE := include
override STATIC_LIB_TEMPLATE := make_templates/static_lib
override LOCAL_STATIC_LIB_TEMPLATE := $(LOCAL_MAKE_INCLUDE)/$(STATIC_LIB_TEMPLATE)

ifneq ($(shell cat $(LOCAL_STATIC_LIB_TEMPLATE) 2> /dev/null),)
include $(LOCAL_STATIC_LIB_TEMPLATE)
else
include $(STATIC_LIB_TEMPLATE)
endif
