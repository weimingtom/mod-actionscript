

PROGRAMS += fcgishell

fcgishell_BASENAME = fcgishell
fcgishell_INCLUDES = -I$(srcdir) -I$(topsrcdir)/extensions
fcgishell_DEFINES = -DAVMPLUS_SHELL
fcgishell_STATIC_LIBRARIES = MMgc avmplus fcgi++ mysqlwrapped mysqlclient
fcgishell_DIR := $(curdir)/
fcgishell_EXTRA_CPPFLAGS := $(AVMSHELL_CPPFLAGS)
fcgishell_EXTRA_LDFLAGS := $(AVMSHELL_LDFLAGS)

ifdef ENABLE_SHELL
fcgishell_BUILD_ALL = 1
endif

fcgishell_CXXSRCS := $(shell_CXXSRCS) \
  $(curdir)/glue.cpp \
  $(curdir)/MySQLClass.cpp \
  $(curdir)/fcgishell.cpp \
  $(curdir)/../shell/ByteArrayGlue.cpp \
  $(curdir)/../shell/ConsoleOutputStream.cpp \
  $(curdir)/../shell/DataIO.cpp \
  $(curdir)/../shell/DebugCLI.cpp \
  $(curdir)/../shell/DomainClass.cpp \
  $(curdir)/../shell/FileClass.cpp \
  $(curdir)/../shell/FileInputStream.cpp \
  $(curdir)/../shell/StringBuilderClass.cpp \
  $(curdir)/SystemClass.cpp \
  $(curdir)/../extensions/DictionaryGlue.cpp \
  $(curdir)/../extensions/SamplerScript.cpp \
  $(curdir)/../extensions/Selftest.cpp \
  $(curdir)/../extensions/SelftestInit.cpp \
  $(curdir)/../extensions/ST_avmplus_basics.cpp \
  $(curdir)/../extensions/ST_avmplus_peephole.cpp \
  $(NULL)

  
#  $(curdir)/../extensions/JavaGlue.cpp \
 