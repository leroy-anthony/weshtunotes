svn up weshtunotes
VERSION=`svnversion weshtunotes | sed s/M//`
VERSION_NAME=`cat weshtunotes/version.txt`
VERSION_KDE=`kde4-config --version | grep KDE`
VERSION_QT=`kde4-config --version | grep Qt`
cd archlinux
../googlecode_upload.py -l "OpSys-Linux, Featured" -s "package archlinux version $VERSION_NAME, revision $VERSION, $VERSION_KDE, $VERSION_QT" -p weshtunotes -u leroy.anthony@gmail.com -w tq7GM6Wf5dm4 "kweshtunotes-svn-$VERSION_NAME-1-i686.pkg.tar.xz"