import OpenImageIO as oiio
import numpy as np
import denoiseEngines

oidn_engine = denoiseEngines.OpenImageDenoiseEngine()
oidn_engine.colorFilterSet("hdr", True)
print(oidn_engine)

bty_buf = oiio.ImageBuf("/mnt/data/PROJECT/ImageTools/image_denoise/docs/car_bty.jpeg")
albedo_buf = oiio.ImageBuf("/mnt/data/PROJECT/ImageTools/image_denoise/docs/car_albedo.jpeg")
normal_buf = oiio.ImageBuf("/mnt/data/PROJECT/ImageTools/image_denoise/docs/car_albedo.jpeg")
width = bty_buf.spec().width
height = albedo_buf.spec().height

bty_pixels = bty_buf.get_pixels(oiio.FLOAT, oiio.ROI(oiio.ROI.All)).flatten()
albedo_pixels = albedo_buf.get_pixels(oiio.FLOAT, oiio.ROI(oiio.ROI.All)).flatten()
normal_pixels = normal_buf.get_pixels(oiio.FLOAT, oiio.ROI(oiio.ROI.All)).flatten()
output_pixels = np.zeros((width * height * 3), dtype=np.float32)

state = oidn_engine.denoise(width, height, bty_pixels, output_pixels, albedo_pixels, normal_pixels)
if not state:
    raise Exception("Denoised failed!")

output_buf = oiio.ImageBuf(bty_buf.spec())
output_buf.set_pixels(oiio.ROI(oiio.ROI.All), output_pixels.reshape((height, width, 3)))
output_buf.write(("/mnt/data/PROJECT/ImageTools/image_denoise/docs/debugDenoiseA.png"))

