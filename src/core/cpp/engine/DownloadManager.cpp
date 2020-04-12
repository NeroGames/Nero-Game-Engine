////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//Poco

//Nero
#include <Nero/core/cpp/engine/DownloadManager.h>
//Cpp
#include <fstream>
#include <future>
////////////////////////////////////////////////////////////
namespace nero
{
	DownloadManager::DownloadManager():
		m_FutureMap()
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

	DownloadProgress::Ptr DownloadManager::downloadFile(const std::string& fileUrl, const std::string& destinationFile)
	{
		nero_log("creating download progress object");
		DownloadProgress::Ptr downloadProgress =  std::make_shared<DownloadProgress>();

		try
		{

		}
		catch (Poco::Exception& exc)
		{
			downloadProgress->m_Downloading = false;
			nero_log(exc.displayText(), nero::logging::LOG_ERROR);
		}

		nero_log("retrieving download size");
		float downloadSize = getFileSize(fileUrl);

		nero_log("download size = " + getPrettyString(downloadSize));

		if(downloadSize > 0.f)
		{

		}

		//download on another thread
		nero_log("download ...");
		downloadProgress->m_Downloading = true;
		downloadProgress->m_DownloadSize = downloadSize;
		my_Future = std::async(std::launch::async, [&](DownloadProgress::Ptr& downloadProgress)
		{
			nero_log("preparing download stream");

			Poco::URI uri(fileUrl);
			std::unique_ptr<std::istream> pStr(Poco::URIStreamOpener::defaultOpener().open(uri));

			nero_log("initializing download progess object");

			downloadProgress->m_DownloadStream = std::make_shared<std::ofstream>(destinationFile, std::ofstream::binary);

			downloadProgress->m_Ready = true;
			std::streamsize size =  Poco::StreamCopier::copyStream(*pStr.get(), *downloadProgress->m_DownloadStream);
			downloadProgress->m_DownloadStream->close();
			downloadProgress->m_Downloading = false;
			return 0;

		}, std::ref(downloadProgress));


		return downloadProgress;
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
				return static_cast<float>(response.getContentLength());
			}
			else
			{
				return -1.f;
			}
		}
		catch (Poco::Exception& exc)
		{
			nero_log(exc.displayText(), nero::logging::LOG_ERROR);
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
			result = string::toString(getGigaBytes(bytes)) + " GB";
		}
		else if(getMegaBytes(bytes) > 1.f)
		{
			result = string::toString(getMegaBytes(bytes)) + " MB";
		}
		else if(getKiloBytes(bytes) > 1.f)
		{
			result = string::toString(getKiloBytes(bytes)) + " KB";
		}
		else
		{
			result = string::toString(bytes) + " B";
		}

		return result;
	}

}

