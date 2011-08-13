svn co https://weshtunotes.googlecode.com/svn/trunk/ weshtunotes/ --depth empty
svn up weshtunotes/version.txt
VERSION=`svnversion weshtunotes | sed s/P//`
VERSION_NAME=`cat weshtunotes/version.txt`
VERSION_KDE=`kde4-config --version | grep KDE`
VERSION_QT=`kde4-config --version | grep Qt`
cd test
rm -rf kwesh*
svn checkout https://weshtunotes.googlecode.com/svn/trunk/ "kweshtunotes-$VERSION" --username leroy.anthony
cd "kweshtunotes-$VERSION"
mkdir build
cd build
cmake ..
make