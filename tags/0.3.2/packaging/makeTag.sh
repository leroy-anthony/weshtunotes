svn co https://weshtunotes.googlecode.com/svn/trunk/ weshtunotes/ --depth empty
svn up weshtunotes/version.txt

VERSION=`cat weshtunotes/version.txt`

svn copy https://weshtunotes.googlecode.com/svn/trunk/ https://weshtunotes.googlecode.com/svn/tags/$VERSION -m "version $VERSION"
