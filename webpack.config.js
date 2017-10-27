const path = require('path');
const webpack = require('webpack');
const GenerateJsonPlugin = require('generate-json-webpack-plugin');
const manifest = require('./src/manifest.json');
const package = require('./package.json');

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
      '__REASON_VERSION__': '"wow"',
      '__OCAML_VERSION__': '"4.02.3"'
    }),
  ],
  node: {
    fs: "empty"
  },
  resolve: {
    alias: {
      react: path.resolve('./node_modules/react'),
    },
  },
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
      '__REASON_VERSION__': '"wow"',
      '__OCAML_VERSION__': '"4.02.3"'
    }),
  ],
  module: commonModule,
  node: {
    fs: "empty"
  },
  resolve: {
    alias: {
      react: path.resolve('./node_modules/react'),
    },
  },
};

module.exports = [Extension, Page];
