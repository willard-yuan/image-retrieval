# bag of words for image retrieval

![](http://yongyuan.name/images/posts/2015-06-16/ukbench00000.jpg)

Python Implementation of Bag of Words for Image Retrieval using OpenCV and
sklearn | [Post](http://yongyuan.name/blog/practical-BoW-for-image-retrieval-with-python.html)

## Training the codebook and quantization
```
python findFeatures.py -t dataset/train/
```

* Query a single image
```
python search.py -i dataset/train/ukbench00000.jpg
```

![](http://yongyuan.name/images/posts/2015-06-16/ukbench00055.jpg)
