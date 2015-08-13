## Bag of Words稳定版

**致谢**：基础框架来源[BoW](https://github.com/grapeot/BoW)，开发版本在此基础上进行，已在Ubuntu、OS X上测试通过，Windows需要支持c++11的编译器(VS2012及其以上)。

## 使用

1. 编译

修改`Makefile`文件，如果你的系统支持多线程技术，将
```sh
CFLAGS = -std=c++11
```
修改为
```sh
CFLAGS = -std=c++11 -fopenMP # if openMP accesses, using this
```
修改完上面后，再修改编译所需的OpenCV和cppsugar，即
```sh
INCPATH = -I/usr/local/include -I/Users/willard/codes/cpp/opencv-computer-vision/cpp/BoVW/cppsugar
LIBPATH = -L/usr/local/lib
```
`/usr/local/include`和`/usr/local/lib`分别是OpenCV所在的包含头文件目录路径和库目录路径，修改为你本机所在的目录即可。后面的cppsugar目录同样换成你本机的目录。
修改。这些修改完成后，执行下面命令进行编译：

```sh
make
```
编译后即可在所在目录生成可执行文件。

1.Generate the images list:

```sh
python imgNamesToTXT.py -t /Users/willard/Pictures/imagesDataSet
```

this will get a file named `imageNamesList.txt`.

2.Construct the index:

```sh
./index imageNamesList.txt
```

this will return two files `bows.dat` and `dict.dat`.

3.Query a image:

```sh
./search /Users/willard/Pictures/first1000/ukbench00499.jpg imageNamesList.txt
```

**Note**: `ukbench00954.jpg`, `ukbench00499.jpg`, `ukbench00399.jpg`, `kbench00729.jpg`, `ukbench00225.jpg`, `ukbench00325.jpg` are good tests for query.

it's will return a html file. Open the file with a browser, and you will see the search result.

4.Experiments

**Ukbench** first 1000 images, the NS-scores: 3.358, with tf*idf and histogram intersection kernel distance.
**Ukbench** first 1000 images, the NS-scores: 3.602, with tf and histogram intersection kernel distance.


## To do

1. Add invert document frequence. (done)
2. Compute **Ukbench** NS score. (nogoing)
2. Replace SIFT with RootSIFT.
3. Add re-ranking.
