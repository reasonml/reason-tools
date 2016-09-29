# RebelExampleProject

[![Build Status](https://travis-ci.org/reasonml/RebelExampleProject.svg?branch=master)](https://travis-ci.org/reasonml/RebelExampleProject)

## Reason via `npm`

Example project using Reason as an `npm` dependency.

> Note: This example will be rapidly changing. It is not officially supported
> yet. Always reclone the repo each time you try it out (rebasing is not
> sufficient).

## Get Started:

```sh
git clone https://github.com/reasonml/RebelExampleProject.git
cd RebelExampleProject
npm install
npm start
```

If you are running as `root` already (you probably aren't) then invoke `npm
install --unsafe-perm` instead.

## Included Top Level

The top level `rtop` is built in to the sandbox: 

```sh
# Opens `rtop` from the sandbox.
npm run top
```

## Editor Support

All of the IDE support, including error highlighting, autocomplete, and
syntax is included inside of the sandbox. 

```sh
# Opens your `$EDITOR` with all the right tools in your `$PATH`
npm run editor
```

To make your editor load the IDE support from the sandbox:

- Make sure your `$EDITOR` variable is set if not already.
  - `export EDITOR=vim`, or `export EDITOR=atom`
- Configure your `EDITOR` to load the `Reason` plugins. See the instructions
  for [Atom](http://facebook.github.io/reason/tools.html#merlin-atom) and
  [Vim](https://github.com/facebook/reason/tree/master/editorSupport/VimReason).


### What's happening
- `npm install` will download and install all your dependencies, and run the
  `postinstall` steps for all of those dependencies, and then finally the
  `postinstall` script step of this project.
- `npm start` will run the script located in the `start` field of the
  `scripts` section of the `package.json` file here. The `start` script simply
  runs the binary that was built in the `postinstall` step.
- No, really - all you need is `npm` (> `3.0`). All of the compiler infrastructure
  has been organized into `npm` packages, and will be compiled on your host
  inside of the `./node_modules` directory. No `PATH`s are poluted. No global
  environment variables destroyed. No trace is left on your system. If you
  delete the `./node_modules` directory, you're back to exactly where you
  started.


### How to customize
- `npm` allows `scripts` to be specified in your project's `package.json`.
  These `scripts` are a named set of commands.
- A few scripts have special meaning, such as the `postinstall` script. The
  `postinstall` script is how your project compiles itself. It is guaranteed
  that the `postinstall` script executes any time you run `npm install` in this
  package, or any time another package installs you as a dependency. You're
  also guaranteed that your `postinstall` script is executed *after* all of
  your dependencies' `postinstall` scripts.
- You can add new named scripts in the `package.json` `scripts` field. Once
  added, you can then run them via `npm run scriptName` from within the project
  root.
- `. dependencyEnv` is commonly used in these `scripts`. The dot `.` sources
  `dependencyEnv` which manages the environment, and ensure that important
  binaries (such as `refmt`) are in the `PATH`. `dependencyEnv` ensures that
  the environment is augmented only for the duration of that `script` running,
  and only in ways that you or your immediate dependencies decide. When
  the entire purpose of developer tools is to generate a binary (such as a
  compiler) to be included in your `PATH`, or produce a library whose path
  should be specified in an special environment variable, it's almost like the
  environment variable is the public API of that package. `dependencyEnv`
  allows your script to see the environment variables that your immediate
  dependencies wanted to publish as their public API. You can learn how
  packages can publish environment variables in the [dependency-env
  repo](https://github.com/npm-ml/dependency-env).

### Recompiling
- To recompile this package (but not your dependencies), remove the local build
  artifacts for this package (usually just the `_build` directory) and then run
  `npm run postinstall`.

### How to turn this project into a library

- To turn this example project into a library that other people can depend on
  via `npm`... (coming soon).

### Troubleshooting:
- Check to make sure everything is installed correctly. There's a `script`
  already setup that will help you test the location of where `Reason` has been
  compiled into.

- If something goes wrong, try deleting the local `node_modules` directory that
  was installed, and then try reinstalling using `npm install -f`.

```
npm run whereisreason
```

### TODO:

- This also installs sandboxed IDE support for Vim/Atom/Emacs. We need to
  upgrade all of the plugins to automatically search for IDE plugins inside of
  the `./node_modules` directory.

