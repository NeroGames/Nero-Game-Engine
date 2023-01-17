#include <gtest/gtest.h>
#include <iostream>
#include <libssh2/libssh2.h>
#include <libssh2/libssh2_publickey.h>
#include <libssh2/libssh2_sftp.h>

TEST(LibSSH2, CreateSession)
{
  // Test Libssh2
  LIBSSH2_SESSION *session;
  // Create a session instance
  session = libssh2_session_init();

  if (!session)
  {
    std::cout << "Could not initialize SSH session ! " << std::endl;
  }
  else
  {
    std::cout << " SSH  session initialize correctly ! " << std::endl;
  }
}
