# Raytrace

Simple raytracer that implements Whitted's algorithm



## Features

* Sphere, triangle and mesh(.obj file) rendering

* Cook-Torrance shading model

* Reflection and refraction

* Texture mapping

* Supersampling anti-aliasing

* Output to PNG

* Scene description in JSON format



## Building

```sh
git clone https://github.com/Jimx-/raytrace.git
cd raytrace
git submodule update --init
mkdir build && cd build
cmake .. && make
```

## Screenshot

![](https://preview.ibb.co/bUekkJ/supersamples.png)
