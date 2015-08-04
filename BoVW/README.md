## Bag of Words (Cpp version)

Most of the code comes from [BoW](https://github.com/grapeot/BoW). I'm improving it now.

## Usage

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
./search /Users/willard/Pictures/imagesDataSet/templerun_3.png imageNamesList.txt
```

it's will return a html file. Open the file with a browser, and you will see the search result.