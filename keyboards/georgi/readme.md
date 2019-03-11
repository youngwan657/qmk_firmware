# Georgi

![Georgi](https://camo.githubusercontent.com/88a1aa79a1dc6c331dad95de7daf7447b8b8c814/68747470733a2f2f6173736574732e62696763617274656c2e636f6d2f70726f647563745f696d616765732f3233323730323435312f35323636313730335f3537313730343232333330353933365f353334313038363735303830303334373133365f6e2e6a70673f6175746f3d666f726d6174266669743d6d617826773d373530)

A compact 20% (12x2) Split Keyboard for steno and QWERTY. 

[More info on qmk.fm](http://qmk.fm/georgi/)

Keyboard Maintainer: [Jeremy Bernhardt](https://github.com/germ)  
Hardware Supported: Georgi   
Hardware Availability: [gboards.ca](http://gboards.ca)  

## Firmware building
After cloning the QMK repo and installing dfu-programmer build and flash with. Be sure to reset your keyboard!

    make georgi:default:dfu

To just test your build with the default keymap
   
    make georgi:default

Build options can be enabled/disabled in keyboards/georgi/keymaps/default/rules.mk . Copy the default directory and make any changes to your layout, if you think they're worth sharing submit a PR!

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## Have an idea?  [Reach out to me!](mailto:bernhardtjeremy@gmail.com)
