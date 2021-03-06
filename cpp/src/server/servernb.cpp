#include <iostream>
#include "Calculator.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PlatformThreadFactory.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using namespace ::apache::thrift::concurrency;

using boost::shared_ptr;

class CalculatorHandler : virtual public CalculatorIf {
public:
	CalculatorHandler() {
		// Your initialization goes here
	}

	int32_t add(const int32_t a, const int32_t b) {
		// Your implementation goes here
		printf("add\n");
		return a + b;
	}

};

int main(int argc, char **argv) {
	int port = 9090;
	shared_ptr<CalculatorHandler> handler(new CalculatorHandler());
	shared_ptr<TProcessor> processor(new CalculatorProcessor(handler));
	shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
	shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
	shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

	const int workerCount = 4;
	boost::shared_ptr<ThreadManager> threadManager =
		ThreadManager::newSimpleThreadManager(workerCount);
	boost::shared_ptr<PlatformThreadFactory> threadFactory =
		boost::shared_ptr<PlatformThreadFactory>(new PlatformThreadFactory());
	threadManager->threadFactory(threadFactory);
	threadManager->start();

	TThreadedServer server(processor,
		serverTransport,
		transportFactory,
		protocolFactory);

	std::cout << "Starting the server..." << std::endl;
	server.serve();
	std::cout << "Done." << std::endl;
	return 0;
}
