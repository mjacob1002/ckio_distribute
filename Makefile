CHARM_HOME = /u/jacob1/charm/
-include $(CHARM_HOME)/test/common.mk
CHARMC=$(CHARM_HOME)/bin/charmc $(OPTS)

all: data

data: data.C data.ci
	$(CHARMC) data.ci 	
	$(CHARMC) data.C -o data

clean: 
	rm -rf *.decl.* *.def.* data
