svn co https://weshtunotes.googlecode.com/svn/trunk/ weshtunotes/ --depth empty
svn up weshtunotes/version.txt
VERSION=`svnversion weshtunotes | sed s/P//`
VERSION_NAME=`cat weshtunotes/version.txt`
VERSION_KDE=`kde4-config --version | grep KDE`
VERSION_QT=`kde4-config --version | grep Qt`
cd archlinux
rm -rf kwesh*
svn checkout https://weshtunotes.googlecode.com/svn/trunk/ "kweshtunotes-$VERSION" --username leroy.anthony
cd "kweshtunotes-$VERSION"
tar -cvzf ../"kweshtunotes-svn-$VERSION".tar.gz *
cd ..
sed s/'<SVN_REVISION>'/"$VERSION"/ PKGBUILD.proto > PKGBUILD
MD5SUM=`md5sum "kweshtunotes-svn-$VERSION".tar.gz | awk '{ print $1 }'`
sed s/'<MD5SUM>'/"$MD5SUM"/ PKGBUILD > PKGBUILD.tmp
mv PKGBUILD.tmp PKGBUILD
makepkg
cp "kweshtunotes-svn-$VERSION-1-i686.pkg.tar.xz" "kweshtunotes-svn-$VERSION_NAME-1-i686.pkg.tar.xz"
#../googlecode_upload.py -l "OpSys-Linux, Featured" -s "package archlinux version $VERSION_NAME, revision $VERSION, $VERSION_KDE, $VERSION_QT" -p weshtunotes -u leroy.anthony@gmail.com -w tq7GM6Wf5dm4 "kweshtunotes-svn-$VERSION_NAME-1-i686.pkg.tar.xz"