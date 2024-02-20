# Matrix Multiply Challenge Makefile
CXX		 = g++
CXXFLAGS = -Wall -std=c++17
TARGET	 = multiplicacion_matrices
SRC		 = multiplicacion_matrices.cpp
LDLIBS	 = 

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) $(LDLIBS) -o $(TARGET)

.PHONY: clean
clean:
	rm -f $(TARGET)