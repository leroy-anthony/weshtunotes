svn co https://weshtunotes.googlecode.com/svn/trunk/ weshtunotes/ --depth empty
svn up weshtunotes/version.txt

VERSION=`cat weshtunotes/version.txt`
VERSION_SVN="r"`svnversion weshtunotes | sed s/P//`

cd sources
rm -rf *
svn checkout https://weshtunotes.googlecode.com/svn/trunk/ "kweshtunotes-$VERSION" --username leroy.anthony
7zr a "kweshtunotes-src-$VERSION-$VERSION_SVN".7z "kweshtunotes-$VERSION"
cp "kweshtunotes-src-$VERSION-$VERSION_SVN".7z ..
#cd ..
#./googlecode_upload.py -l "Type-Archive, Featured" -s "archive sources version $VERSION, revision $VERSION_SVN" -p weshtunotes -u leroy.anthony@gmail.com -w tq7GM6Wf5dm4 "kweshtunotes-src-$VERSION-$VERSION_SVN.7z"