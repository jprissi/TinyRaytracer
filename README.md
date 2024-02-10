# TinyRaytracer

This is a basic raytracer written in C++.

It is heavily inspired by the great [Raycasting From Scratch](https://github.com/nikolausmayer/raytracing-from-scratch) lesson by Nikolaus Mayer.

This project was meant as a way to get familiar with C++ by using the theoretical explanations from the lesson's slides and writing my own implementation. There are many directions to explore and this project is far from being finished.

Warning: We use a left-handed base with z forward (as it is common to do in the computer graphics field)

Next steps:

- Reformat and review code
- Add UV Mapping
    - ~~To spheres~~
    - To other objects
- Improve positioning system (coordinates, matrices, frames, ...)
- ~~Export to other file formats (png instead of ppm)~~
- Import .obj files
  - rapidobj could be a good candidate (according to https://aras-p.info/blog/2022/05/14/comparing-obj-parse-libraries/)

Some *FUN* ideas:
- Transparency (make a glass of water with an object inside)
- Raycasting (for a GUI editor for instance)
- Atmospheric diffusion.
- Clouds

![Example](./img.png)
