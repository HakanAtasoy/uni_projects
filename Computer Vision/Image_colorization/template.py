# Feel free to change / extend / adapt this source code as needed to complete the homework, based on its requirements.
# This code is given as a starting point.
#
# REFEFERENCES
# The code is partly adapted from pytorch tutorials, including https://pytorch.org/tutorials/beginner/blitz/cifar10_tutorial.html

# ---- hyper-parameters ----
# You should tune these hyper-parameters using:
# (i) your reasoning and observations, 
# (ii) by tuning it on the validation set, using the techniques discussed in class.
# You definitely can add more hyper-parameters here.
batch_size = 16
max_num_epoch = 100
hps = {'lr':0.01}

# Candidate hyperparameters
num_conv_layers = 1
num_kernels = 2

# ---- options ----
DEVICE_ID = 'cpu'
LOG_DIR = 'checkpoints'
VISUALIZE = True # set True to visualize input, prediction and the output from the last batch
LOAD_CHKPT = False


# --- imports ---
import torch
import os
import matplotlib.pyplot as plt
import numpy as np
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim
import torchvision.transforms as transforms
import hw3utils
torch.multiprocessing.set_start_method('spawn', force=True)

if not os.path.exists(LOG_DIR):
    os.makedirs(LOG_DIR)
# ---- utility functions -----
def get_loaders(batch_size,device):
    data_root = 'ceng483-hw3-dataset'
    test_data_root = 'images_grayscale'
    train_set = hw3utils.HW3ImageFolder(root=os.path.join(data_root,'train'),device=device)
    train_loader = torch.utils.data.DataLoader(train_set, batch_size=batch_size, shuffle=True, num_workers=0)
    val_set = hw3utils.HW3ImageFolder(root=os.path.join(data_root,'val'),device=device)
    val_loader = torch.utils.data.DataLoader(val_set, batch_size=batch_size, shuffle=False, num_workers=0)
    # loading the test dataset with a similar method used to load training and validation
    test_set = hw3utils.HW3ImageFolderTest(root=test_data_root,device=device)
    test_loader = torch.utils.data.DataLoader(test_set, batch_size=batch_size, shuffle=False, num_workers=0)

    return train_loader, val_loader, test_loader

# ---- ConvNet -----
class Net(nn.Module):
    def __init__(self, num_conv_layers, num_kernels, use_batchnorm, use_tanh, use_16_in_channels=False):
        super(Net, self).__init__()
        self.num_conv_layers = num_conv_layers
        self.num_kernels = num_kernels
        self.use_batchnorm = use_batchnorm
        self.use_tanh = use_tanh
        self.use_16_in_channels = use_16_in_channels
        in_channels = 16 if use_16_in_channels else 1  # Set input channels based on use_16_in_channels
        layers = []
        layers.append(nn.Conv2d(in_channels, num_kernels, 3, padding=1))

        # Adding batch-norm layer if it is set to true
        if use_batchnorm:
            layers.append(nn.BatchNorm2d(num_kernels))

        layers.append(nn.ReLU())

        # Making sure every convolutional layer is followed by a relu activation
        for _ in range(num_conv_layers - 1):
            layers.append(nn.Conv2d(num_kernels, num_kernels, 3, padding=1))

            # Adding batch-norm layer if it is set to true (after every convolutional layer)
            if use_batchnorm:
                layers.append(nn.BatchNorm2d(num_kernels))

            layers.append(nn.ReLU())

        layers.append(nn.Conv2d(num_kernels, 3, 3, padding=1))  # Output should have 3 channels (R, G, B)

        # Adding tanh activation after the very last convolutional layer if it is set to true
        if use_tanh:
            layers.append(nn.Tanh())

        self.conv_layers = nn.Sequential(*layers)

    def forward(self, grayscale_image):
        if self.use_16_in_channels:
            # If use_16_in_channels is True, expand the channels dimension to 16
            grayscale_image = grayscale_image.expand(-1, 16, -1, -1)
        x = self.conv_layers(grayscale_image)
        return x

# ---- training code -----
device = torch.device(DEVICE_ID)
print('device: ' + str(device))
train_loader, val_loader, test_loader = get_loaders(batch_size, device)
# ---- training code -----
def train(net, optimizer, train_loader, val_loader, max_num_epoch, device, checkpoint_path, LOG_DIR):
    # printing the information about the network
    print('Training begins')
    print(f'Number of Convolutional Layers: {net.num_conv_layers}')
    print(f'Number of Kernels: {net.num_kernels}')
    print(f'Learning Rate: {hps["lr"]}')
    print(f'Use BatchNorm: {net.use_batchnorm}')
    print(f'Use Tanh: {net.use_tanh}')
    print(f'Use 16 channels? : {net.use_16_in_channels}')

    criterion = nn.MSELoss()  # This line defines the criterion
    best_val_loss = float('inf')
    best_epoch = 0

    # Lists to store training and validation losses
    train_losses = []
    val_losses = []

    for epoch in range(max_num_epoch):
        running_loss = 0.0
        total_loss = 0.0
        for iteri, data in enumerate(train_loader, 0):
            inputs, targets = data

            optimizer.zero_grad()
            preds = net(inputs)
            loss = criterion(preds, targets)
            loss.backward()
            optimizer.step()

            running_loss += loss.item()
            total_loss += loss.item()

            print_n = 100
            if iteri % print_n == (print_n - 1):
                print('[%d, %5d] network-loss: %.3f' %
                      (epoch + 1, iteri + 1, running_loss / print_n))
                running_loss = 0.0

        avg_train_loss = total_loss / len(train_loader)
        train_losses.append(avg_train_loss)

        if (epoch + 1) % 5 == 0:
            # Validation
            net.eval()
            val_loss = 0.0

            with torch.no_grad():
                for val_data in val_loader:
                    val_inputs, val_targets = val_data
                    val_preds = net(val_inputs)
                    val_loss += criterion(val_preds, val_targets).item()

            avg_val_loss = val_loss / len(val_loader)
            print('[Epoch %d] Validation Loss: %.3f' % (epoch + 1, avg_val_loss))
            val_losses.append(avg_val_loss)

            # Update best val loss and epoch if needed
            if avg_val_loss < best_val_loss:
                best_val_loss = avg_val_loss
                best_epoch = epoch + 1  # Save the best epoch

            if epoch + 1 == max_num_epoch:
                hw3utils.visualize_batch(val_inputs, val_preds, val_targets, os.path.join(LOG_DIR, 'example.png'))

    # Plotting the training and validation losses
    # This part is specifically for part 3 in the report template it can be commented out until testing that part.
    plt.figure(figsize=(10, 6))
    plt.plot(range(1, max_num_epoch + 1), train_losses, label='Training Loss', marker='o')
    plt.plot(range(5, max_num_epoch + 1, 5), val_losses, label='Validation Loss', marker='o')
    plt.title('Training and Validation Loss Over Epochs')
    plt.xlabel('Epoch')
    plt.ylabel('Mean Squared Error Loss')
    plt.legend()
    plt.grid(True)
    plt.show()

    print(f"Best Validation Loss: {best_val_loss} (Epoch {best_epoch})")

"""
# Experiment 1: Adding BatchNorm
net1 = Net(num_conv_layers, num_kernels, use_batchnorm=True, use_tanh=False).to(device=device)
optimizer1 = optim.SGD(net1.parameters(), lr=hps['lr'])
checkpoint_path1 = os.path.join(LOG_DIR, 'checkpoint_batchnorm.pt')

train(net1, optimizer1, train_loader, val_loader, max_num_epoch, device, checkpoint_path1, LOG_DIR)

# Experiment 2: Adding Tanh
net2 = Net(num_conv_layers, num_kernels, use_batchnorm=False, use_tanh=True).to(device=device)
optimizer2 = optim.SGD(net2.parameters(), lr=hps['lr'])
checkpoint_path2 = os.path.join(LOG_DIR, 'checkpoint_tanh.pt')

train(net2, optimizer2, train_loader, val_loader, max_num_epoch, device, checkpoint_path2, LOG_DIR)

# Experiment 3: Setting num_channels to 16
net3 = Net(num_conv_layers, num_kernels, use_batchnorm=False, use_tanh=False, use_16_in_channels=True).to(device=device)
optimizer3 = optim.SGD(net3.parameters(), lr=hps['lr'])
checkpoint_path3 = os.path.join(LOG_DIR, 'checkpoint_numchannels16.pt')

train(net3, optimizer3, train_loader, val_loader, max_num_epoch, device, checkpoint_path3, LOG_DIR)
"""

# PART 3 in the report template
net3 = Net(num_conv_layers, num_kernels, use_batchnorm=True, use_tanh=False, use_16_in_channels=True).to(device=device)
optimizer3 = optim.SGD(net3.parameters(), lr=hps['lr'])
checkpoint_path3 = os.path.join(LOG_DIR, 'checkpoint_numchannels16.pt')

train(net3, optimizer3, train_loader, val_loader, max_num_epoch, device, checkpoint_path3, LOG_DIR)

