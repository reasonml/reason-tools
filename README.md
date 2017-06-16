# Reason Tools

Adds [Reason](http://facebook.github.io/reason/) to the browser.

## Getting started

Directly in the browser: https://reasonml.github.io/reason-tools/popup.html

Or, browser extensions: [Chrome](https://chrome.google.com/webstore/detail/reason-tools/kmdelnjbembbiodplmhgfjpecibfhadd) | [Firefox](https://addons.mozilla.org/en-US/firefox/addon/reason-tools/)

![demo of reason-tools](https://raw.githubusercontent.com/rickyvetter/reason-tools/master/assets/demo.gif)

When you trigger the extension, it will copy the text you have highlighted and put it into an editor. The text is then translated into the corresponding Reason/OCaml text. Reason Tools will automatically convert between `.re`, `.ml`, `.rei`, and `.mli` text. In browsers that support it (currently just Chrome), a shortcut is added to open the extension with the highlighted text: `Alt+D`.

![before and after of auto conversion](https://cloud.githubusercontent.com/assets/1909539/21284240/f5828a68-c3ca-11e6-9e29-13cf1a4f05fa.png)

### OCaml Documentation

Reason Tools tries to detect OCaml documentation pages and automatically convert between Reason and OCaml syntaxes. It provides an escape hatch for use if there is a false positive detection, or if you prefer the default OCamlDoc styles.

### Github Integration

Reason Tools adds a shortcut from interface to implementation files (`.re` <=> `.rei`, `.ml` <=> `.mli`) on Github when it's detected. Triggered by `Alt+I` by default (> v0.0.19).

## Contribute

To get started contributing you can clone and build the extension:

```sh
git clone https://github.com/rickyvetter/reason-tools.git
cd reason-tools
npm install # this will take a few minutes
```

`npm run build` to build the project. You also have the granular steps `build:refmt` `build:self` and `build:js` for faster iteration.

To load in Chrome, go to `chrome://extensions/` and turn on Developer Mode. From there you should be able to select "Load unpacked extension..." and choose `reason-tools/_build/extension`.

### Build Systems

This project is a bit unconventional in nature (compiling a compiler and a parser/printer to web), so it uses a few build systems currently.

- A custom `shell.sh` is used to compile Reason and the compiler to JavaScript.
- [Bsb](http://bucklescript.github.io/bucklescript/Manual.html#_build_system_support) is used to build its Reason logic. Nothing special.
- [Webpack](http://webpack.github.io/) is used to bundle the whole js output into a single pack.

## Thanks

The foundation of the project is, without a doubt, [refmt-web](https://github.com/Schmavery/refmt-web). This is an awesome project by @Schmavery which does the same refmt in a web page.

[reason-web-toplevel](https://github.com/Engil/reason-web-toplevel), by @Engil was also an awesome project where a lot of the work in this project came from.

Also huge thanks to the [js_of_ocaml](https://github.com/ocsigen/js_of_ocaml) team for building a compiler that pretty effortlessly builds Reason and refmt utils in JS.
