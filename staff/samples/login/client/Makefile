include ../../../Makefile.env

TARGET = login_client
TARGETNAME = $(TARGET)

HEADERS := $(filter-out %ServiceFactory.h %Proxy.h,$(wildcard $(SRCDIR)*.h))


CXXFLAGS += -Wno-deprecated -fexceptions -I$(DEPLOYDIR)include 
LDFLAGS += -fexceptions -L$(DEPLOYDIR)lib 
LDFLAGS += -lstaffcommon -lstaffclient -lrise

VPATH = $(subst $(empty) $(empty),:,$(SRCDIR))

CODEGENH := $(patsubst %.h,%Proxy.h,$(HEADERS))
CODEGENCPP := $(patsubst %.h,%Proxy.cpp,$(HEADERS))
CODEGEN := $(CODEGENH) $(CODEGENCPP)

SOURCES = $(CODEGENCPP) $(filter-out %Proxy.cpp,$(wildcard $(SRCDIR)*.cpp))
OBJECTS = $(patsubst %.cpp,$(OBJDIR)%.o,$(notdir $(SOURCES)))

.SECONDARY: $(CODEGEN)

# == make ===========================================
make: check $(CODEGEN) "$(OBJDIR)" "$(OUTDIR)" $(OUTDIR)$(TARGETNAME) deploy

# link
$(OUTDIR)$(TARGETNAME): $(CODEGEN) $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $(OUTDIR)$(TARGETNAME)

# compile
$(OBJDIR)%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(SRCDIR)%Proxy.h $(SRCDIR)%Proxy.cpp:: $(SRCDIR)%.h
	[ "$@" -nt "$^" ] || (cd src && $(STAFF_CODEGEN) $(patsubst $(SRCDIR)%,%,$(HEADERS)) -tclient -n$(TARGET))

# == deploy  ========================================
deploy: "$(DEPLOYDIR)$(SAMPLESDIR)$(BINDIR)" $(OUTDIR)$(TARGETNAME)
	cp -f $(OUTDIR)$(TARGETNAME) "$(DEPLOYDIR)$(SAMPLESDIR)$(BINDIR)"

# == distrib =========================================
distrib:;

# == install ========================================
install: check "$(INSTALLDIR)$(SAMPLESDIR)$(BINDIR)"
	cp -f $(OUTDIR)$(TARGETNAME) $(INSTALLDIR)$(SAMPLESDIR)$(BINDIR)$(TARGETNAME)

# == uninstall ======================================
uninstall: check
	rm -f $(INSTALLDIR)$(BINDIR)$(TARGETNAME)

# == clean ==========================================
clean:
	rm -Rf $(OBJDIR) $(OUTDIR)
	rm -f $(CODEGEN)

# == mkdir ==========================================
"%/":
	@[ -z "$@" -o -d "$@" ] || mkdir -p $@ && chmod g+w $@