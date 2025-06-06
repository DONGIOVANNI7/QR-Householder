CXX = g++
CXXFLAGS = -std=c++11 -O3 -Wall -march=native
TARGET = ask3_QR_HOUS
SRCDIR = src
INCDIR = include
SRCS = $(SRCDIR)/matrix.cpp $(SRCDIR)/qr_householder.cpp $(SRCDIR)/error_metrics.cpp \
       $(SRCDIR)/benchmark.cpp $(SRCDIR)/main.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -I$(INCDIR) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)