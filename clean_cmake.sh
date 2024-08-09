garbage=(build.ninja CMakeCache.txt CMakeFiles cmake_install.cmake)

dirs=("mcu_support" "common")

for file in "${garbage[@]}"
do
    rm -rf "$file"
    for dir in "${dirs[@]}"
    do
        find "$dir" -name "$file" -exec rm -rf {} +
    done
done