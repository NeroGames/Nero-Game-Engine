#ifndef GET_FILE_SIZE_H
#define GET_FILE_SIZE_H

#include "Nero/core/cpp/engine/DownloadManager.h"
#include "SFML/System/Clock.hpp"
#include "iostream"

void get_file_size()
{
	nero::DownloadManager downloadManager;
	std::string fileUrl = "https://nero-game.com/resource/engine/NeroGameEngine1.0.1.rar";
	std::string destination = "NeroGameEngine1.0.1.rar";
	nero::DownloadProgress::Ptr progress = downloadManager.downloadFile(fileUrl, destination);

	nero_log_if("show progress status", progress->m_Downloading);

	nero_log("bonjour");

	//while (progress->m_Downloading)
	while (progress->m_Downloading)
	{
		//std::this_thread::sleep_for(std::chrono::seconds(1));
		nero_log(nero_ss(progress->getPercentage()));

		std::cout << nero_ss(progress->getPercentage()) << "\r" << std::flush;
	}
	//nero_log(downloadManager.getPrettyString(result))
}
#endif // GET_FILE_SIZE_H
