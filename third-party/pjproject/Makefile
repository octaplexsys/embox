
PKG_NAME := pjproject
PKG_VER  := 2.10

PKG_SOURCES := https://github.com/pjsip/pjproject/archive/$(PKG_VER).tar.gz
PKG_MD5     := 13e5c418008ae46c4ce0c1e27cdfe9b5

include $(EXTBLD_LIB)

PKG_PATCHES := pjproject-2.10.patch \
	sha256_error_fix-2.10.patch \
	addr_resolv_sock-2.10.patch

ifeq ($(PJSIP_ENABLE_CXX),false)
PKG_PATCHES	+= pjsua2_disable-2.10.patch
endif

#PKG_PATCHES := pjproject.patch \
#	simpleua_default_loglevel.patch \
#	mutex_loglevel_increase.patch \
#	kmalloc.patch \
#	srtp.patch \
#	sha256_error_fix.patch
#
#ifeq ($(PJSIP_ENABLE_CXX),false)
#PKG_PATCHES	+= pjsua2_disable.patch
#endif

DISABLE_FEATURES := \
	l16-codec   \
	ilbc-codec  \
	speex-codec \
	speex-aec   \
	gsm-codec   \
	g722-codec  \
	g7221-codec \
	libyuv \
	libwebrtc \
	#g711-codec

PJPROJECT_ROOT := $(ROOT_DIR)/third-party/pjproject
PJSIP_CPPFLAGS := -I$(PJPROJECT_ROOT)/include $(EMBOX_DEPS_CPPFLAGS) -I$(SRC_DIR)/compat/cxx/include 
PJSIP_CPPFLAGS += -I$(SRC_DIR)/compat/libc/include -I$(SRC_DIR)/compat/posix/include
BUILD_ROOT  := $(BUILD_DIR)/$(PKG_NAME)-$(PKG_VER)
PJSIP_INSTALL_DIR := $(EXTERNAL_BUILD_DIR)/third_party/pjproject/core/install

$(CONFIGURE) :
	export EMBOX_GCC_LINK=full; \
	cd $(BUILD_ROOT) && ( \
		./configure \
			CC=$(EMBOX_GCC) \
			CXX=$(EMBOX_GXX) \
			CPPFLAGS="$(PJSIP_CPPFLAGS)" \
			--host=$(AUTOCONF_TARGET_TRIPLET) \
			--target=$(AUTOCONF_TARGET_TRIPLET) \
			--prefix=$(PJSIP_INSTALL_DIR) \
			$(DISABLE_FEATURES:%=--disable-%) \
			--with-external-pa; \
		echo export CFLAGS+="$(PJSIP_CPPFLAGS) $(PJSIP_BUILD_FIXUP_CFLAGS)" > user.mak; \
		echo export CXXFLAGS+="$(EMBOX_CXXFLAGS) $(PJSIP_CPPFLAGS)" >> user.mak; \
	)
	cp ./config_site.h $(BUILD_ROOT)/pjlib/include/pj/config_site.h
	touch $@

$(BUILD) :
	cd $(BUILD_ROOT) && ( \
		$(MAKE) -j1 dep; \
		$(MAKE) -j1 MAKEFLAGS='$(EMBOX_IMPORTED_MAKEFLAGS)'; \
	)
	touch $@

$(INSTALL) :
	cd $(BUILD_ROOT) && $(MAKE) install
	# Get rid of AUTOCONF_TARGET_TRIPLET in file names to use them in Mybuild
	for f in $(PJSIP_INSTALL_DIR)/lib/*-$(AUTOCONF_TARGET_TRIPLET).a; do \
		fn=$$(basename $$f); \
		cp $$f $(PJSIP_INSTALL_DIR)/lib/$${fn%-$(AUTOCONF_TARGET_TRIPLET).a}.a; \
	done
	# Copy binaries and
	# get rid of AUTOCONF_TARGET_TRIPLET in file names to use them in Mybuild
	for f in $(BUILD_ROOT)/pjsip-apps/bin/samples/$(AUTOCONF_TARGET_TRIPLET)/*; do \
		cp $$f $(PJSIP_INSTALL_DIR)/$$(basename $$f).o; \
	done
	for f in $(BUILD_ROOT)/pjsip-apps/bin/*-$(AUTOCONF_TARGET_TRIPLET); do \
		fn=$$(basename $$f); \
		cp $$f $(PJSIP_INSTALL_DIR)/$${fn%-$(AUTOCONF_TARGET_TRIPLET)}.o; \
	done
	touch $@
