## Bag of Words stable version

1. Generate the images list:

```sh
python imgNamesToTXT.py -t /Users/willard/Pictures/imagesDataSet
```

this will get a file named `imageNamesList.txt`.

2. Construct the index:

```sh
./index imageNamesList.txt
```

this will return two files `bows.dat` and `dict.dat`.

3. Query a image:

```sh
./search /Users/willard/Pictures/first1000/ukbench00499.jpg imageNamesList.txt
```

**Note**: `ukbench00954.jpg`, `ukbench00499.jpg`, `ukbench00399.jpg`, `kbench00729.jpg`, `ukbench00225.jpg`, `ukbench00325.jpg` are good tests for query.

it's will return a html file. Open the file with a browser, and you will see the search result.

4. Experiments

**Ukbench** first 1000 images, the NS-scores: 3.358, with tf*idf and histogram intersection kernel distance.
**Ukbench** first 1000 images, the NS-scores: 3.602, with tf and histogram intersection kernel distance.


## To do

1. Add invert document frequence. (done)
2. Compute **Ukbench** NS score. (nogoing)
2. Replace SIFT with RootSIFT.
3. Add re-ranking.
