You need libmodbus installed to use the unit-test-client program. It is adapted from the libmodbus unit tests. As is hard to test code on DSPs, I am using this client to certify that the requests are working properly.

Sometimes, depending on the serial adapter that you're using, the program will throw a timeout.

You can add tests to unit-test-client.c code and recompile the program. If you find anything that is not covered by the test, submit a pull request with it.
