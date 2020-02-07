# Ginny!

![Ginny](https://assets.bigcartel.com/product_images/247416878/IMG_20191025_1424438-01.jpeg)

Quite possibly the smallest, most feature-filled board you can fit in your pocket.

With multi-language onboard ASETNIOP and all sorts of fun shenanigans.

This board is more than a novelty, it's an ergonomic wonder!

---

## About

[Product info on gboards.ca](https://www.gboards.ca/product/ginni/)

* Keyboard Maintainer: [Jeremy Bernhardt](https://github.com/germ)  
* Hardware Supported: Ginny (Kit, Partial, Ready)   
* Hardware Availability: [gboards.ca](http://gboards.ca)  

---

## Firmware Building
Clone the QMK repo and submodules, then run the install script: ([Newbs Getting Started Guide](https://beta.docs.qmk.fm/newbs/newbs_getting_started))

    # if needed, uncomment && update '<repository URL>' with the correct URL.
    # git clone --recurse-submodules <repository URL>
    
    # if needed, uncomment && checkout the appropriate branch.
    # git checkout <branch-to-be-flashed>
    
    cd qmk_firmware
    util/qmk_install.sh

Install `dfu-programmer` using your package manager if the `qmk_install.sh` script did not.

    # Ubuntu/Debian    
    sudo apt install dfu-programmer
    # RedHat/CentOS
    sudo yum install dfu-programmer
    # etc...

Build and flash with the command below. Be sure to reset your keyboard!

    make ginny:en:flash

To just test your build with the default keymap:
   
    make ginny:en

Ginny supports many different languages, these can be configured by flashing the relevant keymap.

    make ginny:da
    make ginny:de
    make ginny:en
    make ginny:en-colemak
    make ginny:en-dvorak
    make ginny:en-gb
    make ginny:es
    make ginny:fn
    make ginny:fr
    make ginny:hu
    make ginny:it
    make ginny:nl
    make ginny:nw
    make ginny:pl
    make ginny:pt
    make ginny:ro
    make ginny:sv
    make ginny:tr

---

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information.

Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## [Reach out to me!](mailto:bernhardtjeremy@gmail.com)
