# Daz Studio Tone Mapping Plugin
Adds some filmic tone mapping operators to Daz Studio.

ACES Tone Mapping          |  DAZ Studio Render
:-------------------------:|:-------------------------:
![](https://raw.githubusercontent.com/wiki/nalexandru/ToneMapping/images/PluginDemo_Natural_ACES.png)  |  ![](https://raw.githubusercontent.com/wiki/nalexandru/ToneMapping/images/PluginDemo_Natural.png)

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

More detailed information can be found on the wiki: [Usage](https://github.com/nalexandru/ToneMapping/wiki/Usage)

## 3rd Party Libraries
* tinyexr https://github.com/syoyo/tinyexr
* stb_image_write https://github.com/nothings/stb

## Credits 
### Based on
* http://filmicworlds.com/blog/filmic-tonemapping-operators/
* https://github.com/KhronosGroup/glTF-Sample-Viewer
### Assets used in demo image
* Character: https://www.daz3d.com/sewa-for-genesis-3-genesis-8-female
* Skin: https://www.daz3d.com/victoria-8-1 & https://www.daz3d.com/olivia-hd-for-genesis-8-female
* Hair: https://www.daz3d.com/xu-character-and-hair-for-genesis-8-female
* Outfit: https://www.daz3d.com/dforce-phenom-lingerie-for-genesis-8-females
* HDRI: https://hdrihaven.com/hdri/?c=outdoor&h=spiaggia_di_mondello
* Pose: https://www.daz3d.com/naturally-poses-for-genesis-81-female
