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



/*#include "Poco/Logger.h"
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
}*/

//
// Ping.cpp
//
// This sample demonstrates the Application class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


/*#include "Poco/Util/Application.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/Util/AbstractConfiguration.h"
#include "Poco/Net/ICMPSocket.h"
#include "Poco/Net/ICMPClient.h"
#include "Poco/Net/IPAddress.h"
#include "Poco/Net/ICMPEventArgs.h"
#include "Poco/AutoPtr.h"
#include "Poco/NumberParser.h"
#include "Poco/Delegate.h"
#include <iostream>
#include <sstream>
#include "Poco/Logger.h"
#include "Poco/PatternFormatter.h"
#include "Poco/FormattingChannel.h"
#include "Poco/ConsoleChannel.h"
#include "Poco/FileChannel.h"
#include "Poco/Message.h"


using Poco::Logger;
using Poco::PatternFormatter;
using Poco::FormattingChannel;
using Poco::ConsoleChannel;
using Poco::FileChannel;
using Poco::Message;


using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;
using Poco::Util::AbstractConfiguration;
using Poco::Net::ICMPSocket;
using Poco::Net::ICMPClient;
using Poco::Net::IPAddress;
using Poco::Net::ICMPEventArgs;
using Poco::AutoPtr;
using Poco::NumberParser;
using Poco::Delegate;


class Ping: public Application
	/// This sample demonstrates the Poco::Net::ICMPClient in conjunction with
	/// Poco Foundation C#-like events functionality.
	///
	/// Try Ping --help (on Unix platforms) or Ping /help (elsewhere) for
	/// more information.
{
public:
	Ping():
		_helpRequested(false),
		_icmpClient(IPAddress::IPv4),
		_repetitions(4),
		_target("nero-game.com")
	{
	}

protected:
	void initialize(Application& self)
	{
		loadConfiguration(); // load default configuration files, if present
		Application::initialize(self);

		pFCConsole = new FormattingChannel(new PatternFormatter("%s: %p: %t"));
			pFCConsole->setChannel(new ConsoleChannel);
			pFCConsole->open();

		consoleLogger = &Logger::create("ConsoleLogger", pFCConsole, Message::PRIO_INFORMATION);

		_icmpClient.pingBegin += delegate(this, &Ping::onBegin);
		_icmpClient.pingReply += delegate(this, &Ping::onReply);
		_icmpClient.pingError += delegate(this, &Ping::onError);
		_icmpClient.pingEnd   += delegate(this, &Ping::onEnd);
	}

	void uninitialize()
	{
		_icmpClient.pingBegin -= delegate(this, &Ping::onBegin);
		_icmpClient.pingReply -= delegate(this, &Ping::onReply);
		_icmpClient.pingError -= delegate(this, &Ping::onError);
		_icmpClient.pingEnd   -= delegate(this, &Ping::onEnd);

		Application::uninitialize();
	}

	void defineOptions(OptionSet& options)
	{
		Application::defineOptions(options);

		options.addOption(
			Option("help", "h", "display help information on command line arguments")
				.required(false)
				.repeatable(false));

		options.addOption(
			Option("repetitions", "r", "define the number of repetitions")
				.required(false)
				.repeatable(false)
				.argument("repetitions"));

		options.addOption(
			Option("target", "t", "define the target address")
				.required(false)
				.repeatable(false)
				.argument("target"));
	}

	void handleOption(const std::string& name, const std::string& value)
	{
		Application::handleOption(name, value);

		if (name == "help")
			_helpRequested = true;
		else if (name == "repetitions")
			_repetitions = NumberParser::parse(value);
		else if (name == "target")
			_target = value;
	}

	void displayHelp()
	{
		HelpFormatter helpFormatter(options());
		helpFormatter.setCommand(commandName());
		helpFormatter.setUsage("OPTIONS");
		helpFormatter.setHeader(
			"A sample application that demonstrates the functionality of the "
			"Poco::Net::ICMPClient class in conjunction with Poco::Events package functionality.");
		helpFormatter.format(std::cout);
	}


	int main(const std::vector<std::string>& args)
	{
		if (_helpRequested)
			displayHelp();
		else
			_icmpClient.ping(_target, _repetitions);

		return Application::EXIT_OK;
	}


	void onBegin(const void* pSender, ICMPEventArgs& args)
	{
		std::ostringstream os;
		os << "Pinging " << args.hostName() << " [" << args.hostAddress() << "] with " << args.dataSize() << " bytes of data:"
		   << std::endl << "---------------------------------------------" << std::endl;
		logger().information(os.str());
		consoleLogger->information(os.str());

	}

	void onReply(const void* pSender, ICMPEventArgs& args)
	{
		std::ostringstream os;
		os << "Reply from " << args.hostAddress()
		   << " bytes=" << args.dataSize()
		   << " time=" << args.replyTime() << "ms"
		   << " TTL=" << args.ttl();
		logger().information(os.str());
		consoleLogger->information(os.str());

	}

	void onError(const void* pSender, ICMPEventArgs& args)
	{
		std::ostringstream os;
		os << args.error();
		logger().information(os.str());
		consoleLogger->information(os.str());
		consoleLogger->information(os.str());


	}

	void onEnd(const void* pSender, ICMPEventArgs& args)
	{
		std::ostringstream os;
		os << std::endl << "--- Ping statistics for " << args.hostName() << " ---"
		   << std::endl << "Packets: Sent=" << args.sent() << ", Received=" << args.received()
		   << " Lost=" << args.repetitions() - args.received() << " (" << 100.0 - args.percent() << "% loss),"
		   << std::endl << "Approximate round trip times in milliseconds: " << std::endl
		   << "Minimum=" << args.minRTT() << "ms, Maximum=" << args.maxRTT()
		   << "ms, Average=" << args.avgRTT() << "ms"
		   << std::endl << "------------------------------------------";
		logger().information(os.str());
		consoleLogger->information(os.str());

	}

private:
	bool        _helpRequested;
	ICMPClient  _icmpClient;
	int         _repetitions;
	std::string _target;
	FormattingChannel* pFCConsole;
	Logger* consoleLogger;
};


int main(int argc, char** argv)
{
	Poco::Net::initializeNetwork();

	AutoPtr<Ping> pApp = new Ping;
	try
	{
		pApp->init(argc, argv);
	}
	catch (Poco::Exception& exc)
	{
		pApp->logger().log(exc);
		return Application::EXIT_CONFIG;
	}

	auto result = pApp->run();
	return result;
}*/


/*#include <typeinfo>
#include "iostream"

class Object
{
	public:
	Object():
		class_name(__func__)
	{

	}

	std::string getCallName()
	{
		return class_name;
	}

	std::string class_name;
};

class TestClass : public Object
{
	public:
	TestClass():
		Object ()
	{

	}
};

int main()
{
	TestClass bb;
	std::cout << bb.getCallName() << std::endl;

	return 0;
}*/


/*#include "Poco/Logger.h"
#include "Poco/Data/Session.h"
#include "Poco/Data/SQLite/Connector.h"
#include "Poco/Data/PostgreSQL/Connector.h"
#include "Poco/Data/PostgreSQL/SessionHandle.h"
#include "Poco/Data/PostgreSQL/PostgreSQLException.h"
#include "Poco/Data/PostgreSQL/Utility.h"
#include <vector>
#include <iostream>

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;

struct Person
{
	std::string name;
	std::string address;
	int         age;
};

void dbInfo(Session& session)
{
	std::cout << "Server Info: "  << Poco::Data::PostgreSQL::Utility::serverInfo(session)  << std::endl;
	std::cout << "Server Version: "   << Poco::Data::PostgreSQL::Utility::serverVersion(session)   << std::endl;
	std::cout << "Host Info: "<< Poco::Data::PostgreSQL::Utility::hostInfo(session)<< std::endl;
	std::cout << "Session Encoding: " <<Poco::Data::PostgreSQL::Utility::sessionEncoding(session) << std::endl;
}


int main(int argc, char** argv)
{
	// register SQLite connector
	Poco::Data::PostgreSQL::Connector::registerConnector();
	//Poco::Data::SQLite::Connector::registerConnector();


	// create a session
	std::string dbConnString;
	dbConnString = "host=ns1.us191.siteground.us user=nerogame_poco_user password=L!HBvgnTiUTk dbname=nerogame_poco port=5432";

	//Session session(Poco::Data::PostgreSQL::Connector::KEY, connect);

	try
		{
			std::cout << "Attempting to Connect to [" << dbConnString << "] without database: " << std::endl;
			Session session(Poco::Data::PostgreSQL::Connector::KEY, dbConnString);
			std::cout << "Connected to [" << dbConnString << "] without database." << std::endl;
			dbInfo(session);
			std::cout << "Disconnecting ..." << std::endl;
			session.close();
			std::cout << "Disconnected." << std::endl;
		}
		catch (Poco::Data::ConnectionFailedException& ex)
		{
			std::cout << ex.displayText() << std::endl;
		}
		catch (Poco::Data::PostgreSQL::ConnectionException& ex)
		{
			std::cout << ex.displayText() << std::endl;
		}

	// drop sample table, if it exists
	/*session << "DROP TABLE IF EXISTS Person", now;

	// (re)create table
	session << "CREATE TABLE Person (Name VARCHAR(30), Address VARCHAR, Age INTEGER(3))", now;

	// insert some rows
	Person person =
	{
		"Bart Simpson",
		"Springfield",
		12
	};

	Statement insert(session);
	insert << "INSERT INTO Person VALUES(?, ?, ?)",
		use(person.name),
		use(person.address),
		use(person.age);

	insert.execute();

	person.name    = "Lisa Simpson";
	person.address = "Springfield";
	person.age     = 10;

	insert.execute();

	// a simple query
	Statement select(session);
	select << "SELECT Name, Address, Age FROM Person",
		into(person.name),
		into(person.address),
		into(person.age),
		range(0, 1); //  iterate over result set one row at a time

	while (!select.done())
	{
		select.execute();
		std::cout << person.name << " " << person.address << " " << person.age << std::endl;
	}

	return 0;
}*/


#include "Poco/Environment.h"
#include <iostream>
#include <Nero/core/cpp/utility/Logging.h>

int main()
{
	nero::Logger::init();
	nero_log(Poco::Environment::osName());
	nero_log(Poco::Environment::osArchitecture());
	nero_log(Poco::Environment::osDisplayName());
	nero_log(Poco::Environment::osVersion());
	nero_log(Poco::Environment::processorCount());
	nero_log(nero_ss(Poco::Environment::arch()));

	return 0;
}
