
INCLUDES = -I/usr/include/mysql -I/usr/local/include/mysql++
CFLAGS = -Wall -D_REENTRANT -D_GNU_SOURCE -D_THREAD_SAFE $(INCLUDES)
LDFLAGS = $(LDFLAG) $(PROFILE)
LIBS = -Wl,--start-group -lastxx_manager -lastxx_agi -lpthread \
	-lboost_system -lmysqlpp -Wl,--end-group

.cc.o:
	$(CXX) -MD $(CFLAGS) -c $< -o $@

all:		setspygroupd spygroup.agi
dist:		clean dep all

setspygroupd:	setspygroupd.o
		$(CXX) $(DEBUG) $(LDFLAGS) setspygroupd.o -o setspygroupd $(LIBS)

spygroup.agi:	spygroup.o
		$(CXX) $(DEBUG) $(LDFLAGS) spygroup.o -o spygroup.agi $(LIBS)

clean:
		@rm -f *.o *.a *.so* core *.d *.out *~ setspygroupd spygroup.agi DEAD*

install:
		@cp -f setspygroupd /usr/local/bin
		@cp -f spygroup.agi /var/lib/asterisk/agi-bin
		@mysql < db.sql

-include $(wildcard *.d)

###
