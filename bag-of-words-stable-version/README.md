## Bag of Words稳定版

**致谢**：基础框架来源[BoW](https://github.com/grapeot/BoW)，开发版本在此基础上进行，已在Ubuntu、OS X上测试通过，Windows需要支持c++11的编译器(VS2012及其以上)。

![](http://i300.photobucket.com/albums/nn17/willard-yuan/blog/Screen%20Shot%202015-08-13%20at%209.15.33%20PM_zpsq8qvkew7.png)

## 使用

一. 编译

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

二. 生成图库列表文件

对于待检索的图像库imagesDataSet，执行下面命令
```sh
python imgNamesToTXT.py -t /Users/willard/Pictures/imagesDataSet
```
上面执行后生成一个`imageNamesList.txt`的文件，该文件中包含的是每幅图像的路径及其图像文件名。

三. 建立索引

执行下面命令，会完成特征提取、生成词典、量化生成bag of word向量：
```sh
./index imageNamesList.txt
```
上面命令执行玩，会生成两个文件`bows.dat`和`dict.dat`，分别存放的是图像库每幅图像的bag of word向量以及词典。

四. 查询图像

按下面命令进行查询
```sh
./search /Users/willard/Pictures/first1000/ukbench00499.jpg imageNamesList.txt
```
其中`/Users/willard/Pictures/first1000/ukbench00499.jpg`是查询图像，执行完后，会生成一个`result.html`的文件，应为要显示检索结果，所以这里采用的是用html页面的方式显示检索结果的，用浏览器打开即可。

五. 批量测试

为了评价检索的效果，可以使用`ukbenchScores.cpp`计算在ukbench图像库上的NS score(NS分数)，下面是在ukbench1000张图像上计算的NS score:

**Ukbench** first 1000 images, the NS-scores: 3.358, with tf*idf and histogram intersection kernel distance.

**Ukbench** first 1000 images, the NS-scores: 3.602, with tf and histogram intersection kernel distance.

## Bag of words原理

关于bag of words的原理，可以查阅我的博文[BoW图像检索Python实战](http://yongyuan.name/blog/practical-BoW-for-image-retrieval-with-python.html)和[Bag of Words模型](http://yongyuan.name/blog/bag-of-word-model.html)。**注意**，该框架中采用的相似性度量方式是直方图相交(histogram intersection kernel)的方法，测试发现直方图相交的方法要比用余弦距离度量的方式效果更好，但计算速度较慢。

开发版本[bag-of-words-dev-version](https://github.com/willard-yuan/image-retrieval/tree/master/bag-of-words-dev-version)中加入了逆文档词频以及RANSAC重排，待效果达到预期后，会添加到稳定版中。
