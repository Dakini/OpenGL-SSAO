# OpenGL-SSAO

This is a demo visualising a high resolution Model with Screen space ambient occlusion, tone mapping, auto keying and motion blur. It also has normal mapping and solid texturing as well
A vairety of libraries and shaders have been used most importantly are GLFW3, GLM, stbImage, Devil, and OpenCTM. 

The OpenCTM library was used in the compression of the models, reducing their overall memory size and loading times! More information can be found at http://openctm.sourceforge.net/

Controls:

Left Mouse button rotates the Model. 
Right Mouse Button Pans the model
Mouse scroll Zooms in on the model

1,2,3,4,5,6, load different textures, a simple shader, texture map and solid textures

Q,W,E,R changes the HDR image that is used for the lighting

To Debug and see what textures have been used / written to frame buffers press CTRL + Shift + F while using the demo
a folder will be created next to the .exe file with images and shader information
