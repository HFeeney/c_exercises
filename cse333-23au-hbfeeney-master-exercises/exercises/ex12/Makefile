# Copyright ©2023 Chris Thachuk.  All rights reserved.  Permission is
# hereby granted to students registered for University of Washington
# CSE 333 for use solely during Fall Quarter 2023 for purposes of
# the course.  No other use, copying, distribution, or modification
# is permitted without prior written consent. Copyrights for
# third-party components of this work must be honored.  Instructors
# interested in reusing these course materials should contact the
# author.

CXX = g++
OBJS = SimpleQueue.o ex12.o
EXE  = ex12
HEADERS = SimpleQueue.h
CPPFLAGS = -Wall -g -std=c++17 -pthread

# default target
$(EXE): $(OBJS)
	$(CXX) $(CPPFLAGS) -o $(EXE) $(OBJS)
# source files
%.o: %.cc $(HEADERS)
	$(CXX) $(CPPFLAGS) -c $<
# phony target - remove generated files and backups
clean:
	rm -rf $(EXE) *.o *~ *.dSYM
