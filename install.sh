echo "Compiling vr.cpp ..."
g++ vr.cpp -o vr 2> vr.log
echo "Moving vr ..."
sudo mv vr /usr/local/bin/
echo -e "\e[32mInstallation Compreted\e[0m"