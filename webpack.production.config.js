const webpack = require('webpack');
const config = require('./webpack.config');
var ZipPlugin = require('zip-webpack-plugin');

config.plugins = config.plugins.concat([
  new webpack.DefinePlugin({
    'process.env':{
      'NODE_ENV': JSON.stringify('production')
    }
  }),
  new webpack.optimize.UglifyJsPlugin(),
  new webpack.optimize.DedupePlugin(),
  new webpack.optimize.AggressiveMergingPlugin(),
  new ZipPlugin({
    filename: 'reason-tools.zip',
  }),
]);

module.exports = config;
