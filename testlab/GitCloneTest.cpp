#include <gtest/gtest.h>
#include <git2/git2.h>

TEST(Git, Clone)
{
    git_libgit2_init();

    git_repository *repo = nullptr;
    const char *url = "http://github.com/sk-landry/nero-game-engine.git";
    const char *path = "nero-game-engine-clone-test";
    int error = git_clone(&repo, url, path, nullptr);

    git_clone_options clone_options;
    clone_options.checkout_opts.checkout_strategy = GIT_CHECKOUT_SAFE;

    if (error < 0) {
      const git_error *e = giterr_last();
      printf("Error %d/%d: %s\n", error, e->klass, e->message);
      exit(error);
    }

    git_libgit2_shutdown();
}
