##depends ubuntu
sudo apt-get update && sudo apt-get -y upgrade
sudo apt-get -y install git build-essential libtool autotools-dev automake pkg-config libssl-dev libevent-dev bsdmainutils libboost-all-dev libqt5gui5 libqt5core5a libqt5dbus5 qttools5-dev qttools5-dev-tools libprotobuf-dev protobuf-compiler libqrencode-dev
sudo apt-get -y install software-properties-common
sudo add-apt-repository -y ppa:bitcoin/bitcoin
sudo apt-get update
sudo apt-get -y install libdb4.8-dev libdb4.8++-dev

cd "GCC Source"
./autogen.sh
./configure
make
optional reduce size of executable about 90%
strip ./src/germanccd 
strip ./src/germancc-tx
strip ./src/germancc-cli
strip ./src/qt/germancc-qt
sudo make install

