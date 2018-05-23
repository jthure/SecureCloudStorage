# Note: plugindir is set in configure

##############################################################################
# TODO: change libgstplugin.la to something else, e.g. libmysomething.la     #
##############################################################################
plugin_LTLIBRARIES = libgstpre.la

##############################################################################
# TODO: for the next set of variables, name the prefix if you named the .la, #
#  e.g. libmysomething.la => libmysomething_la_SOURCES                       #
#                            libmysomething_la_CFLAGS                        #
#                            libmysomething_la_LIBADD                        #
#                            libmysomething_la_LDFLAGS                       #
##############################################################################

## Plugin 1
# sources used to compile this plug-in
libgstpre_la_SOURCES = gstpre.c gstpre.h charm_embed_api.c charm_embed_api.h

# compiler and linker flags used to compile this plugin, set in configure.ac
libgstpre_la_CFLAGS = $(GST_CFLAGS) $(PY_CFLAGS)
libgstpre_la_LIBADD = $(GST_LIBS) $(PY_LIBS)
libgstpre_la_LDFLAGS = $(GST_PLUGIN_LDFLAGS)
libgstpre_la_LIBTOOLFLAGS = --tag=disable-static


# headers we need but don't want installed
# include_HEADERS = gstpre.h charm_embed_api.h
noinst_HEADERS = gstpre.h charm_embed_api.h
