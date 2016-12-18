const path = require('path');
const webpack = require('webpack');
const GenerateJsonPlugin = require('generate-json-webpack-plugin');
const manifest = require('./src/manifest.json');
const package = require('./package.json');

manifest.version = package.version;

module.exports = {
  entry: {
    AutoConversionPrompt: './src/AutoConversionPrompt.js',
    Popup: './src/Popup.js',
    Background: './src/Background.js',
  },
  output: {
    path: path.join(__dirname, '_build/extension/'),
    filename: '[name].bundle.js',
  },
  module: {
    loaders: [
      {
        test: /\.js$/,
        exclude: /(node_modules|_build)/,
        loader: 'babel-loader',
        query: {
          presets: ['latest', 'react', 'stage-0']
        }
      },
      {
        test: /\.(png|jpg|gif|html|css)$/,
        loader: 'file-loader?name=[name].[ext]'
      }
    ],
  },
  plugins: [
    new GenerateJsonPlugin('manifest.json', manifest),
  ],
};
