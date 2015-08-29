# CppWeb
HTTP proof-of-concept server written in C++
(under development)

This is a HTTP web server written in, just to prove that websites can be written in C++ too, without sacrificing productivity, or, performance :)

Goals:
- Should be used as a library. One only has to provide the routes/methods, and a callback function
- High performance on low-spec servers, should be able to handle a small website running on a Raspberry PI with no problem
- Minimal use of threading, at least for now
- To be used as a backend for JS frameworks like Angular

Plans for the future:
- Implement support for using a MVC type architecture



2015-08-16 siege log, running on an i5 processor:

Transactions: 7539 hits  
Availability:	100.00 %  
Elapsed time:	63.54 secs  
Data transferred:	3582.85 MB  
Response time: 4.72 secs  
Transaction rate:	118.65 trans/sec  
Throughput: 56.39 MB/sec  
Concurrency: 559.77  
Successful transactions: 7539  
Failed transactions: 0  
Longest transaction: 34.69  
Shortest transaction: 0.01  
