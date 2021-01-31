# ToneMapping
Daz Studio Tone Mapping Plugin
Adds some filmic tone mapping operators to Daz Studio.

The plugin has been tested with Daz Studio 4.15.0.2 64bit on Windows 7. macOS is not supported.

## Building
* Open ToneMapping.sln and build it with Visual Studio 2010. Newer versions of Visual Studio will cause the plugin to crash due to STL differences.

## Installation
* Copy ToneMapping.dll into C:\Program Files\DAZ 3D\DAZStudio4\plugins

## Usage
* Enable the Beauty canvas before rendering
	- Render Settings > Advanced > Canvases
* After the render is complete go to Render > Tone Mapping
* Click "Load Image" and select the Beauty canvas EXR
* Select the tone mapping operator and adjust the exposure as desired
* Save the image

## 3rd Party Libraries
* tinyexr https://github.com/syoyo/tinyexr
* stb_image_write https://github.com/nothings/stb

## Credits 
Based on http://filmicworlds.com/blog/filmic-tonemapping-operators/ and https://github.com/KhronosGroup/glTF-Sample-Viewer
