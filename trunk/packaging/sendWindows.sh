svn up weshtunotes
VERSION=`cat weshtunotes/version.txt`
VERSION_KDE=`wine C://Program\ Files//kde//bin//kde4-config.exe --version | grep KDE`
VERSION_QT=`wine C://Program\ Files//kde//bin//kde4-config.exe --version | grep Qt`
VERSION_SVN="r"`svnversion weshtunotes | sed s/M//`
./googlecode_upload.py -l "OpSys-Windows,Type-Archive, Featured" -s "archive windows version $VERSION, revision $VERSION_SVN, $VERSION_KDE, $VERSION_QT" -p weshtunotes -u leroy.anthony@gmail.com -w tq7GM6Wf5dm4 "kweshtunotes-win-$VERSION-$VERSION_SVN.7z"