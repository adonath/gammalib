## Process this file with automake to produce Makefile.in

# Set include directories
INCLUDES = -I$(top_srcdir)/include \
           -I$(top_builddir)/src \
           -I$(top_srcdir)/src

# Define sources for this directory
sources = GModelPar.cpp \
          GModels.cpp \
          GModel.cpp \
          GModelRegistry.cpp \
          GModelSky.cpp \
          GModelData.cpp \
          GModelSpatial.cpp \
          GModelSpatialRegistry.cpp \
          GModelSpatialPointSource.cpp \
          GModelSpatialRadial.cpp \
          GModelSpatialRadialDisk.cpp \
          GModelSpatialRadialGauss.cpp \
          GModelSpatialRadialShell.cpp \
          GModelSpatialElliptical.cpp \
	  			GModelSpatialEllipticalDisk.cpp \
          GModelSpatialDiffuse.cpp \
          GModelSpatialDiffuseConst.cpp \
          GModelSpatialDiffuseCube.cpp \
          GModelSpatialDiffuseMap.cpp \
          GModelSpectral.cpp \
          GModelSpectralRegistry.cpp \
          GModelSpectralConst.cpp \
          GModelSpectralExpPlaw.cpp \
          GModelSpectralFunc.cpp \
          GModelSpectralNodes.cpp \
          GModelSpectralPlaw.cpp \
          GModelSpectralPlaw2.cpp \
          GModelSpectralLogParabola.cpp \
          GModelTemporal.cpp \
          GModelTemporalRegistry.cpp \
          GModelTemporalConst.cpp \
          GException_model.cpp

# Build libtool library
noinst_LTLIBRARIES = libmodel.la
libmodel_la_SOURCES = $(sources)
libmodel_la_LDFLAGS = -version-info $(GAMMALIB_LT_VERSION)
