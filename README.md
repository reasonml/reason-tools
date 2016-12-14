# Reason Tools

Adds [Reason](http://facebook.github.io/reason/) to the browser.

## Getting started

[Chrome](https://chrome.google.com/webstore/detail/reason-tools/kmdelnjbembbiodplmhgfjpecibfhadd) | [Firefox](https://addons.mozilla.org/en-US/firefox/addon/reason-tools/)

![demo of reason-tools](https://raw.githubusercontent.com/rickyvetter/reason-tools/master/assets/demo.gif)

When you trigger the extension, it will copy the text you have highlighted and put it into an editor. The text is then translated into the corresponding Reason/OCaml text. Reason Tools will automatically convert between `.re`, `.ml`, `.rei`, and `.mli` text. In browsers that support it (currently just Chrome), a shortcut is added to open the extension with the highlighted text: `Alt+D`.

![before and after of code generation](https://raw.githubusercontent.com/rickyvetter/reason-tools/master/assets/before-after.jpg)

## Contribute

To get started contributing you can clone and build the extension:

```sh
git clone https://github.com/rickyvetter/reason-tools.git
cd reason-tools
npm install # this will take a while
```

once installed, you can rebuild the Reason code with `npm run build:re` and the js with `npm run build:js`. `npm run build` will run both.

> [Yarn](https://github.com/yarnpkg/yarn) can also be used to make things a little faster, but there are some edge cases still being worked on: https://github.com/yarnpkg/yarn/milestone/2

To load in Chrome, go to `chrome://extensions/` and turn on Developer Mode. From there you should be able to select "Load unpacked extension..." and choose `reason-tools/_build/extension`.

### Rebel

This project uses [Rebel](https://github.com/reasonml/rebel) as it's Reason build system. It will probably help to familiarize yourself with that system before jumping into changing the build process.

### webpack

This project uses [webpack](http://webpack.github.io/) as it's JavaScript build system. Currently it isn't very integrated with Rebel, so the ability to have one command to watch all changes and incrementally build isn't really available (yet).

## Thanks

The foundation of the project is, without a doubt, [refmt-web](https://github.com/Schmavery/refmt-web). This is an awesome project by @Schmavery which does the same refmt in a web page.

[reason-web-toplevel](https://github.com/Engil/reason-web-toplevel), by @Engil was also an awesome project where a lot of the work in this project came from.

Also huge thanks to the [js_of_ocaml](https://github.com/ocsigen/js_of_ocaml) team for building a compiler that pretty effortlessly builds Reason and refmt utils in JS.
