![travis ci:](https://travis-ci.org/Nuzhny007/Non-Maximum-Suppression.svg?branch=master)

# Non-Maximum-Suppression
C++ implementation of Non-Maximum Suppression algorithm.
Original code on Python are here: http://www.pyimagesearch.com/2014/11/17/non-maximum-suppression-object-detection-python/

And added more options:
1. Simple suppression without scores, minimum neighbours count = 1:
![Simple with neighbours](https://github.com/Nuzhny007/Non-Maximum-Suppression/blob/master/data/simple_n1.png?raw=true "Title")

2. Suppression with scores, minimum neighbours count = 1:
![With scores](https://github.com/Nuzhny007/Non-Maximum-Suppression/blob/master/data/score.png?raw=true "Title")

3. Suppression with scores and with minimum scores sum = 0.8, minimum neighbours count = 1:
![With scores and sum](https://github.com/Nuzhny007/Non-Maximum-Suppression/blob/master/data/score_min08.png?raw=true "Title")
