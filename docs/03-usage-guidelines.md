
# Usage guidelines

Without proper usage at runtime the system does not provide any redundancy and hence no additional value. Following the proposed guidelines is recommended:

1. **Make use of all image slots**
If there are 7 slots available, use all of them and keep them filled with good images.
You can trigger a copy from slot X to slot Y with the bootmanager, without transmitting a whole image!

2. **Update one slot at a time**
Keeping a good image in every slot except the one, that is currently in a update process leaves many redundant images that can be used in case of errors.

3. **Protect good images**
Once an image is considered good, it should be marked protected.
This can elimite the effects of accidentally selecting the wrong index of an image by the oser.
Protected images require one additional step to overwrite them.

4. **Monitoring**
Frequently check the status of all images. Keeping the number of good images with verified integrity is important for redundancy. 
