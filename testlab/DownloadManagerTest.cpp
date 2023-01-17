#include <gtest/gtest.h>

#include <Nero/core/cpp/engine/DownloadManager.h>
#include <SFML/System/Clock.hpp>
#include <iostream>


namespace
{
const std::string FILE_URL    = "https://nero-games.com/resource/download/engine/Nero-Game-Engine-r1.0.0.rar";
const std::string DESTINATION = "Nero-Game-Engine-r1.0.0.rar";
}

TEST(DownloadManager, DownloadFile)
{
  nero::DownloadManager downloadManager;
  nero::DownloadProgress::Ptr progress = downloadManager.downloadFile(FILE_URL, DESTINATION);

  while (progress->m_Downloading)
  {
         std::this_thread::sleep_for(std::chrono::seconds(1));

          std::cout << nero_s(progress->getPercentage()) << "\r" << std::flush;
  }
}

TEST(DownloadManager, RetrieveOnlineFileSize)
{
  nero::DownloadManager downloadManager;

  const auto fileSize = downloadManager.getFileSize(FILE_URL);

  nero_log(downloadManager.getPrettyString(fileSize))
}
