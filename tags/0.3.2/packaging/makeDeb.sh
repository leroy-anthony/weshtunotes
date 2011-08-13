svn co https://weshtunotes.googlecode.com/svn/trunk/ weshtunotes/ --depth empty
svn up weshtunotes/version.txt

VERSION=0.0.`svnversion weshtunotes | sed s/P//`
cd deb
rm -rf *
svn checkout https://weshtunotes.googlecode.com/svn/trunk/ "weshtunotes-$VERSION" --username leroy.anthony
tar -cvzf "weshtunotes-$VERSION.orig.tar.gz" "weshtunotes-$VERSION"/
cd "weshtunotes-$VERSION"
dh_make -e leroy.anthony@gmail.com -c gpl
cd debian

sed s/'<url:\/\/example.com>'/'code.google.com\/p\/weshtunotes'/ copyright > copyright.txt
sed s/"<put author's name and email here>"/'LEROY Anthony <leroy.anthony@gmail.com>'/ copyright.txt > copyright
sed s/'<likewise for another author>'// copyright > copyright.txt
sed s/'<Copyright (C) YYYY Firtname Lastname>'/'Copyright (c) 2009 LEROY Anthony'/ copyright.txt > copyright
sed s/'<Put the license of the package here indented by 4 spaces>'/'GPL version 3'/ copyright > copyright.txt
mv copyright.txt copyright 

echo "usr/share/weshtunotes" >> dirs
echo "usr/share/weshtunotes/data" >> dirs
echo "usr/share/weshtunotes/data/icon" >> dirs

cp ../../../ressources/control.template control

rm -rf *ex *EX README*
cd ..
debuild -S -sa --lintian-opts -i
cd ..
sudo pbuilder build *.dsc
cd ..
./googlecode_upload.py -l "OpSys-Linux,Type-Package" -s "paquet ubuntu/debian revision $VERSION" -p weshtunotes -u leroy.anthony@gmail.com -w tq7GM6Wf5dm4 "/var/cache/pbuilder/result/weshtunotes_$VERSION-1_i386.deb"
