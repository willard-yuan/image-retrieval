# import the necessary packages
import numpy as np
import cv2

class RootSIFT:
	def __init__(self):
		pass
	def compute(self, kps, descs, eps=1e-7):
		# if there are no keypoints or descriptors, return an empty tuple
		if len(kps) == 0:
			return ([], None)

		# apply the Hellinger kernel by first L1-normalizing, taking the
		# square-root, and then L2-normalizing
		#descs /= (np.linalg.norm(descs, axis=0, ord=2) + eps)
		descs /= (descs.sum(axis=0) + eps)
		descs = np.sqrt(descs)
		descs /= (np.linalg.norm(descs, axis=0, ord=2) + eps)

		# return a tuple of the keypoints and descriptors
		return descs