#
###############################################################
#                                                             #
# Makefile for the Modular Base module as described in the:   #
# SELinux Notebook - The Foundations                          #
#                                                             #
###############################################################
#

BASE_DIR = /etc/selinux/modular-test
FLASK_FILES = ../common-policy-files/flask_files
COMMON_FILES = ../common-policy-files
TOOLS = ../../notebook-tools

#
all:	build_policy install_files base_module clean
		echo all: Base Module make complete

build_policy:
	$(TOOLS)/build-policy.py -o base.conf -r $(FLASK_FILES)

install_files:
	mkdir -p $(BASE_DIR)/policy
	mkdir -p $(BASE_DIR)/contexts/files
	mkdir -p $(BASE_DIR)/modules/active/modules
	install -m 644 $(COMMON_FILES)/dbus_contexts $(BASE_DIR)/contexts
	install -m 644 $(COMMON_FILES)/default_contexts $(BASE_DIR)/contexts
	install -m 644 $(COMMON_FILES)/default_type $(BASE_DIR)/contexts
	install -m 644 $(COMMON_FILES)/x_contexts $(BASE_DIR)/contexts

base_module:
	checkmodule -o base.mod base.conf
	semodule_package -o base.pp -m base.mod -f base.fc -s $(COMMON_FILES)/seusers -u users_extra
	semodule -s modular-test -b base.pp

clean:
	rm -f *.pp
	rm -f *.mod
