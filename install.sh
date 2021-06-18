echo -e "Compiling vr.cpp ..."

if [ ! -e "vr.cpp" ]; then
    echo -e "\e[31mError:\e[0m cannot find \"vr.cpp\""
    exit 1
fi

g++ vr.cpp -o vr -std=c++17 2> vr.log
if [ $? -ne 0 ]; then
    echo -e "\e[31mError:\e[0m cannot compile vr.cpp"
fi

echo "Moving vr ..."
if [ ! -e "vr" ]; then
    echo -e "\e[31mError:\e[0m cannot find \"vr\""
    exit 1
fi
sudo mv vr /usr/local/bin/ 2> vr.log

if [ $? -ne 0 ]; then
    echo -e "\e[31mError:\e[0m cannot move vr into /usr/local/bin/"
fi

echo -e "\e[32mInstallation Completed\e[0m"