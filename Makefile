include $(TOPDIR)/rules.mk

PKG_NAME:=conifd
PKG_VERSION:=0.0.1
PKG_RELEASE=2
PKG_MAINTAINER:=Levy Chang <seed678131@gmail.com>

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
	SECTION:=net
	CATEGORY:=Network
	DEPENDS:=+libuci
	MAINTAINER:=Levy Chang <seed678131@gmail.com>
	TITLE:=Check Internet and interface connectivity
endef

define Package/$(PKG_NAME)/description
Checks by using "ping" (ICMP echo)
wether a configured host (normally on the internet) can be reached via a
specific interface. Then makes this information available via triggers
"online" and "offline" scripts.
endef

#define Package/$(PKG_NAME)/conffiles
#/etc/config/conifd
#endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/conifd $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/conifd.init $(1)/etc/init.d/conifd
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_DATA) ./files/conifd.conf $(1)/etc/config/conifd
	$(INSTALL_DIR) $(1)/etc/conifd
	$(INSTALL_BIN) ./files/CONIFD_CHECK.sh $(1)/etc/conifd/
	$(INSTALL_DIR) $(1)/etc/conifd/online.d/
	$(INSTALL_DIR) $(1)/etc/conifd/offline.d/
	$(INSTALL_BIN) ./files/online.d/* $(1)/etc/conifd/online.d/
	$(INSTALL_BIN) ./files/offline.d/* $(1)/etc/conifd/offline.d/
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
