import torch
import torch.nn.functional as F
from torch.autograd import Variable
import cv2
import numpy as np
import matplotlib.pyplot as plt
import torch
from PIL import Image
from torchvision import transforms

class GradCam:
    def __init__(self, model):
        self.model = model.eval()
        self.feature = None
        self.gradient = None

    def save_gradient(self, grad):
        self.gradient = grad

    def __call__(self, x, interested_class):
        image_size = (x.size(-1), x.size(-2))
        datas = Variable(x)
        heat_maps = []
        for i in range(datas.size(0)):
            img = datas[i].data.cpu().numpy()
            img = img - np.min(img)
            if np.max(img) != 0:
                img = img / np.max(img)

            feature = datas[i].unsqueeze(0)
            round = 0
            for name, module in self.model.named_children():
                if name == 'classifier':
                    feature = feature.view(feature.size(0), -1)
                if name == 'features':
                    for submodule in module:
                        feature = submodule(feature)
                        round += 1
                        if round == 11:
                            self.feature = feature  # 最后一个卷积层

                feature.register_hook(self.save_gradient)

            classes = F.softmax(feature)
            # print(self.feature)
            # print(self.feature.shape)

            # 画出特征图片
            fig, axs = plt.subplots(16, 16, figsize=(16, 16))
            count = 0

            for i in range(16):
                for j in range(16):
                    feature_map = self.feature.detach().cpu().numpy()[0, count]
                    axs[i, j].imshow(feature_map, cmap='jet')
                    axs[i, j].axis('off')
                    count += 1
                    if count >= self.feature.shape[1]:
                        break

            FEATURE_SAVE_NAME = 'grad_cam_dog_feature_lastconv.png'
            plt.tight_layout()
            plt.savefig(FEATURE_SAVE_NAME, bbox_inches='tight', pad_inches=0)
            plt.close()

            predicted_class = torch.argmax(classes, dim=1)
            print('predicted_class: ', predicted_class)

            predicted_class = interested_class
            one_hot = torch.zeros_like(classes)
            one_hot[:, predicted_class] = 1

            self.model.zero_grad()
            classes.backward(gradient=one_hot, retain_graph=True)

            weight = self.gradient.mean(dim=-1, keepdim=True).mean(dim=-2, keepdim=True)
            mask = F.relu((weight * self.feature).sum(dim=1)).squeeze(0)
            mask = cv2.resize(mask.data.cpu().numpy(), image_size)
            mask = mask - np.min(mask)
            if np.max(mask) != 0:
                mask = mask / np.max(mask)
            heat_map = np.float32(cv2.applyColorMap(np.uint8(255 * mask), cv2.COLORMAP_JET))
            cam = heat_map + np.float32((np.uint8(img.transpose((1, 2, 0)) * 255)))
            cam = cam - np.min(cam)
            if np.max(cam) != 0:
                cam = cam / np.max(cam)
            heat_maps.append(transforms.ToTensor()(cv2.cvtColor(np.uint8(255 * cam), cv2.COLOR_BGR2RGB)))
        heat_maps = torch.stack(heat_maps)
        return heat_maps

IMAGE_NAME = 'C:\GAP\计算机视觉实验\实验四\实验四模型和测试图片（PyTorch）\data4\\dog.jpg'
SAVE_NAME = 'grad_cam_dog_1_lastconv.png'
test_image = (transforms.ToTensor()(Image.open(IMAGE_NAME))).unsqueeze(dim=0)
model = torch.load('torch_alex.pth')
grad_cam = GradCam(model)

interested_class = 1873  # 选择特定类别
# 4754 or 2507 or 1873

feature_image = grad_cam(test_image, interested_class)[0].squeeze(dim=0)
feature_image = transforms.ToPILImage()(feature_image)
feature_image.save(SAVE_NAME)

print(model.__dict__)
for name, module in model.named_modules():
    print(name, module)