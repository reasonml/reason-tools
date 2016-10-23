const webpack = require('webpack');

module.exports = {
  entry: {
    App: './src/App.js',
    Popup: './src/Popup.js',
  },
  output: {
    path: './_build/extension/',
    filename: '[name].bundle.js',
  },
}
