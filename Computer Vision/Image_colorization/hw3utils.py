# These are utility functions / classes that you probably dont need to alter.

import torchvision
import torchvision.transforms as transforms
import matplotlib.pyplot as plt
from PIL import Image
import os
import torch

def tensorshow(tensor,cmap=None):
    img = transforms.functional.to_pil_image(tensor/2+0.5)
    if cmap is not None:
        plt.imshow(img,cmap=cmap)
    else:
        plt.imshow(img)

class HW3ImageFolder(torchvision.datasets.ImageFolder):
    """A version of the ImageFolder dataset class, customized for the super-resolution task"""

    def __init__(self, root, device):
        super(HW3ImageFolder, self).__init__(root, transform=None)
        self.device = device

    def prepimg(self,img):
        return (transforms.functional.to_tensor(img)-0.5)*2 # normalize tensorized image from [0,1] to [-1,+1]

    def __getitem__(self, index):
        """
        Args:
            index (int): Index

        Returns:
            tuple: (grayscale_image, color_image) where grayscale_image is the decolorized version of the color_image.
        
        ############################################################################################################
        In order to obtain the path of the image with index index you may use following piece of code. As dataset goes
        over different indices you will collect image paths.

        myfile = open('test_images.txt', 'a')
        path = self.imgs[index][0]
        myfile.write(path)
        myfile.write('\n')
        myfile.close()
        ############################################################################################################
        """
        color_image,_ = super(HW3ImageFolder, self).__getitem__(index) # Image object (PIL)
        grayscale_image = torchvision.transforms.functional.to_grayscale(color_image)
        return self.prepimg(grayscale_image).to(self.device), self.prepimg(color_image).to(self.device)

# method to load the test dataset
# similar to method used to load the training and validation dataset, difference being there are no subdirectories
# in the specified root directory hence usage of torch.utils.data.Dataset instead of torchvision.datasets.ImageFolder
class HW3ImageFolderTest(torch.utils.data.Dataset):
    def __init__(self, root, device):
        self.root = root
        self.device = device
        self.image_paths = [os.path.join(root, filename) for filename in os.listdir(root) if os.path.isfile(os.path.join(root, filename))]

    def __len__(self):
        return len(self.image_paths)

    def prepimg(self,img):
        return (transforms.functional.to_tensor(img)-0.5)*2 # normalize tensorized image from [0,1] to [-1,+1]

    def __getitem__(self, index):
        image_path = self.image_paths[index]
        grayscale_image = Image.open(image_path)
        return self.prepimg(grayscale_image).to(self.device), self.prepimg(grayscale_image).to(self.device)

def visualize_batch(inputs,preds,targets,save_path=''):
    inputs = inputs.cpu()
    preds = preds.cpu()
    targets = targets.cpu()
    plt.clf()
    bs = inputs.shape[0]
    for j in range(bs):
        plt.subplot(3,bs,j+1)
        assert(inputs[j].shape[0]==1)
        tensorshow(inputs[j],cmap='gray')
        plt.subplot(3,bs,bs+j+1)
        tensorshow(preds[j])
        plt.subplot(3,bs,2*bs+j+1)
        tensorshow(targets[j])
    if save_path != '':
        plt.savefig(save_path)
    else:
        plt.show(block=True)


