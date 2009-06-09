# ***** BEGIN LICENSE BLOCK *****
# Version: MPL 1.1/GPL 2.0/LGPL 2.1
#
# The contents of this file are subject to the Mozilla Public License Version
# 1.1 (the "License"); you may not use this file except in compliance with
# the License. You may obtain a copy of the License at
# http://www.mozilla.org/MPL/
#
# Software distributed under the License is distributed on an "AS IS" basis,
# WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
# for the specific language governing rights and limitations under the
# License.
#
# The Original Code is [Open Source Virtual Machine.].
#
# The Initial Developer of the Original Code is
# Adobe System Incorporated.
# Portions created by the Initial Developer are Copyright (C) 2007-2008
# the Initial Developer. All Rights Reserved.
#
# Contributor(s):
#   Adobe AS3 Team
#
# Alternatively, the contents of this file may be used under the terms of
# either the GNU General Public License Version 2 or later (the "GPL"), or
# the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
# in which case the provisions of the GPL or the LGPL are applicable instead
# of those above. If you wish to allow use of your version of this file only
# under the terms of either the GPL or the LGPL, and not to allow others to
# use your version of this file under the terms of the MPL, indicate your
# decision by deleting the provisions above and replace them with the notice
# and other provisions required by the GPL or the LGPL. If you do not delete
# the provisions above, a recipient may use your version of this file under
# the terms of any one of the MPL, the GPL or the LGPL.
#
# ***** END LICENSE BLOCK *****

PROGRAMS += shell

shell_BASENAME = avmshell
shell_INCLUDES = -I$(srcdir) -I$(topsrcdir)/extensions
shell_DEFINES = -DAVMPLUS_SHELL
shell_STATIC_LIBRARIES = MMgc avmplus fcgi fcgi++ mysqlclient cgicc
shell_DIR := $(curdir)/
shell_EXTRA_CPPFLAGS := $(AVMSHELL_CPPFLAGS)
shell_EXTRA_LDFLAGS := $(AVMSHELL_LDFLAGS)

ifdef ENABLE_SHELL
shell_BUILD_ALL = 1
endif

shell_CXXSRCS := $(shell_CXXSRCS) \
  $(curdir)/main.cpp \
  $(curdir)/avmshell.cpp \
  $(curdir)/MySQLClass.cpp \
  $(curdir)/HTTPClass.cpp \
  $(curdir)/HTTPFormClass.cpp \
  $(curdir)/ByteArrayGlue.cpp \
  $(curdir)/ConsoleOutputStream.cpp \
  $(curdir)/DataIO.cpp \
  $(curdir)/DebugCLI.cpp \
  $(curdir)/DomainClass.cpp \
  $(curdir)/FileClass.cpp \
  $(curdir)/FileInputStream.cpp \
  $(curdir)/StringBuilderClass.cpp \
  $(curdir)/SystemClass.cpp \
  $(curdir)/../extensions/DictionaryGlue.cpp \
  $(curdir)/../extensions/JavaGlue.cpp \
  $(curdir)/../extensions/SamplerScript.cpp \
  $(curdir)/../extensions/Selftest.cpp \
  $(curdir)/../extensions/SelftestInit.cpp \
  $(curdir)/../extensions/ST_avmplus_basics.cpp \
  $(curdir)/../extensions/ST_avmplus_peephole.cpp \
  $(NULL)