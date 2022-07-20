
test:libSoTest.so
	$(CXX) -lSoTest -L./ test.cpp -o test
	cp libSoTest.so /usr/local/lib/
libSoTest.so:
	$(CXX) -fPIC -shared SoTest.cpp -o libSoTest.so

clean:
	$(RM) *.so test