# name of your binary
bin_PROGRAMS = gst-app encryptor re-encryptor decryptor

# list of source files
# the prefix is the name of the binary
gst_app_SOURCES = main.c play.c
encryptor_SOURCES = encryptor.c
re_encryptor_SOURCES = re-encryptor.c
decryptor_SOURCES = decryptor.c

# list of headers we're not going to install
noinst_HEADERS = gst-app.h play.h

# our CFLAGS and LDFLAGS used for compiling and linking
# make sure you prefix these with the name of your binary
gst_app_CFLAGS = $(GST_CFLAGS)
gst_app_LDFLAGS = $(GST_LIBS)
encryptor_CFLAGS = $(GST_CFLAGS)
encryptor_LDFLAGS = $(GST_LIBS)
re_encryptor_CFLAGS = $(GST_CFLAGS)
re_encryptor_LDFLAGS = $(GST_LIBS)
decryptor_CFLAGS = $(GST_CFLAGS)
decryptor_LDFLAGS = $(GST_LIBS)
