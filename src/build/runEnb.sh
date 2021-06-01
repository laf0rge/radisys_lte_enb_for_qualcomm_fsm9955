echo > passwd.tmp
echo >> passwd.tmp
passwd < passwd.tmp
rm -rf passwd.tmp
rm -rf ../setup

. ./install.sh
. ./start_TeNB.sh
