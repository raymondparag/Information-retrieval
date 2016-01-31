# Information-retrieval
This is a repo for the course Multimedia Information Retrieval at University Leiden.

Programmed using Xcode 7.2 with clang compiler on OSX El Capitan. 

Libraries used are: Libcurl [[Link]](http://curl.haxx.se/libcurl/) and htmlstreamparser [[Link]](https://code.google.com/archive/p/htmlstreamparser/).

##Installation
1. Download **curl-7.47.0.zip** from the Libcurl site
2. Unzip the file
3. Run the following commands in a terminal in the downloaded folder
  * ./configure
  * make
  * make test
  * sudo make install
4. Add libcurl.tbd to "Link Binary with Libraries" in Xcode
5. Download **htmlstreamparser-0.4.tar.gz** from the htmlstreamparser site
6. Unzip the file
7. Go to the source folder of the downloaded folder and copy **htmlstreamparser.c** and **htmlstreamparser.h** into your Xcode project
8. Change the extension of **htmlstreamparser.c** to **.cpp**
9. Done!

For a more detailed installation process go to this [site](http://dahu.co.uk/blog/?p=77).
