////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 SANOU A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//Poco

//Nero
#include <Nero/core/cpp/engine/DownloadManager.h>
//Cpp
#include <fstream>
////////////////////////////////////////////////////////////
namespace nero
{
	DownloadManager::DownloadManager()
	{
		Poco::Net::initializeNetwork();
		Poco::Net::initializeSSL();

		Poco::Net::HTTPStreamFactory::registerFactory();
		Poco::Net::HTTPSStreamFactory::registerFactory();
		Poco::Net::FTPStreamFactory::registerFactory();

		// Note: we must create the passphrase handler prior Context
		Poco::SharedPtr<Poco::Net::InvalidCertificateHandler> ptrCert = new Poco::Net::ConsoleCertificateHandler(false); // ask the user via console
		Poco::Net::Context::Ptr ptrContext = new Poco::Net::Context(Poco::Net::Context::TLSV1_1_CLIENT_USE, "", "", "rootcert.pem", Poco::Net::Context::VERIFY_RELAXED, 9, false, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
		//ptrContext->requireMinimumProtocol(Poco::Net::Context::PROTO_TLSV1_1);
		Poco::Net::SSLManager::instance().initializeClient(0, ptrCert, ptrContext);
	}

	DownloadManager::~DownloadManager()
	{

	}

	void DownloadManager::downloadFile(const std::string& fileUrl, const std::string& destinationFile)
	{
		/*try
		{
			Poco::URI uri(fileUrl);

			std::auto_ptr<std::istream> pStr(Poco::URIStreamOpener::defaultOpener().open(uri));

			pStr->seekg(0, std::ios::end);
			unsigned int uiLength = pStr->tellg();

			nero_log(nero_ss(uiLength));

			//std::ofstream file(destinationFile, std::ofstream::binary);

			//std::streamsize size =  Poco::StreamCopier::copyStream(*pStr.get(), file);

			//file.close();
		}
		catch (Poco::Exception& exc)
		{
			nero_log(exc.displayText(), nero::LOG_ERROR);
		}//*/
	}

	void DownloadManager::downloadFile(const Parameter& parameter, BackgroundTask::Ptr backgroundTask)
	{

	}

	float DownloadManager::getFileSize(const std::string& fileUrl)
	{
		try
		{
			Poco::URI uri(fileUrl);
			std::string path(uri.getPathAndQuery());
			if (path.empty()) path = "/";

			Poco::Net::HTTPSClientSession session(uri.getHost(), uri.getPort());
			Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, path, Poco::Net::HTTPMessage::HTTP_1_1);

			Poco::Net::HTTPResponse response;

			session.sendRequest(request);
			session.receiveResponse(response);
			if (response.getStatus() != Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED)
			{
				if(response.getStatus() ==  Poco::Net::HTTPResponse::HTTP_FOUND)
				{
					std::string redirectUrl = response.get("Location");
					return getFileSize(redirectUrl);
				}

				std::streamsize result = response.getContentLength();
				return static_cast<float>(result);
			}
			else
			{
				return -1.f;
			}
		}
		catch (Poco::Exception& exc)
		{
			nero_log(exc.displayText(), nero::LOG_ERROR);
		}

		return  -1.f;
	}

	float DownloadManager::getMegaBytes(const float bytes)
	{
		return bytes / 1024.f / 1024.f;
	}

	float DownloadManager::getKiloBytes(const float bytes)
	{
		return bytes / 1024.f;
	}

	float DownloadManager::getGigaBytes(const float bytes)
	{
		return bytes / 1024.f / 1024.f / 1024.f;
	}

	std::string DownloadManager::getPrettyString(const float bytes)
	{
		std::string result;

		if(bytes < 0.f)
		{
			result = "null";
		}
		else if(getGigaBytes(bytes) > 1.f)
		{
			result = toString(getGigaBytes(bytes)) + " GB";
		}
		else if(getMegaBytes(bytes) > 1.f)
		{
			result = toString(getMegaBytes(bytes)) + " MB";
		}
		else if(getKiloBytes(bytes) > 1.f)
		{
			result = toString(getKiloBytes(bytes)) + " KB";
		}
		else
		{
			result = toString(bytes) + " B";
		}

		return result;
	}

}

