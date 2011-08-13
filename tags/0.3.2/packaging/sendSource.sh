svn up weshtunotes
VERSION=`cat weshtunotes/version.txt`
VERSION_SVN="r"`svnversion weshtunotes | sed s/M//`
./googlecode_upload.py -l "Type-Archive, Featured" -s "archive sources version $VERSION, revision $VERSION_SVN" -p weshtunotes -u leroy.anthony@gmail.com -w tq7GM6Wf5dm4 "kweshtunotes-src-$VERSION-$VERSION_SVN.7z"