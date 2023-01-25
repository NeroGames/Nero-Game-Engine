# Utility

## Run Clang Format on the Entire Project
find . -name "*.h" -path "*include/*" | xargs clang-format -i -style=file
find . -name "*.cpp" -path "*src/*" | xargs clang-format -i -style=file
