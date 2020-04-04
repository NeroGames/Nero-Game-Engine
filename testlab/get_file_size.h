#ifndef GET_FILE_SIZE_H
#define GET_FILE_SIZE_H

#include "Nero/core/cpp/engine/DownloadManager.h"

void get_file_size()
{
	nero::DownloadManager downloadManager;
	float result = downloadManager.getFileSize("https://nero-game.com/wp-content/uploads/2019/01/cropped-website_logo.png");
	nero_log(downloadManager.getPrettyString(result))
}
#endif // GET_FILE_SIZE_H
