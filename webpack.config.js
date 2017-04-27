const path = require('path');
const webpack = require('webpack');
const GenerateJsonPlugin = require('generate-json-webpack-plugin');
const manifest = require('./src/manifest.json');
const package = require('./package.json');
const ocamlPackage = require('ocaml/package.json');
const reasonPackage = require('reason/package.json');

manifest.version = package.version;

const commonModule = {
  loaders: [
    {
      test: /\.(png|jpg|gif|html|css)$/,
      loader: 'file-loader?name=[name].[ext]'
    }
  ],
};

const Extension = {
  name: "Extension",
  entry: {
    Content: './lib/es6/src/extension/content.js',
    ContentLoader: './lib/es6/src/extension/contentLoader.js',
    Popup: './lib/es6/src/extension/popup.js',
    Background: './lib/es6/src/extension/background.js',
  },
  output: {
    path: path.join(__dirname, '_build/extension/'),
    filename: '[name].bundle.js',
  },
  module: commonModule,
  plugins: [
    new GenerateJsonPlugin('manifest.json', manifest),
    new webpack.DefinePlugin({
      '__REASON_VERSION__': JSON.stringify(reasonPackage.version),
      '__OCAML_VERSION__': JSON.stringify(ocamlPackage.version)
    }),
  ],
};

const Page = {
  name: "Page",
  entry: {
    Popup: './lib/es6/src/extension/page.js',
  },
  output: {
    path: path.join(__dirname, 'docs/'),
    filename: '[name].bundle.js',
  },
  plugins: [
    new webpack.DefinePlugin({
      '__REASON_VERSION__': JSON.stringify(reasonPackage.version),
      '__OCAML_VERSION__': JSON.stringify(ocamlPackage.version)
    }),
  ],
  module: commonModule,
};

module.exports = [Extension, Page];
