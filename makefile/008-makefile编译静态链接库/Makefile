
TARGET=main
LDFLAGS=-L./001 -L./002
LIBS=-lSoTest -laTest

$(TARGET):
	$(CXX) $(LDFLAGS) $(LIBS) main.cpp -o $(TARGET)

clean:
	$(RM) $(TARGET)