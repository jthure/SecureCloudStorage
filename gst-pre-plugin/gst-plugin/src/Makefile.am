# Note: plugindir is set in configure

##############################################################################
# TODO: change libgstplugin.la to something else, e.g. libmysomething.la     #
##############################################################################
plugin_LTLIBRARIES = libgstmyfilter.la

##############################################################################
# TODO: for the next set of variables, name the prefix if you named the .la, #
#  e.g. libmysomething.la => libmysomething_la_SOURCES                       #
#                            libmysomething_la_CFLAGS                        #
#                            libmysomething_la_LIBADD                        #
#                            libmysomething_la_LDFLAGS                       #
##############################################################################

## Plugin 1
# sources used to compile this plug-in
libgstmyfilter_la_SOURCES = gstmyfilter.c gstmyfilter.h charm_embed_api.c charm_embed_api.h

# compiler and linker flags used to compile this plugin, set in configure.ac
libgstmyfilter_la_CFLAGS = $(GST_CFLAGS) $(PY_CFLAGS)
libgstmyfilter_la_LIBADD = $(GST_LIBS) $(PY_LIBS)
libgstmyfilter_la_LDFLAGS = $(GST_PLUGIN_LDFLAGS)
libgstmyfilter_la_LIBTOOLFLAGS = --tag=disable-static


# headers we need but don't want installed
# include_HEADERS = gstmyfilter.h charm_embed_api.h
noinst_HEADERS = gstmyfilter.h charm_embed_api.h
