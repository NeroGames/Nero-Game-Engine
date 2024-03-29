////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H
///////////////////////////HEADERS//////////////////////////
// Poco
#include <Nero/core/cpp/utility/Logging.h>
#include <Poco/URIStreamOpener.h>
#include <Poco/StreamCopier.h>
#include <Poco/Path.h>
#include <Poco/URI.h>
#include <Poco/SharedPtr.h>
#include <Poco/Exception.h>
#include <Poco/NullStream.h>
#include <Poco/FileStreamFactory.h>
#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPStreamFactory.h>
#include <Poco/Net/HTTPSStreamFactory.h>
#include <Poco/Net/FTPStreamFactory.h>
#include <Poco/Net/SSLManager.h>
#include <Poco/Net/KeyConsoleHandler.h>
#include <Poco/Net/ConsoleCertificateHandler.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPCredentials.h>
// Nero
#include <Nero/core/cpp/engine/Parameter.h>
#include <Nero/core/cpp/engine/BackgroundTask.h>
// Cpp
#include <memory>
#include <fstream>
#include <future>
////////////////////////////////////////////////////////////
namespace nero
{
    struct DownloadProgress
    {
        typedef std::shared_ptr<DownloadProgress> Ptr;
        std::shared_ptr<std::ofstream>            m_DownloadStream = nullptr;
        float                                     m_DownloadSize   = 0.f;
        bool                                      m_Downloading    = false;
        bool                                      m_Ready          = false;
        int                                       getPercentage()
        {
            if(!m_Downloading || !m_Ready)
            {
                return 0.f;
            }

            return std::ceil(float(m_DownloadStream->tellp() * 100.f) / m_DownloadSize);
        }
    };

    class DownloadManager
    {
      public:
        typedef std::shared_ptr<DownloadManager> Ptr;

      public:
        DownloadManager();
        ~DownloadManager();

        DownloadProgress::Ptr downloadFile(const std::string& fileUrl,
                                           const std::string& destinationFile);
        void        downloadFile(const Parameter& parameter, BackgroundTask::Ptr backgroundTask);
        float       getFileSize(const std::string& fileUrl);
        float       getMegaBytes(const float bytes);
        float       getKiloBytes(const float bytes);
        float       getGigaBytes(const float bytes);
        std::string getPrettyString(const float bytes);
        std::map<std::string, std::future<int>> m_FutureMap;
        std::future<int>                        my_Future;
        Poco::Net::HTTPSClientSession           m_NeroGameSession;
    };

} // namespace nero
#endif // DOWNLOADMANAGER_H
