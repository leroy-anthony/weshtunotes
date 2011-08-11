svn co https://weshtunotes.googlecode.com/svn/trunk/ weshtunotes/ --depth empty
svn up weshtunotes/version.txt
VERSION=`cat weshtunotes/version.txt`
VERSION_KDE=`wine C://Program\ Files//kde//bin//kde4-config.exe --version | grep KDE`
VERSION_QT=`wine C://Program\ Files//kde//bin//kde4-config.exe --version | grep Qt`
VERSION_SVN="r"`svnversion weshtunotes | sed s/M//`
cd windows
rm -rf *
svn checkout https://weshtunotes.googlecode.com/svn/trunk/ "kweshtunotes-$VERSION" --username leroy.anthony
cd "kweshtunotes-$VERSION"
mkdir build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../Toolchain-mingw32-kde.cmake ..
make
cd ../..
mkdir -p bin
mkdir -p share/apps/kweshtunotes
cp "kweshtunotes-$VERSION"/build/kweshtunotes.exe bin/
cp "kweshtunotes-$VERSION"/src/main/*.rc share/apps/kweshtunotes
7zr a "kweshtunotes-win-$VERSION-$VERSION_SVN".7z bin share/apps/kweshtunotes
cp "kweshtunotes-win-$VERSION-$VERSION_SVN".7z ..
#cd ..
#./googlecode_upload.py -l "OpSys-Windows,Type-Archive, Featured" -s "archive windows version $VERSION, revision $VERSION_SVN, $VERSION_KDE, $VERSION_QT" -p weshtunotes -u leroy.anthony@gmail.com -w tq7GM6Wf5dm4 "kweshtunotes-win-$VERSION-$VERSION_SVN.7z"