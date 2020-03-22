/*#include "Poco/URIStreamOpener.h"
#include "Poco/StreamCopier.h"
#include "Poco/Path.h"
#include "Poco/URI.h"
#include "Poco/SharedPtr.h"
#include "Poco/Exception.h"
#include "Poco/Net/HTTPStreamFactory.h"
#include "Poco/Net/HTTPSStreamFactory.h"
#include "Poco/Net/FTPStreamFactory.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/Net/KeyConsoleHandler.h"
#include "Poco/Net/ConsoleCertificateHandler.h"
#include <memory>
#include <iostream>
#include <time.h>
#include "Poco/FileStreamFactory.h"
#include <time.h>

using Poco::URIStreamOpener;
using Poco::StreamCopier;
using Poco::Path;
using Poco::URI;
using Poco::SharedPtr;
using Poco::Exception;
using Poco::Net::HTTPStreamFactory;
using Poco::Net::HTTPSStreamFactory;
using Poco::Net::FTPStreamFactory;
using Poco::Net::SSLManager;
using Poco::Net::Context;
using Poco::Net::KeyConsoleHandler;
using Poco::Net::PrivateKeyPassphraseHandler;
using Poco::Net::InvalidCertificateHandler;
using Poco::Net::ConsoleCertificateHandler;


class SSLInitializer
{
public:
	SSLInitializer()
	{
		Poco::Net::initializeSSL();
	}

	~SSLInitializer()
	{
		Poco::Net::uninitializeSSL();
	}
};


int main(int argc, char** argv)
{
	//*
	Poco::Net::initializeNetwork();

	SSLInitializer sslInitializer;
	HTTPStreamFactory::registerFactory();
	HTTPSStreamFactory::registerFactory();
	FTPStreamFactory::registerFactory();


	// Note: we must create the passphrase handler prior Context
	SharedPtr<InvalidCertificateHandler> ptrCert = new ConsoleCertificateHandler(false); // ask the user via console
	Context::Ptr ptrContext = new Context(Context::TLSV1_1_CLIENT_USE, "", "", "rootcert.pem", Context::VERIFY_RELAXED, 9, false, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
	//ptrContext->requireMinimumProtocol(Poco::Net::Context::PROTO_TLSV1_1);
	SSLManager::instance().initializeClient(0, ptrCert, ptrContext);

	try
	{
		URI uri("https://nero-game.com/resource/engine/NeroGameEngine1.0.1.rar");

		//Poco::URI

		std::auto_ptr<std::istream> pStr(URIStreamOpener::defaultOpener().open(uri));


		std::ofstream file("NeroGameEngine1.0.1.rar", std::ofstream::binary);

		StreamCopier::copyStream(*pStr.get(), file);

		file.close();
	}
	catch (Exception& exc)
	{
		std::cerr << exc.displayText() << std::endl;
		return 1;
	}


	int test = 1 << 0;

	std::cout <<test << std::endl;


	return 0;
}*/



#include "Poco/Logger.h"
#include "Poco/PatternFormatter.h"
#include "Poco/FormattingChannel.h"
#include "Poco/ConsoleChannel.h"
#include "Poco/FileChannel.h"
#include "Poco/Message.h"
#include "Poco/StreamChannel.h"

#include "sstream"
#include "iostream"

#include "Nero/core/cpp/utility/LogUtil.h"


using Poco::Logger;
using Poco::PatternFormatter;
using Poco::FormattingChannel;
using Poco::ConsoleChannel;
using Poco::ColorConsoleChannel;
using Poco::StreamChannel;
using Poco::FileChannel;
using Poco::Message;


int main(int argc, char** argv)
{
	// set up two channel chains - one to the
	// console and the other one to a log file.
	std::stringstream loggerStream;

	FormattingChannel* pFCConsole = new FormattingChannel(new PatternFormatter("%s: %p: %P:  %N: %u:  %t"));
	pFCConsole->setChannel(new StreamChannel(loggerStream));
	pFCConsole->open();

	FormattingChannel* pFCFile = new FormattingChannel(new PatternFormatter("%Y-%m-%d %H:%M:%S.%c %N[%P]:%s:%q:%t"));
	pFCFile->setChannel(new FileChannel("sample.log"));
	pFCFile->open();

	// create two Logger objects - one for
	// each channel chain.
	Logger& consoleLogger = Logger::create("ConsoleLogger", pFCConsole, Message::PRIO_INFORMATION);
	Logger& fileLogger    = Logger::create("FileLogger", pFCFile, Message::PRIO_WARNING);

	// log some messages
	consoleLogger.error("An error message");
	fileLogger.error("An error message");

	consoleLogger.warning("A warning message");
	fileLogger.error("A warning message");

	consoleLogger.information("An information message");
	fileLogger.information("An information message");

	poco_information(consoleLogger, "Another informational message");
	poco_warning_f2(consoleLogger, "A warning message with arguments: %d, %d", 1, 2);

	Logger::get("ConsoleLogger").error("Another error message");

	std::string result = loggerStream.str();

	std::cout << result << std::endl;

	nero_log("bonjour", nero::LOG_LEVEL::DEBUG);


	return 0;
}
